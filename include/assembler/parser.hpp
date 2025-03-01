#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

struct ASTNode {
    // yes, I used enum class just to get the pretty scope qualifier
    enum class Type {INSTRUCTION, LABEL, DIRECTIVE};
    Type nodeType;
    std::string val;
    std::vector<std::string> args;
};

extern std::vector<ASTNode> AST;

#endif // !PARSER_HPP

