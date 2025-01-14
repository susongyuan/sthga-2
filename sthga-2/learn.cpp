#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  // 包含标准输入输出库
#include <stdlib.h> // 包含标准库，用于rand()和srand()等函数
#include <time.h>   // 包含时间库，用于设置随机种子
#include <string.h> // 包含字符串库，用于strcpy()函数

#define POP_SIZE 100  // 种群大小，定义为100
#define GENE_LENGTH 10  // 染色体长度，定义为10
#define MAX_GEN 1000  // 最大迭代次数，定义为1000
#define MUTATION_RATE 0.01  // 变异率，定义为0.01

// 适应度函数，这里以简单的求和为例
float fitness(char* gene) {
    float sum = 0;
    for (int i = 0; i < GENE_LENGTH; i++) {
        sum += (gene[i] - '0'); // 将字符转换为整数并累加
    }
    return sum; // 返回染色体中1的个数
}

// 生成随机染色体
void generateGene(char* gene) {
    for (int i = 0; i < GENE_LENGTH; i++) {
        gene[i] = '0' + rand() % 2; // 生成0或1，并赋值给染色体的当前位置
    }
    gene[GENE_LENGTH] = '\0'; // 确保字符串以空字符结尾
}

// 选择操作
int select(char genes[POP_SIZE][GENE_LENGTH + 1], float fitnesses[POP_SIZE]) {
    float totalFitness = 0;
    for (int i = 0; i < POP_SIZE; i++) {
        totalFitness += fitnesses[i]; // 计算总适应度
    }
    float r = (float)rand() / RAND_MAX * totalFitness; // 生成一个随机数，用于选择
    float sum = 0;
    for (int i = 0; i < POP_SIZE; i++) {
        sum += fitnesses[i]; // 累加适应度
        if (sum >= r) return i; // 如果累加值大于随机数，返回当前索引
    }
    return POP_SIZE - 1; // 如果没有找到，返回最后一个索引
}

// 交叉操作
void crossover(char parent1[GENE_LENGTH + 1], char parent2[GENE_LENGTH + 1], char child[GENE_LENGTH + 1]) {
    int crossoverPoint = rand() % (GENE_LENGTH); // 随机选择一个交叉点
    for (int i = 0; i < GENE_LENGTH; i++) {
        child[i] = i < crossoverPoint ? parent1[i] : parent2[i]; // 根据交叉点生成子代
    }
    child[GENE_LENGTH] = '\0'; // 确保字符串以空字符结尾
}

// 变异操作
void mutate(char* gene) {
    for (int i = 0; i < GENE_LENGTH; i++) {
        if ((float)rand() / RAND_MAX < MUTATION_RATE) { // 如果随机数小于变异率，则变异
            gene[i] = gene[i] == '0' ? '1' : '0'; // 翻转当前位
        }
    }
}

int main() {
    srand(time(NULL)); // 设置随机种子，使每次运行结果不同
    char genes[POP_SIZE][GENE_LENGTH + 1]; // 存储种群的染色体
    float fitnesses[POP_SIZE]; // 存储种群的适应度
    char newGenes[POP_SIZE][GENE_LENGTH + 1]; // 存储新一代的染色体

    // 初始化种群
    for (int i = 0; i < POP_SIZE; i++) {
        generateGene(genes[i]); // 生成随机染色体
        fitnesses[i] = fitness(genes[i]); // 计算适应度
    }

    for (int gen = 0; gen < MAX_GEN; gen++) { // 迭代最大代数
        // 创建新一代
        for (int i = 0; i < POP_SIZE; i++) {
            int parent1 = select(genes, fitnesses); // 选择两个父代
            int parent2 = select(genes, fitnesses);
            crossover(genes[parent1], genes[parent2], newGenes[i]); // 进行交叉
            mutate(newGenes[i]); // 进行变异
        }

        // 评估新种群
        for (int i = 0; i < POP_SIZE; i++) {
            fitnesses[i] = fitness(newGenes[i]); // 计算新染色体的适应度
        }

        // 替换旧种群
        for (int i = 0; i < POP_SIZE; i++) {
            strcpy(genes[i], newGenes[i]); // 将新染色体复制到旧种群
        }

        // 检查终止条件
        if (fitness(genes[0]) >= GENE_LENGTH) break; // 如果找到最优解，则终止
    }

    printf("Best gene: %s\n", genes[0]); // 打印最优染色体
    printf("Best fitness: %.2f\n", fitness(genes[0])); // 打印最优适应度

    return 0; // 程序结束
}
