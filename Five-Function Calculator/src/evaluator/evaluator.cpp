#include "evaluator.hpp"

Evaluator::Evaluator(Tracelog& tracelog)
	: m_tracelog{ tracelog }
{ }

std::queue<Token> Evaluator::shunt(const std::vector<Token>& tokens)
{
    std::stack<Token> opStack;
    std::queue<Token> outputQueue;

    for (const Token& token : tokens)
    {
        if (!token.isOperator())
        {
            m_tracelog.logMoveToOutputQueue(token.getValue());
            outputQueue.push(token);
            continue;
        }

        m_tracelog.logMoveOperatorToOperatorStack(token.getSymbol());
        while (!opStack.empty() && opStack.top().getPrescedence() >= token.getPrescedence())
        {
            m_tracelog.logHigherPrescedence(token.getPrescedence(), opStack.top().getPrescedence());
            outputQueue.push(opStack.top());
            opStack.pop();
        }

        m_tracelog.logPrescedenceOK(token.getSymbol());
		opStack.push(token);
    }

    m_tracelog.logAllTokensAnalyzed();
    while (!opStack.empty())
    {
        m_tracelog.logOpStackToOuptutQueue(opStack.top().getSymbol());
		outputQueue.push(opStack.top());
		opStack.pop();
    }
    return outputQueue;
}

std::string Evaluator::evaluate(std::queue<Token>& queue)
{
    std::stack<Token> stack;

    while (!queue.empty())
    {
        bool error{ queue.front().getSymbol() == Symbol::invalid};

        m_tracelog.logEvalCheckForErrorResult(error);
        if (error)
        {
            return std::string{ Word::error };
        }

		bool overflow{ queue.front().getSymbol() == Symbol::overflow };

        m_tracelog.logCheckForOverflowFlagSet(overflow);
        if (overflow)
        {
            return std::string{ Word::overflow };
        }

        bool underflow{ queue.front().getSymbol() == Symbol::underflow };

        m_tracelog.logCheckForUnderflowFlagSet(underflow);
        if (underflow)
        {
            return std::string{ Word::underflow };
        }

        if (!queue.front().isOperator())
        {
            m_tracelog.logNumberToOperandStack(queue.front().getValue());
            stack.push(queue.front());
            queue.pop();
            continue;
        }

        m_tracelog.logOperatorFound(queue.front().getSymbol());
        std::vector<Token> operands;
        int operandCount{ 0 };

        m_tracelog.logCheckingAvailableOperands(queue.front().getOperandCount());
        if (queue.front().getOperandCount() > stack.size())
        {
            m_tracelog.logErrorFound(stack.size());
            return std::string{ Word::error };
        }

        m_tracelog.logFoundSufficientOperands(stack.size());
        while (operandCount < queue.front().getOperandCount())
        {
            m_tracelog.logPullingOperandsFromStack(stack.top().getValue());
            operands.push_back(stack.top());
            stack.pop();

            ++operandCount;
        }

		Token result{ doMath(queue.front(), operands) };

        error = result.getSymbol() == Symbol::percent
            || result.getSymbol() == Symbol::invalid;

        m_tracelog.logEvalCheckForErrorResult(error);
        if (error)
        {
            return std::string{ Word::error };
        }

        overflow = result.getSymbol() == Symbol::overflow;

        m_tracelog.logCheckForOverflow(overflow);
        if (overflow)
        {
			return std::string{ Word::overflow };
        }

        underflow =  result.getSymbol() == Symbol::underflow;

        m_tracelog.logCheckForUnderflow(underflow);
        if (underflow)
        {
            return std::string{ Word::underflow };
        }

        stack.push(result);
        queue.pop();
    }

    m_tracelog.logExpectOneToken(stack.size() == 1);
    if (stack.size() != 1)
    {
        return std::string{ Word::error };
    }

    return trim(stack.top().getValue());
}

std::string Evaluator::trim(const long double result)
{
    std::string answer{ std::to_string(result) };

    while (answer.back() == '0')
    {
        answer.pop_back();
        m_tracelog.logTrimExtraZeroes(answer);
    }

	bool wholeNumber{ answer.back() == '.' };
    m_tracelog.logCheckForWholeNumber(wholeNumber);
    if (wholeNumber)
    {
        answer.pop_back();
        m_tracelog.logTrimDecimal(answer);
    }

    return answer;
}

Token Evaluator::doMath(const Token& mathOperator, const std::vector<Token>& operands)
{
    m_tracelog.logCallingArithmeticOperation(mathOperator.getSymbol());
    switch (mathOperator.getSymbol())
    {
    case Symbol::add:
        return performAddition(operands[1], operands[0]);

    case Symbol::subtract:
        return performSubtraction(operands[1], operands[0]);

    case Symbol::multiply:
        return performMultiplication(operands[1], operands[0]);

    case Symbol::divide:
        return performDivision(operands[1], operands[0]);

    case Symbol::percent:
        return mathOperator;

    default:
        return Token{ false, Symbol::invalid, 0.0 };
    }
}

