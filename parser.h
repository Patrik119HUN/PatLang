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

struct AST_Node {
    string type;
    string name;
    int value = -1;
    char op = 0;
    AST_Node *left = nullptr;
    AST_Node *right = nullptr;
};

class Parser {
public:

    explicit Parser(Tokenizer *t_tokenizer) : mp_tokenizer(t_tokenizer) {
        m_lookahead = mp_tokenizer->get_next_token();
    };

    TValue *eat(Tokens tokens, optional<char> data = nullopt);

    AST_Node *expression(int precedence = 0);

    AST_Node *parenthesizedExpression() {
        eat(Tokens::SYMBOL, '(');
        auto expr = expression();
        eat(Tokens::SYMBOL, ')');
        return expr;
    }

    AST_Node *unaryExpression() {
        eat(Tokens::OPERATOR, '-');
        auto *node = new AST_Node;
        node->type = "UnaryExpression";
        node->value = expression(get_precedence('u'))->value;
        return node;
    }

    AST_Node *functionExpression() {
        auto token = eat(Tokens::KEYWORD);
        //if (tok == "sin") ret = static_cast<int>(sin(parenthesizedExpression()));
        //if (tok == "cos") ret =  static_cast<int>(cos(parenthesizedExpression()));
        auto *node = new AST_Node;
        node->type = "Function";
        node->name = get<string>(token->data);
        node->value = parenthesizedExpression()->value;
        return node;
    }

    AST_Node *prefix();

    AST_Node *infix(AST_Node *left, Tokens operatorType);

    AST_Node *parse() {
        return expression();
    }

    [[nodiscard]] int get_precedence(char token) const {
        return precedence_map.at(token);
    }

private:
    TValue *m_lookahead;
    TokenPrinter m_tp;
    Tokenizer *mp_tokenizer;
    map<char, int> precedence_map{{'+', 1},
                                  {'-', 1},
                                  {'*', 2},
                                  {'/', 2},
                                  {'u', 3},
                                  {'^', 4}};

};
