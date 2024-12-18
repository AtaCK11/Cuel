#include "tokenize.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <unordered_set>

bool isNumber(const std::string& word);
bool isBoolean(const std::string& word);
bool isDataType(const std::string& str);
bool isOperator(char ch);
bool isSpecialChar(char ch);
TokenType getTokenType(const std::string& keyword);
TokenType getOperatorType(const std::string& op);
TokenType getStatementType(const std::string& word);
TokenType getDataType(const std::string& word);
TokenType getBooleanType(const std::string& word);
TokenType checkTypedVariable(const std::string& word);

bool isNumber(const std::string& word) {
    return !word.empty() && std::ranges::all_of(word, ::isdigit);
}

bool isBoolean(const std::string& word) {
    return word == "true" || word == "false";
}

bool isOperator(const std::string& word) {
    static const std::unordered_set<std::string> operators = {
        "=", "+", "-", "*", "/", "%", "+=", "-=", "*=", "/=", "%=",
        "++", "--",
        "==", "!=", ">", "<", ">=", "<=",
        "&&", "||", "!", "&", "|", "^", "~", "<<", ">>"
    };
    return operators.contains(word);
}

bool isSpecialChar(const char ch) {
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == '.';
}

TokenType getTokenType(const std::string& keyword) {
    if (keyword == "=") return TOK_ASSIGNMENT;
    if (keyword == "+") return TOK_ADDITION;
    if (keyword == "-") return TOK_SUBTRACTION;
    if (keyword == "*") return TOK_MULTIPLICATION;
    if (keyword == "/") return TOK_DIVISION;
    if (keyword == "%") return TOK_MODULUS;
    if (keyword == "+=") return TOK_ADDITION_ASSIGNMENT;
    if (keyword == "-=") return TOK_SUBTRACTION_ASSIGNMENT;
    if (keyword == "*=") return TOK_MULTIPLICATION_ASSIGNMENT;
    if (keyword == "/=") return TOK_DIVISION_ASSIGNMENT;
    if (keyword == "%=") return TOK_MODULUS_ASSIGNMENT;
    if (keyword == "++") return TOK_INCREMENT;
    if (keyword == "--") return TOK_DECREMENT;
    if (keyword == "==") return TOK_EQUAL;
    if (keyword == "!=") return TOK_NOT_EQUAL;
    if (keyword == ">") return TOK_GREATER;
    if (keyword == "<") return TOK_LESS;
    if (keyword == ">=") return TOK_GREATER_EQUAL;
    if (keyword == "<=") return TOK_LESS_EQUAL;
    if (keyword == "&&") return TOK_AND;
    if (keyword == "||") return TOK_OR;
    if (keyword == "!") return TOK_NOT;
    if (keyword == "&") return TOK_BITWISE_AND;
    if (keyword == "|") return TOK_BITWISE_OR;
    if (keyword == "^") return TOK_BITWISE_XOR;
    if (keyword == "~") return TOK_BITWISE_NOT;
    if (keyword == "<<") return TOK_LEFT_SHIFT;
    if (keyword == ">>") return TOK_RIGHT_SHIFT;

    if (keyword == ";") return TOK_SEMICOLON;

    if (keyword == "if") return TOK_IF_STATEMENT;
    if (keyword == "elseif") return TOK_ELSEIF_STATEMENT;
    if (keyword == "else") return TOK_ELSE_STATEMENT;
    if (keyword == "for") return TOK_FOR_STATEMENT;
    if (keyword == "while") return TOK_WHILE_STATEMENT;
    if (keyword == "switch") return TOK_SWITCH_STATEMENT;
    if (keyword == "case") return TOK_CASE_STATEMENT;
    if (keyword == "default") return TOK_DEFAULT_STATEMENT;
    if (keyword == "break") return TOK_BREAK_STATEMENT;
    if (keyword == "continue") return TOK_CONTINUE_STATEMENT;
    if (keyword == "return") return TOK_RETURN_STATEMENT;
    if (keyword == "true") return TOK_TRUE;
    if (keyword == "false") return TOK_FALSE;
    if (keyword == "var") return TOK_VAR;

    return TOK_UNKNOWN;
}

TokenType getOperatorType(const std::string& op) {
    return isOperator(op) ? getTokenType(op) : TOK_UNKNOWN;
}

TokenType getStatementType(const std::string& word) {
    if (word == "if") return TOK_IF_STATEMENT;
    if (word == "elseif") return TOK_ELSEIF_STATEMENT;
    if (word == "else") return TOK_ELSE_STATEMENT;
    if (word == "for") return TOK_FOR_STATEMENT;
    if (word == "while") return TOK_WHILE_STATEMENT;
    if (word == "switch") return TOK_SWITCH_STATEMENT;
    if (word == "case") return TOK_CASE_STATEMENT;
    if (word == "default") return TOK_DEFAULT_STATEMENT;
    if (word == "break") return TOK_BREAK_STATEMENT;
    if (word == "continue") return TOK_CONTINUE_STATEMENT;
    if (word == "return") return TOK_RETURN_STATEMENT;

    return TOK_UNKNOWN;
}

