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

AST_Node *Parser::expression(int precedence) {
    AST_Node *left = prefix();
    while (m_lookahead != nullptr && precedence < get_precedence(get<char>(m_lookahead->data))) {
        left = infix(left, m_lookahead->type);
    }
    return left;
}

AST_Node *Parser::prefix() {
    if (m_lookahead->type == Tokens::SYMBOL && get<char>(m_lookahead->data) == '(') {
        return parenthesizedExpression();
    }
    if (m_lookahead->type == Tokens::OPERATOR && get<char>(m_lookahead->data) == '-') {
        return unaryExpression();
    }
    if (m_lookahead->type == Tokens::KEYWORD) {
        return functionExpression();
    }
    auto token = eat(Tokens::INTEGER);
    auto *node = new AST_Node;
    node->type = "Number";
    node->value = get<int>(token->data);
    return node;
}

AST_Node *Parser::infix(AST_Node *left, Tokens operatorType) {
    auto token = eat(operatorType);
    char tokType = get<char>(token->data);
    int newPrec = get_precedence(tokType);

    auto *node = new AST_Node;
    node->type = "BinaryExpression";
    node->op = tokType;
    node->right = expression((tokType == '^') ? newPrec - 1 : newPrec);
    node->left = left;
    return node;
}
