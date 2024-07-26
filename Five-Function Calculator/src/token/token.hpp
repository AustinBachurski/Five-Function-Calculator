#ifndef CALCULATOR_TOKEN_HPP
#define CALCULATOR_TOKEN_HPP

#include "../enums/enums.hpp"

class Token
{
public:
    Token(bool isOperator, 
        char symbol = Symbol::none,
        long double numericValue = 0.0);

    int getOperandCount() const;
    Prescedence getPrescedence() const;
    char getSymbol() const;
    long double getValue() const;
    bool isOperator() const;

private:
    Prescedence setPrescedence(const char character);
    int setOperandCount(const char character);

    bool m_operator{ false };
    int m_operandCount{ 0 };
    Prescedence m_prescedence{ Prescedence::notApplicable };
    char m_symbol{ Symbol::none };
    long double m_value{ 0.0 };
};

#endif
