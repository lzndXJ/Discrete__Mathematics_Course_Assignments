#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <cctype>
using namespace std;

typedef map<char, int> Map_ci;
typedef map<int, char> Map_ic;
typedef map<int, int> Map_ii;

Map_ci priority;

Map_ic getProposition(const string formula);
int findProposition(const Map_ic& pSet, char p);
int pow2(int n);
Map_ii toBinary(int n_proposition, int index);
int calculate(string& formula, const Map_ic& pSet, Map_ii& value);
void applyOperator(stack<int>& val, stack<char>& op);
bool validFormula(const string& s);

int main() {
    priority['!'] = 5;
    priority['&'] = 4;
    priority['|'] = 3;
    priority['^'] = 2;
    priority['~'] = 1;
    priority['('] = 0;
    priority['#'] = -1;

	cout << "***************************************\n";
	cout << "**                                   **\n";
	cout << "**         欢迎进入逻辑运算软件      **\n";
	cout << "**   (可运算真值表,主范式,支持括号)  **\n";
	cout << "**                                   **\n";
	cout << "**              用!表示非            **\n";
	cout << "**              用&表示与            **\n";
	cout << "**              用|表示或            **\n";
	cout << "**             用^表示蕴含           **\n";
	cout << "**             用~表示等值           **\n";
	cout << "**                                   **\n";
	cout << "***************************************\n\n";

	string formula;
	while (true) {
		cout << "Please enter a legitimate proposition formula: " << endl;
		cin >> formula;
		if (!validFormula(formula)) {
			cout << "输入不合法，请重新输入！" << endl;
		}
		else
			break;
	}

	Map_ic proposition_set = getProposition(formula);
	cout << "该式子中的变量个数为：" << proposition_set.size() << endl << "输出真值表如下：" << endl;
	for (unsigned int i = 0; i < proposition_set.size(); i++) //打印真值表表头
	{
		cout << proposition_set[i] << "\t";
	}
	cout << formula << endl;

	//计算并打印真值表
	int rowCount = pow2(proposition_set.size());   //有这么多种可能性
	int* results = new int[rowCount];              //存放每种取值情况的计算结果
	for (int i = 0; i < rowCount; i++) {
		Map_ii binary_values = toBinary(proposition_set.size(), i);
		for (size_t j = 0; j < proposition_set.size(); j++)
			cout << binary_values[j] << "\t";
		int result = calculate(formula, proposition_set, binary_values);
		results[i] = result;
		cout << result << endl;
	}

	// 输出主析取范式
	cout << "该命题公式的主析取范式：" << endl;
	bool first = true;
	for (int i = 0; i < rowCount; i++) {
		if (results[i] == 1) {
			if (!first) 
				cout << " ∨ ";
			cout << "m<" << i << ">";
			first = false;
		}
	}
	if (first) cout << "恒假";
	cout << endl;

	// 输出主合取范式
	cout << "该命题公式的主合取范式：" << endl;
	first = true;
	for (int i = 0; i < rowCount; i++) {
		if (results[i] == 0) {
			if (!first) 
				cout << " ∧ ";
			cout << "M<" << i << ">";
			first = false;
		}
	}
	if (first) cout << "恒真";
	cout << endl;

	delete[] results;
	return 0;
}

//查找命题变量的下标
int findProposition(const Map_ic& pSet, char p) {
	Map_ic::const_iterator it = pSet.begin();
	while (it != pSet.end()) {
		if (it->second == p)
			return it->first;
		it++;
	}
	return -1;
}
//该函数返回所输入公式中的所有命题变项(不包括运算符)
Map_ic getProposition(string formula) 
{
	Map_ic proposition;
	int n_proposition = 0;
	for (unsigned int i = 0; i < formula.length(); i++)
	{
		char c = formula[i];
		if (isalpha(c))
		{
			//遍历所有命题变项
			int r = findProposition(proposition, c);
			//cout << r << endl;
			if (r == -1)
			{
				//说明该命题变项尚未被遍历过
				proposition[n_proposition] = c;
				n_proposition++;
			}
		}
		else if (!priority.count(c))
		{
			cout << c << " is undefined!" << endl;
			exit(2);
		}
	}
	return proposition;
}

