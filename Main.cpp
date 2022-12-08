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

    cout << "┌────────────────────────┐" << endl;
    cout << "│                        │" << endl; 
    cout << "│    Lexical Analysis    │" << endl;
    cout << "│                        │" << endl;
    cout << "└────────────────────────┘" << endl;
    for (token word : tokens) {
        cout << word << endl;
    }

    cout << "┌────────────────────────┐" << endl;
    cout << "│                        │" << endl; 
    cout << "│     Syntax Analysis    │" << endl;
    cout << "│                        │" << endl;
    cout << "└────────────────────────┘" << endl;
	SyntaxAnalyzer syntax;
	vector<ProductionFormula> orderedProducts = syntax.parse();  
	for (ProductionFormula product : orderedProducts) {
		cout << product << endl;
	}

    cout << "┌────────────────────────┐" << endl;
    cout << "│                        │" << endl; 
    cout << "│  Abstract Syntax Tree  │" << endl;
    cout << "│                        │" << endl;
    cout << "└────────────────────────┘" << endl;
	SyntaxParsingTree tree(orderedProducts);
    cout << "Leaves:" << endl;
	tree.show_leaves(); 
	cout << endl;
    cout << "Nodes:" << endl;
	tree.show_all(); 
    cout << endl;

    cout << "┌──────────────────────────────┐" << endl;
    cout << "│                              │" << endl; 
    cout << "│ Intermediate Code Generation │" << endl;
    cout << "│                              │" << endl;
    cout << "└──────────────────────────────┘" << endl;
	IntermediateGenerator generator(tree);
	vector<Quaternion> codes = generator.generate();
	int line = 0;
	for (Quaternion code : codes) {
		cout << (line++) << ": " << code << endl;
	}

	return 0;
}
