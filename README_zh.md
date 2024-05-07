# CoCG

#### 介绍

面向未知环境下可并行任务的执行，结合图规划与应急规划两种规划方式，通过将应急规划中的结果确定性部分交由图规划器求解，以此使得该区域内的行为并行化，再通过将多个局部区域融合，得到全局并行化的规划结果。

#### 软件架构

软件架构说明

#### 安装教程

1.  编译项目

```
mkdir build
cd build
cmake .. -DBUILD_TESTING=1
make
```

2.  xxxx
3.  xxxx

#### 使用说明

1.  测试 parser

```
cd build/cocg
./parser pddl_parser/dom1.pddl pddl_parser/prob1.pddl
```

Or

```
cd build/cocg
./pddl_parser_test
```

2.  xxxx
3.  xxxx
