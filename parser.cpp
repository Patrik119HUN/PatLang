#include "parser.h"

TValue *Parser::eat(Tokens tokens, optional<char> data) {
    const auto token = m_lookahead;
    if (token == nullptr) {
        throw runtime_error("Unexpected end of input");
    }
    if (token->type != tokens && data.value() != get<char>(token->data)) {
        string current = m_tp.get_token_string(token->type);
        string expected = m_tp.get_token_string(tokens);
        throw runtime_error("Unexpected token: " + current + ", expected " + expected);
    }

    m_lookahead = mp_tokenizer->get_next_token();
    return token;
}

int Parser::expression() {
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
    return left;
}

int Parser::term() {
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
    return left;
}

int Parser::factor() {
    int left = primary();
    while (m_lookahead != nullptr && isFac(m_lookahead)) {
        eat(Tokens::OPERATOR, '^');
        int right = factor();
        left = static_cast<int>(pow(left, right));
    }
    return left;
}

int Parser::primary() {
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
    return get<int>(token->data);
}