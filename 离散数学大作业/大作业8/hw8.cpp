#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

const int INF = INT_MAX/2;

int main()
{
    int n;
    while (true) {
        cout << "输入城市数量: ";
        cin >> n;
        if (cin.fail()||n<=0) {
            cout << "城市数量必须是正整数，重新输入！" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    // 输入距离矩阵 dist[i][j]
    vector<vector<int>> dist(n, vector<int>(n));
    cout << "输入距离矩阵:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> dist[i][j];

    int fullMask = 1 << n; //状态总数
    vector<vector<int>> dp(fullMask, vector<int>(n, INF));    //记录走到当前mask状态并停在城市i的最短路径
    vector<vector<int>> parent(fullMask, vector<int>(n, -1)); // 最短路径走到这一步的上一步是哪个城市

    //设置起点为城市0
    dp[1][0] = 0;

    //枚举所有
    for (int mask = 1; mask < fullMask; mask++) {
        for (int i = 0; i < n; i++) {   //假设城市i是最后一个访问的城市
            if (!(mask & (1 << i)))
                continue;   //i不在mask里面，跳过
            int preMask = mask ^ (1 << i);
            if (preMask == 0)  //说明i是第一个城市
                continue;
            for (int j = 0; j < n; j++) {  //枚举走到i的上一步是j
                if (!(preMask & (1 << j)))
                    continue;     //j没有访问过，跳过
                int cost = dp[preMask][j] + dist[j][i];
                if (cost < dp[mask][i]) {
                    dp[mask][i] = cost;
                    parent[mask][i] = j;
                }
            }
        }
    }

    //寻找最终答案
    int bestCost = INF, endCity = -1;
    for (int i = 1; i < n; i++) {
        if (dp[fullMask - 1][i] >= INF)
            continue;
        int cost = dp[fullMask-1][i] + dist[i][0];
        if (cost < bestCost) {
            bestCost = cost;
            endCity = i;
        }
    }

    cout << "最短路径长度为：" << bestCost << endl;

    //打印路径,从终点开始回溯
    vector<int> path;
    int mask = fullMask - 1;
    int cur = endCity;
    while (cur != -1) {
        path.push_back(cur);
        int prev = parent[mask][cur];
        mask ^= (1 << cur); // 从 mask 中移除当前城市
        cur = prev;
    }

    reverse(path.begin(), path.end());
    path.push_back(0);

    // 输出路线
    cout << "路线顺序: ";
    for (int v : path) cout << v << ' ';
    cout << endl;

    return 0;
}