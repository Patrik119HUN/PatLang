#pragma once

#include "../symbol_table.h"
#include "ASTNode.h"
#include "visitor_interface.h"

class CodeGenerator : public Visitor {
public:
    CodeGenerator() {
        mp_table = new SymbolTable;
    }

    void visit(ProgramNode &node) final {
        for (auto i: node.body) {
            i->accept(*this);
        }
    }

    void visit(NumberNode &node) final {
        m_variables["temp"] = node.value;
    }

    void visit(VariableNode &node) final {
        mp_table->get(node.name)->accept(*this);
    }

    void visit(BinaryExpressionNode &node) final {
        node.left->accept(*this);
        int left = m_variables["temp"];
        node.right->accept(*this);
        int right = m_variables["temp"];
        switch (node.op) {
            case (Tokens::ADDITION):
                m_variables["temp"] = left + right;
                break;
            case (Tokens::SUBSTRACITON):
                m_variables["temp"] = left - right;
                break;
            case (Tokens::MULTIPLY):
                m_variables["temp"] = left * right;
                break;
            case (Tokens::DIVIDE):
                m_variables["temp"] = left / right;
                break;
            case (Tokens::POWER):
                m_variables["temp"] = static_cast<int>(pow(left, right));
                break;
            case (Tokens::SMALLER):
                m_variables["condition"] = (left < right) ? 1 : 0;
                break;
            default:
                throw runtime_error("No such an operation: ");
        }
    }

    void visit(UnaryExpressionNode &node) final {
        //m_variables["u_expression_temp"] = node.node->accept(*this);
    }

    void visit(FunctionNode &node) final {
        if (node.name == "sin") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else if (node.name == "cos") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else if (node.name == "tan") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else {
            throw runtime_error("There is no such an function: " + node.name);
        }
    }

    void visit(ExpressionStatementNode &node) final {
        node.value->accept(*this);
    }


    void visit(PrintNode &node) final {
        node.value->accept(*this);
        cout << m_variables["temp"] << endl;
    };

    void visit(VariableStatementNode &node) final {
        mp_table->set(node.name, node.node);
    };

    void visit(ForNode &node) final {
        node.variable->accept(*this);
        node.condition->accept(*this);
        while (m_variables["condition"]) {
            for (auto i: node.body) {
                i->accept(*this);
            }
            node.update->accept(*this);
            node.condition->accept(*this);
        }
    }

    void visit(AssignmentNode &node) final {
        node.node->accept(*this);
        auto* numbnode = new NumberNode(m_variables["temp"]);
        mp_table->set(node.name, numbnode);
    }

private:
    std::unordered_map<string, int> m_variables;
    SymbolTable* mp_table;
};
