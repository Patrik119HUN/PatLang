#include <iostream>
#include <bits/stdc++.h>
#include "tokenizer.h"
#include "parser.h"
#include "code_generator.h"

using namespace std;

/*
void print(INode *node, bool nested) {
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
*/

int main() {
    ifstream MyFile("main.ass");
    string input;
    while (MyFile) {
        string tmp;
        MyFile >> tmp;
        input += tmp + "\n";
    }
    MyFile.close();

    try {
        tokenizer.tokenize(input);
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
    Parser parser(&tokenizer);
    CodeGenerator cg;
    parser.program()->accept(cg);
    return 0;
}
