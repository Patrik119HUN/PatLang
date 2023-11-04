#include "parser.h"

TValue* Parser::eat(Tokens tokens) {
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

ASTNode* Parser::expression(int precedence) {
    NumberNode* left = prefix();
    while (m_lookahead != nullptr && precedence < get_precedence(m_lookahead->type)) {
        left = reinterpret_cast<NumberNode*>(infix(left, m_lookahead->type));
    }
    return left;
}

ASTNode* Parser::parenthesizedExpression() {
    eat(Tokens::LEFT_PAR);
    ASTNode* expr = expression();
    eat(Tokens::RIGHT_PAR);
    return expr;
}

ASTNode* Parser::variableOrfunctionExpression() {
    TValue* token = eat(Tokens::IDENTIFIER);
    string name = get<string>(token->data);
    if (m_lookahead->type == Tokens::LEFT_PAR) {
        return functionExpression(get<string>(token->data));
    }
    return variable(name);
}

NumberNode* Parser::prefix() {
    if (m_lookahead->type == Tokens::LEFT_PAR) {
        return reinterpret_cast<NumberNode*>(parenthesizedExpression());
    }
    if (m_lookahead->type == Tokens::SUBSTRACITON) {
        return reinterpret_cast<NumberNode*>(unaryExpression());
    }
    if (m_lookahead->type == Tokens::IDENTIFIER) {
        return reinterpret_cast<NumberNode*>(variableOrfunctionExpression());
    }
    auto token = eat(Tokens::INTEGER);
    return new NumberNode(get<int>(token->data));
}

BinaryExpressionNode* Parser::infix(NumberNode* left, Tokens operatorType) {
    eat(operatorType);
    int newPrec = get_precedence(operatorType);
    if (operatorType == Tokens::POWER) newPrec--;
    return new BinaryExpressionNode(operatorType, left, expression(newPrec));
}
AssignmentNode* Parser::assignVal(){
    string name = get<string>(eat(Tokens::IDENTIFIER)->data);
    eat(Tokens::EQUAL);
    auto* expression = reinterpret_cast<BinaryExpressionNode*>(this->expression());
    eat(Tokens::SEMICOLON);
    return new AssignmentNode(expression, name);
}
PrintNode* Parser::printStatement() {
    eat(Tokens::KEYWORD);
    auto* expression = parenthesizedExpression();
    eat(Tokens::SEMICOLON);
    return new PrintNode(expression);
}

ForNode* Parser::forStatement() {
    eat(Tokens::KEYWORD);
    eat(Tokens::LEFT_PAR);
    VariableStatementNode* variable = variableStatement();
    ASTNode* condition = expressionStatement();
    ASTNode* update = assignVal();
    eat(Tokens::RIGHT_PAR);
    eat(Tokens::LEFT_CURLY_BRACE);
    vector<ASTNode*> body = statement_list();
    eat(Tokens::RIGHT_CURLY_BRACE);

    return new ForNode(variable,condition,update,body);
}
VariableStatementNode* Parser::variableStatement() {
    eat(Tokens::KEYWORD);
    string name = get<string>(eat(Tokens::IDENTIFIER)->data);
    eat(Tokens::EQUAL);
    auto* expression = reinterpret_cast<NumberNode*>(this->expression());
    eat(Tokens::SEMICOLON);
    return new VariableStatementNode(expression, name);
}

ASTNode* Parser::statement() {
    if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "var") {
        return variableStatement();
    }
    if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "print") {
        return printStatement();
    }
    if (m_lookahead->type == Tokens::KEYWORD && get<string>(m_lookahead->data) == "for") {
        return forStatement();
    }
    return expressionStatement();
}

vector<ASTNode*> Parser::statement_list() {
    vector<ASTNode*> statementList = {statement()};
    while (m_lookahead != nullptr && m_lookahead->type != Tokens::RIGHT_CURLY_BRACE) {
        statementList.push_back(statement());
    }
    return statementList;
}

