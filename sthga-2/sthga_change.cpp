//#define _CRT_SECURE_NO_WARNINGS
//
//#include "stdafx.h"
//#include <stdio.h>
//#include <math.h>
//#include <time.h>
//#include <stdlib.h>
//#include <string.h>
//#include <conio.h>
//#include <direct.h>
//
//#define NODEN 1000       // 最大传感器节点数
//#define TRIALNUM 30      // 测试次数
//#define FMAX 10000       // 可能分配的场地索引的最大值
//#define UPAPMIN 800      // 覆盖最小场地的次数的最大值
//#define CRIMAX 800       // 覆盖最少场地的最大数量
//#define GRID_C 600       // 网格列数
//#define GRID_D 600       // 网格行数
//#define PI  3.1415926535897932384626
//#define POPSIZE 3        // 遗传算法中个体数量，即每一代的种群规模
//#define GRID_LEVEL 4     // 每个传感器圆形区域的半径划分数量
//const int CMAX = GRID_LEVEL * GRID_LEVEL * 4;  // 圆形区域的最大网格数
//double desiredFitness = 10; // 示例值
//
//
//int K1 = 5;                   //用于forwSche()的参数
//int K2 = 5;                   //用于mixSche()的参数
//int Gm = 20;                 //逆向变异的执行代数间隔
//double MUI = 0.5;             //突变率
//double R = 4.0;               //每个传感器的覆盖半径
//double energy[NODEN]; // 记录每个传感器的初始能量值
//int activation_time[NODEN][UPAPMIN]; // 记录每个传感器在每个集合中激活的时间
//int sensor_parts[NODEN]; // 记录每个传感器的能量被划分为多少个部分
//bool part_used[NODEN][100]; // 记录每个传感器的每个部分是否已经被使用
//bool active[NODEN]; // 标记传感器是否仍可用
//
//double L = 50.0;              //区域长度
//double W = 50.0;              //区域宽度
//int tmp[UPAPMIN];             //关键场地的调度序列号传感器的覆盖次数
//int nos[UPAPMIN][UPAPMIN];    //nos[场地调度编号][由相同调度传感器覆盖的场地次数] = 传感器索引
//int N = NODEN;                //实例中的传感器节点数量
//int ino;                      //实例编号
//int OPos[NODEN][CMAX + 1][2]; //记录传感器j的覆盖的网格
//int GRID_L;                   //x轴上的网格数量
//int GRID_W;                   //y轴上的网格数量
//int trialNum;                 //记录测试的次数
//
//double stepL;                 // =L/GRID_L
//double stepW;                 // =W/GRID_W
//double point[NODEN][2];       //传感器的位置坐标（从文件中读取）， //替换为3D坐标
//double yita;                  //冗余率
//
//int fieldNum;                 //场地数量
//int OPosf[NODEN][CMAX + 1];   //记录每个传感器覆盖的场地
//int field[FMAX];              //记录场地的覆盖次数
//int grid[GRID_C][GRID_D];     //暂时记录每个网格的场地索引
//int fieldIndex[GRID_C][GRID_D];//记录每个网格的场地索引
//
//int G;                        //代数索引
//int apmin;                    //完整覆盖集的数量上限（子集数量）
//int countEvals;               //记录评估的次数
//clock_t tp1, tp2;            //时钟类型clock_t
//
//char filename[100], filename2[100], filename3[100], filename4[100], filename5[100], ch[30];
//char filename6[30];           //自定义文件名
//FILE* ftotal, * finst, * favgtotal;
//FILE* fcrate;                 //自定义文件输出流，输出中间结果 
//int evalStep = 10;
//int recordCount;              //绘图记录结果的数量
//int maxRecordCount;           //记录所有试验中的最大记录值
////以下数组的大小必须大于 "MAXEVALS/evalStep"
//double evalValueAvg[100000];  //计算中间结果的平均值 
//
//// 定义种群结构
//struct Population {
//    int d[NODEN];             //每个基因的调度序列号，从1到apmin
//    int csetN;                //完整覆盖集的数量
//    double fitness;           //适应度 = csetN + 不完整集的覆盖百分比（即csetN+1集）
//} P[POPSIZE * 2], pbest;       //每个种群有POPSIZE个染色体，最后POPSIZE个染色体记录新种群
//int NP2 = POPSIZE * 2;
//
//// 定义节点结构
//struct node {
//    int no;
//    node* next;
//};
//int cricount;                   //记录关键场地的数量
//node criti[CRIMAX];            //记录覆盖相应关键场地的传感器
//
////--------------------------------------------------------------------------
//// 生成[0,1)范围内的均匀随机数
//double rand01() {
//    return (double)rand() / (double)(RAND_MAX + 1);
//}
//
//// 检查(i, j)网格是否被第k个传感器覆盖
///*
//检查给定网格 (i, j) 是否被传感器 k 覆盖。
//判断覆盖关系时，考虑了网格四个角的坐标和传感器位置之间的距离。
//*/
//int iscovered(int i, int j, int k) {
//    double x = i * stepL;
//    double y = j * stepW;
//    if ((x - point[k][0]) * (x - point[k][0]) + (y - point[k][1]) * (y - point[k][1]) > R * R ||
//        (x + stepL - point[k][0]) * (x + stepL - point[k][0]) + (y - point[k][1]) * (y - point[k][1]) > R * R ||
//        (x - point[k][0]) * (x - point[k][0]) + (y + stepW - point[k][1]) * (y + stepW - point[k][1]) > R * R ||
//        (x + stepL - point[k][0]) * (x + stepL - point[k][0]) + (y + stepW - point[k][1]) * (y + stepW - point[k][1]) > R * R) {
//        return 0; // 未被覆盖
//    }
//    return 1; // 已被覆盖
//}
//
//void init_grid() {
//    int i, j, k;
//    double step = R / GRID_LEVEL;
//
//    // x轴和y轴上的网格数量
//    GRID_L = (int)(L / step);
//    GRID_W = (int)(W / step);
//
//    int i0, j0;
//    stepL = L / GRID_L;    //每个网格的长度
//    stepW = W / GRID_W;
//
//    // 记录每个传感器k覆盖的网格在OPos[k][][]中的信息
//    for (k = 0; k < NODEN; k++) {
//        OPos[k][CMAX][0] = 0;
//        if (point[k][0] - R < 0) i0 = 0;
//        else i0 = (int)((point[k][0] - R) / stepL);
//        if (point[k][1] - R < 0) j0 = 0;
//        else j0 = (int)((point[k][1] - R) / stepW);
//        for (i = i0; i < i0 + GRID_LEVEL * 2 && i < GRID_L; i++) {
//            for (j = j0; j < j0 + GRID_LEVEL * 2 && j < GRID_W; j++) {
//                if (iscovered(i, j, k)) {
//                    OPos[k][OPos[k][CMAX][0]][0] = i;
//                    OPos[k][OPos[k][CMAX][0]][1] = j;
//                    OPos[k][CMAX][0]++;
//                }
//            }
//        }
//    }
//}
//
//
//// 确定传感器在目标区域中形成的场地
///*
//目标：确定传感器在目标区域中覆盖的场地，并将其编号。
//
//初始化：
//    fh[i] = -1：用于标识每个旧场地的初始状态。
//    grid[i][j] = 0：初始化网格，使得所有网格最初都属于场地 0。
//
//更新场地编号：
//    对于每个传感器 h，通过 OPos 获取其覆盖的所有网格 (i, j)，如果网格是新场地，则将其编号更新为新场地索引 count。
//    重置后再次更新场地编号，保证没有重复的索引。
//
//记录场地覆盖信息：
//    OPosf[h][CMAX] 记录传感器 h 能够覆盖的场地索引，用于后续计算覆盖百分比。
//*/
//void calc_field() {
//    int i, j, k, h;
//    int fh[FMAX];                          //fh[i]记录旧场地索引i的新场地索引
//    int count = 1;                         //场地索引，初始只有1个场地
//    fieldNum = 0;
//
//    for (i = 0; i < FMAX; i++) {
//        fh[i] = -1;                        //初始时每个场地i在传感器范围内未被找到
//    }
//    for (i = 0; i < GRID_L; i++) {
//        for (j = 0; j < GRID_W; j++) {
//            grid[i][j] = 0;                //所有网格初始都属于场地0
//        }
//    }
//
//    // 遍历每个传感器h的所有网格
//    for (h = 0; h < N; h++) {
//        count = fieldNum + 1;
//        for (k = 0; k < OPos[h][CMAX][0]; k++) { // 遍历传感器h覆盖的所有网格
//            i = OPos[h][k][0];                //x坐标
//            j = OPos[h][k][1];                //y坐标
//            if (fh[grid[i][j]] == -1) {       //如果在传感器h的范围内找到新场地
//                fh[grid[i][j]] = count;      //记录新场地索引
//                grid[i][j] = count;          //更新场地索引为新场地
//                count++;                     //场地索引增加1
//                if (count > FMAX) {
//                    printf("FMAX is not big enough\n");
//                    exit(0);
//                }
//            }
//            else grid[i][j] = fh[grid[i][j]]; //场地类型已找到，直接更新场地索引
//        }
//        for (k = 0; k < FMAX; k++) {
//            fh[k] = -1; // 重置传感器范围内未找到的场地
//        }
//        fieldNum = 0;
//        for (i = 0; i < GRID_L; i++) {
//            for (j = 0; j < GRID_W; j++) {
//                if (fh[grid[i][j]] == -1) {
//                    fh[grid[i][j]] = fieldNum; //记录新场地索引
//                    fieldNum++;                //场地索引增加1
//                }
//                fieldIndex[i][j] = fh[grid[i][j]]; //记录(i,j)的场地索引
//            }
//        }
//        for (i = 0; i < GRID_L; i++) {
//            for (j = 0; j < GRID_W; j++) {
//                grid[i][j] = fieldIndex[i][j];
//            }
//        }
//        for (k = 0; k < FMAX; k++) {
//            fh[k] = -1; // 重置传感器范围内未找到的场地
//        }
//    }
//
//    fieldNum = 0;
//    for (i = 0; i < GRID_L; i++) {
//        for (j = 0; j < GRID_W; j++) {
//            if (fh[grid[i][j]] == -1) {
//                fh[grid[i][j]] = fieldNum; //记录新场地索引
//                fieldNum++;                //场地索引增加1
//            }
//            fieldIndex[i][j] = fh[grid[i][j]]; //记录(i,j)的场地索引
//        }
//    }
//
//    // OPosf[i][]记录传感器i覆盖的场地索引
//    for (i = 0; i < fieldNum; i++) fh[i] = -1;
//    for (h = 0; h < N; h++) {
//        OPosf[h][CMAX] = 0;
//        for (k = 0; k < OPos[h][CMAX][0]; k++) {
//            i = OPos[h][k][0];
//            j = OPos[h][k][1];
//            if (fh[fieldIndex[i][j]] == -1) {
//                fh[fieldIndex[i][j]] = 0;
//                OPosf[h][OPosf[h][CMAX]] = fieldIndex[i][j];
//                OPosf[h][CMAX]++;
//            }
//        }
//        for (k = 0; k < FMAX; k++) fh[k] = -1;
//    }
//}
//
//// 计算指定调度索引的集的覆盖百分比
///*
//目标：计算指定调度索引 scheIndex 对应的覆盖集的覆盖百分比。
//
//步骤：
//    遍历所有传感器节点 k，如果传感器的调度 P[id].d[k] 等于当前要计算的集合 scheIndex，
//    则将它所覆盖的场地进行标记，表示这些场地已经被覆盖。
//    遍历所有场地 field[i]，统计被标记为覆盖的场地数量（count），然后将 field[i] 重置为未覆盖状态。
//    计算覆盖的百分比为 count / fieldNum，即被覆盖场地的数量与总场地数量的比值。
//
//用途：这个函数用于评估调度方案的覆盖性能，即查看当前传感器调度状态下能覆盖目标区域的百分比。
//*/
//double eval_cover(int scheIndex, int id) {
//    int i, j, k;
//    int count = 0;
//
//    for (k = 0; k < N; k++) {
//        if (P[id].d[k] == scheIndex) {
//            for (j = 0; j < OPosf[k][CMAX]; j++) {
//                field[OPosf[k][j]] = 1; //场地已覆盖
//            }
//        }
//    }
//
//    for (i = 0; i < fieldNum; i++) {
//        if (field[i] == 1) {
//            count++;
//            field[i] = 0; //重置场地状态
//        }
//    }
//
//    return (double)count / (double)(fieldNum);
//}
//
//// 在不同集合之间移动冗余传感器
///*
//目标：通过在不同集合之间移动冗余传感器来优化整体的覆盖效果。
//步骤：
//    随机选择 K2 次传感器进行调度。
//    如果传感器 tmp 当前在完整覆盖集合中（即它并不属于最后的不完整覆盖集合 csetN），则尝试将其调度到不完整集合。
//    如果将传感器移除后原来的集合依旧能够达到完整覆盖，则说明该传感器是冗余的，这时可以将它调度到其他集合中。
//    如果传感器在不完整集合中，则尝试将它移入一个完整集合，以增加覆盖的有效性，但不能降低原有集合的覆盖效果。
//*/
//void mixSche(int id) {
//    int i, j, k;
//    double v;
//    int tmp;
//    for (i = 0; i < K2; i++) {
//        tmp = (int)(rand01() * N); // 随机选择一个传感器
//        k = P[id].d[tmp];
//        if (energy[tmp] <= 0) {
//            continue; // 如果传感器没有能量，跳过该传感器
//        }
//        if (k != P[id].csetN && energy[tmp] > 0) { // 如果传感器在完整覆盖集合中
//            P[id].d[tmp] = P[id].csetN; // 将传感器调度出该集合
//            if (eval_cover(k, id) == 1.0) { // 如果传感器对集合是冗余的
//                j = (int)(rand01() * P[id].csetN); // 随机选择一个完整集合
//                if (j == k) j = P[id].csetN; // 放入不完整集合
//                P[id].d[tmp] = j; // 将传感器调度到所选的完整集合
//                // 更新激活时间
//                activation_time[tmp][j] += 10; // 示例：假设激活时间增加 10 分钟
//                energy[tmp] -= 1; // 调度后消耗部分能量
//                if (energy[tmp] < 0) energy[tmp] = 0; // 确保能量不会变为负值
//            }
//            else {
//                P[id].d[tmp] = k; // 传感器不是冗余的，不改变
//            }
//        }
//        else {
//            // 处理传感器在不完整集合中的情况
//            v = eval_cover(k, id);
//            P[id].d[tmp] = (int)(rand01() * P[id].csetN); // 将传感器调度到随机选定的完整覆盖集合
//            if (eval_cover(k, id) < v) {
//                P[id].d[tmp] = P[id].csetN; // 不改变非冗余传感器
//            }
//            else {
//                activation_time[tmp][P[id].csetN] += 10; // 在新集合中增加激活时间
//                energy[tmp] -= 1; // 调度后消耗部分能量
//                if (energy[tmp] < 0) energy[tmp] = 0; // 确保能量不会变为负值
//            }
//        }
//    }
//}
//
//// 将冗余传感器调度到csetN调度
///*
//目标：尝试将冗余传感器从完整集合调度到不完整集合，以优化覆盖。
//步骤：
//    随机选择 K1 个传感器，如果传感器不在最后一个集合 csetN 中，则将其调度到 csetN 集合中。
//    调度后，评估是否破坏了原有的完整覆盖。如果原集合不能再保持完全覆盖，则将传感器移回原集合。
//*/
//void forwSche(int id) {
//    int i, k;
//    int tmp;
//    for (i = 0; i < K1; i++) {
//        tmp = (int)(rand01() * N); // 随机选择一个传感器
//        if (energy[tmp] <= 0) {
//            continue; // 如果传感器没有能量，跳过该传感器
//        }
//        k = P[id].d[tmp];
//        while (k == P[id].csetN) { // 如果选定的传感器在最后一个集合中，重新选择
//            tmp = (int)(rand01() * N);
//            k = P[id].d[tmp];
//        }
//        P[id].d[tmp] = P[id].csetN;
//        if (eval_cover(k, id) != 1.0) { // 如果移除传感器会导致集合不再完整覆盖，不移动传感器
//            P[id].d[tmp] = k;
//        }
//        else {
//            energy[tmp] -= 1; // 调度后消耗部分能量
//            if (energy[tmp] < 0) energy[tmp] = 0; // 确保能量不会变为负值
//        }
//    }
//}
//
//// 将覆盖关键场地的冗余传感器移到不完整集合中
///*
//目标：将覆盖关键场地的冗余传感器从完整集合调度到不完整集合，以确保资源的有效利用。
//
//步骤：
//    对于每个关键场地 i，检查它是否由冗余传感器覆盖。
//    如果某关键场地 i 没有被不完整集合覆盖，则检查完整集合中是否有冗余传感器可以调度到不完整集合，以优化覆盖。
//    若找到冗余传感器，则将其移到不完整集合中。
//
//这些函数的核心目的是优化无线传感器网络的覆盖方案，通过合理调度冗余传感器，以最小化资源浪费并确保最大程度的覆盖。
//*/
//void critSche(int id) {
//    int i, j, k;
//    int index;
//    bool flag;
//    node* pt;
//
//    for (i = 0; i < cricount; i++) { // 检查每个关键场地是否有冗余传感器覆盖
//        for (j = 0; j <= apmin; j++) tmp[j] = 0;
//        pt = &criti[i];
//        while (pt->next != NULL) { // 检查覆盖关键场地的所有传感器
//            pt = pt->next;
//            nos[P[id].d[pt->no]][tmp[P[id].d[pt->no]]] = pt->no; // 记录相同调度的传感器索引
//            tmp[P[id].d[pt->no]]++; // 覆盖次数加1
//        }
//        if (tmp[P[id].csetN] == 0) { // 如果关键场地在不完整集合中未被覆盖
//            j = 0;
//            flag = false;
//            while (j < P[id].csetN && flag == false) { // 检查所有完整集合的冗余传感器
//                if (tmp[j] >= 2) { // 如果关键场地由至少2个传感器覆盖
//                    for (index = 0; index < tmp[j]; index++) { // 检查覆盖关键场地的传感器
//                        k = nos[j][index];
//                        if (energy[k] <= 0) {
//                            continue; // 如果传感器没有能量，跳过该传感器
//                        }
//                        P[id].d[k] = -1; // 检查该传感器是否是冗余传感器
//                        if (eval_cover(j, id) != 1.0) { // 如果传感器不是冗余的
//                            P[id].d[k] = j; // 不改变
//                        }
//                        else {
//                            P[id].d[k] = P[id].csetN; // 将冗余传感器调度到不完整覆盖集合
//                            energy[k] -= 1; // 调度后消耗部分能量
//                            if (energy[k] < 0) energy[k] = 0; // 确保能量不会变为负值
//                            flag = true; // 找到了满足条件的传感器
//                            break; // 完成对关键场地的覆盖
//                        }
//                    }
//                }
//                j++;
//            }
//        }
//    }
//}
//
//
//// STHGA的一个世代过程
//void GAoptimize() {
//    int i, j, k, a1, a2;
//    double v;                         //覆盖百分比
//    double bv;
//
//    // 交叉与选择
//    /*
//    这个循环从 POPSIZE 开始到 NP2，其中 POPSIZE 表示原始种群的大小，
//    而 NP2 可能是原始种群加上新个体后的大小。
//    随机选择两个父代个体 (a1 和 a2)，while 循环确保两个父代个体不相同。
//    */
//    for (i = POPSIZE; i < NP2; i++) {
//        a1 = (int)(rand01() * POPSIZE); // 随机选择两个染色体
//        a2 = (int)(rand01() * POPSIZE);
//        while (a2 == a1) a2 = (int)(rand01() * POPSIZE);
//
//        for (j = 0; j < N; j++) { // 均匀交叉
//            int part_idx = rand() % sensor_parts[j]; // 随机选择传感器的某个部分
//            if (!part_used[j][part_idx] && energy[j] > 0) {
//                if (rand01() < 0.5) {
//                    P[i].d[j] = P[a1].d[j];
//                }
//                else {
//                    P[i].d[j] = P[a2].d[j];
//                }
//                part_used[j][part_idx] = true; // 标记该部分已使用
//            }
//        }
//
//        // 评估新个体
//        for (k = 0; k <= apmin; k++) {
//            v = eval_cover(k, i);
//            if (v != 1.0) {
//                P[i].csetN = k;
//                P[i].fitness = k + v;
//                break;
//            }
//        }
//
//        countEvals++;
//        if (countEvals % evalStep == 1) {
//            evalValueAvg[recordCount] += pbest.fitness;
//            recordCount++;
//        }
//
//        // 替换劣质个体
//        if (P[a1].fitness < P[a2].fitness) a1 = a2;
//        if (P[i].fitness < P[a1].fitness) {
//            P[i].csetN = P[a1].csetN;
//            for (j = 0; j < N; j++) {
//                P[i].d[j] = P[a1].d[j];
//            }
//            P[i].fitness = P[a1].fitness;
//        }
//        else {
//            if (pbest.fitness < P[i].fitness) {
//                pbest.fitness = P[i].fitness;
//                pbest.csetN = P[i].csetN;
//                for (k = 0; k < N; k++) pbest.d[k] = P[i].d[k];
//            }
//        }
//    }
//
//    // 在 GAoptimize 中增加重新初始化部分种群的逻辑
//    if (G % 5 == 0) {
//        for (int i = POPSIZE / 2; i < NP2; i++) {
//            // 重新初始化部分种群
//            for (int j = 0; j < N; j++) {
//                P[i].d[j] = rand() % (P[i].csetN + 1);
//            }
//            // 重新评估适应度
//            double v = eval_cover(P[i].csetN, i);
//            P[i].fitness = P[i].csetN + v;
//        }
//    }
//
//    // 突变
//    /*
//    每经过 Gm 代后，执行一次突变操作。
//    找到适应度最高的个体，针对这些个体进行突变。
//    根据突变率 MUI，随机修改染色体中某些基因的值，重新计算适应度。
//    */
//    if (G % Gm == 0) { // 每 Gm 代执行一次突变
//        double originalMUI = MUI;
//        MUI = 0.3; // 临时增加突变率以打破局部最优
//        for (i = POPSIZE; i < NP2; i++) {
//            bv = 0; // 找出种群中的最佳适应值
//            for (int idx = 0; idx < POPSIZE; idx++) {
//                if (P[idx].fitness > bv) bv = P[idx].fitness;
//            }
//
//            if (P[i].fitness == bv) { // 对具有最佳适应值的染色体进行突变
//                for (j = 0; j < N; j++) {
//                    if (P[i].d[j] == P[i].csetN && rand01() < MUI && energy[j] > 0) { // 确保传感器能量足够
//                        P[i].d[j] = (int)(rand01() * (P[i].csetN));
//                        energy[j] -= 1; // 消耗能量
//                        if (energy[j] <= 0) {
//                            active[j] = false; // 能量耗尽，设置为不活跃
//                        }
//                    }
//                }
//                // 重新评估适应度
//                v = eval_cover(P[i].csetN, i);
//                P[i].fitness = P[i].csetN + v;
//
//                countEvals++;
//                if (countEvals % evalStep == 1) {
//                    evalValueAvg[recordCount] += pbest.fitness;
//                    recordCount++;
//                }
//            }
//        }
//        MUI = originalMUI; // 恢复原来的突变率
//    }
//
//    // 移动冗余传感器
//    /*
//    这部分代码将冗余传感器在不同的集合之间进行移动，以优化整体覆盖。
//    mixSche(i)：在不同集合之间移动冗余传感器。
//    forwSche(i)：将冗余传感器调度到特定集合（例如 csetN）。
//    critSche(i)：将覆盖关键区域的冗余传感器移动到未完全覆盖的集合中。
//    */
//    for (i = POPSIZE; i < NP2; i++) mixSche(i); //在不同集合之间移动冗余传感器
//    for (i = POPSIZE; i < NP2; i++) forwSche(i); //将冗余传感器调度到csetN调度
//    for (i = POPSIZE; i < NP2; i++) critSche(i); //将覆盖关键场地的冗余传感器移到不完整集合中
//
//    // 评估新种群
//    /*
//    对新生成的种群进行评估，更新适应度。
//    如果覆盖率达到 1.0，则增加完整覆盖集的数量 (csetN)。
//    更新最佳个体 (pbest)，如果发现了更好的适应度。
//    */
//    for (i = POPSIZE; i < NP2; i++) { //评估新种群
//        v = eval_cover(P[i].csetN, i); //计算不完整覆盖集的覆盖百分比
//        P[i].fitness = P[i].csetN + v; //完整覆盖集数量 + 最后一集的百分比
//        if (v == 1.0) // 集合已成为完整覆盖集
//            P[i].csetN++;
//        countEvals++;
//        if (countEvals % evalStep == 1) {
//            evalValueAvg[recordCount] += pbest.fitness;
//            recordCount++;
//        }
//        if (P[i].fitness > pbest.fitness) {
//            pbest.fitness = P[i].fitness;
//            pbest.csetN = P[i].csetN;
//            for (k = 0; k < N; k++) pbest.d[k] = P[i].d[k];
//        }
//    }
//
//    // 新种群替换旧种群
//    /*
//    新生成的种群替换旧种群，以确保下一代由新个体组成。
//    */
//    for (i = 0; i < POPSIZE; i++) {
//        P[i].csetN = P[POPSIZE + i].csetN;
//        P[i].fitness = P[POPSIZE + i].fitness;
//        for (j = 0; j < N; j++) {
//            P[i].d[j] = P[POPSIZE + i].d[j];
//        }
//    }
//}
//
//
//// 初始化种群
//void init() {
//    int i, k;
//    double v;
//
//    // 初始化传感器的可用状态
//    for (int i = 0; i < N; i++) {
//        active[i] = true; // 初始化为所有传感器可用
//    }
//
//    for (k = 0; k < POPSIZE; k++) {
//        for (i = 0; i < N; i++) {
//            P[k].d[i] = 0; // 调度序列号初始化为0
//        }
//        P[k].csetN = 1; // 初始时所有传感器能够完全覆盖目标
//        P[k].fitness = 0;
//    }
//    pbest.fitness = 0;
//
//    // 初始化激活时间
//    for (int i = 0; i < NODEN; i++) {
//        for (int j = 0; j < FMAX; j++) {
//            activation_time[i][j] = 0; // 初始激活时间为0分钟
//        }
//    }
//
//    // 初始化每个传感器的部分使用状态
//    for (i = 0; i < N; i++) {
//        for (int j = 0; j < sensor_parts[i]; j++) {
//            part_used[i][j] = false; // 每个部分都初始化为未被使用
//        }
//    }
//
//    // 初始化并检查传感器的能量是否充足
//    for (i = 0; i < N; i++) {
//        if (energy[i] <= 0) {
//            active[i] = false; // 如果能量不足，则设置为不可用
//        }
//    }
//
//    for (i = 0; i < POPSIZE; i++) forwSche(i); // 移动冗余传感器形成新的覆盖集
//
//    // 评估第一个个体并更新最优个体
//    v = eval_cover(1, 0); // 计算基因1的覆盖百分比
//    if (v == 1.0) // 集合已成为完整覆盖集
//        P[0].csetN++;
//    P[0].fitness = 1 + v;
//    pbest.csetN = P[0].csetN; // 更新最佳解
//    pbest.fitness = P[0].fitness;
//    for (i = 0; i < N; i++) pbest.d[i] = P[0].d[i];
//    countEvals++;
//    if (countEvals % evalStep == 1) {
//        evalValueAvg[recordCount] += pbest.fitness;
//        recordCount++;
//    }
//
//    // 评估剩余个体并更新最优解
//    for (i = 1; i < POPSIZE; i++) {
//        v = eval_cover(1, i);
//        if (v == 1.0)
//            P[i].csetN++;
//        P[i].fitness = 1 + v;
//
//        countEvals++;
//        if (countEvals % evalStep == 1) {
//            evalValueAvg[recordCount] += pbest.fitness;
//            recordCount++;
//        }
//        if (P[i].fitness > pbest.fitness) {
//            pbest.fitness = P[i].fitness;
//            pbest.csetN = P[i].csetN;
//            for (k = 0; k < N; k++) pbest.d[k] = P[i].d[k];
//        }
//    }
//}
//
//// 记录覆盖关键场地的传感器
///*
//目标：计算并记录覆盖关键场地的传感器，这些场地是被覆盖次数最少的场地，也称为 "关键场地"。
//
//步骤：
//    遍历所有场地 i，如果场地的覆盖次数等于 apmin（即覆盖次数最少的场地），则该场地被标记为关键场地。
//    对于每个关键场地，遍历所有传感器 k，找到能够覆盖该场地的传感器，并且确保该传感器能量大于零。
//    使用链表 criti 来记录这些覆盖关键场地的传感器。链表用于动态地添加传感器，pt->next 表示链表中的下一个节点。
//    如果找到了足够多的传感器来覆盖该场地，则停止查找并标记链表的结束。
//
//用途：关键场地的传感器可能是冗余度较低的，因此在调度时需要特别注意这些场地，以保证它们始终能被覆盖，从而避免覆盖的薄弱区域。
//*/
//void calc_criti() {
//    int i, k, h;
//    int cc; //计数器，记录覆盖最少场地的传感器数量
//    node* pt;
//
//    cricount = 0;
//    for (i = 0; i < fieldNum; i++) {
//        if (field[i] == apmin) { // 找到覆盖次数最少的场地
//            cc = 0;
//            pt = &criti[cricount];
//            for (k = 0; k < N; k++) {
//                // 检查传感器是否具有能量以及是否可以覆盖场地 i
//                if (active[k] && energy[k] > 0) {
//                    for (h = 0; h < OPosf[k][CMAX]; h++) {
//                        if (OPosf[k][h] == i) { //如果场地 h 在传感器 k 中覆盖场地 i
//                            pt->next = new node;
//                            pt = pt->next;
//                            pt->no = k; // 记录传感器
//                            cc++;
//                            break;
//                        }
//                    }
//                }
//                if (cc == apmin) {
//                    break;
//                }
//            }
//            pt->next = NULL; // criti[count]->sensor1->sensor2->...->NULL，覆盖关键场地的传感器链表
//            cricount++;
//            if (cricount > CRIMAX) {
//                printf("CRIMAX is not big enough!\n");
//                exit(0);
//            }
//        }
//    }
//}
//
//// 判断传感器的部署是否能够实现完整覆盖，并计算完整覆盖集的上限
///*
//目标：检查传感器的部署是否可以实现完整覆盖，并计算出完整覆盖集的上限。
//
//步骤：
//    初始化 field 数组，用于记录每个场地的覆盖次数。
//    遍历所有传感器 k 和它们所覆盖的场地，统计每个场地被覆盖的次数，只统计能量大于零的传感器。
//    计算出最少被覆盖的次数 apmin。如果有任何场地没有被覆盖到，则返回 false，表示部署失败。
//    如果 apmin 超过了 UPAPMIN 的限制，则说明需要的覆盖次数过多，退出程序。
//    调用 calc_criti() 函数来记录覆盖关键场地的传感器。
//
//用途：这个函数主要用于判断当前部署是否能够覆盖所有目标区域，并找出覆盖最少的场地（apmin），以便后续进行优化和处理冗余传感器。
//*/
//bool deployOK() {
//    int i, j, k;
//    bool flag = true;
//
//    for (i = 0; i < fieldNum; i++) {
//        field[i] = 0;
//    }
//    for (k = 0; k < N; k++) {
//        for (j = 0; j < OPosf[k][CMAX]; j++) {
//            field[OPosf[k][j]] += 1; //计算每个场地的覆盖次数
//        }
//    }
//
//    apmin = field[0];
//    for (i = 1; i < fieldNum; i++) {
//        if (field[i] > 0) {
//            if (apmin > field[i])
//                apmin = field[i];
//        }
//        else { // 如果某个场地没有被任何传感器覆盖，部署失败
//            flag = false;
//            break;
//        }
//    }
//    if (apmin > UPAPMIN) {
//        printf("UPAPMIN is not big enough!\n");
//        exit(0);
//    }
//    calc_criti(); // 记录覆盖关键场地的传感器
//
//    for (i = 0; i < fieldNum; i++) {
//        field[i] = 0;
//    }
//    return flag;
//}
//
//
//// 读取实例数据
///*
//目标：从文件中读取传感器的位置信息。
//
//步骤：
//    文件路径 "input//1.txt" 是实例文件，保存了传感器的坐标。
//    打开文件后，通过 fscanf 读取传感器的二维坐标（point[i][0] 和 point[i][1]）。
//    读取结束后，关闭文件。
//
//用途：将传感器的初始位置输入到程序中，用于计算覆盖范围和优化部署。
//*/
//void readInst() {
//    strcpy(filename, "input/1_with_energy.txt");
//    FILE* fin = fopen(filename, "r");
//
//    if (fin == NULL) {
//        printf("Error: Unable to open input file %s\n", filename);
//        exit(1);
//    }
//
//    for (int i = 0; i < N; i++) {
//        if (fscanf(fin, "%lf %lf %lf", &point[i][0], &point[i][1], &energy[i]) != 3) {
//            printf("Error: Unable to read data for sensor %d from file %s\n", i, filename);
//            fclose(fin);
//            exit(1);
//        }
//        sensor_parts[i] = (int)energy[i]; // 按能量划分部分数量，例如能量为10，则划分为10个部分
//        if (sensor_parts[i] < 1) {
//            sensor_parts[i] = 1;  // 确保至少有一个部分
//        }
//    }
//    fclose(fin);
//
//    for (int i = 0; i < N; i++) {
//        printf("Sensor %d: Position (%lf, %lf), Energy: %lf, Parts: %d\n", i, point[i][0], point[i][1], energy[i], sensor_parts[i]);
//    }
//}
//
//// 负责初始化所有数据并执行基于混合遗传算法的覆盖优化过程。
//int main() {
//    int MAX_EVA; // 最大评估次数
//    srand((unsigned)time(0)); // 随机种子初始化
//
//    // 创建结果文件夹路径
//    strcpy(filename, "STHGA");
//    _mkdir(filename);
//
//    strcat(filename, "\\N");
//    _itoa(N, ch, 10); // 将整数 N 转换为字符串并存储在 ch 中
//    strcat(filename, ch); // 将节点数 N 追加到目录路径中
//    strcat(filename, "R");
//    _itoa((int)R, ch, 10); // 将覆盖半径 R 转换为字符串
//    strcat(filename, ch); // 将 R 追加到目录路径中
//    _mkdir(filename); // 创建目录 "STHGA\\N<R>"
//
//    // 设置文件路径用于保存结果
//    strcpy(filename2, filename);
//    strcat(filename2, "\\total.txt");
//    strcpy(filename3, filename);
//    strcat(filename3, "\\inst.txt");
//    strcpy(filename4, filename);
//    strcat(filename4, "\\avgEval.txt");
//    strcpy(filename6, filename); // mine
//    strcat(filename6, "\\crate.txt"); // mine - 记录中间结果
//
//    // 打开各个文件以追加模式写入
//    ftotal = fopen(filename2, "a");
//    if (ftotal == NULL) {
//        printf("Error: Unable to open output file %s\n", filename2);
//        exit(1);
//    }
//    fcrate = fopen(filename6, "a");
//    if (fcrate == NULL) {
//        printf("Error: Unable to open output file %s\n", filename6);
//        exit(1);
//    }
//
//    // 写入总文件的标题行
//    fprintf(ftotal, "N\tR\tfield\tyita\tUp\tTRIAL\tpop\tK1\tK2\tGm\tMUI\tbest\tmean\tworst\tevalFit\tok%%\tavgT(ms)\n");
//    fclose(ftotal);
//
//    // 记录成功实验数、最佳和最差覆盖数、时间等
//    int i, recordMAX;
//    int countOK = 0, gBest = 0, gWorst = N;
//    int countTime = 0, countResult = 0, avgEval = 0;
//
//    // 打开用于记录实验详细结果的文件
//    ftotal = fopen(filename2, "a");
//    finst = fopen(filename3, "a");
//    favgtotal = fopen(filename4, "a");
//    if (finst == NULL || favgtotal == NULL) {
//        printf("Error: Unable to open output files for instance or average evaluation data\n");
//        exit(1);
//    }
//    fprintf(finst, "no.\tgen\tevalFit\tresult\ttime(ms)\n");
//
//    // 设置最大评估次数 MAX_EVA
//    switch (NODEN) {
//    case 40: MAX_EVA = 15000; break;
//    case 60: MAX_EVA = 15000; break;
//    case 100: MAX_EVA = 15000; break;
//    case 140: MAX_EVA = 15000; break;
//    case 180: MAX_EVA = 15000; break;
//    case 200: MAX_EVA = 15000; break;
//    case 300: MAX_EVA = 15000; break;
//    case 10000: MAX_EVA = 30000; break;
//    default: MAX_EVA = 10000; // 默认值，以防止错误输入
//    }
//
//    // 初始化和读取数据
//    recordMAX = sizeof(evalValueAvg) / sizeof(evalValueAvg[0]);
//    for (i = 0; i < recordMAX; i++) evalValueAvg[i] = 0;
//
//    readInst(); // 读取实例数据，主要是传感器位置
//    init_grid(); // 初始化网格，计算每个传感器的覆盖区域
//    calc_field(); // 计算传感器在目标区域中的覆盖场地
//
//    // 检查部署和计算冗余率
//    printf("%deloyOK:d", deployOK());
//    printf("apmin:%d", apmin);
//    if (deployOK() == true && apmin > 1) {
//        if (apmin >= 800) {
//            printf("Error: The size of timeArray[][] is not big enough!\n");
//            exit(0);
//        }
//
//        // 计算冗余率
//        yita = N * PI * R * R / (double)apmin / L / W;
//        printf("Initial Deployment Success. Calculated Redundancy Rate: %lf\n", yita);
//
//        // 进行实验
//        for (trialNum = 0; trialNum < TRIALNUM; trialNum++) {
//            G = 0; // 代数索引初始化为 0
//            countEvals = 0; recordCount = 0;
//            init(); // 每次实验中重新初始化种群。
//            tp1 = clock(); // 开始计时
//
//            while (countEvals <= MAX_EVA) {
//                GAoptimize(); // 调用混合遗传算法，生成新的一代种群。
//                G++; // 代数增加 1
//
//                if ((G) % 20 == 0) {
//                    fprintf(fcrate, "%d\n", pbest.csetN); // 每 20 代记录一次最优解的覆盖集数量
//                }
//
//                if (pbest.csetN == apmin) {
//                    countOK++;
//                    break;
//                }
//            }
//
//            tp2 = clock(); // 结束计时
//            countTime += tp2 - tp1;
//
//            // 更新评估和实验结果
//            if (maxRecordCount < recordCount) {
//                for (i = maxRecordCount; i < recordCount; i++) {
//                    evalValueAvg[i] += pbest.csetN * trialNum;
//                }
//                maxRecordCount = recordCount;
//            }
//
//            // 更新最佳和最差覆盖数量
//            countResult += pbest.csetN;
//            if (gBest < pbest.csetN) gBest = pbest.csetN;
//            if (gWorst > pbest.csetN) gWorst = pbest.csetN;
//            avgEval += countEvals;
//
//            printf("Test %d:\tG=%d\tEvals=%d\tBest Coverage=%d\tTime=%dms\n", trialNum + 1, G, countEvals, pbest.csetN, tp2 - tp1);
//            fprintf(finst, "%d\t%d\t%d\t%d\t%d\n", trialNum + 1, G, countEvals, pbest.csetN, tp2 - tp1);
//        }
//
//        // 输出实验统计结果到文件和控制台
//        fprintf(ftotal, "%d\t%g\t%d\t%g\t%d\t%d\t%d\t%d\t%d\t%d\t%g\t%d\t%g\t%d\t%d\t%g%%\t%g\n", N, R, fieldNum, yita, apmin, TRIALNUM, POPSIZE, K1, K2, Gm, MUI, gBest, (double)countResult / TRIALNUM, gWorst, avgEval / TRIALNUM, (double)countOK / TRIALNUM * 100, (double)countTime / TRIALNUM);
//        printf("Summary: Field=%d, Up=%d, Best=%d, Mean=%g, Worst=%d, Avg Evals=%d, Success=%g%%, Avg Time=%gms\n", fieldNum, apmin, gBest, (double)countResult / TRIALNUM, gWorst, avgEval / TRIALNUM, (double)countOK / TRIALNUM * 100, (double)countTime / TRIALNUM);
//    }
//    else {
//        printf("The deployment is a failure\n");
//    }
//
//    fclose(ftotal);
//    fclose(finst);
//    fclose(favgtotal);
//    fclose(fcrate);
//
//    // 修改输出部分，改进激活时间的输出逻辑
//    for (int i = 0; i < N; i++) {
//        if (pbest.d[i] != 0) {
//            printf("Sensor %d: Position (%.4f, %.4f), Status: Active in Group %d\n", i, point[i][0], point[i][1], pbest.d[i]);
//
//            // 输出每个传感器在各个集合中的激活时间
//            bool hasActivation = false;
//            for (int j = 0; j < FMAX; j++) {
//                if (activation_time[i][j] > 0) {
//                    hasActivation = true;
//                    break;
//                }
//            }
//
//            if (hasActivation) {
//                printf("  Activation times for Sensor %d:\n", i);
//                for (int j = 0; j < FMAX; j++) {
//                    if (activation_time[i][j] > 0) {
//                        printf("1");
//                        printf("    Group %d: %d minutes\n", j, activation_time[i][j]);
//                    }
//                }
//            }
//        }
//        else {
//            printf("Sensor %d: Position (%.4f, %.4f), Status: Inactive\n", i, point[i][0], point[i][1]);
//        }
//    }
//
//    // 输出平均覆盖集大小
//    return 0;
//}