# STHGA-2

基于强度的混合遗传算法（Strength-based Hybrid Genetic Algorithm）实现，用于无线传感器网络覆盖调度问题。本项目是对经典 STHGA 算法的复现与扩展实验。

## 算法简介

STHGA 是一种混合遗传算法，结合了前向调度（Forward Scheduling）、混合调度（Mixed Scheduling）和关键调度（Critical Scheduling）等启发式策略，用于最大化无线传感器网络寿命。

**参考文献**：*Hybrid Genetic Algorithms with Forward-Coding Scheme for Maximizing Lifetime in Wireless Sensor Networks* (IEEE TEVC, 2010)

## 核心算法流程

### 1. 能量拆分
将异构传感器按能量拆分为多个同源子传感器（每个子传感器能量为1），实现统一调度。

### 2. 遗传算法优化
- **种群初始化**：随机分配传感器到覆盖子集
- **交叉操作**：均匀交叉产生新个体
- **竞争选择**：父代与子代适应度比较
- **变异操作**：对达到最优适应度但未完全覆盖的个体进行突变

### 3. 三种启发式调度
- **前向调度（forwSche）**：将冗余传感器从当前子集移动到新子集，K1=5次尝试
- **混合调度（mixSche）**：尝试重新分配传感器到更优的子集，K2=5次尝试
- **关键调度（critSche）**：检测仅被一个子集覆盖的关键区域，优化冗余分配

## 项目结构

```
sthga-2/
├── sthga-2.sln                     # Visual Studio 解决方案
├── sthga-2/
│   ├── sthga-2.cpp                 # 主算法源代码
│   ├── generate_sensor_data.cpp    # 传感器数据生成器
│   ├── sthga_with_energy.cpp       # 含能量拆分的STHGA版本
│   ├── sthga-2.vcxproj             # VS项目文件
│   ├── input/                      # 输入数据
│   │   ├── input_100.txt ~ input_6000.txt  # 不同规模传感器数据
│   │   ├── sensors_data.txt        # 传感器原始数据
│   │   ├── sensor_output.txt       # 输出结果
│   │   └── activation_times.txt    # 激活时间
│   └── STHGA/                      # 实验结果（58个配置）
│       ├── N100R4/                 # 100传感器，半径4
│       ├── N1000R8/                # 1000传感器，半径8
│       ├── N10000R5/               # 10000传感器，半径5
│       └── ...                     # 更多N/R组合
└── .vscode/                        # VS Code配置
```

### 实验结果文件（每个 N{x}R{y}/ 目录下）

| 文件 | 内容 |
|------|------|
| `total.txt` | 汇总统计：最优、均值、最差、评估次数、成功率、平均时间 |
| `inst.txt` | 每次试验的详细数据 |
| `avgEval.txt` | 平均评估次数 |
| `crate.txt` | 收敛过程中的覆盖集数量 |

### 实验结果示例

`total.txt` 包含以下字段：
```
N    R    field  yita   Up  TRIAL  pop  K1  K2  Gm  MUI  best  mean  worst  evalFit  ok%  avgT(ms)
100  4    400    6.28   2   30     3    5   5   20  0.5  40    37.27  32    15004    0%   4502
```

## 算法参数

| 参数 | 值 | 说明 |
|------|-----|------|
| POPSIZE | 3 | 种群大小 |
| K1 | 5 | 前向调度迭代次数 |
| K2 | 5 | 混合调度迭代次数 |
| Gm | 20 | 变异间隔代数 |
| MUI | 0.5 | 变异强度 |
| TRIALNUM | 30 | 独立试验次数 |
| MAX_EVA | 15000~90000 | 最大评估次数（随问题规模递增） |
| GRID_LEVEL | 4 | 网格细化级别 |

## 输入数据格式

每行三个数值：`x坐标 y坐标 能量值`

```
45.9655 18.8238 24
18.3142 24.7841 5
3.5066 45.6847 15
```

部署区域为 50×50，传感器数量从 100 到 30000 不等。

## 编译与运行

### Visual Studio
1. 打开 `sthga-2.sln`
2. 生成解决方案
3. 运行

### 命令行
```bash
g++ -std=c++11 -o sthga sthga-2/sthga-2.cpp
./sthga
```

确保 `input/` 目录下有对应的输入文件。

## 技术栈

- **语言**：C++
- **开发环境**：Visual Studio / VS Code
- **依赖**：仅标准库

## 相关项目

- [WSN](https://github.com/susongyuan/WSN) - 包含 STHGA 与 PVEGA 的完整对比实验
- [PVEGA](https://github.com/susongyuan/Piecewise-Variable-Energy-Genetic-Algorithm-PVEGA-) - 分段变能量遗传算法（改进算法）
- [Greedy-MCSS](https://github.com/susongyuan/Greedy-MCSS) - 贪心覆盖集调度
- [MCSSA](https://github.com/susongyuan/MCSSA) - 多阶段覆盖集调度
