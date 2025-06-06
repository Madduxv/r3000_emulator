#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "assembler/symbols.hpp"
#include "assembler/lexer.hpp"

std::unordered_map<std::string, uint16_t> r_type = {
  {"syscall", 0x0c}, { "sll", 0x00  },  { "srl", 0x02  },  { "sra", 0x03  },
  { "jr", 0x08  },   { "jalr", 0x09  }, { "mult", 0x18  }, { "multu", 0x19  },
  { "div", 0x1a  },  { "divu", 0x1b  }, { "add", 0x20  },  { "addu", 0x21  },
  { "sub", 0x22  },  { "subu", 0x23  }, { "and", 0x24  },  { "or", 0x25  },
  { "xor", 0x26  },  { "nor", 0x27  },  { "slt", 0x2a  },  { "sltu", 0x2b  },
};

std::unordered_map<std::string, uint16_t> j_type = {
  { "j", 0x02  }, { "jal", 0x03  },
};

std::unordered_map<std::string, uint16_t> i_type = {
  { "beq", 0x04  },  { "bne", 0x05  },  { "blez", 0x06  }, { "bgtz", 0x07  },
  { "addi", 0x08  }, { "addiu", 0x09 }, { "slti", 0x0A  }, { "sltiu", 0x0B  },
  { "andi", 0x0C },  { "ori", 0x0D  },  { "xori", 0x0E },  { "lui", 0x0F  }, 
  { "lb", 0x20   },  { "lh", 0x21  },   { "lw", 0x23  },   { "lbu", 0x24  },  
  { "lhu", 0x25 },   { "sb", 0x28 },    { "sh", 0x29},     { "sw", 0x2b  },
};

