#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "token.h"
#include "mysyntax.h"


extern vector<string> names; // 引入外部的名称

class SymbolNode { //定义符号节点的数据结构
public:
	//static vector<int> nonterminalSymbols;
	int id; //非终结符所用的产生式类型
	token symbol;  //当前节点存的符号
	bool isTerminal; //当前节点是否为终结符
	vector<SymbolNode*> nexts; //子节点的地址
	SymbolNode(token word) : symbol(word), id(-1) {  //这里先把分裂方式设置为不分裂
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
//	S, //从这里开始下面都是非终结符
//	C,
//	L,
//	A,
//	B,
//	K,
//	E
//}

class SyntaxParsingTree {
public:
	SymbolNode* root; //整课树的根节点地址
	stack<SymbolNode*> rightestNonterminalSymbols; //最右非终结符的栈

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
		//这个函数显示整棵树的叶子节点
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
		leftptr->id = orderedProduct.id; //这个时候才能确定父节点的分裂方式
		vector<token> rights = orderedProduct.rights;
		for (token right : rights) {
			SymbolNode* p = new SymbolNode(right);
			if (p->isTerminal == false) {
				rightestNonterminalSymbols.push(p); //丢入最右非终结符栈中
			}
			leftptr->nexts.push_back(p);
		}
	}
}

#endif