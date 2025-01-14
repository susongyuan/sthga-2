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
//#include <assert.h> // 用于断言
//
//#define NODEN 1000       // 最大传感器节点数
//#define TRIALNUM 30      // 测试次数
//#define FMAX 3000       // 可能分配的场地索引的最大值
//#define UPAPMIN 800      // 覆盖最小场地的次数的最大值
//#define CRIMAX 800       // 覆盖最少场地的最大数量
//#define GRID_C 600       // 网格列数
//#define GRID_D 600       // 网格行数
//#define PI  3.1415926535897932384626
//#define POPSIZE 3        // 遗传算法中个体数量，即每一代的种群规模
//#define GRID_LEVEL 4     // 每个传感器圆形区域的半径划分数量
//const int CMAX = GRID_LEVEL * GRID_LEVEL * 4;  // 圆形区域的最大网格数
//
//int K1 = 5;                   //用于forwSche()的参数
//int K2 = 5;                   //用于mixSche()的参数
//int Gm = 20;                 //逆向变异的执行代数间隔
//double MUI = 0.5;             //突变率
//double R = 4.0;               //每个传感器的覆盖半径
//double energy[NODEN]; // 记录每个传感器的初始能量值
//int activation_time[NODEN][UPAPMIN]; // 记录每个传感器在每个集合中激活的时间
//int sensor_parts[NODEN]; // 记录每个传感器的能量被划分为多少个部分
//int sensor_parts_tmp[NODEN]; // sensor_parts疑似出现内存or进程问题，被意外修改，tmp临时sensor_parts
//int Sensor_Subset_Num = 100;// 子集数量
//bool part_used[NODEN][100]; // 记录每个传感器的每个部分是否已经被使用
//bool active[NODEN];     // 标记传感器是否仍可用
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
//struct SensorPart {
//    int sensor_id; // 传感器的 ID
//    int part_id;   // 传感器能量部分的 ID
//};
//
//// 定义修改后的 Population 结构体
//struct Population {
//    SensorPart d[NODEN];      // 每个基因的调度序列，现在由传感器和其能量部分 ID 组成
//    int csetN;                // 完整覆盖集的数量
//    double fitness;           // 适应度 = csetN + 不完整集的覆盖百分比（即csetN+1集）
//}P[POPSIZE * 2], pbest;       //每个种群有POPSIZE个染色体，最后POPSIZE个染色体记录新种群
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
//// 生成[0,1)范围内的均匀随机数
//double rand01() {
//    return (double)rand() / (double)(RAND_MAX + 1);
//}
//
//// 检查(i, j)网格是否被第k个传感器覆盖
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
//// 初始化OPos[k][][]
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
//// 确定传感器在目标区域中形成的覆盖区域
//void calc_field() {
//    int i, j, k, h;
//    int fh[FMAX];                          //fh[i]记录旧区域索引i的新区域索引
//    int count = 1;                         //区域索引，初始只有1个区域
//    fieldNum = 0;
//
//    for (i = 0; i < FMAX; i++) {
//        fh[i] = -1;                        //初始时每个区域i在传感器范围内未被找到
//    }
//    for (i = 0; i < GRID_L; i++) {
//        for (j = 0; j < GRID_W; j++) {
//            grid[i][j] = 0;                //所有网格初始都属于区域0
//        }
//    }
//
//    // 遍历每个传感器h的所有网格
//    for (h = 0; h < N; h++) {
//        count = fieldNum + 1;
//        for (k = 0; k < OPos[h][CMAX][0]; k++) { // 遍历传感器h覆盖的所有网格
//            i = OPos[h][k][0];                //x坐标
//            j = OPos[h][k][1];                //y坐标
//            if (fh[grid[i][j]] == -1) {       //如果在传感器h的范围内找到新区域
//                fh[grid[i][j]] = count;      //记录新区域索引
//                grid[i][j] = count;          //更新区域索引为新区域
//                count++;                     //区域索引增加1
//                if (count > FMAX) {
//                    printf("count is %d,FMAX is not big enough\n", count);
//                    exit(0);
//                }
//            }
//            else grid[i][j] = fh[grid[i][j]]; //区域类型已找到，直接更新区域索引
//        }
//        for (k = 0; k < FMAX; k++) {
//            fh[k] = -1; // 重置传感器范围内未找到的区域
//        }
//        fieldNum = 0;
//        for (i = 0; i < GRID_L; i++) {
//            for (j = 0; j < GRID_W; j++) {
//                if (fh[grid[i][j]] == -1) {
//                    fh[grid[i][j]] = fieldNum; //记录新区域索引
//                    fieldNum++;                //区域索引增加1
//                }
//                fieldIndex[i][j] = fh[grid[i][j]]; //记录(i,j)的区域索引
//            }
//        }
//        for (i = 0; i < GRID_L; i++) {
//            for (j = 0; j < GRID_W; j++) {
//                grid[i][j] = fieldIndex[i][j];
//            }
//        }
//        for (k = 0; k < FMAX; k++) {
//            fh[k] = -1; // 重置传感器范围内未找到的区域
//        }
//    }
//
//    fieldNum = 0;
//    for (i = 0; i < GRID_L; i++) {
//        for (j = 0; j < GRID_W; j++) {
//            if (fh[grid[i][j]] == -1) {
//                fh[grid[i][j]] = fieldNum; //记录新区域索引
//                fieldNum++;                //区域索引增加1
//            }
//            fieldIndex[i][j] = fh[grid[i][j]]; //记录(i,j)的区域索引
//        }
//    }
//
//    // OPosf[i][]记录传感器i覆盖的区域索引
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
//double eval_cover(int scheIndex, int id) {
//    int i, j, k;
//    int count = 0;
//
//    // 遍历每个传感器的部分，检查它们是否属于指定的调度索引
//    for (k = 0; k < N; k++) {
//        SensorPart& sensor_part = P[id].d[k];  // 获取当前染色体中的传感器部分
//
//        // 判断当前传感器部分是否属于指定的调度索引
//        if (sensor_part.part_id == scheIndex) {
//            int sensor_id = sensor_part.sensor_id;
//
//            // 遍历当前传感器的覆盖区域，将其设置为已覆盖状态
//            for (j = 0; j < OPosf[sensor_id][CMAX]; j++) {
//                int covered_field = OPosf[sensor_id][j];
//                if (covered_field >= 0 && covered_field < FMAX) {
//                    field[covered_field] = 1; // 区域已覆盖
//                }
//            }
//        }
//    }
//
//    // 计算总共覆盖的区域数量
//    for (i = 0; i < fieldNum; i++) {
//        if (field[i] == 1) {
//            count++;
//            field[i] = 0; // 重置区域状态以便后续使用
//        }
//    }
//
//    // 计算覆盖百分比
//    return (double)count / (double)(fieldNum);
//}
//
//// 在不同集合之间移动冗余传感器
//void mixSche(int id) {
//    int i, tmp_sensor, part_id, j;
//    double v;
//
//    for (i = 0; i < K2; i++) {
//        // 随机选择一个传感器部分
//        tmp_sensor = (int)(rand01() * N);  // 随机选择一个传感器
//        part_id = (int)(rand01() * sensor_parts[tmp_sensor]);  // 随机选择该传感器的一个部分
//
//        // 获取该部分对应的传感器信息
//        SensorPart& sensor_part = P[id].d[tmp_sensor];
//        sensor_part.sensor_id = tmp_sensor;
//        sensor_part.part_id = part_id;
//
//        if (energy[tmp_sensor] <= 0) {
//            continue;  // 如果传感器没有能量，跳过该传感器
//        }
//
//        int current_set = P[id].d[tmp_sensor].part_id;  // 当前所在集合的部分
//        if (current_set != P[id].csetN && energy[tmp_sensor] > 0) {  // 如果传感器在完整覆盖集合中
//            P[id].d[tmp_sensor].part_id = P[id].csetN;  // 将传感器调度出该集合
//
//            // 检查传感器是否对集合是冗余的
//            if (eval_cover(current_set, id) == 1.0) {
//                j = (int)(rand01() * P[id].csetN);  // 随机选择一个完整集合
//                if (j == current_set) j = P[id].csetN;  // 如果选到当前集合，放入不完整集合
//
//                // 更新调度信息
//                P[id].d[tmp_sensor].part_id = j;  // 将传感器调度到所选的完整集合
//                activation_time[tmp_sensor][j] += 10;  // 示例：假设激活时间增加 10 分钟
//                energy[tmp_sensor] -= 1;  // 调度后消耗部分能量
//
//                if (energy[tmp_sensor] < 0) energy[tmp_sensor] = 0;  // 确保能量不会变为负值
//            }
//            else {
//                P[id].d[tmp_sensor].part_id = current_set;  // 传感器不是冗余的，不改变
//            }
//        }
//        else {
//            // 处理传感器在不完整集合中的情况
//            v = eval_cover(current_set, id);
//            P[id].d[tmp_sensor].part_id = (int)(rand01() * P[id].csetN);  // 将传感器调度到随机选定的完整覆盖集合
//
//            if (eval_cover(current_set, id) < v) {
//                P[id].d[tmp_sensor].part_id = P[id].csetN;  // 不改变非冗余传感器
//            }
//            else {
//                activation_time[tmp_sensor][P[id].csetN] += 10;  // 在新集合中增加激活时间
//                energy[tmp_sensor] -= 1;  // 调度后消耗部分能量
//
//                if (energy[tmp_sensor] < 0) energy[tmp_sensor] = 0;  // 确保能量不会变为负值
//            }
//        }
//    }
//}
//
//// 将冗余传感器调度到 csetN 调度
//void forwSche(int id) {
//    int i, tmp_sensor, part_id, original_set;
//    for (i = 0; i < K1; i++) {
//        // 随机选择一个传感器及其部分
//        tmp_sensor = (int)(rand01() * N);  // 随机选择一个传感器
//        part_id = (int)(rand01() * sensor_parts[tmp_sensor]);  // 随机选择该传感器的一个部分
//
//        SensorPart& sensor_part = P[id].d[tmp_sensor];
//        sensor_part.sensor_id = tmp_sensor;
//        sensor_part.part_id = part_id;
//
//        if (energy[tmp_sensor] <= 0) {
//            continue; // 如果传感器没有能量，跳过该传感器
//        }
//
//        // 获取该部分当前所属的集合
//        original_set = sensor_part.part_id;
//
//        // 如果选定的传感器部分已经在最后一个集合 csetN 中，则重新选择
//        while (original_set == P[id].csetN) {
//            tmp_sensor = (int)(rand01() * N);
//            part_id = (int)(rand01() * sensor_parts[tmp_sensor]);
//            sensor_part.sensor_id = tmp_sensor;
//            sensor_part.part_id = part_id;
//            original_set = sensor_part.part_id;
//        }
//
//        // 将传感器部分调度到集合 csetN
//        sensor_part.part_id = P[id].csetN;
//
//        // 评估移除传感器部分后的覆盖情况
//        if (eval_cover(original_set, id) != 1.0) {
//            // 如果移除传感器会导致集合不再完整覆盖，不移动传感器
//            sensor_part.part_id = original_set;
//        }
//        else {
//            // 如果成功调度，消耗能量
//            energy[tmp_sensor] -= 1;
//            if (energy[tmp_sensor] < 0) energy[tmp_sensor] = 0; // 确保能量不会变为负值
//        }
//    }
//}
//
//// 将覆盖关键场地的冗余传感器移到不完整集合中
//void critSche(int id) {
//    int i, j, k;
//    int index;
//    bool flag;
//    node* pt;
//
//    for (i = 0; i < cricount; i++) { // 检查每个关键场地是否有冗余传感器覆盖
//        for (j = 0; j <= apmin; j++) tmp[j] = 0;
//
//        pt = &criti[i];
//        while (pt->next != NULL) { // 检查覆盖关键场地的所有传感器
//            pt = pt->next;
//            SensorPart& sensor_part = P[id].d[pt->no];  // 获取传感器部分
//            int sensor_id = sensor_part.sensor_id;      // 获取传感器 ID
//            int part_id = sensor_part.part_id;          // 获取传感器部分 ID
//
//            nos[part_id][tmp[part_id]] = sensor_id; // 记录相同调度的传感器索引
//            tmp[part_id]++; // 增加该集合中覆盖的传感器部分数
//        }
//
//        if (tmp[P[id].csetN] == 0) { // 如果关键场地在不完整集合中未被覆盖
//            j = 0;
//            flag = false;
//            while (j < P[id].csetN && flag == false) { // 检查所有完整集合的冗余传感器部分
//                if (tmp[j] >= 2) { // 如果关键场地由至少2个传感器部分覆盖
//                    for (index = 0; index < tmp[j]; index++) { // 检查覆盖关键场地的传感器部分
//                        k = nos[j][index];
//                        SensorPart& sensor_part = P[id].d[k];
//                        int sensor_id = sensor_part.sensor_id;
//
//                        if (energy[sensor_id] <= 0) {
//                            continue; // 如果传感器没有能量，跳过该传感器
//                        }
//
//                        // 临时将传感器部分设置为不参与覆盖，检查是否为冗余
//                        int original_part = sensor_part.part_id;
//                        sensor_part.part_id = -1; // 表示暂时移除该部分
//
//                        // 如果移除导致集合不再覆盖完全，则不改变
//                        if (eval_cover(j, id) != 1.0) {
//                            sensor_part.part_id = original_part; // 恢复原始状态
//                        }
//                        else {
//                            // 成功移除冗余传感器部分并调度到不完整集合
//                            sensor_part.part_id = P[id].csetN; // 将冗余传感器部分调度到不完整集合
//                            energy[sensor_id] -= 1; // 调度后消耗部分能量
//                            if (energy[sensor_id] < 0) energy[sensor_id] = 0; // 确保能量不会变为负值
//                            flag = true; // 找到符合条件的传感器部分
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
////// STHGA的一个世代过程
////void GAoptimize() {
////    printf("Generation %d\n", G);
////    int i, j, k, a1, a2;
////    double v;                         //覆盖百分比
////    double bv;
////
////    // 交叉与选择
////    for (i = POPSIZE; i < NP2; i++) {
////        a1 = (int)(rand01() * POPSIZE); // 随机选择两个染色体
////        a2 = (int)(rand01() * POPSIZE);
////        while (a2 == a1) a2 = (int)(rand01() * POPSIZE);
////
////        for (j = 0; j < N; j++) { // 均匀交叉
////            int part_idx = rand() % sensor_parts[j]; // 随机选择传感器的某个部分
////
////            if (!part_used[j][part_idx] && energy[j] > 0) {
////                if (rand01() < 0.5) {
////                    P[i].d[j].sensor_id = P[a1].d[j].sensor_id;
////                    P[i].d[j].part_id = P[a1].d[j].part_id;
////                }
////                else {
////                    P[i].d[j].sensor_id = P[a2].d[j].sensor_id;
////                    P[i].d[j].part_id = P[a2].d[j].part_id;
////                }
////                part_used[j][part_idx] = true; // 标记该部分已使用
////            }
////        }
////
////        // 评估新个体
////        for (k = 0; k <= apmin; k++) {
////            v = eval_cover(k, i);
////            if (v != 1.0) {
////                P[i].csetN = k;
////                P[i].fitness = k + v;
////                break;
////            }
////        }
////
////        countEvals++;
////        if (countEvals % evalStep == 1) {
////            evalValueAvg[recordCount] += pbest.fitness;
////            recordCount++;
////        }
////
////        // 替换劣质个体
////        if (P[a1].fitness < P[a2].fitness) a1 = a2;
////        if (P[i].fitness < P[a1].fitness) {
////            P[i].csetN = P[a1].csetN;
////            for (j = 0; j < N; j++) {
////                P[i].d[j] = P[a1].d[j];
////            }
////            P[i].fitness = P[a1].fitness;
////        }
////        else {
////            if (pbest.fitness < P[i].fitness) {
////                pbest.fitness = P[i].fitness;
////                pbest.csetN = P[i].csetN;
////                for (k = 0; k < N; k++) pbest.d[k] = P[i].d[k];
////            }
////        }
////    }
////
////    // 在 GAoptimize 中增加重新初始化部分种群的逻辑
////    if (G % 5 == 0) {
////        for (int i = POPSIZE / 2; i < NP2; i++) {
////            // 重新初始化部分种群
////            for (int j = 0; j < N; j++) {
////                P[i].d[j].sensor_id = j;
////                P[i].d[j].part_id = rand() % sensor_parts[j]; // 重新随机选择传感器的部分
////            }
////            // 重新评估适应度
////            double v = eval_cover(P[i].csetN, i);
////            P[i].fitness = P[i].csetN + v;
////        }
////    }
////
////    // 突变
////    if (G % Gm == 0) { // 每 Gm 代执行一次突变
////        double originalMUI = MUI;
////        MUI = 0.3; // 临时增加突变率以打破局部最优
////        for (i = POPSIZE; i < NP2; i++) {
////            bv = 0; // 找出种群中的最佳适应值
////            for (int idx = 0; idx < POPSIZE; idx++) {
////                if (P[idx].fitness > bv) bv = P[idx].fitness;
////            }
////
////            if (P[i].fitness == bv) { // 对具有最佳适应值的染色体进行突变
////                for (j = 0; j < N; j++) {
////                    if (rand01() < MUI && energy[j] > 0) { // 确保传感器能量足够
////                        int new_part = rand() % sensor_parts[j];
////                        if (!part_used[j][new_part]) {
////                            P[i].d[j].part_id = new_part;
////                            energy[j] -= 1; // 消耗能量
////                            if (energy[j] <= 0) {
////                                active[j] = false; // 能量耗尽，设置为不活跃
////                            }
////                        }
////                    }
////                }
////                // 重新评估适应度
////                v = eval_cover(P[i].csetN, i);
////                P[i].fitness = P[i].csetN + v;
////
////                countEvals++;
////                if (countEvals % evalStep == 1) {
////                    evalValueAvg[recordCount] += pbest.fitness;
////                    recordCount++;
////                }
////            }
////        }
////        MUI = originalMUI; // 恢复原来的突变率
////    }
////
////    // 移动冗余传感器
////    for (i = POPSIZE; i < NP2; i++) mixSche(i); //在不同集合之间移动冗余传感器
////    for (i = POPSIZE; i < NP2; i++) forwSche(i); //将冗余传感器调度到csetN调度
////    for (i = POPSIZE; i < NP2; i++) critSche(i); //将覆盖关键场地的冗余传感器移到不完整集合中
////
////    // 评估新种群
////    for (i = POPSIZE; i < NP2; i++) { //评估新种群
////        v = eval_cover(P[i].csetN, i); //计算不完整覆盖集的覆盖百分比
////        P[i].fitness = P[i].csetN + v; //完整覆盖集数量 + 最后一集的百分比
////        if (v == 1.0) // 集合已成为完整覆盖集
////            P[i].csetN++;
////        countEvals++;
////        if (countEvals % evalStep == 1) {
////            evalValueAvg[recordCount] += pbest.fitness;
////            recordCount++;
////        }
////        if (P[i].fitness > pbest.fitness) {
////            pbest.fitness = P[i].fitness;
////            pbest.csetN = P[i].csetN;
////            for (k = 0; k < N; k++) pbest.d[k] = P[i].d[k];
////        }
////    }
////
////    // 新种群替换旧种群
////    for (i = 0; i < POPSIZE; i++) {
////        P[i].csetN = P[POPSIZE + i].csetN;
////        P[i].fitness = P[POPSIZE + i].fitness;
////        for (j = 0; j < N; j++) {
////            P[i].d[j] = P[POPSIZE + i].d[j];
////        }
////    }
////}
//
//// STHGA的一个世代过程
//void GAoptimize() {
//    printf("Generation %d\n", G); // 打印当前代数
//    int i, j, k, a1, a2;
//    double v;  // 覆盖百分比
//    double bv;
//
//    // 交叉与选择
//    for (i = POPSIZE; i < NP2; i++) {
//        a1 = (int)(rand01() * POPSIZE); // 随机选择两个染色体
//        a2 = (int)(rand01() * POPSIZE);
//        while (a2 == a1) a2 = (int)(rand01() * POPSIZE); // 保证a2不等于a1
//
//        for (j = 0; j < N; j++) { // 均匀交叉
//            printf("均匀交叉\n");
//            printf("j:%d, sensor_parts[j] :%d\n",j,sensor_parts[j]);
//
//            int part_idx = rand() % sensor_parts[j]; // 随机选择传感器的某个部分
//          
//            // 检查part_idx是否合法
//            if(j >= 0 && j < N)printf("j:%d\n", j); // 确保j在有效范围内
//            if(part_idx >= 0 && part_idx < sensor_parts[j])printf("part_idx:%d\n", part_idx); // 确保part_idx在有效范围内
//
//            if (!part_used[j][part_idx] && energy[j] > 0) {
//                if (rand01() < 0.5) {
//                    P[i].d[j].sensor_id = P[a1].d[j].sensor_id;
//                    P[i].d[j].part_id = P[a1].d[j].part_id;
//                }
//                else {
//                    P[i].d[j].sensor_id = P[a2].d[j].sensor_id;
//                    P[i].d[j].part_id = P[a2].d[j].part_id;
//                }
//                part_used[j][part_idx] = true; // 标记该部分已使用
//            }
//        }
//
//        // 评估新个体
//        for (k = 0; k <= apmin; k++) {
//            printf("eval_cover(k, i)\n");
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
//                P[i].d[j].sensor_id = j;
//                P[i].d[j].part_id = rand() % sensor_parts[j]; // 重新随机选择传感器的部分
//                // 检查part_idx是否合法
//                assert(j >= 0 && j < N); // 确保j在有效范围内
//                assert(P[i].d[j].part_id >= 0 && P[i].d[j].part_id < sensor_parts[j]); // 确保part_id合法
//            }
//            // 重新评估适应度
//            double v = eval_cover(P[i].csetN, i);
//            P[i].fitness = P[i].csetN + v;
//        }
//    }
//
//    // 突变
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
//                    // 检查传感器是否能量充足
//                    assert(energy[j] >= 0); // 确保能量不为负值
//                    if (rand01() < MUI && energy[j] > 0) { // 确保传感器能量足够
//                        int new_part = rand() % sensor_parts[j];
//                        if (!part_used[j][new_part]) {
//                            P[i].d[j].part_id = new_part;
//                            energy[j] -= 1; // 消耗能量
//                            if (energy[j] <= 0) {
//                                active[j] = false; // 能量耗尽，设置为不活跃
//                            }
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
//    for (i = POPSIZE; i < NP2; i++) mixSche(i); //在不同集合之间移动冗余传感器
//    for (i = POPSIZE; i < NP2; i++) forwSche(i); //将冗余传感器调度到csetN调度
//    for (i = POPSIZE; i < NP2; i++) critSche(i); //将覆盖关键场地的冗余传感器移到不完整集合中
//
//    // 评估新种群
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
//    for (i = 0; i < POPSIZE; i++) {
//        P[i].csetN = P[POPSIZE + i].csetN;
//        P[i].fitness = P[POPSIZE + i].fitness;
//        for (j = 0; j < N; j++) {
//            P[i].d[j] = P[POPSIZE + i].d[j];
//        }
//    }
//}
//
//// 初始化种群
//void init() {
//    int i, k;
//    double v;
//
//    // 初始化传感器的可用状态
//    for (i = 0; i < N; i++) {
//        active[i] = true; // 初始化为所有传感器可用
//    }
//
//    // 初始化种群
//    for (k = 0; k < POPSIZE; k++) {
//        for (i = 0; i < N; i++) {
//            P[k].d[i].sensor_id = i; // 设置传感器的 ID 为 i
//            P[k].d[i].part_id = rand() % sensor_parts[i]; // 随机选择一个能量部分
//        }
//        P[k].csetN = 1; // 初始时所有传感器能够完全覆盖目标
//        P[k].fitness = 0; // 初始适应度设为 0
//    }
//    pbest.fitness = 0; // 初始最优适应度为 0
//
//    // 初始化激活时间
//    for (i = 0; i < NODEN; i++) {
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
//    // 移动冗余传感器形成新的覆盖集
//    for (i = 0; i < POPSIZE; i++) {
//        forwSche(i);
//    }
//
//    // 评估第一个个体并更新最优个体
//    v = eval_cover(1, 0); // 计算第一个个体的覆盖百分比
//    if (v == 1.0) { // 集合已成为完整覆盖集
//        P[0].csetN++;
//    }
//    P[0].fitness = 1 + v;
//
//    // 更新全局最优解 pbest
//    pbest.csetN = P[0].csetN;
//    pbest.fitness = P[0].fitness;
//    for (i = 0; i < N; i++) {
//        pbest.d[i] = P[0].d[i];
//    }
//
//    countEvals++;
//    if (countEvals % evalStep == 1) {
//        evalValueAvg[recordCount] += pbest.fitness;
//        recordCount++;
//    }
//
//    // 评估剩余个体并更新最优解
//    for (i = 1; i < POPSIZE; i++) {
//        v = eval_cover(1, i);
//        if (v == 1.0) {
//            P[i].csetN++;
//        }
//        P[i].fitness = 1 + v;
//
//        countEvals++;
//        if (countEvals % evalStep == 1) {
//            evalValueAvg[recordCount] += pbest.fitness;
//            recordCount++;
//        }
//        if (P[i].fitness > pbest.fitness) {
//            // 更新最优个体
//            pbest.fitness = P[i].fitness;
//            pbest.csetN = P[i].csetN;
//            for (k = 0; k < N; k++) {
//                pbest.d[k] = P[i].d[k];
//            }
//        }
//    }
//}
//
//// 记录覆盖关键场地的传感器
//void calc_criti() {
//    int i, k, h;
//    int cc; // 计数器，记录覆盖最少场地的传感器数量
//    node* pt;
//
//    cricount = 0;
//
//    for (i = 0; i < fieldNum; i++) {
//        if (field[i] == apmin) { // 找到覆盖次数最少的场地
//            cc = 0;
//            pt = &criti[cricount];
//
//            // 遍历所有传感器
//            for (k = 0; k < N; k++) {
//                if (energy[k] > 0) { // 检查传感器是否可用且有剩余能量
//                    // 遍历传感器 k 的所有覆盖区域
//                    for (h = 0; h < OPosf[k][CMAX]; h++) {
//                        if (OPosf[k][h] == i) { // 如果传感器覆盖了场地 i
//                            pt->next = new node;
//                            pt = pt->next;
//                            pt->no = k; // 记录传感器编号
//                            cc++;
//                            break;
//                        }
//                    }
//                }
//
//                if (cc == apmin) { // 如果找到足够多的传感器覆盖该场地，停止查找
//                    break;
//                }
//            }
//
//            pt->next = NULL; // 结束链表
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
//bool deployOK() {
//    int i, j, k;
//    bool flag = true;
//
//    // 初始化 field 数组，用于记录每个场地的覆盖次数
//    for (i = 0; i < fieldNum; i++) {
//        field[i] = 0;
//    }
//
//    // 遍历所有传感器 k，并统计有能量的传感器对场地的覆盖次数
//    for (k = 0; k < N; k++) {
//        if (energy[k] > 0) { // 只统计有能量的传感器
//            for (j = 0; j < OPosf[k][CMAX]; j++) {
//                if (OPosf[k][j] >= 0 && OPosf[k][j] < fieldNum) { // 检查索引是否在合法范围内
//                    field[OPosf[k][j]] += 1; // 计算每个场地的覆盖次数
//                }
//            }
//        }
//    }
//
//    // 计算覆盖最少的场地的覆盖次数
//    apmin = field[0];
//    for (i = 1; i < fieldNum; i++) {
//        if (field[i] > 0) { // 只考虑有被覆盖的场地
//            if (apmin > field[i]) {
//                apmin = field[i];
//            }
//        }
//        else { // 如果某个场地没有被任何传感器覆盖，部署失败
//            flag = false;
//            break;
//        }
//    }
//
//    // 检查是否超出了 UPAPMIN 限制
//    if (apmin > UPAPMIN) {
//        printf("UPAPMIN is not big enough!\n");
//        exit(0);
//    }
//
//    // 调用 calc_criti() 函数来记录覆盖关键场地的传感器
//    if (flag) {
//        calc_criti(); // 只有在所有场地都被覆盖的情况下，才记录覆盖关键场地的传感器
//    }
//
//    // 重置 field 数组
//    for (i = 0; i < fieldNum; i++) {
//        field[i] = 0;
//    }
//
//    return flag; // 返回是否覆盖成功
//}
//
//// 读取实例数据
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
//        printf("Parts: %d\n", sensor_parts[i]);
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
//    // 读取传感器实例、初始化网格和计算覆盖区域
//    readInst();  // 读取实例数据，主要是传感器位置
//    init_grid(); // 初始化网格，计算每个传感器的覆盖区域
//    calc_field(); // 计算传感器在目标区域中的覆盖场地
//
//    for (int i = 0; i < N; i++)
//    {
//        sensor_parts_tmp[i] = sensor_parts[i];
//    }
//
//    // 检查部署是否成功并计算冗余率
//    printf("Deployment OK: %d\n", deployOK());
//    printf("apmin: %d\n", apmin);
//    if (deployOK() == true && apmin > 1) {
//        if (apmin >= 800) {
//            printf("Error: The size of activation_time[][] is not big enough!\n");
//            exit(0);
//        }
//        for (int i = 0; i < N; i++)
//        {
//            printf("%d, part: %d\n", i, sensor_parts_tmp[i]);
//        }
//
//        // 计算冗余率
//        yita = N * PI * R * R / (double)apmin / L / W;
//        printf("Initial Deployment Success. Calculated Redundancy Rate: %lf\n", yita);
//
//        // 开始实验
//        for (trialNum = 0; trialNum < TRIALNUM; trialNum++) {
//            G = 0; // 初始化代数索引
//            countEvals = 0;
//            recordCount = 0;
//            init(); // 每次实验中重新初始化种群
//
//            for (int i = 0; i < N; i++)
//            {
//                printf("%d, part: %d\n", i, sensor_parts_tmp[i]);
//            }
//            tp1 = clock(); // 开始计时
//
//            while (countEvals <= MAX_EVA) {
//                GAoptimize(); // 调用混合遗传算法，生成新的一代种群
//                G++; // 增加代数索引
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
//                for (int i = maxRecordCount; i < recordCount; i++) {
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
//    // 关闭文件流
//    fclose(ftotal);
//    fclose(finst);
//    fclose(favgtotal);
//    fclose(fcrate);
//
//    // 输出激活时间的详细信息
//    for (int i = 0; i < N; i++) {
//        if (pbest.d[i].sensor_id != -1) { // 检查传感器是否被分配激活
//            printf("Sensor %d: Position (%.4f, %.4f), Status: Active in Group %d\n", i, point[i][0], point[i][1], pbest.d[i].part_id);
//
//            // 输出每个传感器在各个集合中的激活时间
//            bool hasActivation = false;
//            for (int j = 0; j < UPAPMIN; j++) {
//                if (activation_time[i][j] > 0) {
//                    hasActivation = true;
//                    break;
//                }
//            }
//
//            if (hasActivation) {
//                printf("  Activation times for Sensor %d:\n", i);
//                for (int j = 0; j < UPAPMIN; j++) {
//                    if (activation_time[i][j] > 0) {
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