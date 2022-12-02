#include "lex.h"
#include "token.h"
#include "syntax.h"
#include <vector>
using namespace std;
vector<token> tokens;

int main(/*int argc, char** argv*/) {
    /*if (argc < 2) {
		cout << "Usage: ./Main  <filename>" << endl;
		exit (1);
	}*/

	LexicalAnalyzer lex((char *)"1.txt");
	lex.parse();

    for (token word : tokens) {
        cout << word << endl;
    }

	SyntaxAnalyzer syntax;
	syntax.parse();

	return 0;
}
