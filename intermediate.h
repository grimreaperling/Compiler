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



class Space { //ר����������ռ����
public:
	static int cnt;
	static string allocSpace() {
		return "$t" + std::to_string(++cnt);
	}
};

int Space::cnt = 0;  //��ʼ��Space��ľ�̬����

class Quaternion {  //������Ԫʽ����
public:
	string op, src1, src2, dst;
	Quaternion(string op, string src1, string src2, string dst) : //˫Ŀ�������+ * and or 
		op(op), src1(src1), src2(src2), dst(dst) {}
	Quaternion(string op, string src1, string dst) : //��Ŀ����� ����- not 
		op(op), src1(src1), dst(dst) {
		src2 = "_";
	}
	Quaternion(string op, string dst) : op(op), dst(dst) { //��������תָ�� jmp dst
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

class IntermediateGenerator {  //�м����������
public:
	SyntaxParsingTree tree;
	IntermediateGenerator(const SyntaxParsingTree& tree) : tree(tree) {}
	vector<Quaternion> codes; // ���ɵ��м����

	int getLine(vector<Quaternion>::iterator pos) {
		return pos - codes.begin(); //���õ�ǰ�ĵ���������±�
	}

	void solve(SymbolNode * nowSymbolNode) {
		if (nowSymbolNode->isTerminal) return; //�ս�����ý�ֱ�ӷ���
		switch (nowSymbolNode->id) {
		case 1: // S->CS
			//����鴴����������������
			{
				SymbolNode* nextSymbolNode = nowSymbolNode->nexts[0]; // C->if B then
				SymbolNode* ptrB = nextSymbolNode->nexts[1];
				solve(ptrB); //Solve(B)
				codes.push_back(Quaternion("j=", ptrB->spaceName, "true", to_string(codes.size() + 2)));
				codes.push_back(Quaternion("j", "")); //S��û�����ȿ���
				int index = codes.size() - 1;
				solve(nowSymbolNode->nexts[1]); // S->CS
				codes[index].dst = to_string(codes.size()); //�����ַ����
			}
			break;
		case 8: //A->id:=E
			solve(nowSymbolNode->nexts[2]); //���A->id:=E�е�E
			codes.push_back(Quaternion(":=", nextSpaceName(2), nextSpaceName(0)));
			break;
		case 9: //E->E+E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�E����ռ�
			codes.push_back(Quaternion("+", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 10: //E->E*E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�E����ռ�
			codes.push_back(Quaternion("*", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 11: //E->-E 
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�E����ռ�
			codes.push_back(Quaternion("-", nextSpaceName(1), nowSymbolNode->spaceName));  //ע��������1��ʾ�Ǹ�E
			break;
		case 12: //E->(E)
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = nextSpaceName(1); //ֱ���������E�Ŀռ���Ϊ����ʽ�ұߵĿռ���
			break;
		case 13: //E->id
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�E����ռ�
			codes.push_back(Quaternion("=", nextSpaceName(0), nowSymbolNode->spaceName));
			break;
		case 14: //B->B or B
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�B����ռ�
			codes.push_back(Quaternion("or", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 15: //B->B and B
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�B����ռ�
			codes.push_back(Quaternion("and", nextSpaceName(0), nextSpaceName(2), nowSymbolNode->spaceName));
			break;
		case 16: //B->not B
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�E����ռ�
			codes.push_back(Quaternion("not", nextSpaceName(1), nowSymbolNode->spaceName));  //ע��������1��ʾ�Ǹ�B
			break;
		case 17: //B->(B)
			solve(nowSymbolNode->nexts[1]);
			nowSymbolNode->spaceName = nextSpaceName(1); //ֱ���������E�Ŀռ���Ϊ����ʽ�ұߵĿռ���
			break;
		case 18: //B->E rop E
			solve(nowSymbolNode->nexts[0]);
			solve(nowSymbolNode->nexts[2]);
			nowSymbolNode->spaceName = Space::allocSpace(); //Ϊ��ߵ�B����ռ�
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
				solve(nowSymbolNode->nexts[i]);  //�Ƚ��ұߵ�
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
