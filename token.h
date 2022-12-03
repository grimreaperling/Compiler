#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <vector>
#include <iostream>
using std::string;      using std::vector;
using std::cout;        using std::endl;

typedef enum {
    BEGIN = 1,
    END = 2,
    IF = 3,
    THEN = 4,
    SEMICOLON = 5,//;
    ID = 6,
    ASSIGNMENT = 7,//:=
    ADD = 8,//+
    MUL = 9,//*
    SUB = 10,//-
    LP = 11,//(
    RP = 12,
    OR = 13,
    AND = 14,
    NOT = 15,
    ROP = 16,
    TRUE = 17,
    FALSE = 18,
    EOFT = 19,
    S = 20,
    C = 21,
    L = 22,
    A = 23,
    B = 24,
    K = 25,
    E = 26,
    ERROR = 27,
    SS = 28, //S'
} yytokentype;   

class token {
public: 
    token();

    token(yytokentype type, string val="");
    
    ~token();

    yytokentype getType() const;

    string getVal() const;
    
    bool isVal() const;

    friend std::ostream& operator<<(std::ostream& oss, const token& word);

private:
    yytokentype type;
    string val;
};
#endif
