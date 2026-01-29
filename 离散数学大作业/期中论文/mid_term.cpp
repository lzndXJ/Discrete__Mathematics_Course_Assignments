#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <limits>
using namespace std;

const int NMAX = 50;       //最大课程数量
const int NAMELEN = 20;    //每个课程名最大字符数
const int EMAX = 200;      //最大边数
const int MAX_ORDER = 1000;//最大拓扑排序数

//========课程名存储与查找========
char nameList[NMAX][NAMELEN];  //用二维数组存储课程名
int courseCount = 0;           //当前添加的课程数量

//在nameList中查找字符串s的下标
int find_course(const char* s) {
	for (int i = 0; i < courseCount; i++) {
		if (strcmp(nameList[i], s) == 0)
			return i;
	}
	return -1;     //没找到
}

int add_course(const char* s) {
	int id = find_course(s);
	if (id != -1) {
		return id;
	}
	if (courseCount >= NMAX) {           // 防越界的安全检查
		cout << "错误：课程数超过 NMAX 限制\n";
		exit(1);
	}

	//添加课程名称到新位置
	strncpy(nameList[courseCount], s, NAMELEN - 1);
	nameList[courseCount][NAMELEN - 1] = '\0';
	return courseCount++;
}

//========图的邻接表========
int head[NMAX];     //存该节点最后一条加入的边的序号
int to[EMAX];       //存边指向的顶点
int nextEdge[EMAX]; //这条边的下一条、同起点节点的边
int edgeCount = 0;  //已添加的边计数

// 添加一条从 u 指向 v 的有向边
void add_edge(int u, int v) {
	if (edgeCount >= EMAX) {
		cout << "错误：边数超过 EMAX 限制\n";
		exit(1);
	}
	to[edgeCount] = v;
	nextEdge[edgeCount] = head[u];
	head[u] = edgeCount;
	edgeCount++;
}

//========入度与回溯状态========
int indegree[NMAX];      //保存原始入度
int degCopy[NMAX];       //在回溯中保存当前节点入度的动态值
int allTopo[MAX_ORDER][NMAX]; //用来保存所有得到的拓扑序，按序号存储每个序列的节点 id
int totalTopo = 0;       //已找到的拓扑序数量
int path[NMAX];          //保存当前回溯路径（深度为 depth 时，path[0..depth-1] 是已选的序列）
bool used[NMAX];         //标注某个课程是否加入当前路径

//回溯枚举所有拓扑序
void dfs_all(int depth) {
	if (totalTopo >= MAX_ORDER) return;    //如果排序数量超过上限，直接截断
	// 如果当前 depth == courseCount，表示已选满一条完整序列，记录
	if (depth == courseCount) {
		if (totalTopo < MAX_ORDER) {          //加入检测
			for (int i = 0; i < courseCount; i++) {
				allTopo[totalTopo][i] = path[i];
			}
			totalTopo++;
		}
		return;
	}

	int candidate[NMAX];      //收集入度为0且未被使用的节点
	int candidateCount = 0;   //候选的节点数
	for (int i = 0; i < courseCount; i++) {
		if (!used[i] && degCopy[i] == 0) {
			candidate[candidateCount++] = i;
		}
	}

	//没有候选节点，直接返回
	if (candidateCount == 0)
		return;

	//对每个候选依次选择递归
	for (int k = 0; k < candidateCount; k++) {
		int u = candidate[k];
		//选择u，写入path
		used[u] = true;
		path[depth] = u;
		//选择u之后，u的后续课程不受u约束，入度需要修改
		int change[NMAX];   //存放改变的节点
		int changeCount = 0;
		for (int e = head[u]; e != -1; e = nextEdge[e]) { //遍历所有u的后继节点
			int v = to[e];
			degCopy[v]--;
			change[changeCount++] = v;
		}

		dfs_all(depth + 1);    //递归下一个深度
		//撤销选择u，恢复入度
		used[u] = false;  
		for (int t = 0; t < changeCount; t++) {
			int v = change[t];
			degCopy[v]++;
		}
		//path 在下一次选择时会被覆盖，不必额外清除
	}
}

