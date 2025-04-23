#include "assembler/parser.hpp"
#include "assembler/lexer.hpp"
#include <cstdlib>
#include <ostream>
#include <vector>
#include <iostream>

/**
 * @brief Turns an AST Token into a string for debug printing
 *
 * @param out             The output stream to write the string to.
 * @param token           The AST node to be converted.
 * @return std::ostream&  The output stream with the string appended.
 */
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


/**
 * @brief Combines associated tokens to create an abstract syntax tree.
 *
 * @param tokens                The list of tokens created from the assembly file.
 * @return std::vector<ASTNode> The new abstract syntax tree
 */
std::vector<ASTNode> parse(std::vector<Token> tokens) {
  std::vector<ASTNode> AST;

  for (int i = 0; i < tokens.size();) {
    Token token = tokens[i];

    if (tokens[i].type == TokenType::INSTRUCTION || 
        tokens[i].type == TokenType::DIRECTIVE || 
        tokens[i].type == TokenType::LABEL_DECLARATION) {

      ASTNode node;
      node.type = token.type;
      node.val = token.val;
      i++;

      // .globl is special
      if (token.type == TokenType::DIRECTIVE && (".globl" == token.val || ".end" == token.val)) {
        if (i < tokens.size() && tokens[i].type == TokenType::LABEL) {
          node.args.push_back(tokens[i]);
          i++;
        } else {
          std::cout << "Error: .globl must be followed by a label\n";
          exit(1);
        }
      }

      while (i < tokens.size() && 
        tokens[i].type != TokenType::INSTRUCTION &&
        tokens[i].type != TokenType::LABEL_DECLARATION && 
        tokens[i].type != TokenType::DIRECTIVE) {

        // We don't really care about the commas or parenthesis
        if (tokens[i].type != TokenType::COMMA && tokens[i].type != TokenType::LPAREN && tokens[i].type != TokenType::RPAREN) {
          node.args.push_back(tokens[i]);
        }
        i++;

      }
      AST.push_back(node);
    } else {
      i++;
    }

  }

	return AST;
}
