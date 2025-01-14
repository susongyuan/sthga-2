#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define NUM_SENSORS 1000
#define MAX_X 50.0  // X坐标的最大值，与设定中的区域大小一致
#define MAX_Y 50.0  // Y坐标的最大值，与设定中的区域大小一致
#define MAX_ENERGY 20.0  // 最大能量值
#define MIN_ENERGY 1.0   // 最小能量值

int main() {
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(0)));

    // 打开输出文件
    std::ofstream outFile("sensors_data.txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    // 生成传感器数据并写入文件
    for (int i = 0; i < NUM_SENSORS; ++i) {
        // 随机生成X和Y坐标
        double x = static_cast<double>(rand()) / RAND_MAX * MAX_X;
        double y = static_cast<double>(rand()) / RAND_MAX * MAX_Y;

        // 随机生成能量值，范围在 MIN_ENERGY 到 MAX_ENERGY 之间
        double energy = MIN_ENERGY + static_cast<int>(rand()) / RAND_MAX * (MAX_ENERGY - MIN_ENERGY);

        // 写入文件 (格式: x y energy)
        outFile << x << " " << y << " " << energy << "\n";
    }

    // 关闭文件
    outFile.close();

    std::cout << "Sensor data generated and written to sensors_data.txt" << std::endl;
    return 0;
}