TokenType getDataType(const std::string& word) {
    if (word == "var") return TOK_VAR;
    if (word == "const") return TOK_VAR_CONST;
    if (word == "global") return TOK_GLOBAL_VAR;
    if (word == "number" || word == "string" || word == "bool") return TOK_VAR_TYPE;

    return TOK_UNKNOWN;
}

TokenType getBooleanType(const std::string& word) {
    return isBoolean(word) ? (word == "true" ? TOK_TRUE : TOK_FALSE) : TOK_UNKNOWN;
}

TokenType checkTypedVariable(const std::string& word) {
    const size_t angleBracketPos = word.find('<');
    if (angleBracketPos != std::string::npos && word.back() == '>') {
        const std::string type = word.substr(angleBracketPos + 1, word.size() - angleBracketPos - 2);
        if (type == "number" || type == "string" || type == "bool") {
            return TOK_VAR;
        }
    }
    return TOK_UNKNOWN;
}

std::vector<Token> tokenize(const std::string& sourceCode) {
    std::vector<Token> tokens;
    std::string currentWord;
    bool insideString = false;
    std::string currentString;

    for (size_t i = 0; i < sourceCode.size(); ++i) {
        const char ch = sourceCode[i];

        if (ch == '"' || ch == '\'') {
            if (insideString) {
                currentString += ch;
                tokens.emplace_back(currentString, TOK_STRING, i);
                insideString = false;
                currentString.clear();
            } else {
                insideString = true;
                currentString += ch;
            }
        } else if (insideString) {
            currentString += ch;
        } else if (std::isspace(ch)) {
            if (!currentWord.empty()) {
                TokenType type = checkTypedVariable(currentWord);
                if (type == TOK_UNKNOWN) {
                    type = getDataType(currentWord);
                    if (type == TOK_UNKNOWN) {
                        type = isNumber(currentWord) ? TOK_NUMBER : getStatementType(currentWord);
                        if (type == TOK_UNKNOWN) {
                            type = isBoolean(currentWord) ? getBooleanType(currentWord) : TOK_IDENTIFIER;
                        }
                    }
                }
                tokens.emplace_back(currentWord, type, i);
                currentWord.clear();
            }
        } else if (std::isalnum(ch) || ch == '_') {
            currentWord += ch;
        } else {
            if (!currentWord.empty()) {
                TokenType type = checkTypedVariable(currentWord);
                if (type == TOK_UNKNOWN) {
                    type = getDataType(currentWord);
                    if (type == TOK_UNKNOWN) {
                        type = isNumber(currentWord) ? TOK_NUMBER : getStatementType(currentWord);
                        if (type == TOK_UNKNOWN) {
                            type = isBoolean(currentWord) ? getBooleanType(currentWord) : TOK_IDENTIFIER;
                        }
                    }
                }
                tokens.emplace_back(currentWord, type, i);
                currentWord.clear();
            }

            switch (ch) {
                case '(': tokens.emplace_back("(", TOK_OPEN_PAREN, i); break;
                case ')': tokens.emplace_back(")", TOK_CLOSE_PAREN, i); break;
                case '{': tokens.emplace_back("{", TOK_OPEN_BRACE, i); break;
                case '}': tokens.emplace_back("}", TOK_CLOSE_BRACE, i); break;
                case '[': tokens.emplace_back("[", TOK_OPEN_BRACKET, i); break;
                case ']': tokens.emplace_back("]", TOK_CLOSE_BRACKET, i); break;
                case ';': tokens.emplace_back(";", TOK_SEMICOLON, i); break;
                case '.': tokens.emplace_back(".", TOK_DOT, i); break;
                case ',': tokens.emplace_back(",", TOK_COMMA, i); break;
                default: {
                    std::string op(1, ch);
                    if (i + 1 < sourceCode.size()) {
                        const char nextChar = sourceCode[i + 1];
                        std::string potentialOp = op + nextChar;
                        if (getOperatorType(potentialOp) != TOK_UNKNOWN) {
                            tokens.emplace_back(potentialOp, getOperatorType(potentialOp), i);
                            ++i;  // skip next ^^^^^^^
                            continue;
                        }
                    }
                    tokens.emplace_back(std::string(1, ch), getOperatorType(op), i);
                } break;
            }
        }
    }

    if (!currentWord.empty()) {
        TokenType type = checkTypedVariable(currentWord);
        if (type == TOK_UNKNOWN) {
            type = getDataType(currentWord);
            if (type == TOK_UNKNOWN) {
                type = isNumber(currentWord) ? TOK_NUMBER : getStatementType(currentWord);
                if (type == TOK_UNKNOWN) {
                    type = isBoolean(currentWord) ? getBooleanType(currentWord) : TOK_IDENTIFIER;
                }
            }
        }
        tokens.emplace_back(currentWord, type, sourceCode.size());
    }

    // add empty token at the end
    tokens.emplace_back("", TOK_EOF, sourceCode.size());
    return tokens;
}


