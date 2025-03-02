#include "assembler/parser.hpp"
#include "assembler/lexer.hpp"
#include <vector>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const ASTNode& node) {
    out << "ASTNode(Type: " << static_cast<int>(node.type)
        << ", Value: " << node.val
        << ", Args: [";
    
    for (size_t i = 0; i < node.args.size(); i++) {
        out << node.args[i]; // I think I already overloaded << for Token
        if (i != node.args.size() - 1) {
            out << ", ";
        }
    }

    out << "])";
    return out;
}


std::vector<ASTNode> parse(std::vector<Token> tokens) {
  std::vector<ASTNode> AST;

  for (int i = 0; i < tokens.size();) {
    Token token = tokens[i];

    if (tokens[i].token == TokenType::INSTRUCTION ||
        tokens[i].token == TokenType::LABEL || 
        tokens[i].token == TokenType::DIRECTIVE) {

      ASTNode node;
      node.type = token.token; // i should probably rename that
      node.val = token.val;
      i++;

      while (i < tokens.size() && 
        tokens[i].token != TokenType::INSTRUCTION &&
        tokens[i].token != TokenType::LABEL && 
        tokens[i].token != TokenType::DIRECTIVE) {

        node.args.push_back(tokens[i]);
        i++;

      }
      AST.push_back(node);
    } else {
      i++; // How did we get here?
    }

  }

	return AST;
}
