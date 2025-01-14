#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <set>

using namespace std;

const int MAX_SENSOR_COUNT = 1000;  // 最大传感器数量

struct Sensor {
    double x, y;  // 横纵坐标
    int energy;   // 能量
    int id;       // 编号
    const double radius = 15.0; // 固定感知范围（单位：例如米）

    Sensor(double x, double y, int energy, int id)
        : x(x), y(y), energy(energy), id(id) {}

    // 拷贝构造函数
    Sensor(const Sensor& other)
        : x(other.x), y(other.y), energy(other.energy), id(other.id) {}

    // 拷贝赋值操作符
    Sensor& operator=(const Sensor& other) {
        if (this != &other) {  // 自赋值检查
            x = other.x;
            y = other.y;
            energy = other.energy;
            id = other.id;
        }
        return *this;
    }

    // 将传感器拆分为多个子传感器
    vector<Sensor> split_sensor(int& next_id) const {
        vector<Sensor> sub_sensors;
        // 根据能量拆分，确保子传感器的编号不会重复
        for (int i = 0; i < energy; i++) {
            // 保证不超过最大传感器数量
            if (next_id < MAX_SENSOR_COUNT) {
                sub_sensors.push_back(Sensor(x, y, 1, next_id++));  // 每个子传感器的编号是递增的
            }
        }
        return sub_sensors;
    }

    // 输出传感器编号，格式为 S<原编号>-<子传感器序号>
    string getSubSensorID(int subSensorIndex) const {
        return "S" + to_string(id) + "-" + to_string(subSensorIndex + 1);
    }
};

// 网格数据结构
struct Grid {
    double x, y; // 网格的左下角坐标
    bool covered;  // 是否被传感器覆盖

    Grid(double x, double y) : x(x), y(y), covered(false) {}
};

// 子集数据结构
struct Subset {
    set<int> sensor_ids;  // 子集内传感器的编号（使用 set 来自动避免重复传感器）
};

// 计算两点之间的距离
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// 读取传感器数据
vector<Sensor> readSensorsFromFile(const string& filename) {
    vector<Sensor> sensors;
    cout << "文件路径: " << filename << endl;
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "无法打开文件!" << endl;
        exit(1);
    }

    double x, y;
    int energy;
    int id = 0;
    while (infile >> x >> y >> energy) {
        sensors.push_back(Sensor(x, y, energy, id++));
    }

    infile.close();
    return sensors;
}

// 生成目标区域的网格
vector<Grid> generateGridMap(double length, double width, double gridSize) {
    vector<Grid> gridMap;
    for (double x = 0; x < length; x += gridSize) {
        for (double y = 0; y < width; y += gridSize) {
            gridMap.push_back(Grid(x, y));
        }
    }
    return gridMap;
}

// 判断传感器是否覆盖网格
bool isCoveredBySensor(const Sensor& sensor, const Grid& grid) {
    return calculateDistance(sensor.x, sensor.y, grid.x, grid.y) <= sensor.radius;
}

// 计算子集的适应度，基于能量消耗与覆盖情况
double calculateFitness(const vector<Sensor>& sensors, const Subset& subset, const vector<Grid>& gridMap) {
    double fitness = 0;
    vector<bool> covered(gridMap.size(), false);

    // 计算覆盖情况
    for (int id : subset.sensor_ids) {
        for (int i = 0; i < gridMap.size(); i++) {
            if (isCoveredBySensor(sensors[id], gridMap[i])) {
                covered[i] = true;
            }
        }
    }

    // 计算覆盖区域的适应度
    for (bool isCovered : covered) {
        if (isCovered) {
            fitness += 1;  // 每覆盖一个网格，加1
        }
    }

    // 同时考虑能量消耗
    for (int id : subset.sensor_ids) {
        fitness -= sensors[id].energy;  // 适应度计算时考虑能量消耗
    }

    return fitness;
}

// 初始化种群
vector<Subset> initializePopulation(const vector<Sensor>& sensors, int populationSize) {
    vector<Subset> population;

    // 随机生成种群
    for (int i = 0; i < populationSize; i++) {
        Subset subset;
        set<int> sensorSet;  // 用 set 来记录已经添加的传感器

        for (int j = 0; j < sensors.size(); j++) {
            if (rand() % 2) {
                // 确保同一传感器的子传感器不会同时出现在同一个子集里
                bool canAdd = true;
                for (int existingId : sensorSet) {
                    if (sensors[existingId].id == sensors[j].id) {
                        canAdd = false;
                        break;
                    }
                }
                if (canAdd) {
                    sensorSet.insert(j);  // 添加子传感器编号
                }
            }
        }

        subset.sensor_ids = sensorSet;
        population.push_back(subset);
    }

    return population;
}

