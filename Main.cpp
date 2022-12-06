#include "lex.h"
#include "token.h"
//#include "syntax.h"
#include "mysyntax.h"
#include "tree.h"
#include "intermediate.h"


#include <vector>
using namespace std;
vector<token> tokens;


int main(int argc, char** argv) {
    /*if (argc < 2) {
		cout << "Usage: ./Main  <filename>" << endl;
		exit (1);
	}*/

	LexicalAnalyzer lex((char*)"2.txt");
	lex.parse();

	cout << "词法分析结果如下：" << endl;
    for (token word : tokens) {
        cout << word << endl;
    }

	cout << "语法分析最右推导如下：" << endl;
	SyntaxAnalyzer syntax;
	vector<ProductionFormula> orderedProducts = syntax.parse();  //最右推导
	for (ProductionFormula product : orderedProducts) {
		cout << product << endl;
	}

	cout << "搜索树的叶子节点结果如下：" << endl;
	SyntaxParsingTree tree(orderedProducts);
	tree.show_leaves(); //
	cout << endl;
	cout << "搜索树的层序遍历结果如下：" << endl;
	tree.show_all(); //层序遍历

	cout << endl;
	cout << "中间代码生成结果如下：" << endl;
	IntermediateGenerator generator(tree);
	vector<Quaternion> codes = generator.generate();  //生成的四元式代码链表
	int line = 0;
	for (Quaternion code : codes) {
		cout << (line++) << ": " << code << endl;
	}

	return 0;
}
