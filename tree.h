#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "token.h"
#include "mysyntax.h"


extern vector<string> names; // �����ⲿ������

class SymbolNode { //������Žڵ�����ݽṹ
public:
	//static vector<int> nonterminalSymbols;
	int id; //���ս�����õĲ���ʽ����
	token symbol;  //��ǰ�ڵ��ķ���
	bool isTerminal; //��ǰ�ڵ��Ƿ�Ϊ�ս��
	vector<SymbolNode*> nexts; //�ӽڵ�ĵ�ַ
	SymbolNode(token word) : symbol(word), id(-1) {  //�����Ȱѷ��ѷ�ʽ����Ϊ������
		if (symbol.getType() >= 20 && symbol.getType() <= 26) {
			isTerminal = false;
		}
		else {
			isTerminal = true;
		}
	}
	//SymbolNode(token word, bool isOver) : symbol(word), isTerminal(isOver) { }
};

ostream& operator<< (ostream& out, const SymbolNode& symbolNode) {
	out << "{" << symbolNode.symbol << "id: " << symbolNode.id << "}";
	return out;
}

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

	void show_leaves() {
		//���������ʾ��������Ҷ�ӽڵ�
		DFS(root);
	}
	void show_all() {
		int depth = 0;
		queue<pair<SymbolNode*, int> > q;
		q.push(make_pair(root, 0));
		while (!q.empty()) {
			pair<SymbolNode*, int> tmpPair = q.front();
			SymbolNode* p = tmpPair.first;
			int nowDepth = tmpPair.second;
			q.pop();
			if (nowDepth > depth) {
				cout << endl;
				depth = nowDepth;
			}
			cout << *p << " ";
			for (SymbolNode* next : p->nexts) {
				q.push(make_pair(next, nowDepth + 1));
			}
		}
	}
};

SyntaxParsingTree::SyntaxParsingTree(vector<ProductionFormula> orderedProducts) {
	root = new SymbolNode(S);
	rightestNonterminalSymbols.push(root);
	for (ProductionFormula orderedProduct : orderedProducts) {
		SymbolNode* leftptr = rightestNonterminalSymbols.top();
		rightestNonterminalSymbols.pop();
		leftptr->id = orderedProduct.id; //���ʱ�����ȷ�����ڵ�ķ��ѷ�ʽ
		vector<token> rights = orderedProduct.rights;
		for (token right : rights) {
			SymbolNode* p = new SymbolNode(right);
			if (p->isTerminal == false) {
				rightestNonterminalSymbols.push(p); //�������ҷ��ս��ջ��
			}
			leftptr->nexts.push_back(p);
		}
	}
}

#endif