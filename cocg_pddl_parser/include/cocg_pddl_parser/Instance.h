#pragma once

#include "cocg_pddl_parser/Domain.h"
// #undef DOMAIN_DEBUG
// #define DOMAIN_DEBUG true
namespace parser {
namespace pddl {

class Instance {
 public:
  Domain &d;
  std::string name;
  GroundVec init;                      // initial state
  std::vector<Condition *> init_cond;  // initial and goal states
  Condition *goal = nullptr;           // Goal condition
  TokenStruct<std::string> ts;
  bool metric;

  Instance(Domain &dom) : d(dom), metric(false) {}

  Instance(Domain &dom, const std::string &s) : Instance(dom) { parse(s); }

  virtual ~Instance() {
    for (unsigned i = 0; i < init.size(); ++i) {
      delete init[i];
    }
    for (unsigned i = 0; i < init_cond.size(); ++i) {
      delete init_cond[i];
    }
    if (nullptr != goal) {
      delete goal;
    }
  }

  void parse(const std::string &s) {
    Stringreader f(s);
    name = f.parseName("problem");

    if (DOMAIN_DEBUG) {
      std::cout << name << "\n";
    }

    for (; f.getChar() != ')'; f.next()) {
      f.assert_token("(");
      f.assert_token(":");
      std::string t = f.getToken();

      if (DOMAIN_DEBUG) {
        std::cout << t << "\n";
      }

      if (t == "domain") {
        parseDomain(f);
      } else if (t == "objects") {
        parseObjects(f);
      } else if (t == "init") {
        parseInit(f);
      } else if (t == "goal") {
        parseGoal(f);
      } else if (t == "metric") {
        parseMetric(f);
      } else {
        f.tokenExit(t);
      }
    }
  }

  std::string getDomainName(const std::string &s) {
    std::string domain_name = "";

    Stringreader f(s);
    f.parseName("problem");

    for (; f.getChar() != ')'; f.next()) {
      f.assert_token("(");
      f.assert_token(":");
      std::string t = f.getToken();
      if (t == "domain") {
        f.next();
        domain_name = f.getToken();
        break;
      }
    }
    return domain_name;
  }

  void parseDomain(Stringreader &f) {
    f.next();
    f.assert_token(d.name);
    f.assert_token(")");
  }

  void parseObjects(Stringreader &f) {
    // TokenStruct< std::string > ts = f.parseTypedList( true, d.types );

    // We need to populate the global ts with the objects read from the
    // object construct to ensure a correct reading in other places of
    // the code.
    ts = f.parseTypedList(true, d.types);

    for (unsigned i = 0; i < ts.size(); ++i) {
      Type *type = d.getType(ts.types[i]);
      std::pair<bool, unsigned> pair = type->parseObject(ts[i]);
      if (pair.first == false) {
        type->objects.insert(ts[i]);
      }
    }

    for (unsigned i = 0; DOMAIN_DEBUG && i < d.types.size(); ++i) {
      std::cout << " ";
      if (d.typed) {
        std::cout << " " << d.types[i] << ":";
      }
      for (unsigned j = 0; j < d.types[i]->objects.size(); ++j) {
        std::cout << " " << d.types[i]->objects[j];
      }
      std::cout << "\n";
    }
  }

  virtual void parseGround(Stringreader &f, GroundVec &v) {
    TypeGround *c = 0;
    if (f.getChar() == '=') {
      f.assert_token("=");
      f.assert_token("(");

      std::string s = f.getToken();
      int i = d.funcs.index(s);
      if (i < 0) {
        f.tokenExit(s);
      }

      if (d.funcs[i]->returnType < 0) {
        c = new GroundFunc<double>(d.funcs[i]);
      } else {
        c = new GroundFunc<int>(d.funcs[i]);
      }
    } else {
      c = new TypeGround(d.preds.get(f.getToken(d.preds)));
    }
    c->parse(f, d.types[0]->constants, d);
    v.push_back(c);
  }

