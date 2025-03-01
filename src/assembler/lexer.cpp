#include "assembler/lexer.hpp"
#include "assembler/symbols.hpp"
#include <cctype>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "Token(Type: " << static_cast<int>(token.token)  // Assuming token.token is an enum
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

		if (isspace(c)) {
			if (c == '\n') line++;
			continue;
		}

		if (c == ',') {
			tokens.push_back({TokenType::COMMA, ",", line});
			continue;
		}

		if (c == '(') {
			tokens.push_back({TokenType::LPAREN, "(", line});
			continue;
		}

		if (c == ')') {
			tokens.push_back({TokenType::RPAREN, ")", line});
			// Yes, I type .append every single time 
			// Java moment :(
			continue;
		}

		if (isalnum(c) || c == '_') {
			word.clear();
			while (i < source.size() && (isalnum(source[i]) || source[i] == '_')) {
				word += source[i++];
			}
			i--; // Was skipping characters

			if (r_type.find(word) != r_type.end() || i_type.find(word) != i_type.end() || j_type.find(word) != j_type.end() || word=="li") {
				tokens.push_back({TokenType::INSTRUCTION, word, line});
				continue;
			} else if (registers.find(word) != registers.end()) {
				tokens.push_back({TokenType::REGISTER, word, line});
			}
			else {
				tokens.push_back({TokenType::LABEL, word, line});
				continue;
			}

		}
	}

	return tokens;
}

