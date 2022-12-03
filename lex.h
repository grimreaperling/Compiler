#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <iomanip>
#include "token.h"

using namespace std;

extern vector<token> tokens;

int state_table[13][6] = {
    {1,12,3,6,9,12},
    {1,1,2,2,2,2},
    {0,0,0,0,0,0},
    {4,4,5,4,4,4},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {7,7,8,7,7,7},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {10,10,11,10,10,10},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
};

class LexicalAnalyzer {
public:
    LexicalAnalyzer();
    LexicalAnalyzer(char* filename);
    yytokentype get_token(ifstream &infile);
    string get_lexeme();
    void print_error();
    void parse();

private:
    ifstream input_file;
    yytokentype T;
    string valid;
    string lexeme;
    string line;
    int ln;
    int pos;
    int errors;
    int cs;
};

LexicalAnalyzer::LexicalAnalyzer() {}

LexicalAnalyzer::LexicalAnalyzer(char* filename) {
    input_file.open(filename);
    if (!input_file) {
        cout << "Error: could not open \"" << filename << "\"" << endl;
        exit(1);
    }
    valid = "a0=><";
    lexeme = "";
    line = "";
    cs = 0;
    ln = 0;
    pos = 0;
    errors = 0;
}

yytokentype LexicalAnalyzer::get_token(ifstream &input_file) {
    // Initialize the state and the lexeme.
    cs = 0;
    lexeme = "";
    if (pos >= line.length()) {
        getline(input_file, line);

        if (input_file.eof()) {
            return EOFT;
        }

        ln++;
        pos = 0;
        cout << "   " << ln << ": " << line << endl;
    }

    // Remove leading whitespace
    while(isspace(line[pos]))
        pos++;
    // Remove trailing whitespace
    while(line[line.size()-1] == ' ')
        line = line.substr(0, line.size()-1);

    while(1) {
            // Consume next available character
        char c = line[pos];
        if (lexeme == ":" and c != '=') {
            pos++;
            print_error();
            errors++;
            return ERROR;
        }
        if (c == ' ' or c == '\n') {
            if (lexeme == "if") {
                pos++;
                return IF;
            }
            if (lexeme == "then") {
                pos++;
                return THEN;
            }
            if (lexeme == "begin") {
                pos++;
                return BEGIN;
            }
            if (lexeme == "true") {
                pos++;
                return TRUE;
            }
            if (lexeme == "false") {
                pos++;
                return FALSE;
            }
            if (lexeme == "and") {
                pos++;
                return AND;
            }
            if (lexeme == "or") {
                pos++;
                return OR;
            }
            if (lexeme == "not") {
                pos++;
                return NOT;
            }
        }
        lexeme += c;
        if (lexeme == "end") {
            pos++;
            return END;
        }
        if (lexeme == "+") {
            pos++;
            return ADD;
        }
        if (lexeme == "-") {
            pos++;
            return SUB;
        }
        if (lexeme == "*") {
            pos++;
            return MUL;
        }
        if (lexeme == "(") {
            pos++;
            return LP;
        }
        if (lexeme == ")") {
            pos++;
            return RP;
        }
        if (lexeme == ";") {
            pos++;
            return SEMICOLON;
        }
        if (lexeme == ":=") {
            pos++;
            return ASSIGNMENT;
        }

        if(isalpha(c) || c == '_')
            c = 'a';
        if(isdigit(c))
            c = '0';

        int col = 0;
        // Find the correct column in the state table
        while(col < 5 && c != valid[col]) {
            col++;
        }
        // Update state based on character just consumed
        cs = state_table[cs][col];

        switch(cs) {
            case 0:
            case 1:
                pos++;
                break;
            case 2:
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return ID;
            case 3:
                pos++;
                break;
            case 4:
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return ASSIGNMENT;
            case 5:
                pos++;
                return ROP;
            case 6:
                pos++;
                break;
            case 7:
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return ROP;
            case 8:
                pos++;
                return ROP;
            case 9:
                pos++;
                break;
            case 10:
                lexeme = lexeme.substr(0, lexeme.size()-1);
                return ROP;
            case 11:
                pos++;
                return ROP;
            case 12:
                pos++;
                if (c != ':') {
                    print_error();
                    errors++;
                    return ERROR;
                }
            }
    }
}

string LexicalAnalyzer::get_lexeme() {
    return lexeme;
}

void LexicalAnalyzer::print_error() {
    cout << "Error at " << ln << "," << pos << ": ";
    cout << "Invalid character found: " << lexeme << endl;
}

void LexicalAnalyzer::parse() {
    while ((T = get_token(input_file)) != EOFT) {
        string word = "";
        if (T == ROP or T == ID) {
            word = get_lexeme();
        }
        token res(T, word);
        tokens.push_back(res);
    if (errors > 0)
        cout << errors << " errors found in input file" << endl;
    }
    token end(EOFT, "#");
    tokens.push_back(end);
}
