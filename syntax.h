#ifndef SYNTAX_H
#define SYNTAX_H
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include "token.h"
using namespace std;
extern vector<token> tokens; //引入外部变量数组
extern vector<string> names; // 引入外部的名称
class ProductionFormula {  //产生式类型
public:
	int id; //产生式的序号
	token left;
	vector<token> rights;
	ProductionFormula(int id, token left, vector<token> rights) : id(id), left(left), rights(rights) {}
};

ostream& operator<< (ostream& out, ProductionFormula produc) {
	out << produc.id << ": ";
	out << "{" << names[produc.left.getType() - 1] << "," << produc.left.getVal() << "}";
	out << " -> ";
	for (token right : produc.rights) {
		out << "{";
		out << names[right.getType() - 1];
		out << ",";
		out << right.getVal();
		out << "} ";
	}
	return out;
}


typedef vector<ProductionFormula> Syntax;

class SyntaxAnalyzer {
public:
	//SyntaxAnalyzer(Syntax syntax);
	SyntaxAnalyzer();
	vector<ProductionFormula> parse(); //解析出一个所使用的产生式序列(正向的)
	//SymbolNode* getSyntaxParsingTree(); //

private:
	static int state_table_SLR[43][27]; //SLR自动机数组
	stack<int> state; //状态栈
	stack<token> tokenst; //符号栈
	int cs; // 当前状态
	Syntax syntax; //表示当前分析器所能调用的全部产生式文法
	//
};
//语法分析类的静态成员初始化
int SyntaxAnalyzer::state_table_SLR[43][27] = {
		{0,3,0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,4,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,200,0,0,0,0,0,0,0},
		{0,3,0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7,2,0,4,0,0,0},
		{0,3,0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,10,2,9,4,0,8,0},
		{0,0,103,0,0,103,0,0,0,0,0,0,0,0,0,0,0,0,0,103,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,0,0,15,13,0,0,0,12,0,17,18,0,0,0,0,0,11,0,14},
		{0,0,0,0,0,0,0,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,101,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0},
		{0,3,0,5,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,42,2,0,4,0,0,0},
		{0,0,20,0,0,21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,105,0,0,105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,22,0,0,0,0,0,0,0,0,23,24,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,0,0,15,13,0,23,24,12,0,17,18,0,0,0,0,0,25,0,14},
		{0,0,0,0,0,0,16,0,0,0,15,13,0,0,0,12,0,17,18,0,0,0,0,0,27,0,26},
		{0,0,0,0,0,0,0,0,28,30,0,0,0,0,0,0,29,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,28,30,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31},
		{0,0,113,0,113,113,0,0,113,113,0,0,113,113,113,0,113,0,0,113,0,0,0,0,0,0,0},
		{0,0,0,0,119,0,0,0,0,0,0,0,119,119,119,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,120,0,0,0,0,0,0,0,120,120,120,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,0,0,15,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33},
		{0,0,102,0,0,102,0,0,0,0,0,0,0,0,0,0,0,0,0,102,0,0,0,0,0,0,0},
		{0,107,0,107,0,0,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,104,0,104,0,0,104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,0,0,15,13,0,23,0,12,0,17,18,0,0,0,0,0,34,0,14},
		{0,0,0,0,0,0,16,0,0,0,15,13,0,23,24,12,0,17,18,0,0,0,0,0,35,0,14},
		{0,0,0,0,116,0,0,0,0,0,0,0,116,116,116,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,28,30,0,0,36,0,0,0,29,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,37,23,24,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,28,0,15,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38},
		{0,0,0,0,0,0,16,0,0,0,15,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39},
		{0,0,0,0,0,0,16,0,28,30,15,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40},
		{0,0,111,0,111,111,0,0,111,111,0,0,111,111,111,0,111,0,0,111,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,16,0,0,0,15,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41},
		{0,0,108,0,0,108,0,0,28,30,0,0,0,0,0,0,0,0,0,108,0,0,0,0,0,0,0},
		{0,0,0,0,114,0,0,0,0,0,0,0,114,114,24,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,115,0,0,0,0,0,0,0,115,115,115,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,112,0,112,112,0,0,112,112,0,0,112,112,112,0,112,0,0,112,0,0,0,0,0,0,0},
		{0,0,0,0,117,0,0,0,0,0,0,0,117,117,117,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,109,0,109,109,0,0,109,30,0,0,109,109,109,0,109,0,0,109,0,0,0,0,0,0,0},
		{0,0,0,0,118,0,0,0,28,30,0,0,118,118,118,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,110,0,110,110,0,0,110,110,0,0,110,110,110,0,110,0,0,110,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,28,30,0,0,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,106,0,0,106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};




SyntaxAnalyzer::SyntaxAnalyzer() {
	cs = 0;
	//构造产生式对象
	syntax.push_back(ProductionFormula(0, SS, vector<token>{S}));// S' -> S
	syntax.push_back(ProductionFormula(1, S, vector<token>{C, S})); // S -> CS
	syntax.push_back(ProductionFormula(2, S, vector<token>{BEGIN, L, END})); //S -> begin L end
	syntax.push_back(ProductionFormula(3, S, vector<token>{A}));
	syntax.push_back(ProductionFormula(4, C, vector<token>{IF, B, THEN}));
	syntax.push_back(ProductionFormula(5, L, vector<token>{S}));
	syntax.push_back(ProductionFormula(6, L, vector<token>{K, S}));
	syntax.push_back(ProductionFormula(7, K, vector<token>{L, SEMICOLON})); //K -> L ;
	syntax.push_back(ProductionFormula(8, A, vector<token>{ID, ASSIGNMENT, E}));
	syntax.push_back(ProductionFormula(9, E, vector<token>{E, ADD, E}));
	syntax.push_back(ProductionFormula(10, E, vector<token>{E, MUL, E}));
	syntax.push_back(ProductionFormula(11, E, vector<token>{SUB, E}));
	syntax.push_back(ProductionFormula(12, E, vector<token>{LP, E, RP}));
	syntax.push_back(ProductionFormula(13, E, vector<token>{ID})); //13: 注意ID
	syntax.push_back(ProductionFormula(14, B, vector<token>{B, OR, B}));
	syntax.push_back(ProductionFormula(15, B, vector<token>{B, AND, B}));
	syntax.push_back(ProductionFormula(16, B, vector<token>{NOT, B}));
	syntax.push_back(ProductionFormula(17, B, vector<token>{LP, B, RP}));
	syntax.push_back(ProductionFormula(18, B, vector<token>{E, ROP, E})); //18: 注意ROP的val
	syntax.push_back(ProductionFormula(19, B, vector<token>{TRUE}));
	syntax.push_back(ProductionFormula(20, B, vector<token>{FALSE}));
}


vector<ProductionFormula> SyntaxAnalyzer::parse() {
	int i = 0;
	int ts = 0; //临时保存状态
	tokenst.push(EOFT); //栈底压入结束状态类型值 EOFT
	cs = 0;
	state.push(cs); //0号状态入栈
	vector<ProductionFormula> productionFormulas; //规约顺序
	while (true) {
		ts = state_table_SLR[cs][tokens[i].getType()];
		if (ts == 200) {
			cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>Success<<<<<<<<<<<<<<<<<<<<<<<" << endl;
			break;
		}
		else if (ts <= 99) { //移入操作
			cs = ts; //修改当前状态
			tokenst.push(tokens[i]); //tokens[i].getType()
			state.push(cs);
			i++; //表示读取了一个字符
		}
		else { //规约动作
			ts %= 100; //准备进行规约操作
			//cout << "使用了产生式" << ts << ":\t";
			//cout << syntax[ts] << endl; //输出所用的产生式对象
			//for (int j = syntax[ts].rights.size() - 1; j >= 0; j--) {
			//	rights.push_back(tokens[i - 1 - j]);
			//}
			//ProductionFormula tmpProductionFormula = syntax[ts];
			//productionFormulas.push_back(tmpProductionFormula); //按照规约顺序放入产生式
			vector<token> rights;
			int lenRight = syntax[ts].rights.size();
			for (int i = 0; i < lenRight; i++) {  //状态栈和符号栈出栈
				rights.push_back(tokenst.top());
				tokenst.pop();
				state.pop();
			}
			reverse(rights.begin(), rights.end());
			ProductionFormula tmpProductionFormula(syntax[ts].id, syntax[ts].left, rights);
			productionFormulas.push_back(tmpProductionFormula); //按照规约顺序放入产生式

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
	reverse(productionFormulas.begin(), productionFormulas.end());
	//转化成推导顺序
	return productionFormulas;
}
#endif
