#ifndef PARSER_HPP
#define PARSER_HPP

#include "assembler/lexer.hpp"
#include <string>
#include <vector>

/*enum class Type {INSTRUCTION, LABEL, DIRECTIVE};*/

struct ASTNode {
public:
  // yes, I used enum class just to get the pretty scope qualifier
  TokenType type;
  std::string val;
  std::vector<Token> args;
};

std::vector<ASTNode> parse(std::vector<Token> tokens);
std::ostream& operator<<(std::ostream& out, const ASTNode& node);

#endif // !PARSER_HPP

