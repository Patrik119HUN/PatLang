#pragma once

#include <bits/stdc++.h>

using namespace std;
enum class Tokens {
    INTEGER,
    BOOLEAN,
    STRING,
    OPERATOR,
    SYMBOL,
    KEYWORD,
    IDENTIFIER,
    LEFT_PAR,
    RIGHT_PAR,
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE,
    SEMICOLON,
    COLON,
    EQUAL,
    SUBSTRACITON,
    ADDITION,
    MULTIPLY,
    DIVIDE,
    POWER,
    UNARY,
    GREATER,
    LESS,
    GREATER_OR_EQUAL,
    LESS_OR_EQUAL,
    EQUAL_TO,
    NOT_EQUAL,
    OR,
    AND
};

unordered_map<char, Tokens> symbol_table{{'+', Tokens::ADDITION},
                                         {'-', Tokens::SUBSTRACITON},
                                         {'*', Tokens::MULTIPLY,},
                                         {'/', Tokens::DIVIDE},
                                         {'^', Tokens::POWER},
                                         {'(', Tokens::LEFT_PAR},
                                         {')', Tokens::RIGHT_PAR},
                                         {';', Tokens::SEMICOLON},
                                         {',', Tokens::COLON},
                                         {'=', Tokens::EQUAL},
                                         {'{', Tokens::LEFT_CURLY_BRACE},
                                         {'}', Tokens::RIGHT_CURLY_BRACE}};

unordered_map<string, Tokens> logic_table{
        {"<",  Tokens::LESS},
        {">",  Tokens::GREATER},
        {">=", Tokens::GREATER_OR_EQUAL},
        {"<=", Tokens::LESS_OR_EQUAL},
        {"==", Tokens::EQUAL_TO},
        {"=",  Tokens::EQUAL},
        {"||", Tokens::OR},
        {"&&", Tokens::AND},
        {"!=", Tokens::NOT_EQUAL}};

bool isLogicalSymbol(char ch) {
    if (ch == '<' || ch == '>' || ch == '!' || ch == '=' || ch == '|' || ch == '&') {
        return true;
    }
    return false;
}

bool isSymbol(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')' ||
        ch == ';' || ch == '{' || ch == '}' || ch == ',') {
        return true;
    }
    return false;
}

struct TValue {
    Tokens type;
    variant<char, int, string> data;
};

class Tokenizer {
private:
    void tokenizers(vector<TValue> &token_vector, string &current_token) {
        bool contains_number = false;
        bool contains_char = false;
        for (auto i: current_token) {
            if (isdigit(i)) contains_number = true;
            if (isalpha(i)) contains_char = true;
        }
        if ((key_words.count(current_token) > 0)) {
            token_vector.push_back({Tokens::KEYWORD, current_token});
        } else if ((boolean_words.count(current_token) > 0)) {
            token_vector.push_back({Tokens::BOOLEAN, current_token});
        } else if (contains_number && !contains_char) {
            token_vector.push_back({Tokens::INTEGER, stoi(current_token)});
        } else {
            token_vector.push_back({Tokens::IDENTIFIER, current_token});
        }
        current_token.clear();
    }

public:
    void tokenize(const string &in) {
        for (char ch: in) {
            char_to_token(ch);
        }
        if (!current_token.empty()) tokenizers(tokens, current_token);
        if (incoming_string) throw runtime_error("Lexer error: Unterminated string literal.");
    }

    [[nodiscard]] const vector<TValue> &get_tokens() const {
        return tokens;
    }

    TValue* get_next_token() {
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
    void logic_tokenizer(char ch) {
        if (!isLogicalSymbol(ch)) {
            incoming_symbol = false;
            tokens.push_back({logic_table[current_logical_token]});
            current_logical_token.clear();
            char_to_token(ch);
        } else {
            current_logical_token += ch;
        }
    }

    void string_tokenizer(char ch) {
        if (ch == '"') {
            incoming_string = false;
            tokens.push_back({Tokens::STRING, current_token});
            current_token.clear();
        } else {
            current_token += ch;
        }
    }

    void char_to_token(char ch) {
        if (incoming_string) {
            string_tokenizer(ch);
        } else if (incoming_symbol) {
            logic_tokenizer(ch);
        } else if (isalnum(ch) || ch == '_') {
            current_token += ch;
        } else if (ch == '\"') {
            incoming_string = true;
            current_token.clear();
        } else if (isSymbol(ch)) {
            if (!current_token.empty()) tokenizers(tokens, current_token);
            tokens.push_back({symbol_table[ch], ch});
        } else if (isLogicalSymbol(ch)) {
            if (!current_token.empty()) tokenizers(tokens, current_token);
            incoming_symbol = true;
            logic_tokenizer(ch);
        } else if (isspace(ch)) {
            if (!current_token.empty()) tokenizers(tokens, current_token);
        } else {
            throw runtime_error(string("Lexer error: Unrecognized character: ").append(1, ch));
        }
    }

    vector<TValue> tokens;
    unsigned m_current_token = 0;

    const unordered_set<string> key_words = {"if", "else", "while", "for", "return", "print", "sin", "cos", "func",
                                             "var"};
    const unordered_set<string> boolean_words = {"true", "false"};

    string current_token;
    string current_logical_token;
    bool incoming_string = false;
    bool incoming_symbol = false;
};

Tokenizer tokenizer;
