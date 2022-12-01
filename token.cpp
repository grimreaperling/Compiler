#include "token.h"

vector<string> names {
    "BEGIN",
    "END",
    "IF",
    "THEN",
    "OR",
    "AND",
    "NOT",
    "SEMICOLON",
    "ASSIGNMENT",
    "ADD",
    "MUL",
    "SUB",
    "TRUE",
    "FALSE",
    "LP",
    "RP",
    "ROP",
    "ID",
    "ERROR",
    "EOFT"
};

token::token() {}
token::~token() {}

token::token(yytokentype type, string val) :
    type(type),
    val(val) {}

yytokentype token::getType() const {
    return type;
}

string token::getVal() const {
    return val;
}

/* Return whether this token contains a value.*/
bool token::isVal() const {
    if (type >=1 and type <= 16) {
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& oss, const token& word) {
    oss << "Type: " << names[word.getType() - 1];
    if (word.isVal()) {
        oss << " Value: " << word.getVal() << endl;
    } else {
        oss << " " << endl;
    }
    return oss;
}
