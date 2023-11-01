#include "tokenizer.h"

class TokenPrinter {
public:
    TokenPrinter() = default;

    explicit TokenPrinter(vector<TValue> *tokens) : mp_tokens(tokens) {};

    string get_token_string(Tokens tokens) { return m_token_to_string.at(tokens); }

    void print() {
        for (auto i: *mp_tokens) {
            cout << m_token_to_string.at(i.type) << " ";
            if (i.type == Tokens::OPERATOR || i.type == Tokens::SYMBOL) cout << get<char>(i.data) << endl;
            if (i.type == Tokens::KEYWORD || i.type == Tokens::IDENTIFIER || i.type == Tokens::BOOLEAN ||
                i.type == Tokens::STRING)
                cout << get<string>(i.data) << endl;
            if (i.type == Tokens::INTEGER) cout << get<int>(i.data) << endl;
        }
    }

private:
    vector<TValue> *mp_tokens;
    map<Tokens, string> m_token_to_string{{Tokens::INTEGER,    "INTEGER"},
                                          {Tokens::STRING,     "STRING"},
                                          {Tokens::BOOLEAN,    "BOOL"},
                                          {Tokens::OPERATOR,   "OPERATOR"},
                                          {Tokens::SYMBOL,     "SYMBOL"},
                                          {Tokens::KEYWORD,    "KEYWORD"},
                                          {Tokens::IDENTIFIER, "IDENTIFIER"}};
};
