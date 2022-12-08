#ifndef INTERMIDIATE_H
#define INTERMIDIATE_H

#include "tree.h"
#include <iostream>
#include <string>
#include <list>

#define nextSpaceName(num) ((nowSymbolNode->nexts[num])->spaceName)

using std::string;
using std::ostream;
using std::list;



class Space { //专门用来分配空间的类
public:
	static int cnt;
	static string allocSpace() {
		return "$t" + std::to_string(++cnt);
	}
};

int Space::cnt = 0;  //初始化Space类的静态变量

class Quaternion {  //构造四元式类型
public:
	string op, src1, src2, dst;
	Quaternion(string op, string src1, string src2, string dst) : //双目运算比如+ * and or 
		op(op), src1(src1), src2(src2), dst(dst) {}
	Quaternion(string op, string src1, string dst) : //单目运算符 比如- not 
		op(op), src1(src1), dst(dst) {
		src2 = "_";
	}
	Quaternion(string op, string dst) : op(op), dst(dst) { //无条件跳转指令 jmp dst
		src1 = src2 = "_";
	} 
};

ostream& operator<< (ostream& out, const Quaternion& q) {
	out << "(";
	out << q.op << ", ";
	out << q.src1 << ", ";
	out << q.src2 << ", ";
	out << q.dst;
	out << ")";
	return out;
}

class IntermediateGenerator {  //中间代码生成类
public:
	SyntaxParsingTree tree;
	IntermediateGenerator(const SyntaxParsingTree& tree) : tree(tree) {}
	vector<Quaternion> codes; // 生成的中间代码

	int getLine(vector<Quaternion>::iterator pos) {
		return pos - codes.begin(); //利用当前的迭代器获得下标
	}

	void solve(SymbolNode * nowSymbolNode) {
		if (nowSymbolNode->isTerminal) return; //终结符不用解直接返回
		switch (nowSymbolNode->id) {
		case 1: // S->CS
			//代码块创建变量的生存周期
			{
				SymbolNode* nextSymbolNode = nowSymbolNode->nexts[0]; // C->if B then
				SymbolNode* ptrB = nextSymbolNode->nexts[1];
				solve(ptrB); //Solve(B)
				codes.push_back(Quaternion("j=", ptrB->spaceName, "true", to_string(codes.size() + 2)));
				codes.push_back(Quaternion("j", "")); //S还没解析先空着
				int index = codes.size() - 1;
				solve(nowSymbolNode->nexts[1]); // S->CS
				codes[index].dst = to_string(codes.size()); //回填地址行数
			}
			break;
		case 8: //A->id:=E
			solve(nowSymbolNode->nexts[2]); //解决A->id:=E中的E
			codes.push_back(Quaternion(":=", nextSpaceName(2), nextSpaceName(0)));
			break;
		case 9: //E->E+E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的E分配空间
			codes.push_back(Quaternion("+", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 10: //E->E*E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的E分配空间
			codes.push_back(Quaternion("*", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 11: //E->-E 
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的E分配空间
			codes.push_back(Quaternion("-", nextSpaceName(1), nowSymbolNode->spaceName));  //注意这里是1表示那个E
			break;
		case 12: //E->(E)
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = nextSpaceName(1); //直接设置左边E的空间名为产生式右边的空间名
			break;
		case 13: //E->id
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的E分配空间
			codes.push_back(Quaternion("=", nextSpaceName(0), nowSymbolNode->spaceName));
			break;
		case 14: //B->B or B
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的B分配空间
			codes.push_back(Quaternion("or", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 15: //B->B and B
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的B分配空间
			codes.push_back(Quaternion("and", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 16: //B->not B
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的E分配空间
			codes.push_back(Quaternion("not", nextSpaceName(1), nowSymbolNode->spaceName));  //注意这里是1表示那个B
			break;
		case 17: //B->(B)
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = nextSpaceName(1); //直接设置左边E的空间名为产生式右边的空间名
			break;
		case 18: //B->E rop E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //为左边的B分配空间
			codes.push_back(Quaternion(nextSpaceName(1), nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 19: //B->true
			nowSymbolNode->spaceName = "true";
			//codes.push_back(Quaternion("=", nextSpaceName(0), nowSymbolNode->spaceName));
			break;
		case 20: //B->false
			nowSymbolNode->spaceName = "false";
			//codes.push_back(Quaternion("=", nextSpaceName(0), nowSymbolNode->spaceName));
			break;
		default:
			/*int len = nowSymbolNode->nexts.size();
			for (int i = len - 1; i >= 0; i--) {
				solve(nowSymbolNode->nexts[i]);  //先解右边的
			}*/
			for (SymbolNode* next : nowSymbolNode->nexts) {
				solve(next);
			}
		}
	}

	vector<Quaternion> generate() {
		solve(tree.root);
		return codes;
	}
};

#endif
