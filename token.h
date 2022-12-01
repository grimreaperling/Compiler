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
    OR = 5,
    AND = 6,
    NOT = 7,
    SEMICOLON = 8,
    ASSIGNMENT = 9,
    ADD = 10,
    MUL = 11,
    SUB = 12,
    TRUE = 13,
    FALSE = 14,
    LP = 15,
    RP = 16,
    ROP = 17,
    ID = 18,
    ERROR = 19,
    EOFT = 20
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