std::unordered_map<std::string, uint16_t> pseudo = {
  { "li", 0  }, {"la", 0}, {"move", 0}, {"nop", 0}, {"bnez", 0}, 
  {"beqz", 0}, {"b", 0}, {"blt", 0}, {"bgt", 0}, {"bge", 0}
};
// I probably should have just used the number after the $
std::unordered_map<std::string, uint16_t> registers = {
  {"$zero", 0}, {"$at", 1},  {"$v0", 2},  {"$v1", 3},
  {"$a0", 4},   {"$a1", 5},  {"$a2", 6},  {"$a3", 7},
  {"$t0", 8},   {"$t1", 9},  {"$t2", 10}, {"$t3", 11},
  {"$t4", 12},  {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
  {"$s0", 16},  {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
  {"$s4", 20},  {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
  {"$t8", 24},  {"$t9", 25}, {"$k0", 26}, {"$k1", 27},
  {"$gp", 28},  {"$sp", 29}, {"$fp", 30}, {"$ra", 31},
  {"$0", 0},    {"$1", 1},   {"$2", 2},   {"$3", 3},
  {"$4", 4},    {"$5", 5},   {"$6", 6},   {"$7", 7},
  {"$8", 8},    {"$9", 9},   {"$10", 10}, {"$11", 11},
  {"$12", 12},  {"$13", 13}, {"$14", 14}, {"$15", 15},
  {"$16", 16},  {"$17", 17}, {"$18", 18}, {"$19", 19},
  {"$20", 20},  {"$21", 21}, {"$22", 22}, {"$23", 23},
  {"$24", 24},  {"$25", 25}, {"$26", 26}, {"$27", 27},
  {"$28", 28},  {"$29", 29}, {"$30", 30}, {"$31", 31},
};

std::unordered_map<std::string, uint32_t> labels;

/**
 * @brief Converts label declarations and static variables in the AST into memory addresses.
 *
 * @param ast The abstract syntax tree with the parsed assembly file.
 * @param mem A reference to the Memory object where static variables will be stored.
 */
void getSymbols(const std::vector<ASTNode>& ast, Memory& mem) {
  int idx = 0;
  int globalAddrPtr = 0x5000;
  while (ast.at(idx).val != ".data") { idx++; }

  for (int i = idx; i < ast.size()-1; i++) {
    if (ast.at(i).type == TokenType::LABEL_DECLARATION && ast.at(++i).type == TokenType::DIRECTIVE) {
      labels[ast.at(i-1).val] = globalAddrPtr;

      if (".ascii" == ast.at(i).val) {
        for (int j = 0; j < ast.at(i).args.at(0).val.size(); j++) {
          if ('\"' == ast.at(i).args.at(0).val.at(j)) {continue;}
          allocateAscii(ast, mem, globalAddrPtr, i, j);
        }

      } else if (".asciiz" == ast.at(i).val) {
        for (int j = 0; j < ast.at(i).args.at(0).val.size(); j++) {
          if ('\"' == ast.at(i).args.at(0).val.at(j)) {continue;}
          allocateAscii(ast, mem, globalAddrPtr, i, j);
          mem.write8(globalAddrPtr, '\0');
        }

      } else if (".word" == ast.at(i).val && i < ast.size()-1) {
        if (ast.at(i).args.at(0).val.find(',') == std::string::npos) {
          mem.write32(globalAddrPtr++, std::stoull(ast.at(i).args.at(0).val));
        } else {
          std::vector<std::string> v;
          std::stringstream ss(ast.at(i).args.at(0).val);
          while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
          }
          for (std::string strWord : v) {
            int word = std::stoi(strWord);
            mem.write8(globalAddrPtr, word);
            globalAddrPtr += 4;
          }
        }
      } else if (".space" == ast.at(i).val) {
        globalAddrPtr += std::stoull(ast.at(i).args.at(0).val);
      }
    }
  }

  uint32_t addr = 0;
  for (const ASTNode& node : ast) {
    if (node.type == TokenType::LABEL_DECLARATION && labels.find(node.val) == labels.end()) {
      labels[node.val] = addr;
      // ^^ not sure how I feel about this syntax
    } else {
      addr +=4;
    }
  }

  /*for (auto key : labels) {*/
  /*  std::cout << key.first << ": " << key.second << std::endl;*/
  /*}*/

}

/**
 * @brief Writes a byte of ascii data to the Memory object.
 *
 * @param ast     The abstract syntax tree with the parsed assembly file.
 * @param mem     A reference to the Memory object where static variables will be stored.
 * @param addrPtr The address to write the byte to.
 * @param i       The index of this ASTNode in the abstract syntax tree
 * @param j       The index of this byte in the ascii variable
 */
void allocateAscii(const std::vector<ASTNode>& ast, Memory& mem, int& addrPtr, const int i, int& j) {

  if ('\\' == ast.at(i).args.at(0).val.at(j)) {
    j++;
    if ('n' == ast.at(i).args.at(0).val.at(j)) {
      mem.write8(addrPtr, (uint8_t)('\n'));
    } else if ('t' == ast.at(i).args.at(0).val.at(j)) {
      mem.write8(addrPtr, (uint8_t)('\t'));
    } else if ('r' == ast.at(i).args.at(0).val.at(j)) {
      mem.write8(addrPtr, (uint8_t)('\r'));
    }
  } else {
    mem.write8(addrPtr, ast.at(i).args.at(0).val.at(j));
  }
  addrPtr++;
}


/**
 * @brief Resolves the labels in the abstract syntax tree into their associated addresses
 *
 * @param ast The abstract syntax tree with the labels and static variables having associated addresses
 */
void resolveSymbols(std::vector<ASTNode>& ast) {
  for (ASTNode& node : ast) {
    for (Token& arg : node.args) {
      if (arg.type == TokenType::LABEL) {
        if (labels.find(arg.val) != labels.end()) {
          arg.val = std::to_string(labels[arg.val]);
        } else {
          std::cout << "Error: Undefined label " << arg.val << "\n";
          exit(1);
        }
      }
    }
  }
}

