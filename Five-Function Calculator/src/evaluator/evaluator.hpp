#ifndef CALCULATOR_EVALUATOR_HPP
#define CALCULATOR_EVALUATOR_HPP

#include "../enums/enums.hpp"
#include "../token/token.hpp" 
#include "../tracelog/tracelog.hpp"

#include <limits>
#include <cmath>
#include <queue>
#include <stack>
#include <string>
#include <vector>

class Evaluator
{
public:
    Evaluator(Tracelog& tracelog);

    std::queue<Token> shunt(const std::vector<Token>& tokens);
    std::string evaluate(std::queue<Token>& queue);

private:
    Token doMath(const Token& mathOperator, const std::vector<Token>& operands);
    Token performAddition(const Token& left, const Token& right);
	Token performSubtraction(const Token& left, const Token& right);
	Token performMultiplication(const Token& left, const Token& right);
	Token performDivision(const Token& left, const Token& right);
    Token performPercentage(const Token& percentage, const Token& left);
    std::string trim(const long double result);

    Tracelog& m_tracelog;
};

#endif
