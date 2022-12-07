#include "token.h"

vector<string> names {
    "BEGIN",
    "END",
    "IF",
    "THEN",
    "SEMICOLON",
    "ID",
    "ASSIGNMENT",
    "ADD",
    "MUL",
    "SUB",
    "LP",
    "RP",
    "OR",
    "AND",
    "NOT",
    "ROP",
    "TRUE",
    "FALSE",
    "EOFT",
    "S",
    "C",
    "L",
    "A",
    "B",
    "K",
    "E",
    "ERROR",
    "S\'"
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
    if (type == ID or type == ROP) {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& oss, const token& word) {
    oss << "Type: " << names[word.getType() - 1];
    if (word.isVal()) {
        oss << " Value: " << word.getVal() << " ";
    } else {
        oss << " ";
    }
    return oss;
}

void token::setVal(string value) {
    this->val = value;
}