// 遗传算法中的选择操作（基于适应度）
int selectParent(const vector<Subset>& population, const vector<Sensor>& sensors, const vector<Grid>& gridMap) {
    int bestIndex = -1;
    double bestFitness = -1e9;

    // 遍历种群，选择最优的个体（适应度最大）
    for (int i = 0; i < population.size(); i++) {
        double fitness = calculateFitness(sensors, population[i], gridMap);
        if (fitness > bestFitness || bestIndex == -1) {
            bestFitness = fitness;
            bestIndex = i;
        }
    }
    return bestIndex;
}

// 遗传算法中的交叉操作
Subset crossover(const Subset& parent1, const Subset& parent2) {
    Subset child;
    int crossoverPoint = rand() % parent1.sensor_ids.size();
    auto it = parent1.sensor_ids.begin();
    advance(it, crossoverPoint);

    for (auto i = parent1.sensor_ids.begin(); i != it; i++) {
        child.sensor_ids.insert(*i);
    }
    for (auto i = parent2.sensor_ids.begin(); i != parent2.sensor_ids.end(); i++) {
        child.sensor_ids.insert(*i);
    }

    return child;
}

// 遗传算法中的变异操作
void mutate(Subset& child) {
    int mutatePoint = rand() % child.sensor_ids.size();
    auto it = child.sensor_ids.begin();
    advance(it, mutatePoint);
    child.sensor_ids.erase(it);  // 假设变异为删除一个传感器
}

// 输出每个子集的传感器编号（包含子传感器编号）
void printSubsetIds(const vector<Subset>& subsets, const vector<Sensor>& sensors, ofstream& outputFile) {
    // 输出表头
    outputFile << "子集编号,传感器编号（子传感器编号）" << endl;

    for (int i = 0; i < subsets.size(); i++) {
        outputFile << "子集 " << i + 1 << ": ";
        bool first = true;
        for (int id : subsets[i].sensor_ids) {
            int sensorID = sensors[id].id;
            for (int subIndex = 0; subIndex < sensors[id].energy; subIndex++) {
                if (!first) {
                    outputFile << " ";
                }
                first = false;
                outputFile << sensorID << "(" << sensors[id].getSubSensorID(subIndex) << ")";
            }
        }
        outputFile << endl;
    }
}

int main() {
    srand(time(0));

    // 读取传感器数据
    string filename = "input//sensors_data.txt"; // Windows路径
    vector<Sensor> sensors = readSensorsFromFile(filename);

    // 初始化子传感器ID计数器
    int next_id = 0;
    vector<Sensor> sub_sensors;
    for (auto& sensor : sensors) {
        vector<Sensor> sub_sensors_for_this = sensor.split_sensor(next_id);
        sub_sensors.insert(sub_sensors.end(), sub_sensors_for_this.begin(), sub_sensors_for_this.end());
    }

    // 生成网格地图
    double length = 100.0, width = 100.0, gridSize = 10.0;
    vector<Grid> gridMap = generateGridMap(length, width, gridSize);

    // 初始化种群
    int populationSize = 10;  // 种群大小
    vector<Subset> population = initializePopulation(sub_sensors, populationSize);

    // 创建输出文件
    ofstream outputFile("sensor_subsets_output.csv");

    // 进行遗传算法迭代
    int generations = 100;  // 最大迭代次数
    for (int gen = 0; gen < generations; gen++) {
        // 选择父母
        int parent1Index = selectParent(population, sub_sensors, gridMap);
        int parent2Index = selectParent(population, sub_sensors, gridMap);

        // 交叉产生子代
        Subset child = crossover(population[parent1Index], population[parent2Index]);

        // 变异
        mutate(child);

        // 在种群中替换最差个体
        population.push_back(child);
        population.erase(population.begin());  // 删除最差个体

        // 输出当前代的子集编号
        cout << "代 " << gen + 1 << " 的子集编号: " << endl;
        printSubsetIds(population, sensors, outputFile);
    }

    // 关闭文件
    outputFile.close();

    return 0;
}
