#include "lex.h"
#include "token.h"
#include "syntax.h"
#include <vector>
using namespace std;
vector<token> tokens;

int main(int argc, char **argv) {
    if (argc < 2) {
		cout << "Usage: ./Main  <filename>" << endl;
		exit (1);
	}

	LexicalAnalyzer lex(argv[1]);
	lex.parse();

    for (token word : tokens) {
        cout << word << endl;
    }

	SyntaxAnalyzer syntax(argv[1]);
	syntax.parse();

	return 0;
}
