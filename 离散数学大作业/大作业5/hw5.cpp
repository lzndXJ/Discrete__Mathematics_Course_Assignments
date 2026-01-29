#include <iostream>
#include <vector>

#include <string>
using namespace std;

struct Node {
	int weight;  //权值
	int left;    //左子树
	int right;   //右子树
	bool used;   //是否使用过

};

//递归生成编码
void generateCodes(const vector<Node>& tree, int index, const string& code, vector<string>& result) {
	if (tree[index].left == -1 && tree[index].right == -1) {  //出口条件
		result[index] = code.empty() ? "0" : code;
		return;
	}
	if (tree[index].left != -1)
		generateCodes(tree, tree[index].left, code + "0", result);
	if (tree[index].right != -1)
		generateCodes(tree, tree[index].right, code + "1", result);
}


int main() {
	int n;
	while(true) {
		cout << "请输入节点数量：";
		cin >> n;
		if (cin.fail() || n <= 0) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "节点数量必须是正整数，重新输入！" << endl;
			continue;
		}
		break;
	}

	cout << "请输入" << n << "个节点：";
	vector<Node> tree(2 * n - 1);
	for (int i = 0; i < n; i++) {
		cin >> tree[i].weight;
		tree[i].left = tree[i].right = -1;
		tree[i].used = false;
	}

	int nextIndex = n;
	for (int i = 0; i < n - 1; i++) {

		int min1 = -1, min2 = -1; //取数组里面最小的两个节点

		for (int j = 0; j < nextIndex; j++) {
			if (tree[j].used) //使用过直接跳过
				continue;
			if (min1 == -1 || tree[j].weight < tree[min1].weight) {
				min2 = min1;
				min1 = j;
			}
			else if (min2 == -1 || tree[j].weight < tree[min2].weight) {
				min2 = j;
			}
		}

		tree[min1].used = true;
		tree[min2].used = true;

		//加入新节点
		tree[nextIndex].weight = tree[min1].weight + tree[min2].weight;
		tree[nextIndex].left = min1;
		tree[nextIndex].right = min2;
		tree[nextIndex].used = false;

		nextIndex++;
	}

	//生成编码
	vector<string> codes(n); 
	generateCodes(tree, nextIndex - 1, "", codes);

	//打印编码
	for (int i = 0; i < n; i++) {
		cout << "权值 " << tree[i].weight << " -> " << codes[i] << endl;
	}

	return 0;
}