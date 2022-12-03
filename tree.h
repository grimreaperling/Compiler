#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stack>

#include "token.h"
#include "mysyntax.h"


extern vector<string> names; // �����ⲿ������

class SymbolNode { //������Žڵ�����ݽṹ
public:
	//static vector<int> nonterminalSymbols;
	token symbol;  //��ǰ�ڵ��ķ���
	bool isTerminal; //��ǰ�ڵ��Ƿ�Ϊ�ս��
	vector<SymbolNode*> nexts; //�ӽڵ�ĵ�ַ
	SymbolNode(token word) : symbol(word) {
		if (symbol.getType() >= 20 && symbol.getType() <= 26)
			isTerminal = false;
		else
			isTerminal = true;
	}
	//SymbolNode(token word, bool isOver) : symbol(word), isTerminal(isOver) { }
};

//vector<int> SymbolNode::nonterminalSymbols = {
//	S, //�����￪ʼ���涼�Ƿ��ս��
//	C,
//	L,
//	A,
//	B,
//	K,
//	E
//}

class SyntaxParsingTree {
public:
	SymbolNode* root; //�������ĸ��ڵ��ַ
	stack<SymbolNode*> rightestNonterminalSymbols; //���ҷ��ս����ջ

	SyntaxParsingTree(vector<ProductionFormula> orderedProducts);
	~SyntaxParsingTree() {
		delete root;
	}

	void DFS(SymbolNode* root) {
		if (!root->nexts.size()) {
			token symbol = root->symbol;
			if (symbol.isVal()) cout << symbol.getVal();
			else cout << names[symbol.getType() - 1];
			cout << " ";
			return;
		}
		for (SymbolNode* childptr : root->nexts) {
			DFS(childptr);
		}
	}

	void show() {
		//���������ʾ��������Ҷ�ӽڵ�
		DFS(root);
	}
};

SyntaxParsingTree::SyntaxParsingTree(vector<ProductionFormula> orderedProducts) {
	root = new SymbolNode(S);
	rightestNonterminalSymbols.push(root);
	for (ProductionFormula orderedProduct : orderedProducts) {
		SymbolNode* leftptr = rightestNonterminalSymbols.top();
		rightestNonterminalSymbols.pop();
		vector<token> rights = orderedProduct.rights;
		for (token right : rights) {
			SymbolNode* p = new SymbolNode(right);
			if (p->isTerminal == false) {
				rightestNonterminalSymbols.push(p);
			}
			leftptr->nexts.push_back(p);
		}
	}
}

#endif