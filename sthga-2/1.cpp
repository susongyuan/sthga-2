#define _CRT_SECURE_NO_WARNINGS

/*
﻿代码来自论文 "采用前向编码方案的混合遗传算法，在无线传感器网络中实现寿命最大化，" 发表在IEEE进化计算期刊，2010年。
﻿
﻿提醒：
﻿1. 在readInst()中更改输入文件名
﻿2. 更改节点数量，例如 #define NODEN100
﻿3. 此代码用于 '区域覆盖'
*/

/*
这段代码的目的是通过混合遗传算法（STHGA）来优化无线传感器网络的覆盖，以最大化传感器网络的寿命。
具体来说，这段代码主要解决了如何通过有效的传感器调度和部署，使得目标区域能够被传感器网络覆盖，
且尽可能减少能量消耗，延长网络工作寿命。代码通过评估、选择、交叉、突变等遗传算法的过程来进行优化。

下面总结这段代码的最终输出结果和优化目标：

1、代码的主要结果
（1）最佳传感器覆盖方案：
代码的一个核心结果是通过遗传算法找到一个使传感器网络覆盖效果最优的调度方案。
这个最佳方案尽可能保证在有限的传感器数量下对目标区域的完全覆盖，并通过对冗余传感器的有效利用和调度来优化资源利用率。
最终输出的pbest记录了种群中的最优个体，包含最优的覆盖集合数量 (pbest.csetN)、
调度序列 (pbest.d[])、适应度值 (pbest.fitness) 等信息。

（2）覆盖效果的度量：
代码会输出覆盖的最终结果，主要衡量的指标包括：
覆盖集合的数量 (csetN)：表示完整覆盖集的数量。这是主要的优化目标，越多的完整覆盖集表示覆盖效果越好。
适应度 (fitness)：适应度函数计算完整覆盖集合的数量加上不完整集合的覆盖百分比。适应度越高表示覆盖越好。
覆盖冗余度 (yita)：衡量冗余覆盖的比例，旨在减少冗余的覆盖以节约资源。

（3）多次实验的统计数据：
代码中进行了多次实验（TRIALNUM次），每次实验的结果包括：
每次实验的最佳结果 (gBest)、最差结果 (gWorst) 和平均结果。
成功实验次数：即达到完全覆盖目标的实验次数。
实验成功率：表示在总实验次数中，有多少次实验达到了期望的完整覆盖。
平均评估次数 (avgEval)：每次实验需要进行多少次适应度评估，以达到最佳结果。这也是衡量算法效率的重要指标。
平均时间 (avgT(ms))：表示完成实验所需的时间，衡量算法的运行效率。

（4）覆盖情况与优化信息的文件输出：
代码会将覆盖情况与优化过程中的重要数据记录到文件中。
文件包括：
    total.txt：提供了整体实验的汇总信息，包括传感器数量、覆盖效果、实验耗时、成功率等。这些信息有助于快速比较不同实验设置下算法的效果。
    inst.txt：记录每次实验的详细过程，包括代数、评估次数、最优解和耗时。这个文件更适合深入分析某个实验的过程。
    avgEval.txt：保存了每次评估的平均适应度，可以绘制优化曲线，用于分析遗传算法的收敛性和效率。
    crate.txt：保存实验过程中每隔 20 代的最优覆盖集数量，有助于实时观察算法在优化过程中的表现。
*/

