#include <iostream>
#include <bits/stdc++.h>
#include "tokenizer.h"
#include "token_printer.h"
#include "parser.h"

using namespace std;

int main() {
    string input("cos ( 0 )");
    try {
        tokenizer.tokenize(input);
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
    vector<TValue> c = tokenizer.get_tokens();
    Parser parser(&tokenizer);
    cout << parser.parse() << endl;
    TokenPrinter tp(&c);
    tp.print();
    return 0;
}
