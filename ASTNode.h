#pragma once

#include <string>
#include <utility>

#include "node_interface.h"

struct ProgramNode : INode {
    explicit ProgramNode(const vector<INode*> &body) : body(body) {}

    vector<INode*> body;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct ExpressionStatementNode : INode {
    ExpressionStatementNode() = default;

    ExpressionStatementNode(INode* pNode) : value(pNode) {}

    INode* value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

struct FunctionDeclarationNode : INode {
    FunctionDeclarationNode(const string &name, vector<string> params, vector<INode*> body) : name(name),
                                                                                              parameters(params),
                                                                                              body(body) {
    };
    string name;
    vector<string> parameters;
    vector<INode*> body;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct VariableStatementNode : INode {
    VariableStatementNode(INode* pNode, string basicString) : node(pNode),
                                                              name(std::move(basicString)) {}

    INode* node;
    string name;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};


struct VarNode : INode {
    VarNode(VarNode &t) {
        this->value = t.value;
    }

    VarNode() = default;
    explicit VarNode(variant<bool,int,float,string> t_value): value(std::move(t_value)){}
    explicit VarNode(int t_value) : value(t_value) {}
    explicit VarNode(float t_value) : value(t_value) {}
    explicit VarNode(string t_value) : value(t_value) {}

    variant<bool,int,float,string> value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct StringExpressionNode : INode {
    StringExpressionNode(Tokens op, INode* left, INode* right) : op(op), left(left), right(right) {}

    Tokens op;
    INode* left;
    INode* right;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};
struct BooleanExpressionNode : INode {
    BooleanExpressionNode(Tokens op, INode* left, INode* right) : op(op), left(left), right(right) {}

    Tokens op;
    INode* left;
    INode* right;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};
struct BinaryExpressionNode : INode {
    BinaryExpressionNode(Tokens op, INode* left, INode* right) : op(op), left(left), right(right) {}

    Tokens op;
    INode* left;
    INode* right;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct VariableNode : INode {
    VariableNode() = default;

    VariableNode(string t_name) : name(std::move(t_name)) {}

    string name;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct UnaryExpressionNode : INode {
    UnaryExpressionNode(INode* numberNode) : node(numberNode) {}

    INode* node;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct FunctionNode : INode {
    FunctionNode(vector<INode*> params, string basicString) : params(params), name(std::move(basicString)) {}

    INode* node;
    string name;
    vector<INode*> params;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct PrintNode : INode {
    PrintNode(INode* pNode) : value(pNode) {}

    INode* value;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct whileNode : INode {
    whileNode(INode* cond, vector<INode*> body) : condition(cond),
                                                  body(std::move(body)) {}

    vector<INode*> body;
    INode* condition;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct ifNode : INode {
    ifNode(INode* cond, vector<INode*> body) : condition(cond), body(std::move(body)) {}

    vector<INode*> body;
    INode* condition;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct ForNode : INode {
    ForNode(VariableStatementNode* var, INode* cond, INode* updt, vector<INode*> body) : variable(var),
                                                                                         condition(cond),
                                                                                         update(updt),
                                                                                         body(std::move(body)) {}

    vector<INode*> body;
    VariableStatementNode* variable;
    INode* condition;
    INode* update;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct AssignmentNode : INode {
    AssignmentNode(INode* node, string name) : name(std::move(name)), node(node) {};
    string name;
    INode* node;

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};