/*
1、调度序列(d[])：
调度序列(d[])用于表示每个传感器在无线传感器网络中的调度状态。
调度序列可以简单理解为控制每个传感器何时参与覆盖任务，以及该传感器属于哪个覆盖集合。

struct Population {
    int d[NODEN];             // 每个基因的调度序列号，从1到apmin
    int csetN;                // 完整覆盖集的数量
    double fitness;           // 适应度 = csetN + 不完整集的覆盖百分比（即csetN+1集）
} P[POPSIZE * 2], pbest;
在结构体 Population 中，d[] 是一个长度为 NODEN（传感器节点数量）的一维数组。
数组 d[i] 中的每一个元素对应于第 i 个传感器的调度序列号。

具体来说：
    d[i] 表示第 i 个传感器的调度集合。
    该值通常是一个整数，从 1 到 apmin，表示当前传感器属于哪个覆盖集合。
    csetN 则表示当前个体中可以形成的完整覆盖集合的数量。

d[i] 的取值范围从 0 到 csetN（不包括 csetN），其中：
    0 可能表示传感器当前没有被调度。
    1 到 csetN 表示该传感器属于某一个完整覆盖集合。
    例如，d[i] = 2 表示传感器 i 属于第二个覆盖集合。

2、Population：
（1）Population 是一个结构体（struct Population），用于表示遗传算法中的个体信息，
尤其是在优化无线传感器网络的覆盖问题中，每个个体（染色体）代表一个特定的调度方案或配置。

struct Population {
    int d[NODEN];             // 每个基因的调度序列号，从1到apmin
    int csetN;                // 完整覆盖集的数量
    double fitness;           // 适应度 = csetN + 不完整集的覆盖百分比（即csetN+1集）
} P[POPSIZE * 2], pbest;       // 每个种群有POPSIZE个染色体，最后POPSIZE个染色体记录新种群
int NP2 = POPSIZE * 2;

（2）P 数组和 pbest 变量
P[POPSIZE * 2]：
    P 是 Population 类型的数组，表示整个种群。这个种群中有 POPSIZE * 2 个个体，其中：
        前 POPSIZE 个表示原始种群。
        后 POPSIZE 个用来存储新生成的个体，这些个体是在交叉、变异等操作后生成的。
        通过这种结构，代码可以保留当前种群的个体，同时生成下一代种群，便于替换和选择。
pbest：
    pbest 是一个 Population 结构体变量，用于记录当前种群中的最优个体。
    在每一代的进化过程中，算法会不断比较所有个体的适应度，将适应度最高的个体保存到 pbest 中。
    pbest 最终代表的是当前找到的最佳调度方案，即覆盖效果最好的传感器调度方案。

（3）遗传算法中的应用
初始化：
    在遗传算法的初始化阶段，种群中的每一个个体都需要进行初始化，设置传感器的初始调度方案（即 d[]）。
    在 init() 函数中，所有个体的 d[] 被初始化为 0，表示所有传感器起始时未被调度，或者属于初始覆盖集合。

选择、交叉和突变：
    在 GAoptimize() 中，遗传算法对种群中的个体进行选择、交叉和突变操作，从而生成新一代个体。
    每个新个体的调度序列 (d[]) 是通过对两个父代个体的调度序列交叉生成的，或者对某些基因进行突变。
    csetN 和 fitness 随着调度方案的变化而重新计算和更新，以反映当前调度方案的覆盖效果。

适应度评估：
    每个个体在交叉、突变后都需要重新计算适应度值 (fitness)。适应度越高的个体更有可能被选择用来生成下一代。
    适应度评估是基于 eval_cover() 函数来计算的，这个函数计算了该个体的覆盖效果，即有多少目标区域被覆盖，以及覆盖集合的数量。

最优解更新：
    在每一代中，如果找到适应度高于 pbest 的个体，则会更新 pbest，保存当前最优的调度方案。
    这个过程不断重复，直至达到设定的代数或者满足停止条件。

3、各个概念之间的关系
种群（Population）与个体（Individual）：
    种群是由多个个体组成的集合。
    在代码中，P[] 数组表示整个种群，而 P[i] 是数组中的一个元素，表示种群中的一个个体。

个体（Individual）与染色体（Chromosome）：
    个体表示一个可能的解，而每个个体都有自己的染色体，即编码该个体的具体解信息。
    在代码中，Population 结构体是个体，d[] 是该个体的染色体。

染色体（Chromosome）与基因（Gene）：
    染色体是由多个基因组成的结构，基因是染色体上的最小单位，决定了个体的特征。
    在代码中，d[i] 就是染色体 d[] 上的基因，它表示某个传感器的状态。

4、（1）POPSIZE 是遗传算法中个体数量，即每一代的种群规模。
（2）OPos[][][]：记录传感器的覆盖网格。
（3）Gm：突变间隔，即每 Gm 代执行一次突变。
（4）pbest：保存当前种群中最优个体的信息。
（5）P[POPSIZE * 2]：存储当前种群及新生成的个体，用于下一代选择。
（6）apmin 记录了所有传感器所能够形成的完整覆盖集合的最小数量，表示可以用最少的几个传感器组合成多少个完整覆盖集。
*/


#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <direct.h>

#define NODEN 200       // 最大传感器节点数
#define MAX_Parts 20    // 能量最大值
#define TRIALNUM 30      // 测试次数
#define FMAX 10000       // 可能分配的场地索引的最大值
#define UPAPMIN 800      // 覆盖最小场地的次数的最大值
#define CRIMAX 800       // 覆盖最少场地的最大数量
#define GRID_C 600       // 网格列数
#define GRID_D 600       // 网格行数
#define PI  3.1415926535897932384626
#define POPSIZE 3        // 遗传算法中个体数量，即每一代的种群规模
#define GRID_LEVEL 4     // 每个传感器圆形区域的半径划分数量
const int CMAX = GRID_LEVEL * GRID_LEVEL * 4;  // 圆形区域的最大网格数

