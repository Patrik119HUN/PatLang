#include <iostream>
#include <bits/stdc++.h>
#include "tokenizer.h"
#include "token_printer.h"
#include "parser.h"

using namespace std;

void print(AST_Node *node, bool nested) {
    string n = ", ";
    if (!nested) {
        n = ",\n";
    }
    cout << "type: " << node->type << n;
    cout << "operator: " << node->op << n;
    if (node->value != -1) {
        cout << "value: " << node->value << n;
    }
    if (node->left != nullptr) {
        cout << "left: {";
        print(node->left, true);
        cout << " }";
    }
    if (node->right != nullptr) {
        cout << "right: {";
        print(node->right, true);
        cout << " }";
    }
}

class NodeVisitor {
public:
    int visit(AST_Node *node) {
        if (node->type == "Number") {
            return visitNumber(node);
        } else if (node->type == "BinaryExpression") {
            return visitBinaryExpression(node);
        } else if (node->type == "UnaryExpression") {
            return visitUnaryExpression(node);
        } else if (node->type == "Function") {
            return visitFunction(node);
        }
    }

private:
    int visitNumber(AST_Node *node) {
        return node->value;
    };

    int visitBinaryExpression(AST_Node *node) {
        switch (node->op) {
            case ('+'):
                return visit(node->left) + visit(node->right);
            case ('-'):
                return visit(node->left) - visit(node->right);
            case ('*'):
                return visit(node->left) * visit(node->right);
            case ('/'):
                return visit(node->left) / visit(node->right);
            case ('^'):
                return static_cast<int>(visit(node->left), visit(node->right));
            default:
                throw runtime_error("No such an operation: " + node->op);
        }
    };

    int visitUnaryExpression(AST_Node *node) {
        return 0;
    };

    int visitFunction(AST_Node *node) {
        return 0;
    };


};

int main() {
    string input("1 + 2 * 3");
    try {
        tokenizer.tokenize(input);
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
    vector<TValue> c = tokenizer.get_tokens();
    Parser parser(&tokenizer);
    //print(parser.parse(), false);

    NodeVisitor visitor;
    cout << visitor.visit(parser.expression(0)) << endl;
    //TokenPrinter tp(&c);
    //tp.print();
    return 0;
}
