#include "lex.h"
#include "token.h"
#include "syntax.h"
#include "tree.h"
#include "intermediate.h"
#include <vector>
using namespace std;
vector<token> tokens;


int main(int argc, char** argv) {
    if (argc < 2) {
		cout << "Usage: ./Main  <filename>" << endl;
		exit (1);
	}

	LexicalAnalyzer lex(argv[1]);
	lex.parse();

	cout << "This is the result of the lexical analysis." << endl;
    for (token word : tokens) {
        cout << word << endl;
    }

	cout << "This is the result of the syntax analysis." << endl;
	SyntaxAnalyzer syntax;
	vector<ProductionFormula> orderedProducts = syntax.parse();  
	for (ProductionFormula product : orderedProducts) {
		cout << product << endl;
	}

	cout << "The leaves of the Abstract Syntax Tree." << endl;
	SyntaxParsingTree tree(orderedProducts);
	tree.show_leaves(); //
	cout << endl;
	cout << "All the nodes from the Abstract Synatx Tree." << endl;
	tree.show_all(); 

	cout << endl;
	cout << "This is the result of the intermediate code generation." << endl;
	IntermediateGenerator generator(tree);
	vector<Quaternion> codes = generator.generate();
	int line = 0;
	for (Quaternion code : codes) {
		cout << (line++) << ": " << code << endl;
	}

	return 0;
}