Token Evaluator::performAddition(const Token& left, const Token& right)
{
    long double leftValue{ left.getValue() };
    long double rightValue{ right.getValue() };

    bool percent{ right.getSymbol() == Symbol::percent };

    m_tracelog.logCheckForPercentOperator(percent);
    if (percent)
    {
        Token percentResult{ performPercentage(right, left) };

        if (percentResult.getSymbol() == Symbol::overflow
            || percentResult.getSymbol() == Symbol::underflow)
        {
            return percentResult;
        }

        rightValue = percentResult.getValue();
    }

    m_tracelog.logCheckForOverflow(std::numeric_limits<long double>::max() - leftValue < rightValue);
    if (std::numeric_limits<long double>::max() - leftValue < rightValue)
    {
        return Token{ false, Symbol::overflow, std::numeric_limits<long double>::max() };
    }

    long double result{ leftValue + rightValue };
	m_tracelog.logPerformArithmetic(Symbol::add, leftValue, rightValue, result);

    return Token{ false, Symbol::none, result };
}

Token Evaluator::performSubtraction(const Token& left, const Token& right)
{
    long double leftValue{ left.getValue() };
    long double rightValue{ right.getValue() };

    bool percent{ right.getSymbol() == Symbol::percent };

    m_tracelog.logCheckForPercentOperator(percent);
    if (percent)
    {
        Token percentResult{ performPercentage(right, left) };

        if (percentResult.getSymbol() == Symbol::overflow
            || percentResult.getSymbol() == Symbol::underflow)
        {
            return percentResult;
        }

        rightValue = percentResult.getValue();
    }

    bool overflow{ std::numeric_limits<long double>::lowest() + rightValue > leftValue};

    m_tracelog.logCheckForUnderflow(overflow);
    if (overflow)
    {
        return Token{ false, Symbol::underflow, std::numeric_limits<long double>::lowest() };
    }

    long double result = leftValue - rightValue;
	m_tracelog.logPerformArithmetic(Symbol::subtract, leftValue, rightValue, result);

    return Token{ false, Symbol::none, result };
}

Token Evaluator::performMultiplication(const Token& left, const Token& right)
{
    long double leftValue{ left.getValue() };
    long double rightValue{ right.getValue() };

    m_tracelog.logCheckForPercentOperator(right.getSymbol() == Symbol::percent);
    if (right.getSymbol() == Symbol::percent)
    {
        Token percentResult{ performPercentage(right, left) };
        if (percentResult.getSymbol() == Symbol::overflow
            || percentResult.getSymbol() == Symbol::underflow)
        {
            return percentResult;
        }

        rightValue = percentResult.getValue();
    }

    long double result{ leftValue * rightValue };

    bool overflow{ result / rightValue != leftValue };

    m_tracelog.logCheckForOverflow(overflow);
    if (overflow)
    {
        return Token{ false, Symbol::overflow, std::numeric_limits<long double>::max() };
    }

	m_tracelog.logPerformArithmetic(Symbol::multiply, leftValue, rightValue, result);
    return Token{ false, Symbol::none, result };
}

Token Evaluator::performDivision(const Token& left, const Token& right)
{
    long double leftValue{ left.getValue() };
    long double rightValue{ right.getValue() };

    m_tracelog.logCheckForPercentOperator(right.getSymbol() == Symbol::percent);
    if (right.getSymbol() == Symbol::percent)
    {
        Token percentResult{ performPercentage(right, left) };
        if (percentResult.getSymbol() == Symbol::overflow
            || percentResult.getSymbol() == Symbol::underflow)
        {
            return percentResult;
        }

        rightValue = percentResult.getValue();
    }

    m_tracelog.logCheckForDivideByZero(rightValue == 0);
    if (rightValue == 0)
    {
        return Token{ false, Symbol::divideByZero, 0 };
    }

    long double result{ leftValue / rightValue };

    bool overflow{ result * rightValue != leftValue };
    
	m_tracelog.logCheckForOverflow(overflow);
    if (overflow)
    {
        return Token{ false, Symbol::overflow, std::numeric_limits<long double>::max() };
    }

    m_tracelog.logPerformArithmetic(Symbol::divide, leftValue, rightValue, result);
    return Token{ false, Symbol::none, result };
}

Token Evaluator::performPercentage(const Token& percentage, const Token& left)
{
    long double leftValue{ left.getValue() };
    long double rightValue{ percentage.getValue() };

    long double result{ leftValue * rightValue };

    bool overflow{ false };
    bool divideByZero{ rightValue == 0 };

    m_tracelog.logCheckForDivideByZero(divideByZero);
    if (!divideByZero)
    {
		overflow = result / rightValue != leftValue;
    }

	m_tracelog.logCheckForOverflow(overflow);
    if (overflow)
    {
        return Token{ false, Symbol::overflow, std::numeric_limits<long double>::max() };
    }

    m_tracelog.logPercentArithmetic(percentage.getValue(), left.getValue(), result);
    return Token{ false, Symbol::none, result };
}
