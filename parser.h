//
// Created by atack on 09/17/2024.
//

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <stdexcept>
#include <string>
#include "tokenize.h"

class Token;

enum LiteralType {
    LITERAL_NUMBER,
    LITERAL_STRING,
    LITERAL_TRUE,
    LITERAL_FALSE
};

enum VariableType {
    VARIABLE_NUMBER,
    VARIABLE_STRING,
    VARIABLE_BOOLEAN,
    VARIABLE_GENERIC
};

enum NodeType {
    NODE_VARIABLE,
    NODE_LITERAL,
    EXPRESSION_FUNCTION_CALL,
    EXPRESSION_MEMBER_ACCESS,
    EXPRESSION_BINARY_OPERATION,
    STATEMENT_EMPTY,
    STATEMENT_BLOCK,
    STATEMENT_ASSIGNMENT,
    STATEMENT_VARIABLE_DECLARATION,
    STATEMENT_GLOBAL_DECLARATION,
    STATEMENT_IF,
    STATEMENT_ELSE_IF,
    STATEMENT_ELSE,
    STATEMENT_WHILE,
    STATEMENT_RETURN,
    STATEMENT_FUNCTION_CALL,
    STATEMENT_BREAK,
    STATEMENT_CONTINUE
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class VariableNode final : public ASTNode {
public:
    std::string name;
    explicit VariableNode(std::string name) : name(std::move(name)) { }
};

class NumberNode final : public ASTNode {
public:
    int value;
    explicit NumberNode(const int value) : value(value) {}
};

class StringNode final : public ASTNode {
public:
    std::string value;
    explicit StringNode(std::string value) : value(std::move(value)) {}
};

class BooleanNode final : public ASTNode {
public:
    bool value;
    explicit BooleanNode(const bool value) : value(value) {}
};

class LiteralNode final : public ASTNode {
public:
    LiteralType type;
    std::shared_ptr<ASTNode> value;
    explicit LiteralNode(const LiteralType type, std::shared_ptr<ASTNode> value) : type(type), value(std::move(value)) {}
};

// Expressions
class FunctionCallNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> object;
    std::vector<std::shared_ptr<ASTNode>> arguments;
    FunctionCallNode(std::shared_ptr<ASTNode> object, std::vector<std::shared_ptr<ASTNode>> arguments)
        : object(std::move(object)), arguments(std::move(arguments)) {}
};

class MemberAccessNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> object;
    std::string member;
    MemberAccessNode(std::shared_ptr<ASTNode> object, std::string member)
        : object(std::move(object)), member(std::move(member)) {}
};

class BinaryOperationNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    TokenType operation; // This could represent the operator

    BinaryOperationNode(std::shared_ptr<ASTNode> left, const TokenType operation, std::shared_ptr<ASTNode> right)
        : left(std::move(left)), right(std::move(right)), operation(operation) {}
};

// Statements
class EmptyStatementNode final : public ASTNode {
public:
    EmptyStatementNode() = default;
};

class BlockStatementNode final : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> statements;
    explicit BlockStatementNode(std::vector<std::shared_ptr<ASTNode>> statements) : statements(std::move(statements)) {}
};

class AssignmentStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> variable;
    std::shared_ptr<ASTNode> value;
    AssignmentStatementNode(std::shared_ptr<ASTNode> var, std::shared_ptr<ASTNode> val)
        : variable(std::move(var)), value(std::move(val)) {}
};

class VariableDeclarationStatementNode final : public ASTNode {
public:
    std::shared_ptr<VariableNode> variable;
    std::shared_ptr<ASTNode> value;
    VariableDeclarationStatementNode(std::shared_ptr<VariableNode> var, std::shared_ptr<ASTNode> val)
        : variable(std::move(var)), value(std::move(val)) {}
};

class GlobalDeclarationStatementNode final : public ASTNode {
public:
    std::shared_ptr<VariableNode> variable;
    std::shared_ptr<ASTNode> value;
    GlobalDeclarationStatementNode(std::shared_ptr<VariableNode> var, std::shared_ptr<ASTNode> val)
        : variable(std::move(var)), value(std::move(val)) {}
};

class IfStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    std::vector<std::shared_ptr<ASTNode>> elseifBodies;
    std::shared_ptr<ASTNode> elseBody;
    IfStatementNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> body,
                    std::vector<std::shared_ptr<ASTNode>> elseifBodies, std::shared_ptr<ASTNode> elseBody)
        : condition(std::move(cond)), body(std::move(body)), elseifBodies(std::move(elseifBodies)), elseBody(std::move(elseBody)) {}
};

class ElseIfStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    ElseIfStatementNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> body)
        : condition(std::move(cond)), body(std::move(body)) {}
};

class ElseStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> body;
    explicit ElseStatementNode(std::shared_ptr<ASTNode> body) : body(std::move(body)) {}
};

class WhileStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    WhileStatementNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> body)
        : condition(std::move(cond)), body(std::move(body)) {}
};

class ReturnStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> expressions;
    explicit ReturnStatementNode(std::shared_ptr<ASTNode> expressions) : expressions(std::move(expressions)) {}
};

