#pragma once

#include "symbol_table.h"
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

    void visit(VarNode &node) final {
        m_temp = node.value;
    }

    void visit(VariableNode &node) final {
        mp_table->get(node.name)->accept(*this);
    }

    void stringExpr(Tokens op, const string &left, const string &right) {
        switch (op) {
            case (Tokens::ADDITION):
                m_temp = left + right;
                break;
            default:
                break;
        }
    }

    void binExpr(Tokens op, int left, int right) {
        switch (op) {
            case (Tokens::ADDITION):
                m_temp = left + right;
                break;
            case (Tokens::SUBSTRACITON):
                m_temp = left - right;
                break;
            case (Tokens::MULTIPLY):
                m_temp = left * right;
                break;
            case (Tokens::DIVIDE):
                m_temp = left / right;
                break;
            case (Tokens::POWER):
                m_temp = static_cast<int>(pow(left, right));
                break;
            case (Tokens::GREATER):
                m_temp = left > right;
                break;
            case (Tokens::LESS):
                m_temp = left < right;
                break;
            case (Tokens::GREATER_OR_EQUAL):
                m_temp = left >= right;
                break;
            case (Tokens::LESS_OR_EQUAL):
                m_temp = left <= right;
                break;
            case (Tokens::EQUAL_TO):
                m_temp = left == right;
                break;
            case (Tokens::NOT_EQUAL):
                m_temp = left != right;
                break;
            case (Tokens::OR):
                m_temp = left || right;
                break;
            case (Tokens::AND):
                m_temp = left && right;
                break;
            default:
                break;
        }
    }

    void visit(BinaryExpressionNode &node) final {
        node.left->accept(*this);
        if (std::holds_alternative<string>(m_temp)) {
            string left = get<string>(m_temp);
            node.right->accept(*this);
            if (std::holds_alternative<string>(m_temp)) {
                string right = get<string>(m_temp);
                stringExpr(node.op, left, right);
                return;
            } else if (std::holds_alternative<int>(m_temp)) {
                int right = get<int>(m_temp);
                stringExpr(node.op, left, std::to_string(right));
                return;
            }

        } else if (std::holds_alternative<int>(m_temp)) {
            int left = get<int>(m_temp);
            node.right->accept(*this);
            if (std::holds_alternative<string>(m_temp)) {
                string right = get<string>(m_temp);
                stringExpr(node.op, std::to_string(left), right);
                return;
            } else if (std::holds_alternative<int>(m_temp)) {
                int right = get<int>(m_temp);
                binExpr(node.op, left, right);
                return;
            }

        }

        throw runtime_error("No such an operation: ");
    }

    void visit(UnaryExpressionNode &node) final {
        //m_variables["u_expression_temp"] = node.node->accept(*this);
    }

    void visit(BooleanExpressionNode &node) final {

    }

    void visit(StringExpressionNode &node) final {
        node.left->accept(*this);
        string left = get<string>(m_temp);
        node.right->accept(*this);
        string right = get<string>(m_temp);
        if (node.op == Tokens::ADDITION) {
            m_temp = left + right;
        }
    }

    void visit(FunctionNode &node) final {
        if (node.name == "sin") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else if (node.name == "cos") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else if (node.name == "tan") {
            //m_variables["f_temp"] = static_cast<int>(sin(node.value));
        } else {

            auto* n = reinterpret_cast<FunctionDeclarationNode*>(m_function_map[node.name]);
            for (int i = 0; i < n->parameters.size(); ++i) {
                //de.params[i]->accept(*this);
                mp_table->set(n->parameters[i], node.params[i]);
                //[n->parameters[i]] = m_variables["temp"];
            }
            for (auto i: n->body) {
                i->accept(*this);
            }
        }/*
        else {
            throw runtime_error("There is no such an function: " + node.name);
        }*/
    }

    void visit(FunctionDeclarationNode &node) final {
        m_function_map[node.name] = &node;
    }

    void visit(ExpressionStatementNode &node) final {
        node.value->accept(*this);
    }


    void visit(PrintNode &node) final {
        node.value->accept(*this);
        std::visit([](const auto &a) { cout << a << endl; }, m_temp);
    };

    void visit(VariableStatementNode &node) final {
        mp_table->set(node.name, node.node);
    };

    void visit(whileNode &node) final {
        node.condition->accept(*this);
        while (get<bool>(m_temp)) {
            for (auto i: node.body) i->accept(*this);
            node.condition->accept(*this);
        }
    }

    void visit(ifNode &node) final {
        node.condition->accept(*this);
        if (get<bool>(m_temp)) {
            for (auto i: node.body) i->accept(*this);
        }
    }

    void visit(ForNode &node) final {
        if (node.variable != nullptr) node.variable->accept(*this);
        node.condition->accept(*this);
        while (get<bool>(m_temp)) {
            for (auto i: node.body) i->accept(*this);
            if (node.update != nullptr)node.update->accept(*this);
            node.condition->accept(*this);
        }
    }

    void visit(AssignmentNode &node) final {
        node.node->accept(*this);
        auto* var = new VarNode(m_temp);
        mp_table->set(node.name, var);
    }

private:
    variant<bool, int, float, string> m_temp;
    std::unordered_map<string, INode*> m_function_map;
    SymbolTable* mp_table;
};