//该函数返回命题变项的二进制(1或0)取值,用于列举真值表时各个变量可能的取值
Map_ii toBinary(int n_proposition, int index)  
{
	Map_ii result;
	for (int i = 0; i < n_proposition; i++)
	{
		int r = index % 2;
		result[n_proposition - 1 - i] = r;
		index = index / 2;
	}
	return result;
}

int pow2(int n)    //该函数返回指定数字的二次方的值
{
	if (n == 0)
		return 1;
	else
		return 2 * pow2(n - 1);
}

void applyOperator(stack<int>& value, stack<char>& opter) {
	char opt = opter.top();
	opter.pop();
	bool right, left;   //用左操作数和右操作数来命名更直观
	switch (opt) {
		case'!':
			right = value.top();
			value.pop();
			value.push(!right);
			break;
		case '&':
		case '|':
		case '^':
		case '~':
			right = value.top();
			value.pop();
			left = value.top();
			value.pop();
			if (opt == '&') value.push(left && right);
			else if (opt == '|') value.push(left || right);
			else if (opt == '^') value.push(!left || right); // 蕴含
			else if (opt == '~') value.push((left && right) || (!left && !right)); // 等值
			break;
	}
}

int calculate(string& formula, const Map_ic& pSet, Map_ii& value) {
	stack<char> opter;     // 运算符栈
	stack<int> pvalue;     // 值栈
	opter.push('#');       // 放入起始标记
	formula = formula + "#";   // 在末尾加入结束标记

	for (unsigned int i = 0; i < formula.length(); ++i) {
		char c = formula[i];

		if (isalpha(c)) {         // c 是命题变量
			int idx = findProposition(pSet, c);
			Map_ii::iterator it = value.find(idx);
			if (it != value.end())
				pvalue.push(it->second);
			else
				pvalue.push(0);   // 若未找到对应值，默认为0（可按需改为报错）
		}
		else if (c == '(')
			opter.push(c);
		else if (c == ')') {
			while (!opter.empty() && opter.top() != '(') {// 在入栈前，先把栈顶所有优先级 >= 当前运算符的算掉（遇左括号停）
				applyOperator(pvalue, opter);
			}
			if (!opter.empty() && opter.top() == '(')  // 弹出 '('
				opter.pop();
		}
		// 在入栈前，先把栈顶所有优先级 >= 当前运算符的算掉（遇左括号停）
		else {
			while (!opter.empty() && opter.top() != '(' && priority[opter.top()] >= priority[c]) {
				applyOperator(pvalue, opter);
			}
			opter.push(c);
		}
	}

	// 扫描完毕后，清算栈中剩余运算符直到起始标记 '#'
	while (!opter.empty() && opter.top() != '#')
	{
		applyOperator(pvalue, opter);
	}

	// 最终结果在值栈顶
	return pvalue.top();
}



//================ 输入合法性验证 ==================
//判断是不是运算符
bool isOperator(char c) {
	return c == '!' || c == '&' || c == '|' || c == '^' || c == '~';
}

//判断符号是否允许出现在逻辑公式里
bool isAllowed(char c) {
	return isalpha(c) || isOperator(c) || c == '(' || c == ')';
}

//判断输入的逻辑公式是否正确
bool validFormula(const string& s) {
	if (s.empty()) return false;

	int bracket = 0;  //括号是否两两匹配
	bool preIsOp = true; // 当前字符的前一个是不是运算符（不能有连续的运算符，开头不能是二元运算符）
	bool preIsVar = false;  //当前字符的前一个是不是命题变量（不能有连续的变量命题）

	for (size_t i = 0; i < s.size(); i++) {
		char c = s[i];
		if (!isAllowed(c))
			return false;
		if (isalpha(c)) {
			if (preIsVar)
				return false;  // 连续变量，如ab
			preIsOp = false;
			preIsVar = true;
		}
		else if (isOperator(c)) {
			if (c != '!' && preIsOp)
				return false; // 连续运算符，除非是!
			preIsOp = true;
			preIsVar = false;
		}
		else if (c == '(') {
			bracket++;
			if (preIsVar)
				return false; // a( 不合法
			preIsOp = true;
			preIsVar = false;
		}
		else if (c == ')') {
			bracket--;
			if (bracket < 0 || preIsOp) 
				return false; // 括号不匹配或空括号
			preIsVar = true;
			preIsOp = false;
		}
	}

	if (bracket != 0 || preIsOp)
		return false;   // 括号未匹配,结尾不能是运算符
	return true;
}