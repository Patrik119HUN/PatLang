#pragma once

#include <bits/stdc++.h>

using namespace std;
enum class Tokens {
    INTEGER, BOOLEAN, STRING, OPERATOR, SYMBOL, KEYWORD, IDENTIFIER
};

struct TValue {
    Tokens type;
    variant<char, int, string> data;
};

class Tokenizer {
private:
    void tokenizers(vector<TValue> &token_vector, string &current_token);

public:
    void tokenize(const string &in);

    [[nodiscard]] const vector<TValue> &get_tokens() const {
        return tokens;
    }

    TValue *get_next_token() {
        if (m_current_token < tokens.size()) {
            return &tokens.at(m_current_token++);
        } else {
            return nullptr;
        }
    }

    [[nodiscard]] unsigned long size() const {
        return tokens.size();
    }

private:
    vector<TValue> tokens;
    unsigned m_current_token = 0;
    const unordered_set<string> key_words = {"if", "else", "while", "for", "return", "print" , "sin", "cos"};
    const unordered_set<string> boolean_words = {"true", "false"};
};

extern Tokenizer tokenizer;
