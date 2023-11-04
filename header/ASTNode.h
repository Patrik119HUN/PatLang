#pragma once

#include <string>
#include <utility>
#include "visitor_interface.h"

class ASTNode {
public:
    virtual void accept(Visitor &visitor) = 0;

};

class ProgramNode : public ASTNode {
public:

    explicit ProgramNode(const vector<ASTNode*> &body) : body(body) {}

    vector<ASTNode*> body;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class ExpressionStatementNode : public ASTNode {
public:
    ExpressionStatementNode() = default;

    ExpressionStatementNode(ASTNode* pNode) : value(pNode) {}

    ASTNode* value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

class VariableStatementNode : public ASTNode {
public:
    VariableStatementNode(ASTNode* pNode, string basicString) : node(pNode),
                                                                name(std::move(basicString)) {}

    ASTNode* node;
    string name;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};


class NumberNode : public ASTNode {
public:
    NumberNode(NumberNode &t) {
        this->value = t.value;
    }

    NumberNode() = default;

    explicit NumberNode(int t_value) : value(t_value) {}

    int value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class BinaryExpressionNode : public ASTNode {
public:
    BinaryExpressionNode(Tokens op, ASTNode* left, ASTNode* right) : op(op), left(left), right(right) {}

    Tokens op;
    ASTNode* left;
    ASTNode* right;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class VariableNode : public ASTNode {
public:
    VariableNode() = default;

    VariableNode(string t_name) : name(std::move(t_name)) {}

    string name;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class UnaryExpressionNode : public ASTNode {
public:
    UnaryExpressionNode(ASTNode* numberNode) : node(numberNode) {}

    ASTNode* node;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class FunctionNode : public ASTNode {
public:
    FunctionNode(ASTNode* pNode, string basicString) : node(pNode), name(std::move(basicString)) {}

    ASTNode* node;
    string name;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class PrintNode : public ASTNode {
public:
    PrintNode(ASTNode* pNode) : value(pNode) {}

    ASTNode* value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class ForNode : public ASTNode {
public:
    ForNode(VariableStatementNode* var, ASTNode* cond, ASTNode* updt, vector<ASTNode*> body) : variable(var),
                                                                                               condition(cond),
                                                                                               update(updt),
                                                                                               body(std::move(body)) {}

    vector<ASTNode*> body;
    VariableStatementNode* variable;
    ASTNode* condition;
    ASTNode* update;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(ASTNode* node, string name) : name(std::move(name)), node(node) {};
    string name;
    ASTNode* node;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};