  void parseInit(Stringreader &f) {
    for (f.next(); f.getChar() != ')'; f.next()) {
      f.assert_token("(");
      if (f.checkNext("unknown") || f.checkNext("oneof") || f.checkNext("or")) {
        parseConditional(f, init_cond);
      } else {
        parseGround(f, init);
      }
    }
    ++f.c;

    for (unsigned i = 0; DOMAIN_DEBUG && i < init.size(); ++i) {
      std::cout << "  " << init[i];
    }
  }

  virtual void parseGoal(Stringreader &f) {
    f.next();
    f.assert_token("(");
    if (f.getChar() != ')') {
      goal = d.createCondition(f);
      goal->parse(f, ts, d);
    }
    f.next();
    f.assert_token(")");
  }

  // for the moment only parse total-cost/total-time
  void parseMetric(Stringreader &f) {
    if (!d.temp && !d.costs) {
      std::cerr << "metric only defined for temporal actions or actions with "
                   "costs!\n";
      std::exit(1);
    }

    metric = true;

    f.next();
    f.assert_token("minimize");
    f.assert_token("(");
    if (d.temp) {
      f.assert_token("total-time");
    } else {
      f.assert_token("total-cost");
    }
    f.assert_token(")");
    f.assert_token(")");
  }

  virtual void parseUnknownGround(Stringreader &f, Condition *&c) {
    auto s = f.getToken();
    c = new Unknown();
  }

  virtual void parseOneOf(Stringreader &f, Condition *&c) {
    auto s = f.getToken();
    c = new Oneof();
  }

  virtual void parseOr(Stringreader &f, Condition *&c) {
    auto s = f.getToken();
    c = new Or();
  }

  virtual void parseConditional(Stringreader &f, std::vector<Condition *> &v) {
    Condition *c = 0;
    if (f.checkNext("unknown")) {
      parseUnknownGround(f, c);
    } else if (f.checkNext("oneof")) {
      parseOneOf(f, c);
    } else if (f.checkNext("or")) {
      parseOr(f, c);
    } else {
      throw UnknownToken(f.getToken());
    }
    c->parse(f, ts, d);
    v.push_back(c);
  }

  // add an object of a certain type
  void addObject(const std::string &name, const std::string &type) {
    // It is not enough to insert the name as an object of the given
    // type.  We need also to populate the TS, by insering the object
    // into the ts with the below two instructions to then inform
    // further calls to the parser, that there are the objects of the
    // given type.
    ts.insert(name);
    ts.types.insert(ts.types.end(), ts.size(), type);
    d.getType(type)->objects.insert(name);
  }

  // add a predicate fluent to the initial state
  void addInit(const std::string &name, const StringVec &v = StringVec()) {
    TypeGround *tg = new TypeGround(d.preds.get(name));
    tg->insert(d, v);
    init.push_back(tg);
  }

  // add a function value to the initial state
  void addInit(const std::string &name, int value,
               const StringVec &v = StringVec()) {
    GroundFunc<int> *gf = new GroundFunc<int>(d.funcs.get(name), value);
    gf->insert(d, v);
    init.push_back(gf);
  }

  // add a function value to the initial state
  void addInit(const std::string &name, double value,
               const StringVec &v = StringVec()) {
    GroundFunc<double> *gf = new GroundFunc<double>(d.funcs.get(name), value);
    gf->insert(d, v);
    init.push_back(gf);
  }

  // add a fluent (predicate or function) to the initial state
  void addInit(TypeGround *g, const StringVec &v = StringVec()) {
    TypeGround *tg = 0;
    GroundFunc<int> *f1 = dynamic_cast<GroundFunc<int> *>(g);
    GroundFunc<double> *f2 = dynamic_cast<GroundFunc<double> *>(g);
    if (f1) {
      tg = new GroundFunc<int>(d.funcs.get(g->name), f1->value);
    } else if (f2) {
      tg = new GroundFunc<double>(d.funcs.get(g->name), f2->value);
    } else {
      tg = new TypeGround(d.preds.get(g->name));
    }
    tg->insert(d, v);
    init.push_back(tg);
  }

