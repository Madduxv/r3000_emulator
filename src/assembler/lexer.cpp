#include "assembler/lexer.hpp"
#include "assembler/symbols.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "Token(Type: " << static_cast<int>(token.type)
       << ", Value: " << token.val << ")";
    return out;
}

// I will look back at this and think that I was an idiot
std::vector<Token> tokenize(const std::string& source) {
	std::vector<Token> tokens;
	std::string word;
	int line = 1;

	for (size_t i = 0; i < source.size(); i++) {
		char c = source[i];

		// really wish I could make this a switch statement
		if (isspace(c)) {
			if (c == '\n') {line++;}
			continue;
		}

		if (std::isdigit(c) || '-' == c) {
			word.clear();
			while (i < source.size() && (isalnum(source[i]))) { // I'll handle casting later
				word += source[i++];
			}
			tokens.push_back({TokenType::IMMEDIATE, word, line});
			continue;
		}

		if (isalnum(c) ||  '_' == c) {
			word.clear();
			while (i < source.size() && (isalnum(source[i]) || '_' == source[i])) {
				word += source[i++];
			}
			i--; // was skipping characters

			// I'll handle pseudo-instructions later
			if (r_type.find(word) != r_type.end() || i_type.find(word) != i_type.end() || j_type.find(word) != j_type.end()) {
				tokens.push_back({TokenType::INSTRUCTION, word, line});
				continue;
			} else {
        if (i+1 < source.size() && ':' == source[i+1]) {
          tokens.push_back({TokenType::LABEL_DECLARATION, word, line});
        } else {
          tokens.push_back({TokenType::LABEL, word, line});
        }
				continue;
			}

		}

		if ('$' == c) {
			word.clear();
			word += source[i++];
			while (i < source.size() && (isalnum(source[i]))) {
				word += source[i++];
			}
			i--; // Was skipping characters
			if (registers.find(word) != registers.end()) {
				tokens.push_back({TokenType::REGISTER, word, line});
			}
		}

		if ('#' == c) {
			while (i < source.size() && source[i] != '\n') {i++;}
			continue;
		}

		// I am so glad that c++ halts checking when it does, so I don't
		// need a more creative solution for checking for checking for //
		if ('/' == c && i + 1 < source.size() && '/' == source[i + 1]) {
			while (i < source.size() && source[i] != '\n') {
				i++;  
			}
			continue;
		}

		if ('.' == c) {
			word.clear();
			word += source[i++];
			while (i < source.size() && (isalnum(source[i]) || '_' == source[i])) {
				word += source[i++];
			}
			tokens.push_back({TokenType::DIRECTIVE, word, line});
			continue;
		}

		if (',' == c) {
			tokens.push_back({TokenType::COMMA, ",", line});
			continue;
		}

		if ('(' == c) {
			tokens.push_back({TokenType::LPAREN, "(", line});
			continue;
		}

		if (')' == c) {
			tokens.push_back({TokenType::RPAREN, ")", line});
			// Yes, I type .append every single time 
			// Java moment :(
			continue;
		}

    if ('\"' == c) {
      word.clear();
      word += source[i++];
      while (i<source.size() && '\"' != source[i]) {
        word += source[i++];
      }
      word += '\"'; // consume that last "
      tokens.push_back({TokenType::STRING, word, line});
      continue;
    }

	}

	return tokens;
}

