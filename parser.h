#pragma once

#include <bits/stdc++.h>

#include <utility>
#include "ASTNode.h"
#include "symbol_table.h"

using namespace std;

class Parser {
public:
    explicit Parser(Tokenizer* t_tokenizer) : mp_tokenizer(t_tokenizer) {
        m_lookahead = mp_tokenizer->get_next_token();
    };

    TValue* eat(Tokens tokens) {
        const auto token = m_lookahead;
        if (token == nullptr) {
            throw runtime_error("Unexpected end of input");
        }
        if (token->type != tokens) {
            throw runtime_error("Unexpected token:  , expected ");
        }

        m_lookahead = mp_tokenizer->get_next_token();
        return token;
    }

    INode* expression(int precedence = 0) {
        VarNode* left = prefix();
        while (m_lookahead != nullptr && precedence < get_precedence(m_lookahead->type)) {
            left = reinterpret_cast<VarNode*>(infix(left, m_lookahead->type));
        }
        return left;
    }

    INode* parenthesizedExpression() {
        eat(Tokens::LEFT_PAR);
        INode* expr = expression();
        eat(Tokens::RIGHT_PAR);
        return expr;
    }

    INode* variableOrfunctionExpression(const string &name) {
        if (m_lookahead->type == Tokens::LEFT_PAR) {
            return functionExpression(name);
        }
        return variable(name);
    }

    VarNode* prefix() {
        if (m_lookahead->type == Tokens::LEFT_PAR) {
            return reinterpret_cast<VarNode*>(parenthesizedExpression());
        }
        if (m_lookahead->type == Tokens::SUBSTRACITON) {
            return reinterpret_cast<VarNode*>(unaryExpression());
        }
        if (m_lookahead->type == Tokens::IDENTIFIER) {
            string name = get<string>(eat(Tokens::IDENTIFIER)->data);
            return reinterpret_cast<VarNode*>(variableOrfunctionExpression(name));
        }
        if (m_lookahead->type == Tokens::INTEGER) {
            auto token = eat(Tokens::INTEGER);
            return new VarNode(get<int>(token->data));
        }
        auto token = eat(Tokens::STRING);
        return new VarNode(get<string>(token->data));
    }

    BinaryExpressionNode* infix(VarNode* left, Tokens operatorType) {
        eat(operatorType);
        int newPrec = get_precedence(operatorType);
        if (operatorType == Tokens::POWER) newPrec--;
        return new BinaryExpressionNode(operatorType, left, expression(newPrec));
    }

    AssignmentNode* assignVal(const string &name) {
        eat(Tokens::EQUAL);
        INode* expression = reinterpret_cast<BinaryExpressionNode*>(this->expression());
        /*if (m_lookahead->type == Tokens::INTEGER) {
            expression = reinterpret_cast<BinaryExpressionNode*>(this->expression());
        } else {
            expression = new StringExpressionNode(Tokens::ADDITION, new VarNode(get<string>(eat(Tokens::STRING)->data)), new VarNode(string("c")));
        }*/
        eat(Tokens::SEMICOLON);
        return new AssignmentNode(expression, name);
    }

    PrintNode* printStatement() {
        eat(Tokens::KEYWORD);
        auto* expression = parenthesizedExpression();
        eat(Tokens::SEMICOLON);
        return new PrintNode(expression);
    }

    whileNode* whileStatement() {
        eat(Tokens::KEYWORD);
        eat(Tokens::LEFT_PAR);
        INode* condition = expression();
        eat(Tokens::RIGHT_PAR);
        eat(Tokens::LEFT_CURLY_BRACE);
        vector<INode*> body = statement_list();
        eat(Tokens::RIGHT_CURLY_BRACE);

        return new whileNode(condition, body);
    }

    ifNode* ifStatement() {
        eat(Tokens::KEYWORD);
        eat(Tokens::LEFT_PAR);
        INode* condition = expressionStatement();
        eat(Tokens::RIGHT_PAR);
        eat(Tokens::LEFT_CURLY_BRACE);
        vector<INode*> body = statement_list();
        eat(Tokens::RIGHT_CURLY_BRACE);

        return new ifNode(condition, body);
    }

