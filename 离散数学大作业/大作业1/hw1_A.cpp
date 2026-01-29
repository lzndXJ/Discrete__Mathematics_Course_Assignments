#include <iostream>
#include <string>
#include <limits>
using namespace std;

//输出提示，并完成布尔型变量的输入
int inputBool(const string& prompt) {
	int x;
	while (true) {
		cout << prompt;
		if (cin >> x && (x == 0 || x == 1)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清理可能的多余输入
			return x;
		}
		else {
			cout << "输入错误，请输入1或0。" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

int main() {
	cout << "***************************************" << endl;  //标语
	cout << "**                                   **" << endl;;
	cout << "**        欢迎进入逻辑运算程序       **" << endl;
	cout << "**                                   **" << endl;
	cout << "***************************************\n" << endl;

	bool again = true; //是否再次进行运算
	while (again) {
		int p = inputBool("请输入p的值(1或0),以回车结束:");
		int q = inputBool("请输入q的值(1或0),以回车结束:");

		cout << "合取：\n\tp /\\ q = " << (p && q) << endl;
		cout << "析取：\n\tp \\/ q = " << (p || q) << endl;
		cout << "条件：\n\tp -> q = " << ((!p) || q) << endl;
		cout << "双条件：\n\tp <-> q = " << (((!p) || q) && ((!q) || p)) << endl<<endl;

		char choice;
		while (true) {
			cout << "是否继续运算？[y/n]：";
			cin >> choice;
			if (choice == 'y' || choice == 'Y') { //考虑大小写情况
				break;
			}
			else if (choice == 'n' || choice == 'N') {
				again = false;
				break;
			}
			else {
				cout << "输入错误，请输入 y 或 n。"<<endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}
	cout << "程序结束，再见！" << endl;
}