std::string tokenTypeToString(const TokenType type)
{
    switch (type)
    {
        case TOK_NUMBER: return "TOK_NUMBER";
        case TOK_STRING: return "TOK_STRING";
        case TOK_TRUE: return "TOK_TRUE";
        case TOK_FALSE: return "TOK_FALSE";
        case TOK_OPEN_PAREN: return "TOK_OPEN_PAREN";
        case TOK_CLOSE_PAREN: return "TOK_CLOSE_PAREN";
        case TOK_OPEN_BRACE: return "TOK_OPEN_BRACE";
        case TOK_CLOSE_BRACE: return "TOK_CLOSE_BRACE";
        case TOK_OPEN_BRACKET: return "TOK_OPEN_BRACKET";
        case TOK_CLOSE_BRACKET: return "TOK_CLOSE_BRACKET";
        case TOK_SEMICOLON: return "TOK_SEMICOLON";
        case TOK_DOT: return "TOK_DOT";
        case TOK_IDENTIFIER: return "TOK_IDENTIFIER";
        case TOK_COMMENT: return "TOK_COMMENT";
        case TOK_WHITESPACE: return "TOK_WHITESPACE";
        // statements
        case TOK_IF_STATEMENT: return "TOK_IF_STATEMENT";
        case TOK_ELSEIF_STATEMENT: return "TOK_ELSEIF_STATEMENT";
        case TOK_ELSE_STATEMENT: return "TOK_ELSE_STATEMENT";
        case TOK_FOR_STATEMENT: return "TOK_FOR_STATEMENT";
        case TOK_WHILE_STATEMENT: return "TOK_WHILE_STATEMENT";
        case TOK_SWITCH_STATEMENT: return "TOK_SWITCH_STATEMENT";
        case TOK_CASE_STATEMENT: return "TOK_CASE_STATEMENT";
        case TOK_BREAK_STATEMENT: return "TOK_BREAK_STATEMENT";
        case TOK_CONTINUE_STATEMENT: return "TOK_CONTINUE_STATEMENT";
        case TOK_RETURN_STATEMENT: return "TOK_RETURN_STATEMENT";
        // data types
        case TOK_VAR: return "TOK_VAR";
        case TOK_VAR_TYPE: return "TOK_VAR_TYPE";
        case TOK_VAR_CONST: return "TOK_VAR_CONST";
        case TOK_GLOBAL_VAR: return "TOK_GLOBAL_VAR";
        // operators
        case TOK_ASSIGNMENT: return "TOK_ASSIGNMENT";
        case TOK_ADDITION: return "TOK_ADDITION";
        case TOK_SUBTRACTION: return "TOK_SUBTRACTION";
        case TOK_MULTIPLICATION: return "TOK_MULTIPLICATION";
        case TOK_DIVISION: return "TOK_DIVISION";
        case TOK_MODULUS: return "TOK_MODULUS";
        case TOK_ADDITION_ASSIGNMENT: return "TOK_ADDITION_ASSIGNMENT";
        case TOK_SUBTRACTION_ASSIGNMENT: return "TOK_SUBTRACTION_ASSIGNMENT";
        case TOK_MULTIPLICATION_ASSIGNMENT: return "TOK_MULTIPLICATION_ASSIGNMENT";
        case TOK_DIVISION_ASSIGNMENT: return "TOK_DIVISION_ASSIGNMENT";
        case TOK_MODULUS_ASSIGNMENT: return "TOK_MODULUS_ASSIGNMENT";
        case TOK_INCREMENT: return "TOK_INCREMENT";
        case TOK_DECREMENT: return "TOK_DECREMENT";
        case TOK_EQUAL: return "TOK_EQUAL";
        case TOK_NOT_EQUAL: return "TOK_NOT_EQUAL";
        case TOK_GREATER: return "TOK_GREATER";
        case TOK_LESS: return "TOK_LESS";
        case TOK_GREATER_EQUAL: return "TOK_GREATER_EQUAL";
        case TOK_LESS_EQUAL: return "TOK_LESS_EQUAL";
        case TOK_AND: return "TOK_AND";
        case TOK_OR: return "TOK_OR";
        case TOK_NOT: return "TOK_NOT";
        case TOK_BITWISE_AND: return "TOK_BITWISE_AND";
        case TOK_BITWISE_OR: return "TOK_BITWISE_OR";
        case TOK_BITWISE_XOR: return "TOK_BITWISE_XOR";
        case TOK_BITWISE_NOT: return "TOK_BITWISE_NOT";
        case TOK_LEFT_SHIFT: return "TOK_LEFT_SHIFT";
        case TOK_RIGHT_SHIFT: return "TOK_RIGHT_SHIFT";

        case TOK_EOF: return "TOK_EOF";
        case TOK_UNKNOWN: return "whats this";

        default: return "UNIMPLEMENTED";
    }
}