    ForNode* forStatement() {
        eat(Tokens::KEYWORD);
        eat(Tokens::LEFT_PAR);
        VariableStatementNode* variable = nullptr;
        INode* update = nullptr;

        if (m_lookahead->type == Tokens::SEMICOLON) {
            eat(Tokens::SEMICOLON);
        } else {
            variable = variableStatement();
        }

        INode* condition = expressionStatement();

        if (m_lookahead->type == Tokens::SEMICOLON) {
            eat(Tokens::SEMICOLON);
        } else {
            string name = get<string>(eat(Tokens::IDENTIFIER)->data);
            update = assignVal(name);
        }

        eat(Tokens::RIGHT_PAR);
        eat(Tokens::LEFT_CURLY_BRACE);
        vector<INode*> body = statement_list();
        eat(Tokens::RIGHT_CURLY_BRACE);

        return new ForNode(variable, condition, update, body);
    }

    VariableStatementNode* variableStatement() {
        eat(Tokens::KEYWORD);
        string name = get<string>(eat(Tokens::IDENTIFIER)->data);
        eat(Tokens::EQUAL);
        auto* expression = reinterpret_cast<VarNode*>(this->expression());
        eat(Tokens::SEMICOLON);
        return new VariableStatementNode(expression, name);
    }

    FunctionDeclarationNode* functionDeclaration() {
        eat(Tokens::KEYWORD);
        string name = get<string>(eat(Tokens::IDENTIFIER)->data);
        eat(Tokens::LEFT_PAR);
        vector<string> params;
        while (m_lookahead->type == Tokens::IDENTIFIER) {
            params.push_back(get<string>(eat(Tokens::IDENTIFIER)->data));
            if (m_lookahead->type != Tokens::RIGHT_PAR) eat(Tokens::COLON);
        }
        eat(Tokens::RIGHT_PAR);
        eat(Tokens::LEFT_CURLY_BRACE);
        vector<INode*> body = statement_list();
        eat(Tokens::RIGHT_CURLY_BRACE);
        return new FunctionDeclarationNode(name, params, body);
    }

    INode* statement() {
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "var") {
            return variableStatement();
        }
        if (m_lookahead->type == Tokens::IDENTIFIER) {
            string name = get<string>(eat(Tokens::IDENTIFIER)->data);
            if (m_lookahead->type == Tokens::EQUAL) {
                return assignVal(name);
            } else {
                INode* ret = variableOrfunctionExpression(name);
                eat(Tokens::SEMICOLON);
                return ret;
            }
        }
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "print") {
            return printStatement();
        }
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "for") {
            return forStatement();
        }
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "while") {
            return whileStatement();
        }
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "if") {
            return ifStatement();
        }
        if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "func") {
            return functionDeclaration();
        }
        return expressionStatement();
    }

    vector<INode*> statement_list() {
        vector<INode*> statementList = {statement()};
        while (m_lookahead != nullptr && m_lookahead->type != Tokens::RIGHT_CURLY_BRACE) {
            statementList.push_back(statement());
        }
        return statementList;
    }

    UnaryExpressionNode* unaryExpression() {
        eat(Tokens::SUBSTRACITON);
        return new UnaryExpressionNode(expression(get_precedence(Tokens::UNARY)));
    }

    FunctionNode* functionExpression(string id) {
        eat(Tokens::LEFT_PAR);
        vector<INode*> params;
        while (m_lookahead->type == Tokens::INTEGER) {
            params.push_back(expression());
            if (m_lookahead->type != Tokens::RIGHT_PAR) eat(Tokens::COLON);
        }
        eat(Tokens::RIGHT_PAR);
        return new FunctionNode(params, std::move(id));
    }

    ExpressionStatementNode* expressionStatement() {
        auto expression = this->expression();
        eat(Tokens::SEMICOLON);
        return new ExpressionStatementNode(expression);
    }

    VariableNode* variable(const string &name) {
        return new VariableNode(name);
    }

    ProgramNode* program() {
        return new ProgramNode(statement_list());
    }

    [[nodiscard]] int get_precedence(Tokens token) const try {
        return precedence_map.at(token);
    } catch (const std::out_of_range &e) {
        return 0;
    }

private:
    TValue* m_lookahead;
    Tokenizer* mp_tokenizer;
    map<Tokens, int> precedence_map{{Tokens::ADDITION,         1},
                                    {Tokens::SUBSTRACITON,     1},
                                    {Tokens::LESS,             1},
                                    {Tokens::LESS_OR_EQUAL,    1},
                                    {Tokens::GREATER,          1},
                                    {Tokens::GREATER_OR_EQUAL, 1},
                                    {Tokens::EQUAL_TO,         1},
                                    {Tokens::NOT_EQUAL,        1},
                                    {Tokens::MULTIPLY,         2},
                                    {Tokens::OR,               2},
                                    {Tokens::AND,              2},
                                    {Tokens::DIVIDE,           2},
                                    {Tokens::UNARY,            3},
                                    {Tokens::POWER,            4}};
};
