#include "token.h"
#include <string>
#include <vector>

class SymbolNode {
public:
	token symbol;  //当前节点存的符号
	bool isTerminal; //当前节点是否为终结符
	vector<SymbolNode*> nexts; //子节点的地址
	SymbolNode(token word, bool isOver): symbol(word), isTerminal(isOver) { }

};
class SyntaxParsingTree {
public:
	SymbolNode* root; //整课树的根节点地址
	SyntaxParsingTree(token word, bool isOver) {
		root = new SymbolNode(word, isOver);
	}
	~SyntaxParsingTree() {
		delete root;
	}
	void show() {
		//这个函数显示整棵树
	}
};