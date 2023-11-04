
#include "tokenizer.h"

void Tokenizer::tokenizers(vector<TValue> &token_vector, string &current_token) {
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

void Tokenizer::tokenize(const string &in) {
    string current_token;
    bool incoming_string = false;
    for (char ch: in) {
        if (incoming_string) {
            if (ch == '"') {
                incoming_string = false;
                tokens.push_back({Tokens::STRING, current_token});
                current_token.clear();
            } else {
                current_token += ch;
            }
        } else if (isalnum(ch) || ch == '_') {
            current_token += ch;
        } else if (ch == '\"') {
            incoming_string = true;
            current_token.clear();
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')' ||
                   ch == ';' || ch == '=' || ch == '<' || ch == '{' || ch == '}') {
            if (!current_token.empty()) tokenizers(tokens, current_token);
            switch (ch) {
                case ('+'):
                    tokens.push_back({Tokens::ADDITION, ch});
                    break;
                case ('-'):
                    tokens.push_back({Tokens::SUBSTRACITON, ch});
                    break;
                case ('*'):
                    tokens.push_back({Tokens::MULTIPLY, ch});
                    break;
                case ('/'):
                    tokens.push_back({Tokens::DIVIDE, ch});
                    break;
                case ('^'):
                    tokens.push_back({Tokens::POWER, ch});
                    break;
                case ('('):
                    tokens.push_back({Tokens::LEFT_PAR, ch});
                    break;
                case (')'):
                    tokens.push_back({Tokens::RIGHT_PAR, ch});
                    break;
                case ('{'):
                    tokens.push_back({Tokens::LEFT_CURLY_BRACE, ch});
                    break;
                case ('}'):
                    tokens.push_back({Tokens::RIGHT_CURLY_BRACE, ch});
                    break;
                case (';'):
                    tokens.push_back({Tokens::SEMICOLON, ch});
                    break;
                case ('='):
                    tokens.push_back({Tokens::EQUAL, ch});
                    break;
                case ('<'):
                    tokens.push_back({Tokens::SMALLER, ch});
                    break;
            }
        } else if (isspace(ch)) {
            if (!current_token.empty()) tokenizers(tokens, current_token);
        } else {
            throw runtime_error(string("Lexer error: Unrecognized character: ").append(1, ch));
        }
    }
    if (!current_token.empty()) tokenizers(tokens, current_token);
    if (incoming_string) throw runtime_error("Lexer error: Unterminated string literal.");
}

Tokenizer tokenizer;
