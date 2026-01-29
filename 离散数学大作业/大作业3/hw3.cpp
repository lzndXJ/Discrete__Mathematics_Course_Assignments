#include <iostream>
#include <limits>
using namespace std;

void output(bool s[][100], int n);  // 输出关系矩阵
void zifan(bool s2[][100], int n);  // 求自反闭包
void duichen(bool s2[][100], int n);  // 求对称闭包
void chuandi(bool s2[][100], int n);  // 求传递闭包
void select();  // 选择算法
void exit_program();  // 程序退出
bool s[100][100];  // 关系矩阵
int z, n;  // z：算法方式编号；n：矩阵阶数

int main()
{
    // 打印标语
    cout << "*******************************************\n";
    cout << "**                                       **\n";
    cout << "**        欢迎进入关系的自反、对称       **\n";
    cout << "**           和传递闭包求解程序          **\n";
    cout << "**                                       **\n";
    cout << "*******************************************\n\n\n";
    select();  // 选择并执行算法
    return 0;
}

// 输入关系矩阵，选择闭包算法
void select()
{
    // 输入矩阵阶数n，并确保其合法
    cout << "请输入关系矩阵的行(或列)数(阶数)(>=2且<=100):";
    while (true) {
        cin >> n;
        if (n >= 2 && n <= 100) break;  // 阶数有效，跳出循环
        cout << "输入有误，请重新输入！\n";
    }

    // 输入关系矩阵的元素
    cout << "\n请输入关系矩阵:\n";
    for (int i = 0; i < n; ++i) {
        while (true) {
            cout << "请输入矩阵的第" << i + 1 << "行元素(只能输入1或0，元素以空格分隔):";
            bool valid = true;
            for (int j = 0; j < n; ++j) {
                cin >> s[i][j];
                if (s[i][j] != 0 && s[i][j] != 1) {
                    cout << "输入错误，请重新输入！" << endl;
                    valid = false;
                    break;  // 输入错误，重新输入当前行
                }
            }
            if (valid) break;  // 当前行输入有效，跳出循环
        }
    }

    // 输入并选择要执行的闭包类型
    cout << "\n输入对应序号选择算法:\n1: 自反闭包\n2: 对称闭包\n3: 传递闭包\n4: 退出\n";
    cin >> z;
    switch (z) {
        case 1: zifan(s, n); break;  // 自反闭包
        case 2: duichen(s, n); break;  // 对称闭包
        case 3: chuandi(s, n); break;  // 传递闭包
        case 4: exit_program(); break;  // 退出程序
        default:
            cout << "无效选项，请重新输入。\n";
            select();  // 重新选择
            break;
    }
}

// 继续运行或退出
void re()
{
    char x;
    cout << "\n是否继续运行该程序？（y/n）：";
    cin >> x;
    if (x == 'y') {
        select();  // 重新选择算法
    }
    else if (x == 'n') {
        exit_program();  // 退出程序
    }
    else {
        cout << "输入错误，请重新输入。\n";
        re();  // 继续询问
    }
}

// 显示算法结果
void output(bool s[][100], int n)
{
    cout << "\n所求关系矩阵为:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << s[i][j] << " ";  // 输出矩阵元素
        cout << endl;
    }
}

// 求自反闭包
void zifan(bool s2[100][100], int n)
{
    // 自反闭包：将矩阵的所有对角线元素设为1
    // 原理：自反闭包要求每个元素都与自己有关系
    for (int i = 0; i < n; ++i)
        s2[i][i] = 1;  // 对角线元素设为1
    output(s2, n);
    re();
}

// 求对称闭包
void duichen(bool s2[100][100], int n)
{
    bool s1[100][100];  // 转置矩阵

    // 计算原矩阵的转置矩阵
    // 原理：如果(a,b)在关系中，对称闭包需要包含(b,a)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            s1[j][i] = s2[i][j];  // 转置操作：行列互换

    // 合并原矩阵与转置矩阵（逻辑或运算）
    // 原理：对称闭包 = 原关系 ∪ 逆关系
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            s2[i][j] = s2[i][j] || s1[i][j];  // 使用逻辑或合并关系
    output(s2, n);
    re();
}

// 求传递闭包
void chuandi(bool s2[100][100], int n)
{
    bool flag = false;  // 收敛标志，当结果不再变化时为true
    bool t[100][100],    // 存放累加结果：M + M^2 + M^3 + ...
        temp[100][100], // 存放上一次的矩阵幂结果（M^(k-1)）
        power[100][100], // 存放当前矩阵幂结果（M^k）
        compare[100][100]; // 用于比较的矩阵，存储上一次的累加结果

    // 初始化所有矩阵
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            power[i][j] = 0;        // 初始化当前幂次结果
            t[i][j] = s2[i][j];     // 初始累加结果 = 原矩阵M
            compare[i][j] = s2[i][j]; // 初始比较矩阵 = 原矩阵M
            temp[i][j] = s2[i][j];  // 初始临时矩阵 = 原矩阵M（即M^1）
        }

    // 使用Warshall算法的思想：计算传递闭包 = M + M^2 + M^3 + ... + M^n
    // 当结果不再变化时停止（即t[k] = t[k+1]）
    while (!flag) {
        // 计算矩阵的下一幂次：power = temp × s2（即M^k = M^(k-1) × M）
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                power[i][j] = 0;  // 重置当前幂次结果

        // 矩阵乘法：计算M^k
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                for (int h = 0; h < n; ++h)
                    // 逻辑与代替乘法，逻辑或代替加法
                    power[i][j] = power[i][j] || (temp[i][h] && s2[h][j]);

        // 将当前幂次结果累加到总和中：t = t ∪ power
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                t[i][j] = t[i][j] || power[i][j];  // 逻辑或相当于集合的并

        // 假设已经收敛
        flag = true;

        // 检查当前结果是否与上一次相同（是否收敛）
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (t[i][j] != compare[i][j]) {
                    flag = false;  // 如果发现不同，继续迭代
                    break;
                }

        // 更新临时矩阵和比较矩阵，为下一次迭代做准备
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                temp[i][j] = power[i][j];  // 更新临时矩阵为当前幂次结果
                compare[i][j] = t[i][j];   // 更新比较矩阵为当前累加结果
            }
    }

    output(t, n);
    re();
}
// 程序退出
void exit_program()
{
    cout << "程序结束。\n";
    ::exit(0);  // 正常退出
}

