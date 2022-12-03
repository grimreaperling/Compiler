#include "lex.h"
#include "token.h"
//#include "syntax.h"
#include "mysyntax.h"
#include "tree.h"

#include <vector>
using namespace std;
vector<token> tokens;

int main(int argc, char** argv) {
    /*if (argc < 2) {
		cout << "Usage: ./Main  <filename>" << endl;
		exit (1);
	}*/

	LexicalAnalyzer lex((char*)"4.txt");
	lex.parse();

    for (token word : tokens) {
        cout << word << endl;
    }

	SyntaxAnalyzer syntax;
	vector<ProductionFormula> orderedProducts = syntax.parse();  //最右推导
	for (ProductionFormula product : orderedProducts) {
		cout << product << endl;
	}
	cout << "搜索树的叶子节点结果如下：" << endl;
	SyntaxParsingTree tree(orderedProducts);
	tree.show();

	return 0;
}