int K1 = 5;
int K2 = 5;
int Gm = 20;
double MUI = 0.5;
double R = 4.0;

double L = 20.0;
double W = 20.0;
int tmp[UPAPMIN];
int nos[UPAPMIN][UPAPMIN];
int N = NODEN;
int ino;
int OPos[NODEN][CMAX + 1][2];
int GRID_L;
int GRID_W;
int trialNum;

double stepL;
double stepW;
double point[NODEN][2];
int sensorEnergy[NODEN];
double yita;
int parts;

int fieldNum;
int OPosf[NODEN][CMAX + 1];
int field[FMAX];
int grid[GRID_C][GRID_D];
int fieldIndex[GRID_C][GRID_D];

int G;
int apmin;
int countEvals;
clock_t tp1, tp2;

char filename[100], filename2[100], filename3[100], filename4[100], filename5[100], ch[30];
char filename6[30];
FILE* ftotal, * finst, * favgtotal;
FILE* fcrate;
int evalStep = 10;
int recordCount;
int maxRecordCount;
double evalValueAvg[100000];

struct Population {
    int d[NODEN][MAX_Parts];
    int csetN;
    double fitness;
} P[POPSIZE * 2], pbest;
int NP2 = POPSIZE * 2;

struct node {
    int no;
    int part;
    node* next;
};
int cricount;
node criti[CRIMAX];


double rand01() {
    return (double)rand() / (double)(RAND_MAX + 1);
}

int iscovered(int i, int j, int k) {
    double x = i * stepL;
    double y = j * stepW;
    if ((x - point[k][0]) * (x - point[k][0]) + (y - point[k][1]) * (y - point[k][1]) > R * R ||
        (x + stepL - point[k][0]) * (x + stepL - point[k][0]) + (y - point[k][1]) * (y - point[k][1]) > R * R ||
        (x - point[k][0]) * (x - point[k][0]) + (y + stepW - point[k][1]) * (y + stepW - point[k][1]) > R * R ||
        (x + stepL - point[k][0]) * (x + stepL - point[k][0]) + (y + stepW - point[k][1]) * (y + stepW - point[k][1]) > R * R) {
        return 0;
    }
    return 1;
}

void init_grid() {
    int i, j, k;
    double step = R / GRID_LEVEL;
    GRID_L = (int)(L / step);
    GRID_W = (int)(W / step);

    int i0, j0;
    stepL = L / GRID_L;
    stepW = W / GRID_W;

    for (k = 0; k < NODEN; k++) {
        OPos[k][CMAX][0] = 0;
        if (point[k][0] - R < 0) i0 = 0;
        else i0 = (int)((point[k][0] - R) / stepL);
        if (point[k][1] - R < 0) j0 = 0;
        else j0 = (int)((point[k][1] - R) / stepW);
        for (i = i0; i < i0 + GRID_LEVEL * 2 && i < GRID_L; i++) {
            for (j = j0; j < j0 + GRID_LEVEL * 2 && j < GRID_W; j++) {
                if (iscovered(i, j, k)) {
                    OPos[k][OPos[k][CMAX][0]][0] = i;
                    OPos[k][OPos[k][CMAX][0]][1] = j;
                    OPos[k][CMAX][0]++;
                }
            }
        }
    }
}

