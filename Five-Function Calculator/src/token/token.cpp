#include "token.hpp"

Token::Token(bool isOperator, char character, long double numericValue)
    : m_operator{ isOperator },
    m_operandCount{ setOperandCount(character) },
    m_symbol{ character },
    m_prescedence{ setPrescedence(character) },
    m_value{ numericValue }
{ }

int Token::getOperandCount() const
{
    return m_operandCount;
}

Prescedence Token::getPrescedence() const
{
    return m_prescedence;
}

char Token::getSymbol() const
{
    return m_symbol;
}

long double Token::getValue() const
{
    return m_value;
}

bool Token::isOperator() const
{
    return m_operator;
}

int Token::setOperandCount(const char character)
{
    switch (character)
    {
    case Symbol::negative:
        return 1;

    case Symbol::multiply:
        [[fallthrough]];
    case Symbol::divide:
        [[fallthrough]];
    case Symbol::add:
        [[fallthrough]];
    case Symbol::subtract:
        return 2;

    default:
        return 0;
    }
}

Prescedence Token::setPrescedence(const char character)
{
    switch (character)
    {
    case Symbol::negative:
        return Prescedence::negative;

    case Symbol::multiply:
        [[fallthrough]];
    case Symbol::divide:
        return Prescedence::multiplyDivide;

    case Symbol::add:
        [[fallthrough]];
    case Symbol::subtract:
        return Prescedence::addSubtract;

    default:
        return Prescedence::notApplicable;
    }
}