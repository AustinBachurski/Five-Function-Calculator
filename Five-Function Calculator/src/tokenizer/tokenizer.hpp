#ifndef CALCULATOR_TOKENIZER_HPP
#define CALCULATOR_TOKENIZER_HPP

#include "../enums/enums.hpp"
#include "../token/token.hpp" 
#include "../tracelog/tracelog.hpp"

#include <algorithm>
#include <array>
#include <climits>
#include <charconv>
#include <string>
#include <string_view>
#include <vector>

class Tokenizer
{
public:
    Tokenizer(Tracelog& tracelog);

    std::vector<Token> tokenize(const std::string_view expression);

private:
    bool isOperator(const char c);
    std::vector<Token> lex(const std::vector<Token>& tokens);
	Token performNegation(const Token& left);

    Tracelog& m_tracelog;
};

#endif
