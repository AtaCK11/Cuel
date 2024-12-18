#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <string>
#include <utility>
#include <vector>

enum TokenType
{
    // constant tokens
    TOK_NUMBER,
    TOK_STRING,
    TOK_TRUE,
    TOK_FALSE,
    //TOK_CHAR,
    // para ()
    TOK_OPEN_PAREN,
    TOK_CLOSE_PAREN,
    // para {}
    TOK_OPEN_BRACE,
    TOK_CLOSE_BRACE,
    // para []
    TOK_OPEN_BRACKET,
    TOK_CLOSE_BRACKET,

    // statements
    TOK_IF_STATEMENT,
    TOK_ELSEIF_STATEMENT,
    TOK_ELSE_STATEMENT,
    TOK_FOR_STATEMENT,
    TOK_WHILE_STATEMENT,
    TOK_SWITCH_STATEMENT,
    TOK_CASE_STATEMENT,
    TOK_DEFAULT_STATEMENT,
    TOK_BREAK_STATEMENT,
    TOK_CONTINUE_STATEMENT,
    TOK_RETURN_STATEMENT,

    // data types
    TOK_VAR,                                // var
    TOK_VAR_TYPE,                           // <number>, <string>, <bool>
    TOK_VAR_CONST,                          // const
    TOK_GLOBAL_VAR,                         // global

    // operators
    TOK_ASSIGNMENT,                         // =
    TOK_ADDITION,                           // +
    TOK_SUBTRACTION,                        // -
    TOK_MULTIPLICATION,                     // *
    TOK_DIVISION,                           // /
    TOK_MODULUS,                            // %
    TOK_ADDITION_ASSIGNMENT,                // +=
    TOK_SUBTRACTION_ASSIGNMENT,             // -=
    TOK_MULTIPLICATION_ASSIGNMENT,          // *=
    TOK_DIVISION_ASSIGNMENT,                // /=
    TOK_MODULUS_ASSIGNMENT,                 // %=
    TOK_INCREMENT,                          // ++
    TOK_DECREMENT,                          // --
    TOK_EQUAL,                              // ==
    TOK_NOT_EQUAL,                          // !=
    TOK_GREATER,                            // >
    TOK_LESS,                               // <
    TOK_GREATER_EQUAL,                      // >=
    TOK_LESS_EQUAL,                         // <=
    TOK_AND,                                // &&
    TOK_OR,                                 // ||
    TOK_NOT,                                // !
    TOK_BITWISE_AND,                        // &
    TOK_BITWISE_OR,                         // |
    TOK_BITWISE_XOR,                        // ^
    TOK_BITWISE_NOT,                        // ~
    TOK_LEFT_SHIFT,                         // <<
    TOK_RIGHT_SHIFT,                        // >>

    TOK_IDENTIFIER,
    TOK_SEMICOLON,
    TOK_COMMENT,
    TOK_DOT,
    TOK_COMMA,
    TOK_WHITESPACE,
    TOK_UNKNOWN,
    TOK_EOF,
};

class Token {
public:
    std::string value;
    TokenType type;
    int position;

    Token(std::string  val, const TokenType t, const int i) : value(std::move(val)), type(t), position(i){}

};

std::vector<Token> tokenize(const std::string& sourceCode);
std::string tokenTypeToString(TokenType type);

#endif //TOKENIZE_H
