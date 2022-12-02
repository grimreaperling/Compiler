#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "token.h"
using namespace std;
extern vector<token> tokens; //引入外部变量数组

class ProductionFormula {  //产生式类型
public:
	token left;
	vector<token> rights;
	ProductionFormula(token left, vector<token> rights) : left(left), rights(rights) {}
};

typedef vector<ProductionFormula> Syntax;

class SyntaxAnalyzer {
public:
	SyntaxAnalyzer(Syntax syntax);
	void parse();

private:
	static int state_table_SLR[47][27]; //SLR自动机数组
	stack<int> state; //状态栈
	stack<token> tokenst; //符号栈
	int cs; // 当前状态
};
//语法分析类的静态成员初始化
int SyntaxAnalyzer::state_table_SLR[47][27] = {
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
		{0,0,0,0,0,0,21,0,0,0,10,32,0,0,0,13,0,14,15,0,0,0,0,0,31,0,30},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,33,34,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,33,34,13,0,14,15,0,0,0,0,0,35,0,9},
		{0,0,0,0,119,0,0,0,0,0,0,0,119,119,119,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,120,0,0,0,0,0,0,0,120,120,120,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,101,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,36,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,105,0,0,105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,37,0,0,0,0,0,0,0,0,33,34,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,113,0,113,113,0,0,113,113,0,0,113,113,113,0,113,0,0,113,0,0,0,0,0,0,0},
		{0,0,106,0,0,106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,107,0,107,0,0,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38},
		{0,0,0,0,0,0,21,0,25,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40},
		{0,0,0,0,0,0,21,0,25,27,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41},
		{0,0,111,0,111,111,0,0,111,111,0,0,111,111,111,0,111,0,0,111,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42},
		{0,0,0,0,0,0,0,0,25,27,0,0,43,0,0,0,26,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,44,33,34,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,0,0,13,0,14,15,0,0,0,0,0,31,0,9},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,33,0,13,0,14,15,0,0,0,0,0,45,0,9},
		{0,0,0,0,0,0,21,0,0,0,10,11,0,33,34,13,0,14,15,0,0,0,0,0,46,0,9},
		{0,0,0,0,116,0,0,0,0,0,0,0,116,116,116,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,102,0,0,102,0,0,0,0,0,0,0,0,0,0,0,0,0,102,0,0,0,0,0,0,0},
		{0,104,0,104,0,0,104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,108,0,0,108,0,0,25,27,0,0,0,0,0,0,0,0,0,108,0,0,0,0,0,0,0},
		{0,0,109,0,109,109,0,0,109,27,0,0,109,109,109,0,109,0,0,109,0,0,0,0,0,0,0},
		{0,0,0,0,118,0,0,0,25,27,0,0,118,118,118,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,110,0,110,110,0,0,110,110,0,0,110,110,110,0,110,0,0,110,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,25,27,0,0,43,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,112,0,112,112,0,0,112,112,0,0,112,112,112,0,112,0,0,112,0,0,0,0,0,0,0},
		{0,0,0,0,117,0,0,0,0,0,0,0,117,117,117,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,114,0,0,0,0,0,0,0,114,114,114,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,115,0,0,0,0,0,0,0,115,115,115,0,0,0,0,0,0,0,0,0,0,0,0}
};

SyntaxAnalyzer::SyntaxAnalyzer(Syntax syntax) {
	cs = 0;

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
			ts %= 100; //进行规约操作
			switch (ts) {
			case 1:
				cout << "S->CS" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				state.pop();
				state.pop();
				tokenst.push(S);  //状态栈栈顶变成了S
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 2:
				cout << "S->begin L end" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(S);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 3:
				cout << "S->A" << endl;
				tokenst.pop();//出栈，归约替换
				state.pop();
				tokenst.push(S);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 4:
				cout << "C->if B then" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(C);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 5:
				cout << "L->S" << endl;
				tokenst.pop();//出栈，归约替换
				state.pop();
				tokenst.push(L);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 6:
				cout << "L->K S" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				state.pop();
				state.pop();
				tokenst.push(L);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 7:
				cout << "K->L;" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				state.pop();
				state.pop();
				tokenst.push(K);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 8:
				cout << "A->id:=E" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(A);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 9:
				cout << "E->E+E" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(E);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 10:
				cout << "E->E*E" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(E);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 11:
				cout << "E->-E" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				state.pop();
				state.pop();
				tokenst.push(E);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 12:
				cout << "E->(E)" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(E);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 13:
				cout << "E->id" << endl;
				tokenst.pop();//出栈，归约替换
				state.pop();
				tokenst.push(E);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 14:
				cout << "B->B or B" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 15:
				cout << "B-> B and B" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 16:
				cout << "B->not B" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				state.pop();
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 17:
				cout << "B->(B)" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 18:
				cout << "B->E rop E" << endl;
				tokenst.pop();//出栈，归约替换
				tokenst.pop();
				tokenst.pop();
				state.pop();
				state.pop();
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 19:
				cout << "B->true" << endl;
				tokenst.pop();//出栈，归约替换
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			case 20:
				cout << "B->false" << endl;
				tokenst.pop();//出栈，归约替换
				state.pop();
				tokenst.push(B);
				state.push(state_table_SLR[state.top()][tokenst.top().getType()]);
				cs = state.top();
				break;
			}
		}
	}
}