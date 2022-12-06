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

	cout << "�ʷ�����������£�" << endl;
    for (token word : tokens) {
        cout << word << endl;
    }

	cout << "�﷨���������Ƶ����£�" << endl;
	SyntaxAnalyzer syntax;
	vector<ProductionFormula> orderedProducts = syntax.parse();  //�����Ƶ�
	for (ProductionFormula product : orderedProducts) {
		cout << product << endl;
	}

	cout << "��������Ҷ�ӽڵ������£�" << endl;
	SyntaxParsingTree tree(orderedProducts);
	tree.show_leaves(); //
	cout << endl;
	cout << "�������Ĳ������������£�" << endl;
	tree.show_all(); //�������

	cout << endl;
	cout << "�м�������ɽ�����£�" << endl;
	IntermediateGenerator generator(tree);
	vector<Quaternion> codes = generator.generate();  //���ɵ���Ԫʽ��������
	int line = 0;
	for (Quaternion code : codes) {
		cout << (line++) << ": " << code << endl;
	}

	return 0;
}