//判断先修关系是否重复
bool edge_exist(int u, int v) {
	for (int e = head[u]; e != -1; e = nextEdge[e])
		if (to[e] == v)
			return true;
	return false;
}

//========kahn算法检测是否有环========
bool has_cycle(int n) {
	int tmp[NMAX];
	for (int i = 0; i < n; i++) {
		tmp[i] = indegree[i];
	}

	int count = 0;
	bool visited[NMAX] = { false };

	while (true) {
		int found = -1;
		for (int i = 0; i < n; i++) {
			if (!visited[i] && tmp[i] == 0) {   //没有被访问过并且入度为0，可以取出
				found = i;
				break;
			}
		}

		if (found == -1)                      //没有满足条件的顶点
			break;

		visited[found] = true;
		count++;

		for (int e = head[found]; e != -1; e = nextEdge[e]) {
			int v = to[e];
			tmp[v]--;
		}
	}

	return count != n;       //没有取出所有点说明存在环
}

//========主程序（输入/初始化/调用函数）========
int main()
{
	for (int i = 0; i < NMAX; i++)
		head[i] = -1;
	for (int i = 0; i < NMAX; i++) {
		indegree[i] = 0;
		used[i] = false;
	}

	//输入课程数量
	int n;
	while (true) {
		cout << "请输入课程数量：";
		cin >> n;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "课程数量必须是整数，请重新输入！" << endl;
			continue;
		}
		else if (n <= 1 || n > NMAX) {
			cout << "课程数量超出范围，应该在2到" << NMAX << "之间！" << endl;
			continue;
		}
		break;
	}

	//输入课程名称
	char buf[NAMELEN];
	cout << "请输入 " << n << " 个课程名称（每个不超过 " << (NAMELEN - 1)
		<< " 个字符，用空格或换行分隔）：" << endl;
	for (int i = 0; i < n;) {
		cin >> buf;
		if (find_course(buf) != -1)     //防止重复输入同名的课程
			cout << "请不要输入同名的课程！" << endl;
		else {
			add_course(buf);
			i++;
		}
	}

	//输入先修关系数量
	int m;
	while (true) {
		cout << "请输入先修关系数量：";
		cin >> m;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "先修关系数量必须是整数，请重新输入！" << endl;
			continue;
		}
		else if (m <= 0 || m > EMAX) {
			cout << "先修关系数量超出范围，应该在1到" << EMAX << "之间！" << endl;
			continue;
		}
		break;
	}

	//输入各个先修关系
	cout << "请输入每条先修关系（格式A B 表示A是B的先修课）" << endl;
	for (int i = 0; i < m; i++) {
		char a[NAMELEN], b[NAMELEN];
		int u, v;                     //课程的对应下标
		while (true) {
			cout << i + 1 << ":";     //第几个关系
			cin >> a >> b;
			u = find_course(a);
			v = find_course(b);
			if (u == -1 || v == -1) {
				cout << "输入的课程不存在，请重新输入!" << endl;
				continue;
			}
			if (u == v) {
				cout << "课程不能作为自己的先修课，请重新输入!" << endl;
				continue;
			}
			if (edge_exist(u, v)) {
				cout << "该先修关系已经存在，请重新输入！" << endl;
				continue;
			}
			break;
		}
		add_edge(u, v);     //加入邻接表
		indegree[v]++;      //更新入度
	}

	//检测是否有环
	if (has_cycle(n)) {
		cout << "\n该课程依赖图中存在环！没有任何拓扑序（课程顺序无解）。\n";
		return 0;
	}

	// 复制入度用于 dfs 回溯
	for (int i = 0; i < n; i++)
		degCopy[i] = indegree[i];

	//生成所有拓扑排序
	dfs_all(0);

	cout << "\n共找到 " << totalTopo << " 个合法的修课顺序：\n\n";
	for (int t = 0; t < totalTopo; t++) {
		cout << "方案 " << t + 1 << ": ";
		for (int i = 0; i < n; i++) {
			cout << nameList[allTopo[t][i]];
			if (i != n - 1) cout << " -> ";
		}
		cout << "\n";
	}

	return 0;
}