void calc_field() {
    int i, j, k, h;
    int fh[FMAX];
    int count = 1;
    memset(fh, -1, sizeof(fh));
    fieldNum = 0;

    for (i = 0; i < GRID_L; i++) {
        for (j = 0; j < GRID_W; j++) {
            grid[i][j] = 0;
        }
    }

    for (h = 0; h < N; h++) {
        for (k = 0; k < OPos[h][CMAX][0]; k++) {
            i = OPos[h][k][0];
            j = OPos[h][k][1];

            if (grid[i][j] == 0) {
                grid[i][j] = count++;
                if (count > FMAX) {
                    printf("Error: FMAX is not big enough\n");
                    exit(1);
                }
            }
        }
    }

    printf("Grid to Field Mapping:\n");
    for (i = 0; i < GRID_L; i++) {
        for (j = 0; j < GRID_W; j++) {
            if (grid[i][j] != 0 && fh[grid[i][j]] == -1) {
                fh[grid[i][j]] = fieldNum++;
            }
            fieldIndex[i][j] = fh[grid[i][j]];
            if (grid[i][j] != 0) {
                printf("Grid (%d, %d) -> Field %d\n", i, j, fieldIndex[i][j]);
            }
        }
    }

    printf("Total fields: %d\n", fieldNum);

    for (h = 0; h < N; h++) {
        OPosf[h][CMAX] = 0;
        for (k = 0; k < OPos[h][CMAX][0]; k++) {
            i = OPos[h][k][0];
            j = OPos[h][k][1];
            int fieldID = fieldIndex[i][j];

            int alreadyAdded = 0;
            for (int m = 0; m < OPosf[h][CMAX]; m++) {
                if (OPosf[h][m] == fieldID) {
                    alreadyAdded = 1;
                    break;
                }
            }
            if (!alreadyAdded) {
                OPosf[h][OPosf[h][CMAX]++] = fieldID;
            }
        }

        printf("Sensor %d covers fields: [", h + 1);
        for (int m = 0; m < OPosf[h][CMAX]; m++) {
            printf("%d", OPosf[h][m]);
            if (m != OPosf[h][CMAX] - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    int coverage[FMAX] = { 0 };
    for (h = 0; h < N; h++) {
        for (k = 0; k < OPosf[h][CMAX]; k++) {
            coverage[OPosf[h][k]]++;
        }
    }

    printf("Field coverage statistics:\n");
    for (i = 1; i <= fieldNum; i++) {
        printf("Field %d is covered by %d sensor(s)\n", i, coverage[i]);
    }

    if (fieldIndex[GRID_L - 1][GRID_W - 1] == fieldNum - 1) {
        printf("Field %d corresponds to grid (%d, %d)\n", fieldNum, GRID_L - 1, GRID_W - 1);
    }

    for (h = 0; h < N; h++) {
        for (k = 0; k < OPosf[h][CMAX]; k++) {
            if (OPosf[h][k] == fieldNum - 1) {
                printf("Field %d is covered by Sensor %d\n", fieldNum, h + 1);
            }
        }
    }
}

double eval_cover(int scheIndex, int id) {
    int i, j, k;
    int count = 0;

    double coverageWeight = 0.0;
    for (k = 0; k < N; k++) {
        parts = (int)sensorEnergy[k];
        for (int i = 0; i < parts; i++)
        {
            if (P[id].d[k][i] == scheIndex) {
                for (j = 0; j < OPosf[k][CMAX]; j++) {
                    field[OPosf[k][j]] = 1;
                }
                coverageWeight += 1.0 / parts;
            }
        }
    }

    for (i = 0; i < fieldNum; i++) {
        if (field[i] == 1) {
            count++;
            field[i] = 0;
        }
    }

    return (double)count / (double)(fieldNum);
}

void mixSche(int id) {
    int i, j, k, tmp, part;
    double v;

    for (i = 0; i < K2; i++) {
        if (sensorEnergy[i] <= 0) continue;
        tmp = (int)(rand01() * NODEN);
        part = (int)(rand01() * MAX_Parts);
        k = P[id].d[tmp][part];

        if (k == -1) continue;

        if (k != P[id].csetN) {
            P[id].d[tmp][part] = -1;
            if (eval_cover(k, id) == 1.0) {
                j = (int)(rand01() * P[id].csetN);
                if (j == k) j = P[id].csetN;
                P[id].d[tmp][part] = j;
            }
            else {
                P[id].d[tmp][part] = k;
            }
        }
        else {
            v = eval_cover(k, id);
            P[id].d[tmp][part] = (int)(rand01() * P[id].csetN);
            if (eval_cover(k, id) < v) {
                P[id].d[tmp][part] = P[id].csetN;
            }
        }
    }
}

void forwSche(int id) {
    int i, k, part, tmp;

    for (i = 0; i < K1; i++) {
        if (sensorEnergy[i] <= 0) continue;
        tmp = (int)(rand01() * NODEN);
        part = (int)(rand01() * MAX_Parts);
        k = P[id].d[tmp][part];

        while (k == P[id].csetN || k == -1) {
            tmp = (int)(rand01() * NODEN);
            part = (int)(rand01() * MAX_Parts);
            k = P[id].d[tmp][part];
        }

        P[id].d[tmp][part] = P[id].csetN;

        if (eval_cover(k, id) != 1.0) {
            P[id].d[tmp][part] = k;
        }
    }
}

void critSche(int id) {
    int i, j, k, selectedPart;
    int sensorIndex;
    bool foundRedundant;
    node* pt;

    for (i = 0; i < cricount; i++) {
        for (j = 0; j <= apmin; j++) {
            tmp[j] = 0;
        }

        pt = &criti[i];
        while (pt->next != NULL) {
            pt = pt->next;
            int currentSet = P[id].d[pt->no][0];
            nos[currentSet][tmp[currentSet]] = pt->no;
            tmp[currentSet]++;
        }

        if (tmp[P[id].csetN] == 0) {
            j = 0;
            foundRedundant = false;

            while (j < P[id].csetN && !foundRedundant) {
                if (tmp[j] >= 2) {
                    for (int index = 0; index < tmp[j]; index++) {
                        sensorIndex = nos[j][index];

                        for (selectedPart = 0; selectedPart < (int)sensorEnergy[sensorIndex]; selectedPart++) {
                            if (P[id].d[sensorIndex][selectedPart] == j) {
                                P[id].d[sensorIndex][selectedPart] = -1;
                                if (eval_cover(j, id) == 1.0) {
                                    P[id].d[sensorIndex][selectedPart] = P[id].csetN;
                                    foundRedundant = true;
                                    break;
                                }
                                else {
                                    P[id].d[sensorIndex][selectedPart] = j;
                                }
                            }
                        }

                        if (foundRedundant) {
                            break;
                        }
                    }
                }
                j++;
            }
        }
    }
}

void GAoptimize() {
    printf("Genetic Algorithm Optimization running...\n");
    int i, j, k, part, a1, a2;
    double v;
    double bv;

    for (i = POPSIZE; i < NP2; i++) {
        a1 = (int)(rand01() * POPSIZE);
        a2 = (int)(rand01() * POPSIZE);
        while (a2 == a1) a2 = (int)(rand01() * POPSIZE);

        for (j = 0; j < NODEN; j++) {
            parts = (int)sensorEnergy[j];
            for (part = 0; part < parts; part++) {
                if (rand01() < 0.5)
                    P[i].d[j][part] = P[a1].d[j][part];
                else
                    P[i].d[j][part] = P[a2].d[j][part];
            }
        }

        for (k = 0; k <= apmin; k++) {
            v = eval_cover(k, i);
            if (v != 1.0) {
                P[i].csetN = k;
                double totalEnergyUsed = 0.0;
                double totalEnergyAvailable = 0.0;
                for (int j = 0; j < NODEN; j++) {
                    parts = (int)sensorEnergy[j];
                    for (part = 0; part < parts; part++) {
                        if (P[i].d[j][part] != -1) {
                            totalEnergyUsed += 1.0;
                        }
                    }
                    totalEnergyAvailable += (double)sensorEnergy[j];
                }

                double energyPenalty = totalEnergyUsed / totalEnergyAvailable;
                P[i].fitness = k + v - energyPenalty;
                break;
            }
        }

        countEvals++;
        if (countEvals % evalStep == 1) {
            evalValueAvg[recordCount] += pbest.fitness;
            recordCount++;
        }

        if (P[a1].fitness < P[a2].fitness) a1 = a2;
        if (P[i].fitness < P[a1].fitness) {
            P[i].csetN = P[a1].csetN;
            for (j = 0; j < NODEN; j++) {
                parts = (int)sensorEnergy[j];
                for (part = 0; part < parts; part++) {
                    P[i].d[j][part] = P[a1].d[j][part];
                }
            }
            P[i].fitness = P[a1].fitness;
        }
        else if (pbest.fitness < P[i].fitness) {
            pbest.fitness = P[i].fitness;
            pbest.csetN = P[i].csetN;
            for (j = 0; j < NODEN; j++) {
                parts = (int)sensorEnergy[j];
                for (part = 0; part < parts; part++) {
                    pbest.d[j][part] = P[i].d[j][part];
                }
            }
        }
    }

    if (G % Gm == 0) {
        bv = 0;
        for (i = POPSIZE; i < NP2; i++) {
            if (P[i].fitness > bv) bv = P[i].fitness;
        }
        if (bv > ((int)bv)) {
            for (i = POPSIZE; i < NP2; i++) {
                if (P[i].fitness == bv) {
                    for (j = 0; j < NODEN; j++) {
                        parts = (int)sensorEnergy[j];
                        for (part = 0; part < parts; part++) {
                            if (P[i].d[j][part] == P[i].csetN && rand01() < MUI) {
                                P[i].d[j][part] = (int)(rand01() * (P[i].csetN));
                            }
                        }
                    }
                    v = eval_cover(P[i].csetN, i);
                    P[i].fitness = P[i].csetN + v;

                    countEvals++;
                    if (countEvals % evalStep == 1) {
                        evalValueAvg[recordCount] += pbest.fitness;
                        recordCount++;
                    }
                }
            }
        }
    }

    for (i = POPSIZE; i < NP2; i++) mixSche(i);
    for (i = POPSIZE; i < NP2; i++) forwSche(i);
    for (i = POPSIZE; i < NP2; i++) critSche(i);

    for (i = POPSIZE; i < NP2; i++) {
        v = eval_cover(P[i].csetN, i);
        P[i].fitness = P[i].csetN + v;
        if (v == 1.0) P[i].csetN++;
        countEvals++;
        if (countEvals % evalStep == 1) {
            evalValueAvg[recordCount] += pbest.fitness;
            recordCount++;
        }
        if (P[i].fitness > pbest.fitness) {
            pbest.fitness = P[i].fitness;
            pbest.csetN = P[i].csetN;
            for (j = 0; j < NODEN; j++) {
                parts = (int)sensorEnergy[j];
                for (part = 0; part < parts; part++) {
                    pbest.d[j][part] = P[i].d[j][part];
                }
            }
        }
    }

    for (i = 0; i < POPSIZE; i++) {
        P[i].csetN = P[POPSIZE + i].csetN;
        P[i].fitness = P[POPSIZE + i].fitness;
        for (j = 0; j < NODEN; j++) {
            parts = (int)sensorEnergy[j];
            for (part = 0; part < parts; part++) {
                P[i].d[j][part] = P[POPSIZE + i].d[j][part];
            }
        }
    }
}

void init() {
    int i, k, part;
    double v;

    for (k = 0; k < POPSIZE; k++) {
        for (i = 0; i < NODEN; i++) {
            parts = (int)sensorEnergy[i];
            for (part = 0; part < parts; part++) {
                P[k].d[i][part] = rand() % apmin;
            }
        }
        P[k].csetN = apmin;
        P[k].fitness = 0.0;
    }

    pbest.csetN = 0;
    pbest.fitness = 0.0;

    for (i = 0; i < POPSIZE; i++) forwSche(i);

    v = eval_cover(1, 0);
    if (v == 1.0) {
        P[0].csetN++;
    }
    P[0].fitness = P[0].csetN + v;

    pbest.csetN = P[0].csetN;
    pbest.fitness = P[0].fitness;
    for (i = 0; i < NODEN; i++) {
        parts = (int)sensorEnergy[i];
        for (part = 0; part < parts; part++) {
            P[0].d[i][part] = P[0].d[i][part];
            pbest.d[i][part] = P[0].d[i][part];
        }
    }
    countEvals++;
    if (countEvals % evalStep == 1) {
        evalValueAvg[recordCount] += pbest.fitness;
        recordCount++;
    }

    for (i = 1; i < POPSIZE; i++) {
        v = eval_cover(1, i);
        if (v == 1.0) {
            P[i].csetN++;
        }
        P[i].fitness = P[i].csetN + v;

        countEvals++;
        if (countEvals % evalStep == 1) {
            evalValueAvg[recordCount] += pbest.fitness;
            recordCount++;
        }

        if (P[i].fitness > pbest.fitness) {
            pbest.fitness = P[i].fitness;
            pbest.csetN = P[i].csetN;
            for (k = 0; k < NODEN; k++) {
                parts = (int)sensorEnergy[k];
                for (part = 0; part < parts; part++) {
                    pbest.d[k][part] = P[i].d[k][part];
                }
            }
        }
    }
}

void calc_criti() {
    int i, k, h, part;
    int cc;
    node* pt;

    cricount = 0;

    for (i = 0; i < fieldNum; i++) {
        if (field[i] == apmin) {
            cc = 0;
            pt = &criti[cricount];

            for (k = 0; k < NODEN; k++) {
                parts = (int)sensorEnergy[k];
                for (part = 0; part < MAX_Parts; part++) {
                    if (P[0].d[k][part] == -1) continue;

                    for (h = 0; h < OPosf[k][CMAX]; h++) {
                        if (OPosf[k][h] == i) {
                            pt->next = new node;
                            pt = pt->next;
                            pt->no = k;
                            pt->part = part;
                            cc++;
                            break;
                        }
                    }
                    if (cc == apmin) break;
                }
                if (cc == apmin) break;
            }

            pt->next = NULL;
            cricount++;
            if (cricount > CRIMAX) {
                printf("CRIMAX is not big enough!\n");
                exit(0);
            }
        }
    }
}

bool deployOK() {
    int i, j, k;
    bool flag = true;

    for (i = 0; i < fieldNum; i++) {
        field[i] = 0;
    }

    for (k = 0; k < N; k++) {
        for (j = 0; j < OPosf[k][CMAX]; j++) {
            int fieldID = OPosf[k][j];
            field[fieldID]++;
        }
    }

    apmin = field[0];
    for (i = 0; i < fieldNum; i++) {
        if (field[i] > 0) {
            if (apmin > field[i]) {
                apmin = field[i];
            }
        }
        else {
            flag = false;
            break;
        }
    }

    if (apmin > UPAPMIN) {
        printf("UPAPMIN is not big enough!\n");
        exit(0);
    }

    if (flag) {
        calc_criti();
    }

    for (i = 0; i < fieldNum; i++) {
        field[i] = 0;
    }

    return flag;
}

void readInst() {
    strcpy(filename, "input//input_200.txt");
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        double x, y;
        fscanf(fin, "%lf %lf", &x, &y);
        point[i][0] = x;
        point[i][1] = y;
        sensorEnergy[i] = 1; // 固定能量值20

        for (int part = 0; part < (int)sensorEnergy[i]; part++) {
            P[0].d[i][part] = -1;
        }

        printf("Sensor %d: X=%lf, Y=%lf, Energy=%d\n", i, x, y, sensorEnergy[i]);
    }
    fclose(fin);
}

int main() {
    int MAX_EVA;
    srand((unsigned)time(0));

    strcpy(filename, "STHGA");
    _mkdir(filename);

    strcat(filename, "\\N");
    _itoa(N, ch, 10);
    strcat(filename, ch);
    strcat(filename, "R");
    _itoa((int)R, ch, 10);
    strcat(filename, ch);
    _mkdir(filename);

    strcpy(filename2, filename);
    strcat(filename2, "\\total.txt");
    strcpy(filename3, filename);
    strcat(filename3, "\\inst.txt");
    strcpy(filename4, filename);
    strcat(filename4, "\\avgEval.txt");
    strcpy(filename6, filename);
    strcat(filename6, "\\crate.txt");

    ftotal = fopen(filename2, "a");
    fcrate = fopen(filename6, "a");

    fprintf(ftotal, "N\tR\tfield\tyita\tUp\tTRIAL\tpop\tK1\tK2\tGm\tMUI\tbest\tmean\tworst\tevalFit\tok%%\tavgT(ms)\n");
    fclose(ftotal);

    int i, recordMAX;
    int countOK, gBest, gWorst;
    int countTime, countResult, avgEval;

    ftotal = fopen(filename2, "a");
    finst = fopen(filename3, "a");
    favgtotal = fopen(filename4, "a");
    fprintf(finst, "no.\tgen\tevalFit\tresult\ttime(ms)\n");

    switch (NODEN) {
    case 40: MAX_EVA = 15000; break;
    case 60: MAX_EVA = 15000; break;
    case 100: MAX_EVA = 15000; break;
    case 140: MAX_EVA = 15000; break;
    case 180: MAX_EVA = 15000; break;
    case 200: MAX_EVA = 15000; break;
    case 250: MAX_EVA = 15000; break;
    case 300: MAX_EVA = 15000; break;
    case 400: MAX_EVA = 15000; break;
    case 600: MAX_EVA = 15000; break;
    case 800: MAX_EVA = 15000; break;
    case 1000: MAX_EVA = 15000; break;
    case 1500: MAX_EVA = 15000; break;
    case 2500: MAX_EVA = 15000; break;
    case 5000: MAX_EVA = 15000; break;
    case 10000: MAX_EVA = 30000; break;
    case 15000: MAX_EVA = 45000; break;
    case 20000: MAX_EVA = 60000; break;
    case 25000: MAX_EVA = 75000; break;
    case 30000: MAX_EVA = 90000; break;
    default: printf("MAX_EVA error!\n");
    }

    printf("MAX_EVA = %d\n", MAX_EVA);

    recordMAX = sizeof(evalValueAvg) / sizeof(evalValueAvg[0]);
    for (i = 0; i < recordMAX; i++) evalValueAvg[i] = 0;

    readInst();
    init_grid();
    calc_field();

    clock_t total_start = clock();

    printf("Deploying : %d\n", deployOK());
    printf("apmin = %d\n", apmin);
    if (deployOK() && apmin > 1) {
        if (apmin >= 800) {
            printf("the size of timeArray[][] is not big enough!\n");
            return -1;
        }

        yita = N * PI * R * R / (double)apmin / L / W;
        countOK = countTime = countResult = 0;
        gBest = 0;
        gWorst = N;
        maxRecordCount = 0;
        avgEval = 0;

        for (trialNum = 0; trialNum < TRIALNUM; trialNum++) {
            G = 0;
            countEvals = 0;
            recordCount = 0;
            init();
            tp1 = clock();

            while (countEvals <= MAX_EVA) {
                GAoptimize();
                G++;

                if ((G) % 20 == 0) {
                    printf("test%d\t:\tG=%d\t%d\tcheck=%d\ttime=%dms\n", trialNum + 1, G, countEvals, pbest.csetN, clock() - tp1);
                    fprintf(fcrate, "%d\n", pbest.csetN);
                }

                if (pbest.csetN == apmin) {
                    countOK++;
                    break;
                }
                countEvals++;
            }

            tp2 = clock();
            countTime += tp2 - tp1;

            if (maxRecordCount < recordCount) {
                for (i = maxRecordCount; i < recordCount; i++) {
                    evalValueAvg[i] += pbest.fitness;
                }
                maxRecordCount = recordCount;
            }
            else {
                for (i = recordCount; i < maxRecordCount; i++) {
                    evalValueAvg[i] += pbest.fitness;
                }
            }

            countResult += pbest.csetN;
            if (gBest < pbest.csetN) gBest = pbest.csetN;
            if (gWorst > pbest.csetN) gWorst = pbest.csetN;
            avgEval += countEvals;

            printf("test%d\t:\tG=%d\t%d\tcheck=%d\ttime=%dms\n", trialNum + 1, G, countEvals, pbest.csetN, tp2 - tp1);
            fprintf(finst, "%d\t%d\t%d\t%d\t%d\n", trialNum + 1, G, countEvals, pbest.csetN, tp2 - tp1);
        }

        clock_t total_end = clock();
        double total_time = ((double)(total_end - total_start)) / CLOCKS_PER_SEC;

        fprintf(ftotal, "%d\t%g\t%d\t%g\t%d\t%d\t%d\t%d\t%d\t%d\t%g\t%d\t%g\t%d\t%d\t%g%%\t%g\n", N, R, fieldNum, yita, apmin, TRIALNUM, POPSIZE, K1, K2, Gm, MUI, gBest, (double)countResult / TRIALNUM, gWorst, avgEval / TRIALNUM, (double)countOK / TRIALNUM * 100, (double)countTime / TRIALNUM);
        printf("Field\tUp\tbest\tmean\tworst\tevalFit\tok%%\tavgT(ms)\n");
        printf("%d\t%d\t%d\t%g\t%d\t%d\t%g%%\t%g\n", fieldNum, apmin, gBest, (double)countResult / TRIALNUM, gWorst, avgEval / TRIALNUM, (double)countOK / TRIALNUM * 100, (double)countTime / TRIALNUM);

        for (i = 0; i < maxRecordCount; i++) {
            fprintf(favgtotal, "%d\t%g\n", evalStep * i + 1, evalValueAvg[i] / (double)TRIALNUM);
        }

    for (int i = 0; i < N; i++) {
        parts = (int)sensorEnergy[i];
        for (int part = 0; part < parts; part++) {
            if (P[0].d[i][part] != -1) {
                printf("Sensor %d, Part %d: Position (%.4f, %.4f), Status: Active in Group %d\n", i, part, point[i][0], point[i][1], P[0].d[i][part]);
            }
        }
    }
    for (int i = 0; i < NODEN; i++) {
        for (int part = 0; part < (int)sensorEnergy[i]; part++) {
            if (P[0].d[i][part] != -1) {
                printf("Sensor %d, Part %d: Position (%.4f, %.4f), Status: Active in Group %d\n",
                    i, part, point[i][0], point[i][1], P[0].d[i][part]);
            }
        }
    }
    printf("\nOptimization Results: Sensor Time Allocation\n");
    printf("---------------------------------------------------\n");
    printf("Sensor\tPart\tAllocated Group\tTime (s)\n");
    printf("---------------------------------------------------\n");

    // ------------------- 以下为修改后的输出格式 -------------------
// 输出最终子覆盖集合及总运行时间，与图片类似的格式
    printf("\n子覆盖集合 (子集数 = %d):\n", pbest.csetN);
    for (int subset = 0; subset < pbest.csetN; subset++) {
        int sensorCount = 0;
        // 先统计每个子集有多少传感器
        for (int sensor = 0; sensor < NODEN; sensor++) {
            parts = (int)sensorEnergy[sensor];
            for (int part = 0; part < parts; part++) {
                if (pbest.d[sensor][part] == subset) {
                    sensorCount++;
                    break;
                }
            }
        }

        printf("子集%d (", subset + 1);
        int printedCount = 0;
        for (int sensor = 0; sensor < NODEN; sensor++) {
            parts = (int)sensorEnergy[sensor];
            int inThisSubset = 0;
            for (int part = 0; part < parts; part++) {
                if (pbest.d[sensor][part] == subset) {
                    inThisSubset = 1;
                    break;
                }
            }
            if (inThisSubset) {
                if (printedCount > 0) printf(", ");
                printf("s_%d", sensor + 1);
                printedCount++;
            }
        }
        printf(")包含 %d 个传感器\n", sensorCount);
    }

    printf("\n总的生命周期 (子集数): %d\n", pbest.csetN);
    printf("Elapsed time: %.2f ms\n", total_time * 1000.0);

    // ------------------- 修改结束 -------------------

    }
    else {
        printf("The deployment is a failure\n");
    }

    fclose(ftotal);
    fclose(finst);
    fclose(favgtotal);
    fclose(fcrate);

    return 0;
}
