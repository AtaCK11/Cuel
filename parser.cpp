//
// Created by atack on 09/17/2024.
//

#include "parser.h"

#include <unordered_set>

const std::unordered_set<std::string> rightNeededExpressionSet = {
    "=", "+", "-", "*", "/", "%", "+=", "-=", "*=", "/=", "%=",
    "==", "!=", ">", "<", ">=", "<=", "&&", "||",
    "&", "|", "^", "<<", ">>"
};

std::shared_ptr<ASTNode> Parser::parsePrimary()
{
    Token token = currentToken();
    std::shared_ptr<ASTNode> expression;

    switch (token.type) {
    case TOK_IDENTIFIER:
        expression = std::make_shared<VariableNode>(token.value);
        consume(TOK_IDENTIFIER);

        while (lookCurrent(TOK_DOT)) {
            consume(TOK_DOT);
            const Token nextToken = currentToken();
            expect(TOK_IDENTIFIER);

            std::string nextTokenValue = nextToken.value;
            consume(TOK_IDENTIFIER);

            expression = std::make_shared<MemberAccessNode>(expression, nextTokenValue);

            if (lookCurrent(TOK_OPEN_PAREN)) {
                std::vector<std::shared_ptr<ASTNode>> arguments;
                consume(TOK_OPEN_PAREN);

                while (!lookCurrent(TOK_CLOSE_PAREN)) {
                    arguments.push_back(parseExpression());
                    if (lookCurrent(TOK_COMMA)) {
                        consume(TOK_COMMA);
                    }
                }

                consume(TOK_CLOSE_PAREN);
                expression = std::make_shared<FunctionCallNode>(expression, arguments);
            }
        }

        break;

    case TOK_NUMBER:
        expression = std::make_shared<LiteralNode>(LITERAL_NUMBER, std::make_shared<NumberNode>(std::stoi(token.value)));
        consume(TOK_NUMBER);
        break;

    case TOK_STRING:
        expression = std::make_shared<LiteralNode>(LITERAL_STRING, std::make_shared<StringNode>(token.value));
        consume(TOK_STRING);
        break;

    case TOK_TRUE:
        expression = std::make_shared<LiteralNode>(LITERAL_TRUE, std::make_shared<BooleanNode>(true));
        consume(TOK_TRUE);
        break;

    case TOK_FALSE:
        expression = std::make_shared<LiteralNode>(LITERAL_FALSE, std::make_shared<BooleanNode>(false));
        consume(TOK_FALSE);
        break;

    case TOK_OPEN_PAREN: { // Handle '(' for sub-expressions
            consume(TOK_OPEN_PAREN); // Consume '('

            // Parse the sub-expression inside the parentheses
            auto subExpr = parseExpression(0); // Reset precedence to 0 inside the parentheses

            // Ensure we have a matching ')'
            if (currentToken().type != TOK_CLOSE_PAREN) {
                throw std::runtime_error("Expected ')' after sub-expression");
            }
            consume(TOK_CLOSE_PAREN); // Consume ')'

            return subExpr; // Return the result of the sub-expression
    }

    case TOK_EOF:
        throw std::runtime_error("Unexpected end of file");

    default:
        throw std::runtime_error("Unexpected token: " + tokenTypeToString(currentToken().type) +
            + " Position: " + std::to_string(currentToken().position) + " | String: " + currentToken().value);
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::parseExpression(int precedence) {
    auto left = parsePrimary();

    while (true) {
        Token token = currentToken();
        int currentPrecedence = getOperatorPrecedence(token.type);


        if (rightNeededExpressionSet.contains(token.value) && currentPrecedence >= precedence) {
            consume(token.type);

            auto right = parseExpression(currentPrecedence + 1); // recurse into expression to find if theres more

            left = std::make_shared<BinaryOperationNode>(left, token.type, right);
        } else {
            break;
        }
    }

    return left;
}

std::shared_ptr<ASTNode> Parser::parseAssignmentStatement(std::shared_ptr<ASTNode> primary)
{
    consume(TOK_ASSIGNMENT);
    auto valueNode = parseExpression();
    return std::make_shared<AssignmentStatementNode>(primary, valueNode);
}

std::shared_ptr<ASTNode> Parser::parseVariableDeclarationStatement()
{

    consume(TOK_VAR);
    Token variableToken = currentToken();
    auto variableNode = std::make_shared<VariableNode>(variableToken.value);
    // check if its var<identifier>
    if (lookCurrent(TOK_ELSEIF_STATEMENT)) {
        consume(TOK_IDENTIFIER);
    }
    consume(TOK_IDENTIFIER);
    consume(TOK_ASSIGNMENT);
    auto valueNode = parseExpression();
    consume(TOK_SEMICOLON);
    return std::make_shared<VariableDeclarationStatementNode>(variableNode, valueNode);
}

std::shared_ptr<ASTNode> Parser::parseGlobalDeclarationStatement()
{
    consume(TOK_GLOBAL_VAR);
    Token variableToken = currentToken();
    auto variableNode = std::make_shared<VariableNode>(variableToken.value);
    consume(TOK_IDENTIFIER);
    consume(TOK_ASSIGNMENT);
    auto valueNode = parseExpression();
    consume(TOK_SEMICOLON);
    return std::make_shared<GlobalDeclarationStatementNode>(variableNode, valueNode);
}

std::shared_ptr<ASTNode> Parser::parseIfStatement()
{
    consume(TOK_IF_STATEMENT);
    consume(TOK_OPEN_PAREN);
    auto condition = parseExpression(); // parse the condition
    consume(TOK_CLOSE_PAREN);
    consume(TOK_OPEN_BRACE);
    if (lookCurrent(TOK_CLOSE_BRACE)) {
        // empty body
    }
    auto body = parseStatement(true); // parse body
    consume(TOK_CLOSE_BRACE);

    // look for elseif statemets, can be more than one
    auto elseifBodies = std::vector<std::shared_ptr<ASTNode>>();
    while (lookCurrent(TOK_ELSEIF_STATEMENT)) {
        consume(TOK_ELSEIF_STATEMENT);
        consume(TOK_OPEN_PAREN);
        auto elseifCondition = parseExpression(); // parse the condition
        consume(TOK_CLOSE_PAREN);
        consume(TOK_OPEN_BRACE);
        auto elseifBody = parseStatement(true); // parse body
        consume(TOK_CLOSE_BRACE);
        elseifBodies.push_back(std::make_shared<ElseIfStatementNode>(elseifCondition, elseifBody));
    }

    // look for else statement
    std::shared_ptr<ASTNode> elseBody = nullptr;
    if (lookCurrent(TOK_ELSE_STATEMENT)) {
        consume(TOK_ELSE_STATEMENT);
        consume(TOK_OPEN_BRACE);
        elseBody = parseStatement(true); // parse body
        consume(TOK_CLOSE_BRACE);
    }

    return std::make_shared<IfStatementNode>(condition, body, elseifBodies, elseBody);
}

std::shared_ptr<ASTNode> Parser::parseForStatement()
{
    throw std::runtime_error("for loop not implemented");
}


std::shared_ptr<ASTNode> Parser::parseWhileStatement()
{
    consume(TOK_WHILE_STATEMENT);
    consume(TOK_OPEN_PAREN);
    auto condition = parseExpression(); // parse the condition
    consume(TOK_CLOSE_PAREN);
    consume(TOK_OPEN_BRACE);
    auto body = parseStatement(true); // parse body
    consume(TOK_CLOSE_BRACE);
    return std::make_shared<WhileStatementNode>(condition, body);
}


std::shared_ptr<ASTNode> Parser::parseReturnStatement()
{
    consume(TOK_RETURN_STATEMENT);
    auto expression = parseExpression();
    consume(TOK_SEMICOLON);
    return std::make_shared<ReturnStatementNode>(expression);
}

std::shared_ptr<ASTNode> Parser::parseStatement(const bool isBody) {
    std::vector<std::shared_ptr<ASTNode>> statements;

    while (isBody) {
        if (lookCurrent(TOK_EOF) || lookCurrent(TOK_CLOSE_BRACE)) {
            break;
        }

        if (lookCurrent(TOK_VAR)) {
            statements.push_back(parseVariableDeclarationStatement());
        } else if (lookCurrent(TOK_GLOBAL_VAR)) {
            statements.push_back(parseGlobalDeclarationStatement());
        } else if (lookCurrent(TOK_FOR_STATEMENT)) {
            statements.push_back(parseForStatement());
        } else if (lookCurrent(TOK_WHILE_STATEMENT)) {
            statements.push_back(parseWhileStatement());
        } else if (lookCurrent(TOK_IF_STATEMENT)) {
            statements.push_back(parseIfStatement());
        } else if (lookCurrent(TOK_RETURN_STATEMENT)) {
            statements.push_back(parseReturnStatement());
        } else if (lookCurrent(TOK_IDENTIFIER)) {
            auto primaryExpression = parsePrimary();

            // check if current is assignment
            if (lookCurrent(TOK_ASSIGNMENT)) {
                statements.push_back(parseAssignmentStatement(primaryExpression));
            } else {
                // if not, its a standalone function call or expression
                statements.push_back(primaryExpression);
            }
        } else if (lookCurrent(TOK_BREAK_STATEMENT)) {
            consume(TOK_BREAK_STATEMENT);
            consume(TOK_SEMICOLON);
            statements.push_back(std::make_shared<BreakStatementNode>());
        } else if (lookCurrent(TOK_CONTINUE_STATEMENT)) {
            consume(TOK_CONTINUE_STATEMENT);
            consume(TOK_SEMICOLON);
            statements.push_back(std::make_shared<ContinueStatementNode>());
        } else {
            consume(currentToken().type);
            continue;
        }

        //consume(TOK_SEMICOLON);
    }

    // if empty, add EmptyStatementNode
    if (statements.empty()) {
        statements.push_back(std::make_shared<EmptyStatementNode>());
    }
    return std::make_shared<BlockStatementNode>(statements);
}
