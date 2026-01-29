#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
using namespace std;
using ll = long long;
const ll INF = LLONG_MAX / 4; // 防止加法溢出

//边结构体
struct Edge {
	int u, v;
	ll w;
};

//重建最短路径,用于构建多重图
vector<int> reconstruct_path(int u, int v, const vector<vector<int>>& next) {
    vector<int> path;      //用于存储u到v的路径
    if (next[u][v] == -1)  //没有路径
        return path;
    int cur = u;
    path.push_back(cur);
    while (cur != v) {
        cur = next[cur][v];
        path.push_back(cur);
    }
    return path;
}

int main()
{
    //========初始化========
    cout << "请输入顶点数和边数：";
    int n, m;        //顶点数，边数
    cin >> n >> m;

    vector<Edge> edge;
    edge.reserve(m);   //预先分配空间

    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INF));  //顶点之间最短路径距离
    vector<vector<int>> next(n + 1, vector<int>(n + 1, -1));  //辅助判断矩阵,下一步走的位置
    vector<int> deg(n + 1, 0);    //顶点的度数
    ll sumWeight=0;                 //总权重
    vector<vector<int>> adj(n + 1);  //邻接表，用于连通性检查

    for (int i = 1; i <= n; ++i) {
        dist[i][i] = 0;
        next[i][i] = i;
    }

    cout << "请输入每条边的连个端点和权值："<<endl;
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edge.push_back({ u, v, w });
        sumWeight += w;

        if (w < dist[u][v]) {
            dist[u][v] = dist[v][u] = w;
            next[u][v] = v;
            next[v][u] = u;
        }

        deg[u]++;
        deg[v]++;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    //========连通性检查========
    vector<int> visit(n + 1, 0);  //是否被访问过
    int start = -1;
    for (int i = 1; i <= n; i++) {  //找到一个度不为0的顶点作为起点，度为0的点不影响连通性
        if (deg[i] > 0) {
            start = i;
            break;
        }
    }

    //BFS
    vector<int> q = { start };  //相当于队列，将邻接的顶点入队
    visit[start] = 1;
    for (size_t idx = 0; idx < q.size(); ++idx) {
        int u = q[idx];
        for (int v : adj[u]) if (!visit[v]) { visit[v] = 1; q.push_back(v); }
    }

    for (int i = 1; i <= n; ++i) {
        if (deg[i] > 0 && !visit[i]) {   //有边却没有被访问到，说明图不连通
            cout << "图不连通，无法完成任务。\n";
            return 0;
        }
    }

    //========用warshall算法求出最短路径========
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            if (dist[i][k] < INF)
                for (int j = 1; j <= n; j++)
                    if (dist[k][j]<INF && dist[i][j]>dist[i][k]+dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];//更新最短距离
                        next[i][j] = next[i][k];            //更新路径
                    }
        }
    }


    //========奇度顶点匹配========
    vector<int> odd;
    for (int i = 1; i <= n; i++) {
        if (deg[i] % 2 == 1)
            odd.push_back(i);
    }
    int k = odd.size();  //奇数度顶点的个数

    ll matchCost = 0;    //总路径
    vector<pair<int, int>> matching_pairs;   //配对数组

    if (k > 0) {
        int FULL = 1 << k;  //总状态数
        vector<ll> dp(FULL, INF);      //当前匹配状态下的最小总权值
        vector<int> choice(FULL, -1);  //达到状态mask前，最后一次配对的使那两个点
        dp[0] = 0;                     //初始状态：没有顶点被匹配，代价为 0

        for (int mask = 0; mask < FULL; mask++) {   //枚举所有可能
            if (dp[mask] >= INF)
                continue;
            int i = 0;
            while (i < k && (mask & (1 << i)))//第i位已经是1，已匹配，跳过
                i++;
            if (i >= k)  //如果 i >= k，说明 mask 已经全为 1（所有顶点已匹配），不用再配对
                continue;
            for (int j = i + 1; j < k; j++) //枚举j与i配对
                if (!(mask & (1 << j))) {  //j没有匹配才考虑
                    int newMask = mask | (1 << i) | (1 << j);
                    ll cost = dist[odd[i]][odd[j]];
                    if (dp[newMask] > dp[mask] + cost) {  
                        dp[newMask] = dp[mask] + cost;
                        choice[newMask] = (i << 8) | j; //选择在nmask状态下最好的一种配对
                    }
                }
        }

        matchCost = dp[FULL - 1];//总花费
        int cur = FULL - 1;             // 从最终状态开始回溯配对方案
        while (cur) {
            int enc = choice[cur];      //取出编码过的配对信息
            int i = enc >> 8, j = enc & 0xFF;  //解码取出两个点
            matching_pairs.push_back({ odd[i], odd[j] });  //加入到配对结果
            cur = cur & (~(1 << i)) & (~(1 << j));    //从状态中移除，继续回溯
        }
    }

    //========构建多重图========
    map<pair<int, int>, int> multCount;
    // 添加原始边
    for (auto& e : edge) {                     
        int a = e.u, b = e.v;                  
        if (a > b) 
            swap(a, b);                  // 统一无向边顺序，保证键为 (小, 大)
        multCount[{a, b}] += 1;               
    }

    // 将匹配对之间的最短路径加入多重图
    for (auto& p : matching_pairs) {
        int u = p.first, v = p.second;
        vector<int> path = reconstruct_path(u, v, next); 
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            int a = path[i], b = path[i + 1];
            if (a > b) 
                swap(a, b);
            multCount[{a, b}] += 1; // 每条边增加一次
        }
    }

    // 构建邻接表，存储每条边的剩余数量
    vector<map<int, int>> adj_mult(n + 1);//<邻居顶点编号，边的数量>
    for (auto& it : multCount) {
        int a = it.first.first, b = it.first.second;
        int count = it.second;
        if (count <= 0) 
            continue;
        adj_mult[a][b] += count;
        adj_mult[b][a] += count;
    }

    //========生成欧拉回路========
    vector<int> circuit;   //顶点序列
    stack<int> st;

    int sta = 1;  //选择奇度顶点作为起点
    for (int u = 1; u <= n; u++)
        if (deg[u] % 2 == 1) 
        { sta = u; break; }  // 奇度顶点优先
    st.push(sta);

    while (!st.empty()) {
        int u = st.top();
        if (!adj_mult[u].empty()) {  //u的邻边还没有走完
            int v = adj_mult[u].begin()->first;   //取一个和u相邻的顶点
            if (--adj_mult[v][u] == 0) 
                adj_mult[v].erase(u);
            if (--adj_mult[u][v] == 0)
                adj_mult[u].erase(v);
            
            st.push(v);
        }
        else {  //u的邻边走完了，出栈
            circuit.push_back(u);
            st.pop();
        }
    }

    reverse(circuit.begin(), circuit.end());

    ll totalLen = sumWeight + matchCost;
    cout << "最短总路程 = " << totalLen << "\n";
    cout << "欧拉回路顶点序列（包含重复经过的顶点）:";
    for (size_t i = 0; i < circuit.size(); ++i) {
        if (i) cout << " -> ";
        cout << circuit[i];
    }
    cout << "\n";

    return 0;
}