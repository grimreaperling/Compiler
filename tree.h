#include "token.h"
#include <string>
#include <vector>

class SymbolNode {
public:
	token symbol;  //��ǰ�ڵ��ķ���
	bool isTerminal; //��ǰ�ڵ��Ƿ�Ϊ�ս��
	vector<SymbolNode*> nexts; //�ӽڵ�ĵ�ַ
	SymbolNode(token word, bool isOver): symbol(word), isTerminal(isOver) { }

};
class SyntaxParsingTree {
public:
	SymbolNode* root; //�������ĸ��ڵ��ַ
	SyntaxParsingTree(token word, bool isOver) {
		root = new SymbolNode(word, isOver);
	}
	~SyntaxParsingTree() {
		delete root;
	}
	void show() {
		//���������ʾ������
	}
};