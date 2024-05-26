# CoCG
- [English](README.md)
- 中文

## 介绍

面向未知环境下可并行任务的执行，结合图规划与应急规划两种规划方式，通过将应急规划中的结果确定性部分交由图规划器求解，以此使得该区域内的行为并行化，再通过将多个局部区域融合，得到全局并行化的规划结果。

## 项目模块

本项目主要包含如下部分：

- `cocg_pddl_parser`: 用于解析 PDDL 格式文件，在 PDDL 2.1 的基础上增加了对`unknown`和`oneof`两个 token，已支持 contingent planning 中的常规语法（如：Contingent-FF）。
  `cocg_ast`: 上述 PDDL 格式文件解析完成之后，会建立相应的数据结构，主要包含如下部分： - `node`: 最基础的、抽象语法树中的单个节点，predicate 是其叶节点，其他为分支节点 - `tree`: `node`的集合，用于表示`oneof`，`unknown`等复合语句 - `action`: PDDL domain 文件解析后的各个模板动作，也可表示实例化后的基元动作。 - `param`: `action`中的实参和类型 - `plan_item`: Contingent planner 规划得到的单个动作节点。 - `plan`: Contingent planner 规划得到的整棵动作树。
- `cocg_core`: CoCG 核心算法的实现。包含如下部分：
  - `cocg_domain_reader, cocg_domain_expert`: 读取、存储 domain 文件中的信息。
  - `cocg_problem_expert`: 保存二次规划过程中的各个状态。
  - `cont_plan_sovler_base`: 所有的应急规划器都应该继承自该基类。
  - `cocg_graphplan`: 基于规划图的优化算法的实现。
  - `cocg_core`: CoCG 的核心流程算法。
- `cocg_cff_plan_solver`: 内置了 Contingent-FF 规划器的实现，通过继承`cont_plan_sovler_base`类来完成与 CoCG 之间的交互。
- `cocg_tests`: 一些测试用例，同时实现了 Contingent-FF + CoCG 的集成示例。

## 项目编译

```
mkdir build
cd build
cmake ..
make
```

## 使用说明

1.  使用默认案例

```
cd build/cocg
./cff_cocg_test
```

2. 指定外部案例

```
cd build/cocg
./cff_cocg <domain_file> <problem_file>
```
