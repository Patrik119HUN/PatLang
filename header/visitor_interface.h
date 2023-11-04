#pragma once

class ProgramNode;

class NumberNode;

class VariableNode;

class FunctionNode;

class PrintNode;

class VariableStatementNode;

class BinaryExpressionNode;

class UnaryExpressionNode;

class ExpressionStatementNode;

class ForNode;

class AssignmentNode;

class Visitor {
public:
    virtual void visit(ProgramNode &node) = 0;

    virtual void visit(NumberNode &node) = 0;

    virtual void visit(VariableNode &node) = 0;

    virtual void visit(FunctionNode &node) = 0;

    virtual void visit(PrintNode &node) = 0;

    virtual void visit(VariableStatementNode &node) = 0;

    virtual void visit(BinaryExpressionNode &node) = 0;

    virtual void visit(UnaryExpressionNode &node) = 0;

    virtual void visit(ExpressionStatementNode &node) = 0;

    virtual void visit(ForNode &node) = 0;

    virtual void visit(AssignmentNode &node) = 0;

};
