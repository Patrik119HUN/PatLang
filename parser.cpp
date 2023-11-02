#include "parser.h"

TValue *Parser::eat(Tokens tokens, optional<char> data) {
    const auto token = m_lookahead;
    if (token == nullptr) {
        throw runtime_error("Unexpected end of input");
    }
    if (token->type != tokens) {
        string current = m_tp.get_token_string(token->type);
        string expected = m_tp.get_token_string(tokens);
        throw runtime_error("Unexpected token: " + current + ", expected " + expected);
    }

    m_lookahead = mp_tokenizer->get_next_token();
    return token;
}

AST_Node *Parser::expression() {/*
    int left = term();
    while (m_lookahead != nullptr && (isAddition(m_lookahead) || isSub(m_lookahead))) {
        if (isAddition(m_lookahead)) {
            eat(Tokens::OPERATOR, '+');
            int right = term();
            left += right;
        } else if (isSub(m_lookahead)) {
            eat(Tokens::OPERATOR, '-');
            int right = term();
            left -= right;
        }
    }
    return left;*/
    return binary_expression(&Parser::term, &Parser::term, isAddition, isSub);
}

AST_Node *Parser::term() {
    /*
    int left = factor();
    while (m_lookahead != nullptr && (isMul(m_lookahead) || isDiv(m_lookahead))) {
        if (isMul(m_lookahead)) {
            eat(Tokens::OPERATOR, '*');
            int right = term();
            left *= right;
        } else if (isDiv(m_lookahead)) {
            eat(Tokens::OPERATOR, '/');
            int right = term();
            left /= right;
        }
    }
    return left;*/

    return binary_expression(&Parser::factor, &Parser::factor, isMul, isDiv);
}


AST_Node *Parser::factor() {
    return binary_expression(&Parser::primary, &Parser::factor, isFac, isFac);
}

AST_Node *Parser::primary() {
    if (m_lookahead->type == Tokens::SYMBOL && get<char>(m_lookahead->data) == '(') {
        return parenthesizedExpression();
    }
    if (m_lookahead->type == Tokens::OPERATOR && get<char>(m_lookahead->data) == '-') {
        return unaryExpression();
    }
    if (m_lookahead->type == Tokens::KEYWORD) {
        return functionExpression();
    }
    const auto token = eat(Tokens::INTEGER);
    auto *node = new AST_Node;
    node->type = "Number";
    node->value = get<int>(token->data);
    return node;
}

AST_Node *
Parser::binary_expression(AST_Node *(Parser::*t_left)(), AST_Node *(Parser::*t_right)(), bool (*checker)(TValue *),
                          bool (*checker2)(TValue *)) {
    AST_Node *left = (this->*t_left)();
    while (m_lookahead != nullptr && (checker(m_lookahead) || checker2(m_lookahead))) {
        auto op = eat(m_lookahead->type)->data;
        auto *node = new AST_Node;

        node->op = get<char>(op);
        node->right = (this->*t_right)();
        node->left = left;
        node->type = "BinaryExpression";
        left = node;
    }

    return left;
}