  // Add the goal represented by s to the Instance
  void addGoal(const std::string &s) {
    // In order to invoke the parseGoal, the input string shall also
    // contain an ")" for simulating the right parenthesis of "(goal
    // ....)", and an additional left ")" parenthesis for the part
    // outside. We need to handle also the empty goal case
    //
    // Alternatively, one can define another parseGoal function
    // that does not require these two additional characters.
    std::string ss = "";
    if (0 == s.length()) {
      ss = "(and ) ))";
    } else {
      ss = s + "))";
    }
    Stringreader f(ss);
    parseGoal(f);
  }

  // add an unknown predicate to the initial state
  void addInitUnknown(const std::string &name,
                      const StringVec &v = StringVec()) {
    auto unknown = new Unknown();
    auto *tg = new TypeGround(d.preds.get(name));
    tg->insert(d, v);
    unknown->add(tg);
    init_cond.push_back(unknown);
  }

  // add one of predicate to the initial state
  void addInitOneOf(const std::vector<std::string> &name,
                    const std::vector<StringVec> &v = {}) {
    auto one_of = new Oneof();
    for (int i = 0; i < name.size(); i++) {
      auto *tg = new TypeGround(d.preds.get(name[i]));
      tg->insert(d, v[i]);
      one_of->add(tg);
    }
    init_cond.push_back(one_of);
  }

  // add an or predicate to the initial state
  void addInitOr(const std::pair<std::string, std::string> &names,
                 const std::pair<StringVec, StringVec> &vecs = {},
                 const std::pair<bool, bool> negates = {}) {
    auto or_ = new Or();
    auto *tg1 = new TypeGround(d.preds.get(names.first));
    tg1->insert(d, vecs.first);
    auto *tg2 = new TypeGround(d.preds.get(names.second));
    tg2->insert(d, vecs.second);
    Condition *cond1 = tg1;
    if (negates.first) {
      cond1 = new Not(tg1);
    }
    Condition *cond2 = tg2;
    if (negates.second) {
      cond2 = new Not(tg2);
    }

    or_->first = cond1;
    or_->second = cond2;
    init_cond.push_back(or_);
  }

  friend std::ostream &operator<<(std::ostream &os, const Instance &o) {
    return o.print(os);
  }
  virtual std::ostream &print(std::ostream &stream) const {
    stream << "( define ( problem " << name << " )\n";
    stream << "( :domain " << d.name << " )\n";

    stream << "( :objects\n";
    for (unsigned i = 0; i < d.types.size(); ++i) {
      if (d.types[i]->objects.size()) {
        stream << "\t";
        for (unsigned j = 0; j < d.types[i]->objects.size(); ++j) {
          stream << d.types[i]->objects[j] << " ";
        }
        if (d.typed) {
          stream << "- " << d.types[i]->name;
        }
        stream << "\n";
      }
    }
    stream << ")\n";

    stream << "( :init\n";
    for (unsigned i = 0; i < init.size(); ++i) {
      ((TypeGround *)init[i])
          ->PDDLPrint(stream, 1, TokenStruct<std::string>(), d);
      stream << "\n";
    }

    for (unsigned i = 0; i < init_cond.size(); ++i) {
      ((Condition *)init_cond[i])
          ->PDDLPrint(stream, 1, TokenStruct<std::string>(), d);
      stream << "\n";
    }
    stream << ")\n";

    stream << "( :goal\n";
    if (nullptr != goal) {
      goal->PDDLPrint(stream, 1, ts /* TokenStruct< std::string >() */, d);
    }
    stream << ")\n";

    if (metric) {
      stream << "( :metric minimize ( total-";
      if (d.temp) {
        stream << "time";
      } else {
        stream << "cost";
      }
      stream << " ) )\n";
    }

    stream << ")\n";
    return stream;
  }
};

}  // namespace pddl
}  // namespace parser
