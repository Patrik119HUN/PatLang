#pragma once

struct ProgramNode;

struct VarNode;

struct VariableNode;

struct FunctionNode;

struct PrintNode;

struct VariableStatementNode;

struct BinaryExpressionNode;
struct BooleanExpressionNode;
struct StringExpressionNode;

struct UnaryExpressionNode;

struct ExpressionStatementNode;

struct ForNode;

struct AssignmentNode;

struct whileNode;

struct ifNode;

struct FunctionDeclarationNode;


class Visitor {
public:
    virtual void visit(ProgramNode &node) = 0;

    virtual void visit(VarNode &node) = 0;

    virtual void visit(VariableNode &node) = 0;

    virtual void visit(FunctionNode &node) = 0;

    virtual void visit(PrintNode &node) = 0;

    virtual void visit(VariableStatementNode &node) = 0;

    virtual void visit(BinaryExpressionNode &node) = 0;
    virtual void visit(BooleanExpressionNode &node) = 0;
    virtual void visit(StringExpressionNode &node) = 0;

    virtual void visit(FunctionDeclarationNode &node) = 0;

    virtual void visit(UnaryExpressionNode &node) = 0;

    virtual void visit(ExpressionStatementNode &node) = 0;

    virtual void visit(ForNode &node) = 0;

    virtual void visit(whileNode &node) = 0;

    virtual void visit(AssignmentNode &node) = 0;

    virtual void visit(ifNode &node) = 0;

};
