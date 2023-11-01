#pragma once

#include "tokenizer.h"
#include <bits/stdc++.h>
#include "token_printer.h"

using namespace std;

inline bool isAddition(TValue *val) {
    return val->type == Tokens::OPERATOR && get<char>(val->data) == '+';
}

inline bool isSub(TValue *val) {
    return val->type == Tokens::OPERATOR && get<char>(val->data) == '-';
}

inline bool isMul(TValue *val) {
    return val->type == Tokens::OPERATOR && get<char>(val->data) == '*';
}

inline bool isDiv(TValue *val) {
    return val->type == Tokens::OPERATOR && get<char>(val->data) == '/';
}

inline bool isFac(TValue *val) {
    return val->type == Tokens::OPERATOR && get<char>(val->data) == '^';
}
class Parser {
public:

    explicit Parser(Tokenizer *t_tokenizer) : mp_tokenizer(t_tokenizer) {
        m_lookahead = mp_tokenizer->get_next_token();
    };

    TValue *eat(Tokens tokens, optional<char> data = nullopt);

    int expression();

    int term();

    int factor();


    int primary();

    int unaryExpression() {
        eat(Tokens::OPERATOR, '-');
        return -factor();
    }

    int parenthesizedExpression() {
        eat(Tokens::SYMBOL, '(');
        int expr = expression();
        eat(Tokens::SYMBOL, ')');
        return expr;
    }

    int functionExpression() {
        string tok = get<string>(eat(Tokens::KEYWORD)->data);
        if (tok == "sin") return static_cast<int>(sin(parenthesizedExpression()));
        if (tok == "cos") return static_cast<int>(cos(parenthesizedExpression()));
    }

    int parse() {
        return expression();
    }

private:
    TValue *m_lookahead;
    TokenPrinter m_tp;
    Tokenizer *mp_tokenizer;
    map<char, int> precedence_map{{'+', 1},
                                  {'-', 1},
                                  {'*', 2},
                                  {'/', 2}};
};
