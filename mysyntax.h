#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "token.h"
using namespace std;
extern vector<token> tokens; //引入外部变量数组
extern vector<string> names; // 引入外部的名称
class ProductionFormula {  //产生式类型
public:
	token left;
	vector<token> rights;
	ProductionFormula(token left, vector<token> rights) : left(left), rights(rights) {}
};

ostream& operator<< (ostream& out, ProductionFormula produc) {
	out << names[produc.left.getType() - 1];
	out << " -> ";
	for (token right : produc.rights) {
		out << names[right.getType() - 1];
		out << " ";
	}
	return out;
}

typedef vector<ProductionFormula> Syntax;

class SyntaxAnalyzer {
public:
	//SyntaxAnalyzer(Syntax syntax);
	SyntaxAnalyzer();
	void parse();

private:
	static int state_table_SLR[46][27]; //SLR自动机数组
	stack<int> state; //状态栈
	stack<token> tokenst; //符号栈
	int cs; // 当前状态
	Syntax syntax; //表示当前分析器所能调用的全部产生式文法
};
//语法分析类的静态成员初始化
int SyntaxAnalyzer::state_table_SLR[46][27] = {
		{0,3,0,5,0,0,8,0,0,0,10,11,0,0,0,13,0,14,15,0,1,2,7,4,12,6,9},
		{0,0,105,0,0,105,0,0,0,0,0,0,0,0,0,0,0,0,0,200,0,0,0,0,0,0,0},
		{0,3,0,5,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,16,2,0,4,0,0,0},
		{0,3,0,5,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,19,2,18,4,0,6,0},
		{0,0,103,0,0,103,0,0,0,0,0,0,0,0,0,0,0,0,0,103,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,0,0,13,0,0,0,0,0,0,0,0,20,0,9},
		{0,3,0,5,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,22,2,0,4,0,0,0},
		{0,0,0,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,113,0,113,113,0,24,113,113,0,0,113,113,113,0,113,0,0,113,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,25,27,0,0,0,0,0,0,26,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,25,27,0,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,0,0,13,0,14,15,0,0,0,0,0,31,0,30},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,32,33,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,32,33,13,0,14,15,0,0,0,0,0,34,0,9},
		{0,0,0,0,119,0,0,0,0,0,0,0,119,119,119,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,120,0,0,0,0,0,0,0,120,120,120,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,101,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,35,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,105,0,0,105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,36,0,0,0,0,0,0,0,0,32,33,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,113,0,113,113,0,0,113,113,0,0,113,113,113,0,113,0,0,113,0,0,0,0,0,0,0},
		{0,0,106,0,0,106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,107,0,107,0,0,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37},
		{0,0,0,0,0,0,21,0,25,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39},
		{0,0,0,0,0,0,21,0,25,27,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40},
		{0,0,111,0,111,111,0,0,111,111,0,0,111,111,111,0,111,0,0,111,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41},
		{0,0,0,0,0,0,0,0,25,27,0,0,42,0,0,0,26,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,43,32,33,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,32,0,13,0,14,15,0,0,0,0,0,44,0,9},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,32,33,13,0,14,15,0,0,0,0,0,45,0,9},
		{0,0,0,0,116,0,0,0,0,0,0,0,116,116,116,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,102,0,0,102,0,0,0,0,0,0,0,0,0,0,0,0,0,102,0,0,0,0,0,0,0},
		{0,104,0,104,0,0,104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,108,0,0,108,0,0,25,27,0,0,0,0,0,0,0,0,0,108,0,0,0,0,0,0,0},
		{0,0,109,0,109,109,0,0,109,27,0,0,109,109,109,0,109,0,0,109,0,0,0,0,0,0,0},
		{0,0,0,0,118,0,0,0,25,27,0,0,118,118,118,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,110,0,110,110,0,0,110,110,0,0,110,110,110,0,110,0,0,110,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,25,27,0,0,42,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,112,0,112,112,0,0,112,112,0,0,112,112,112,0,112,0,0,112,0,0,0,0,0,0,0},
		{0,0,0,0,117,0,0,0,0,0,0,0,117,117,117,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,114,0,0,0,0,0,0,0,114,114,114,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,115,0,0,0,0,0,0,0,115,115,115,0,0,0,0,0,0,0,0,0,0,0,0}
};

SyntaxAnalyzer::SyntaxAnalyzer() {
	cs = 0;
	//构造产生式对象
	syntax.push_back(ProductionFormula(SS, vector<token>{S}));// S' -> S
	syntax.push_back(ProductionFormula(S, vector<token>{C, S})); // S -> CS
	syntax.push_back(ProductionFormula(S, vector<token>{BEGIN, L, END})); //S -> begin L end
	syntax.push_back(ProductionFormula(S, vector<token>{A}));
	syntax.push_back(ProductionFormula(C, vector<token>{IF, B, THEN}));
	syntax.push_back(ProductionFormula(L, vector<token>{S}));
	syntax.push_back(ProductionFormula(L, vector<token>{K, S}));
	syntax.push_back(ProductionFormula(K, vector<token>{L, SEMICOLON}));
	syntax.push_back(ProductionFormula(A, vector<token>{ID, ASSIGNMENT, E}));
	syntax.push_back(ProductionFormula(E, vector<token>{E, ADD, E}));
	syntax.push_back(ProductionFormula(E, vector<token>{E, MUL, E}));
	syntax.push_back(ProductionFormula(E, vector<token>{SUB, E}));
	syntax.push_back(ProductionFormula(E, vector<token>{LP, E, RP}));
	syntax.push_back(ProductionFormula(E, vector<token>{ID}));
	syntax.push_back(ProductionFormula(B, vector<token>{B, OR, B}));
	syntax.push_back(ProductionFormula(B, vector<token>{B, AND, B}));
	syntax.push_back(ProductionFormula(B, vector<token>{NOT, B}));
	syntax.push_back(ProductionFormula(B, vector<token>{LP, B, RP}));
	syntax.push_back(ProductionFormula(B, vector<token>{E, ROP, E}));
	syntax.push_back(ProductionFormula(B, vector<token>{TRUE}));
	syntax.push_back(ProductionFormula(B, vector<token>{FALSE}));
}

void SyntaxAnalyzer::parse() {
	int i = 0;
	int ts = 0; //临时保存状态
	tokenst.push(EOFT); //栈底压入结束状态类型值 EOFT
	cs = 0;
	state.push(cs); //0号状态入栈
	while (true) {
		ts = state_table_SLR[cs][tokens[i].getType()];
		if (ts == 200) {
			cout << "finish!" << endl;
			break;
		}
		else if (ts <= 99) { //移入操作
			cs = ts; //修改当前状态
			tokenst.push(tokens[i]); //tokens[i].getType()
			state.push(cs);
			i++; //表示读取了一个字符
		}
		else {
			ts %= 100; //准备进行规约操作
			cout << "使用了产生式" << ts << ":\t";
			cout << syntax[ts] << endl; //输出所用的产生式对象
			int lenRight = syntax[ts].rights.size();
			for (int i = 0; i < lenRight; i++) {  //状态栈和符号栈出栈
				tokenst.pop();
				state.pop();
			}
			tokenst.push(syntax[ts].left);  //栈顶变成了产生式左部
			state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
			cs = state.top();
		}
		if (cs == 0) {
			cout << "error" << endl;
			break;
		}
		//cout << "cs = " << cs << endl;
	}
}