class FunctionCallStatementNode final : public ASTNode {
public:
    std::shared_ptr<ASTNode> object;
    std::vector<std::shared_ptr<ASTNode>> arguments;
    FunctionCallStatementNode(std::shared_ptr<ASTNode> object, std::vector<std::shared_ptr<ASTNode>> arguments)
        : object(std::move(object)), arguments(std::move(arguments)) {}
};

class BreakStatementNode final : public ASTNode {
public:
    BreakStatementNode() = default;
};

class ContinueStatementNode final : public ASTNode {
public:
    ContinueStatementNode() = default;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;

    static int getOperatorPrecedence(const TokenType type) {
        switch (type) {
        case TOK_INCREMENT:       return 15;
        case TOK_DECREMENT:       return 15;
        case TOK_NOT:             return 14;
        case TOK_BITWISE_NOT:     return 14;
        case TOK_MULTIPLICATION:  return 13;
        case TOK_DIVISION:        return 13;
        case TOK_MODULUS:         return 13;
        case TOK_ADDITION:        return 12;
        case TOK_SUBTRACTION:     return 12;
        case TOK_LEFT_SHIFT:      return 11;
        case TOK_RIGHT_SHIFT:     return 11;
        case TOK_GREATER:         return 10;
        case TOK_LESS:            return 10;
        case TOK_GREATER_EQUAL:   return 10;
        case TOK_LESS_EQUAL:      return 10;
        case TOK_EQUAL:           return 9;
        case TOK_NOT_EQUAL:       return 9;
        case TOK_BITWISE_AND:     return 8;
        case TOK_BITWISE_XOR:     return 7;
        case TOK_BITWISE_OR:      return 6;
        case TOK_AND:             return 5;
        case TOK_OR:              return 4;

        case TOK_ASSIGNMENT:      return 1;
        case TOK_ADDITION_ASSIGNMENT:  return 1;
        case TOK_SUBTRACTION_ASSIGNMENT: return 1;
        case TOK_MULTIPLICATION_ASSIGNMENT: return 1;
        case TOK_DIVISION_ASSIGNMENT: return 1;
        case TOK_MODULUS_ASSIGNMENT: return 1;

        default:                  return 0;  // unknown opr
        }
    }

    Token getTokenAt(const int offset) {
        if (offset >= tokens.size()) {
            return Token("", TOK_EOF, -1);
        }
        return tokens[offset];
    }

    Token previousToken() {
        if (current == 0) {
            throw std::runtime_error("Unexpected start of tokens");
        }
        return tokens[current - 1];
    }

    Token currentToken() {
        if (current >= tokens.size()) {
            throw std::runtime_error("Unexpected end of tokens");
        }
        return tokens[current];
    }

    Token nextToken() {
        if (current + 1 >= tokens.size()) {
            return Token("", TOK_EOF, -1);
        }
        return tokens[current + 1];
    }

    void consume(const TokenType expectedType) {
        if (currentToken().type != expectedType) {
            throw std::runtime_error("Unexpected token: " + tokenTypeToString(currentToken().type) +
                ", expected: " + tokenTypeToString(expectedType) + " Position: " + std::to_string(currentToken().position) + " | String: " + currentToken().value);
        }
        current++;
    }

    void expect(const TokenType expectedType) {
        if (currentToken().type != expectedType) {
            throw std::runtime_error("Unexpected token: " + tokenTypeToString(currentToken().type) +
                ", expected: " + tokenTypeToString(expectedType) + " Position: " + std::to_string(currentToken().position) + " | String: " + currentToken().value);
        }
    }

    [[nodiscard]] bool lookCurrent(const TokenType expectedType) const
    {
        return tokens[current].type == expectedType;
    }

    [[nodiscard]] bool lookAhead(const TokenType expectedType, const int offset = 1) const
    {
        return tokens[current + offset].type == expectedType;
    }


    std::shared_ptr<ASTNode> parsePrimary(); // parse primary expressions

    std::shared_ptr<ASTNode> parseExpression(int precedence = 0);

    std::shared_ptr<ASTNode> parseVariableDeclarationStatement();

    std::shared_ptr<ASTNode> parseGlobalDeclarationStatement();

    std::shared_ptr<ASTNode> parseAssignmentStatement(std::shared_ptr<ASTNode> primary);

    std::shared_ptr<ASTNode> parseIfStatement();

    std::shared_ptr<ASTNode> parseForStatement();

    std::shared_ptr<ASTNode> parseWhileStatement();

    std::shared_ptr<ASTNode> parseSwitchStatement();

    std::shared_ptr<ASTNode> parseBreakStatement();

    std::shared_ptr<ASTNode> parseContinueStatement();

    std::shared_ptr<ASTNode> parseReturnStatement();

    std::shared_ptr<ASTNode> parseStatement(bool isBody);

public:

    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    std::vector<std::shared_ptr<ASTNode>> parse() {
        std::vector<std::shared_ptr<ASTNode>> statements;
        while (current < tokens.size()-1) { // last is EOF
            statements.push_back(parseStatement(true));
        }
        return statements;
    }
};

#endif //PARSER_H
