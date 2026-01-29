#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//并查集
vector<int> parent;

//初始化parent数组
void init_parent(int n) {
	parent.resize(n + 1);
	for (int i = 1; i <= n; i++)
		parent[i] = -1;
}

//边的结构体
struct Edge {
	int u;  //起点
	int v;  //终点
	int w;  //权值
};

//寻找根节点
int find_root(int x) {
	if (parent[x] <0) 
		return x;
	return parent[x] = find_root(parent[x]);
}

//按集合大小合并
void union_set(int root1, int root2) {
	int r1 = find_root(root1);
	int r2 = find_root(root2);

	//在同一个集合
	if (r1 == r2)
		return;

	if (parent[r1] < parent[r2]) {
		parent[r1] += parent[r2];
		parent[r2] = r1;
	}
	else {
		parent[r2] += parent[r1];
		parent[r1] = r2;
	}
}

//按照权值进行排序
bool cmp(const Edge& a, const Edge& b) {
	return a.w < b.w;
}

int main() {
	int n, m;
	cout << "请输入顶点数n和边数m：";
	cin >> n >> m;

	vector<Edge> edges(m);

	cout << "请依次输入每条边：起点 终点 权值" << endl;
	for (int i = 0; i < m; i++) {
		cin >> edges[i].u >> edges[i].v >> edges[i].w;
	}

	init_parent(n);

	//将所有边按权值进行排序
	sort(edges.begin(), edges.end(), cmp);

	int minWeight = 0;   //最小生成树总权值
	int count = 0;       //已经加入的边的数量

	for (auto& e : edges) {
		int ru = find_root(e.u);
		int rv = find_root(e.v);

		if (ru != rv) {
			union_set(e.u, e.v);
			minWeight += e.w;
			count++;
			cout << e.u << " - " << e.v << " : " << e.w << endl;
		}
		if (count == n - 1)  //构造完毕
			break;
	}
	cout << "\n最小生成树的总权值 = " << minWeight << endl;
}