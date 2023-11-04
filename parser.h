#pragma once

#include <bits/stdc++.h>

#include <utility>
#include "token_printer.h"
#include "header/ASTNode.h"
#include "symbol_table.h"

using namespace std;

class Parser {
public:
    explicit Parser(Tokenizer* t_tokenizer) : mp_tokenizer(t_tokenizer) {
        m_lookahead = mp_tokenizer->get_next_token();
    };

    TValue* eat(Tokens tokens);

    ASTNode* expression(int precedence = 0);

    ASTNode* parenthesizedExpression();

    UnaryExpressionNode* unaryExpression() {
        eat(Tokens::SUBSTRACITON);
        return new UnaryExpressionNode(expression(get_precedence(Tokens::UNARY)));
    }

    FunctionNode* functionExpression(string id) {
        return new FunctionNode(parenthesizedExpression(), std::move(id));
    }

    NumberNode* prefix();

    BinaryExpressionNode* infix(NumberNode* left, Tokens operatorType);

    ExpressionStatementNode* expressionStatement() {
        auto expression = this->expression();
        eat(Tokens::SEMICOLON);
        return new ExpressionStatementNode(expression);
    }

    PrintNode* printStatement();

    VariableStatementNode* variableStatement();

    ASTNode* statement();

    ASTNode* variableOrfunctionExpression();

    VariableNode* variable(const string &name) {
        return new VariableNode(name);
    }

    ForNode* forStatement();

    vector<ASTNode*> statement_list();

    ProgramNode* program() {
        return new ProgramNode(statement_list());
    }
    AssignmentNode* assignVal();
    [[nodiscard]] int get_precedence(Tokens token) const try {
        return precedence_map.at(token);
    } catch (const std::out_of_range &e) {
        return 0;
    }

private:
    TValue* m_lookahead;
    TokenPrinter m_tp;
    Tokenizer* mp_tokenizer;
    map<Tokens, int> precedence_map{{Tokens::ADDITION,     1},
                                    {Tokens::SUBSTRACITON, 1},
                                    {Tokens::SMALLER,      1},
                                    {Tokens::MULTIPLY,     2},
                                    {Tokens::DIVIDE,       2},
                                    {Tokens::UNARY,        3},
                                    {Tokens::POWER,        4}};
    /*map<char, int> precedence_map{{'+', 1},
                                  {'-', 1},
                                  {'*', 2},
                                  {'/', 2},
                                  {'u', 3},
                                  {'^', 4}};*/

};
