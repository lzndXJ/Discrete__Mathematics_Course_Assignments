#include <iostream>
#include <vector>
using namespace std;

/*
=========================================
    函数：检查封闭性（Closure）
    逻辑：对所有 a,b ∈ G，运算 a*b 的结果必须仍然在集合 G 中
    输入：op —— Cayley 表（运算表）
          n —— 集合大小
=========================================
*/
bool checkClosure(const vector<vector<int>>& op, int n) {
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < n; b++) {

            int c = op[a][b];  // 计算 a * b 的结果

            // 检查结果是否仍然属于集合 {0,1,...,n-1}
            if (c < 0 || c >= n) {
                return false;   // 有结果超出集合范围 → 不封闭
            }
        }
    }
    return true;  // 所有结果均在集合中 → 封闭
}

/*
=========================================
    函数：检查结合性（Associativity）
    逻辑：对所有 a,b,c ∈ G，必须满足
          (a*b)*c == a*(b*c)
    输入：op —— Cayley 表
          n —— 集合大小
=========================================
*/
bool checkAssociativity(const vector<vector<int>>& op, int n) {
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < n; b++) {
            for (int c = 0; c < n; c++) {

                // 左结合：(a*b)*c
                int left = op[op[a][b]][c];

                // 右结合：a*(b*c)
                int right = op[a][op[b][c]];

                // 只要有一个不相等，就不满足结合律
                if (left != right)
                    return false;
            }
        }
    }
    return true;  // 所有三元组均满足结合式
}

/*
=========================================
    函数：寻找单位元（Identity element）
    逻辑：枚举集合中的每个元素 e，检查是否满足：
          对所有 a： e*a = a 且 a*e = a
    输入：op —— Cayley 表
          n —— 集合大小
    返回：找到的单位元 e；若不存在，返回 -1
=========================================
*/
int findIdentity(const vector<vector<int>>& op, int n) {
    // 尝试将每个元素 e 当作候选单位元
    for (int e = 0; e < n; e++) {
        bool isIdentity = true;

        for (int a = 0; a < n; a++) {
            // 单位元必须满足：e * a = a 且 a * e = a
            if (op[e][a] != a || op[a][e] != a) {
                isIdentity = false;
                break;
            }
        }

        if (isIdentity)
            return e;  // 找到单位元
    }

    return -1;  // 没有任何元素满足单位元条件
}

/*
=========================================
    函数：检查每个元素是否有逆元（Inverse）
    逻辑：对每个 a ∈ G，必须存在 b ∈ G 使得：
          a*b = e 且 b*a = e
    输入：op —— Cayley 表
          n —— 集合大小
          e —— 单位元
=========================================
*/
bool checkInverse(const vector<vector<int>>& op, int n, int e) {
    // 枚举所有元素 a
    for (int a = 0; a < n; a++) {
        bool hasInverse = false;

        // 尝试所有 b，看是否存在逆元
        for (int b = 0; b < n; b++) {
            if (op[a][b] == e && op[b][a] == e) {
                hasInverse = true;
                break;  // 找到逆元即可停止
            }
        }

        // 若某个元素找不到逆元，则不是群
        if (!hasInverse)
            return false;
    }
    return true;  // 每个元素都存在逆元
}

int main() {
    int n;
    while (true) {
        cout << "请输入集合大小 n: ";
        cin >> n;
        if (cin.fail() || n <= 2) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "集合大小必须是大于2的正整数，请重新输入！" << endl;
            continue;
        }
        break;
    }


    // 创建 n×n 的 Cayley 表：op[a][b] = a*b
    vector<vector<int>> op(n, vector<int>(n));

    cout << "请输入 Cayley 运算表（元素编号为 0~n-1）:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> op[i][j];
        }
    }


    //检查封闭性
    if (!checkClosure(op, n)) {
        cout << "该结构不是群：二元运算不封闭。" << endl;
        return 0;
    }

    //检查结合律
    if (!checkAssociativity(op, n)) {
        cout << "该结构不是群：不满足结合律。" << endl;
        return 0;
    }

    //寻找单位元
    int e = findIdentity(op, n);
    if (e == -1) {
        cout << "该结构不是群：不存在单位元。" << endl;
        return 0;
    }
    cout << "找到单位元： " << e << endl;

    //检查逆元
    if (!checkInverse(op, n, e)) {
        cout << "该结构不是群：存在无逆元的元素。" << endl;
        return 0;
    }

    cout << "该结构满足封闭性、结合律、单位元、逆元，是一个群！" << endl;
    return 0;
}
