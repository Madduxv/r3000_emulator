#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/parser.hpp"
#include <iostream>
#include <ostream>
#include "test/testEncoding.hpp"


/**
 * @brief Encodes an assembly instruction for testing purposes.
 *
 * @param input     The assembly instruction to be encoded.
 * @return uint32_t The resulting machine-code representation of the assembly instruction.
 */
uint32_t testEncoding(const std::string input) {
  ASMInstruction encoder;
  std::vector<Token> tokens = tokenize(input);
  std::vector<ASTNode> nodes = parse(tokens);
  uint32_t encoded = encoder.encode(nodes[0], 0);


  // debug prints--------------------

  // for(const ASTNode node : nodes) {
  //   if ("lb" == node.val) {
  //     std::cout << node << std::endl;
  //     std::cout << encoded << std::endl;
  //   }
  // }

  // --------------------------------

  return encoded;
}
