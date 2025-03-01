#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

// yes, I used enum class just to get the pretty scope qualifier (same with AST)
enum class TokenType {
    INSTRUCTION,
    REGISTER,
    IMMEDIATE,
    LABEL,
    COMMA,
    LPAREN,
    RPAREN,
    DIRECTIVE, 
    END
};

struct Token {
    TokenType token;
    std::string val;
    int line;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
std::vector<Token> tokenize(const std::string& source);

#endif // !LEXER_HPP
