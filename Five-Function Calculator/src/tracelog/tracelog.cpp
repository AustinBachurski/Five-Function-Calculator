#include "tracelog.hpp"

Tracelog::Tracelog(const std::filesystem::path& filePath, wxNotebookPage* traceTab)
	: m_traceTab{ static_cast<TraceTab*>(traceTab) },
	m_filePath{ filePath }
{
	checkForExistingFile();
	resetCounter();
}

void Tracelog::disableLogging()
{
	m_enabled = false;
}

void Tracelog::enableLogging()
{
	m_enabled = true;
}

bool Tracelog::getLogState() const
{
	return m_enabled;
}

void Tracelog::log(const std::string& message) const
{
	if (m_enabled)
	{
		m_traceTab->logMessage(message);
	}

	std::ofstream file{ m_filePath, std::ios::out | std::ios::app };

	if (!file.is_open())
	{
		m_traceTab->logMessage("UNABLE TO OPEN LOG FILE!");
		return;
	}
	file << message;
}

void Tracelog::checkForExistingFile()
{
	if (!std::filesystem::exists(m_filePath))
	{
		// Make sure we can create the file.
		std::ofstream test{ m_filePath, std::ios::out };
		if (!test.is_open())
		{
			m_traceTab->logMessage("UNABLE TO OPEN LOG FILE!");
		}
		return;
	}

	// Erase existing contents.
	std::ofstream{ m_filePath, std::ios::out | std::ios::trunc };
}

void Tracelog::resetCounter()
{
	counter.fill(0);
}

// Trace Log Messages

void Tracelog::logButtonPressed(const ButtonID button)
{
	++counter[Index::buttonPressed];

	std::string message{ "CalculatorUI::Button Clicked\n  (count: "
		+ std::to_string(counter[Index::buttonPressed])
		+ ") -> "
		+ asString(button)
		+ "\n\n" };

	log(message);
}

void Tracelog::logKeyPressed(const wxKeyEvent& event)
{
	++counter[Index::keyPressed];

	std::string message{ "CalculatorUI::Key Pressed\n  (count: "
		+ std::to_string(counter[Index::keyPressed])
		+ ") -> "
		+ asString(event)
		+ "\n\n" };

	log(message);
}

// Pass by value intentional - wxString conversion.
void Tracelog::logClearInvalidWarning(std::string displayed) 
{        
	++counter[Index::clearWarning];

	std::string message{ "CalculatorUI::Clear Invalid Expression Warning\n  (count: "
		+ std::to_string(counter[Index::clearWarning])
		+ ") -> "
		+ displayed
		+ "\n\n" };

	log(message);
}

void Tracelog::logSendEquationToTokenizer(const std::string& equation)
{
	++counter[Index::sendEquationToTokenizer];

	std::string message{ "CalculatorUI::Sending Equation to Tokenizer\n  (count: "
		+ std::to_string(counter[Index::sendEquationToTokenizer])
		+ ") -> "
		+ equation
		+ "\n\n" };

	log(message);
}

void Tracelog::logIsCharacterOperator(const bool result)
{
	++counter[Index::isCharacterOperator];

	std::string message{ "Tokenizer::Is Character Operator\n  (count: "
		+ std::to_string(counter[Index::isCharacterOperator])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logGenerateOperatorToken(const char symbol)
{
	++counter[Index::generateOperatorToken];

	std::string message{ "Tokenizer::Generate Operator Token\n  (count: "
		+ std::to_string(counter[Index::generateOperatorToken])
		+ ") -> operator "
		+ symbol
		+ "\n\n" };

	log(message);
}

void Tracelog::logFoundNumberComponent(const char component)
{
	++counter[Index::foundNumberComponent];

	std::string message{ "Tokenizer::Found Number Component\n  (count: "
		+ std::to_string(counter[Index::foundNumberComponent])
		+ ") -> "
		+ component
		+ "\n\n" };

	log(message);
}

void Tracelog::logGenerateNumberToken(const std::string_view number)
{
	++counter[Index::generateNumberToken];

	std::string message{ "Tokenizer::Generate Number Token\n  (count: "
		+ std::to_string(counter[Index::generateNumberToken])
		+ ") -> "
		+ std::string{ number }
		+ "\n\n" };

	log(message);
}

void Tracelog::logInvalidNumber(const std::string_view number)
{
	++counter[Index::invalidNumber];

	std::string message{ "Tokenizer::Invalid Number Found\n  (count: "
		+ std::to_string(counter[Index::invalidNumber])
		+ ") -> "
		+ std::string{ number }
		+ "\n\n" };

	log(message);
}

void Tracelog::logTokenizerGeneratedCount(const size_t count)
{
	++counter[Index::tokenizerGeneratedCount];

	std::string message{ "Tokenizer::Generated "
		+ std::to_string(count)
		+" Tokens\n  (count: "
		+ std::to_string(counter[Index::tokenizerGeneratedCount])
		+ ") -> Sending tokens to Lexer.\n\n" };

	log(message);
}

void Tracelog::logDetectedPercentSymbol(const long double consumed, const long double percentage)
{
	++counter[Index::detectedPercentSymbol];

	std::string message{ "Lexer::Detected Percent Symbol\n  (count: "
		+ std::to_string(counter[Index::detectedPercentSymbol])
		+ ") Consumed number token with value "
		+ std::to_string(consumed)
		+ " -> Generated new Token with percentage value"
		+ std::to_string(percentage)
		+ "\n\n" };

	log(message);
}

void Tracelog::logDetectedNegativeSymbol(const long double consumed)
{
	++counter[Index::dectedNegativeSymbol];

	std::string message{ "Lexer::Detected Negative Symbol\n  (count: "
		+ std::to_string(counter[Index::dectedNegativeSymbol])
		+ ") Consumed number token with value "
		+ std::to_string(consumed)
		+ " -> generated new Token with value "
		+ std::to_string(-consumed)
		+ "\n\n" };

	log(message);
}

void Tracelog::logNoAnalysisNeeded(const Token& token)
{
	++counter[Index::noAnalysisNeeded];

	std::string message;
	if (!token.isOperator())
	{
		message = "Lexer::No Analysis Needed\n  (count: "
			+ std::to_string(counter[Index::noAnalysisNeeded])
			+ ") Transfer Token with value -> "
			+ std::to_string(token.getValue())
			+ "\n\n";
	}
	else
	{
		message = "Lexer::No Analysis Needed\n  (count: "
			+ std::to_string(counter[Index::noAnalysisNeeded])
			+ ") Transfer Token with operator -> "
			+ token.getSymbol()
			+ "\n\n";
	}

	log(message);
}

void Tracelog::logLexerGeneratedCount(const size_t count)
{
	++counter[Index::lexerGeneratedCount];

	std::string message{ "Lexer::Generated Tokens\n  (count: "
	+ std::to_string(counter[Index::lexerGeneratedCount])
	+ ") -> "
	+ std::to_string(count)
	+ "\n\n" };

	log(message);
}

void Tracelog::logSendForShunting(const size_t count)
{
	++counter[Index::sendForShunting];

	std::string message{ "Calculator::Sending Tokens to Shunting Yard Algorithm\n  (count: "
		+ std::to_string(counter[Index::sendForShunting])
		+ ") -> "
		+ std::to_string(count)
		+ " tokens.\n\n" };

	log(message);
}

void Tracelog::logMoveToOutputQueue(const long double value)
{
	++counter[Index::moveToOutputQueue];

	std::string message{ "Shunting Yard::Moving Number Token to Ouput Queue\n  (count: "
		+ std::to_string(counter[Index::moveToOutputQueue])
		+ ") Token value -> "
		+ std::to_string(value)
		+ "\n\n" };

	log(message);
}

void Tracelog::logMoveOperatorToOperatorStack(const char symbol)
{
	++counter[Index::moveOperatorToOperatorStack];

	std::string message{ "Shunting Yard::Operator Found, Moving to Operator Stack\n  (count: "
		+ std::to_string(counter[Index::moveOperatorToOperatorStack])
		+ ") -> operator "
		+ symbol
		+ "\n\n" };

	log(message);
}

void Tracelog::logHigherPrescedence(const Prescedence lower, const Prescedence higher)
{
	++counter[Index::higherPrescedence];

	std::string message{ "Shunting Yard::Operator Stack Has Higher Prescedence Operator\n  (count: "
		+ std::to_string(counter[Index::higherPrescedence])
		+ ") -> "
		+ asString(higher)
		+ " > "
		+ asString(lower)
		+ " moving "
		+ asString(higher)
		+ " to output queue."
		+ "\n\n" };

	log(message);
}

void Tracelog::logPrescedenceOK(const char symbol)
{
	++counter[Index::prescedenceOK];

	std::string message{ "Shunting Yard::Operator Prescedence OK\n  (count: "
		+ std::to_string(counter[Index::prescedenceOK])
		+ ") -> moving operator "
		+ symbol
		+ " to output queue."
		+ "\n\n" };

	log(message);
}

void Tracelog::logAllTokensAnalyzed()
{
	++counter[Index::allTokensAnalyzed];

	std::string message{ "Shunting Yard::All Tokens Analyzed\n  (count: "
		+ std::to_string(counter[Index::allTokensAnalyzed])
		+ ")\n\n" };

	log(message);
}

void Tracelog::logOpStackToOuptutQueue(const char symbol)
{
	++counter[Index::opStackToOutputQueue];

	std::string message{ "Shunting Yard::Moving Remaining Operators From Operator Stack to Output Queue\n  (count: "
		+ std::to_string(counter[Index::opStackToOutputQueue])
		+ ") -> operator "
		+ symbol
		+ "\n\n" };

	log(message);
}

void Tracelog::logShuntingComplete(const size_t count)
{
	++counter[Index::shuntingComplete];

	std::string message{ "Calculator::Shunting Complete, Performing Arithmetic Operations On\n  (count: "
		+ std::to_string(counter[Index::shuntingComplete])
		+ ") -> "
		+ std::to_string(count)
		+ " tokens."
		+ "\n\n" };

	log(message);
}

void Tracelog::logNumberToOperandStack(const long double value)
{
	++counter[Index::numberToOperandStack];

	std::string message{ "Evaluator::Moving Number to Operand Stack\n  (count: "
		+ std::to_string(counter[Index::numberToOperandStack])
		+ ") -> "
		+ std::to_string(value)
		+ "\n\n" };

	log(message);
}

void Tracelog::logOperatorFound(const char symbol)
{
	++counter[Index::operatorFound];

	std::string message{ "Evaluator::Operator Found\n  (count: "
		+ std::to_string(counter[Index::operatorFound])
		+ ") -> operator "
		+ symbol
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckingAvailableOperands(const int count)
{
	++counter[Index::checkingAvailableOperands];

	std::string message{ "Evaluator::Checking for Available Operands\n  (count: "
		+ std::to_string(counter[Index::checkingAvailableOperands])
		+ ") -> "
		+ std::to_string(count)
		+" required.\n\n" };

	log(message);
}

void Tracelog::logErrorFound(const size_t available)
{
	++counter[Index::errorFound];

	std::string message{ "Evaluator::ERROR!\n  (count: "
		+ std::to_string(counter[Index::errorFound])
		+ ") -> only "
		+ std::to_string(available)
		+ " operands available! -> Invalid Expression!\n\n" };

	log(message);
}

void Tracelog::logFoundSufficientOperands(const size_t available)
{
	++counter[Index::foundSufficientOperands];

	std::string message{ "Evaluator::Found Sufficient Operands\n  (count: "
		+ std::to_string(counter[Index::foundSufficientOperands])
		+ ") -> "
		+ std::to_string(available)
		+ " available.\n\n" };

	log(message);
}

void Tracelog::logPullingOperandsFromStack(const long double value)
{
	++counter[Index::pullingOperandsFromStack];

	std::string message{ "Evaluator::Pulling Operands from Operand Stack\n  (count: "
		+ std::to_string(counter[Index::pullingOperandsFromStack])
		+ ") -> "
		+ std::to_string(value)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCallingArithmeticOperation(const char symbol)
{
	++counter[Index::callingArithmeticOperation];

	std::string message{ "Evaluator::Calling Arithmetic Operation\n  (count: "
		+ std::to_string(counter[Index::callingArithmeticOperation])
		+ ") -> "
		+ operation(symbol)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForPercentOperator(const bool result)
{
	++counter[Index::checkForPercentOperator];

	std::string message{ "Evaluator::Check for Percent Operator\n  (count: "
		+ std::to_string(counter[Index::checkForPercentOperator])
		+ ") Found? -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logPercentArithmetic(const long double percentage, const long double value, const long double result)
{
	++counter[Index::percentArithmetic];

	std::string message{ "Evaluator::Percent Arithmetic\n  (count: "
		+ std::to_string(counter[Index::percentArithmetic])
		+ ") "
		+ std::to_string(percentage * 100)
		+ "% of "
		+ std::to_string(value)
		+ " -> "
		+ std::to_string(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForOverflow(const bool result)
{
	++counter[Index::checkForOverflow];

	std::string message{ "Evaluator::Check for Overflow\n  (count: "
		+ std::to_string(counter[Index::checkForOverflow])
		+ ") Found? -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForUnderflow(const bool result)
{
	++counter[Index::checkForUnderflow];

	std::string message{ "Evaluator::Check for Underflow\n  (count: "
		+ std::to_string(counter[Index::checkForUnderflow])
		+ ") Found? -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForOverflowFlagSet(const bool result)
{
	++counter[Index::checkForOverflowFlag];

	std::string message{ "Evaluator::Check for Overflow Flag Set\n  (count: "
		+ std::to_string(counter[Index::checkForOverflowFlag])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForUnderflowFlagSet(const bool result)
{
	++counter[Index::checkForUnderflowFlag];

	std::string message{ "Evaluator::Check for Underflow Flag Set\n  (count: "
		+ std::to_string(counter[Index::checkForUnderflowFlag])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForDivideByZero(const bool result)
{
	++counter[Index::checkForDivideByZero];

	std::string message{ "Evaluator::Check for Divide by Zero\n  (count: "
		+ std::to_string(counter[Index::checkForDivideByZero])
		+ ") Found? -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logPerformArithmetic(const char symbol,
	const long double left, const long double right, const long double result)
{
	++counter[Index::performArithmetic];

	std::string message{ "Evaluator::Perform Arithmetic\n  (count: "
		+ std::to_string(counter[Index::performArithmetic])
		+ ") "
		+ std::to_string(left)
		+ ' '
		+ symbol
		+ ' '
		+ std::to_string(right)
		+ " -> "
		+ std::to_string(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logCheckForWholeNumber(const bool result)
{
	++counter[Index::checkForWholeNumber];

	std::string message{ "Evaluator::Checking for Whole Number\n  (count: "
		+ std::to_string(counter[Index::checkForWholeNumber])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logExpectOneToken(const bool result)
{
	++counter[Index::expectOneToken];

	std::string message{ "Evaluator::Expect Stack to Have One Token Remaining\n  (count: "
		+ std::to_string(counter[Index::expectOneToken])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logRemovingDecimal(const std::string& result)
{
	++counter[Index::removingDecimal];

	std::string message{ "Evaluator::Removing Decimal\n  (count: "
		+ std::to_string(counter[Index::removingDecimal])
		+ ") -> "
		+ result
		+ "\n\n" };

	log(message);
}

void Tracelog::logTrimExtraZeroes(const std::string& result)
{
	++counter[Index::trimExtraZeroes];

	std::string message{ "Evaluator::Trimming Extra Zeroes\n  (count: "
		+ std::to_string(counter[Index::trimExtraZeroes])
		+ ") -> "
		+ result
		+ "\n\n" };

	log(message);
}

void Tracelog::logCalcCheckForErrorResult(const bool result)
{
	++counter[Index::calcCheckForErrorResult];

	std::string message{ "Calculator::Check for Error Result\n  (count: "
		+ std::to_string(counter[Index::calcCheckForErrorResult])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

void Tracelog::logEvalCheckForErrorResult(const bool result)
{
	++counter[Index::evalCheckForErrorResult];

	std::string message{ "Evaluator::Check for Error Result\n  (count: "
		+ std::to_string(counter[Index::evalCheckForErrorResult])
		+ ") -> "
		+ asString(result)
		+ "\n\n" };

	log(message);
}

// Pass by value intentional - wxString conversion.
void Tracelog::logDisplayError(const std::string error)
{
	++counter[Index::displayError];

	std::string message{ "Calculator::Display Error\n  (count: "
		+ std::to_string(counter[Index::displayError])
		+ ") -> "
		+ error
		+ "\n\n" };

	log(message);
}

void Tracelog::logDisplayAnswer(const std::string& answer)
{
	++counter[Index::displayAnswer];

	std::string message{ "Calculator::Display Answer\n  (count: "
		+ std::to_string(counter[Index::displayAnswer])
		+ ") -> "
		+ answer
		+ "\n\n" };

	log(message);
}

void Tracelog::logTrimDecimal(const std::string& result)
{
	++counter[Index::trimDecimal];

	std::string message{ "Evaluator::Trimming Decimal\n  (count: "
		+ std::to_string(counter[Index::trimDecimal])
		+ ") -> "
		+ result
		+ "\n\n" };

	log(message);
}

std::string Tracelog::asString(const wxKeyEvent& event) const
{
    switch (event.GetKeyCode())
    {
    case WXK_NUMPAD0:
        [[fallthrough]];
    case static_cast<int>(ASCII::zero):
		return "0";

    case WXK_NUMPAD1:
        [[fallthrough]];
    case static_cast<int>(ASCII::one):
		return "1";

    case WXK_NUMPAD2:
        [[fallthrough]];
    case static_cast<int>(ASCII::two):
		return "2";

    case WXK_NUMPAD3:
        [[fallthrough]];
    case static_cast<int>(ASCII::three):
		return "3";

    case WXK_NUMPAD4:
        [[fallthrough]];
    case static_cast<int>(ASCII::four):
		return "4";

    case WXK_NUMPAD5:
        [[fallthrough]];
    case static_cast<int>(ASCII::five):
        if (event.GetModifiers() == wxMOD_SHIFT)
        {
			return "%";
        }
		return "5";
        break;

    case WXK_NUMPAD6:
        [[fallthrough]];
    case static_cast<int>(ASCII::six):
		return "6";

    case WXK_NUMPAD7:
        [[fallthrough]];
    case static_cast<int>(ASCII::seven):
		return "7";

    case WXK_NUMPAD8:
        [[fallthrough]];
    case static_cast<int>(ASCII::eight):
		return "8";

    case WXK_NUMPAD9:
        [[fallthrough]];
    case static_cast<int>(ASCII::nine):
		return "9";

    case WXK_NUMPAD_DECIMAL:
        [[fallthrough]];
    case WXK_DECIMAL:
        [[fallthrough]];
    case static_cast<int>(ASCII::decimal):
		return ".";

    case WXK_ESCAPE:
		return "Clear";

    case WXK_BACK:
		return "Clear Entry";

    case WXK_NUMPAD_ADD:
        [[fallthrough]];
    case WXK_ADD:
        [[fallthrough]];
    case static_cast<int>(ASCII::plus):
		return "+";

    case WXK_NUMPAD_SUBTRACT:
        [[fallthrough]];
    case WXK_SUBTRACT:
        [[fallthrough]];
    case static_cast<int>(ASCII::minus):
		return "-";

    case WXK_NUMPAD_MULTIPLY:
        [[fallthrough]];
    case WXK_MULTIPLY:
        [[fallthrough]];
    case static_cast<int>(ASCII::asterisk):
		return "*";

    case WXK_NUMPAD_DIVIDE:
        [[fallthrough]];
    case WXK_DIVIDE:
        [[fallthrough]];
    case static_cast<int>(ASCII::slash):
		return "/";

    case WXK_NUMPAD_EQUAL:
        [[fallthrough]];
    case WXK_RETURN:
		return "=";

    default:
		// Usually triggered by the shift key when pressing %.
		return "Invalid Key Ignored";
    }
}

std::string Tracelog::asString(const ButtonID value) const
{
	switch (value)
	{
	case ButtonID::zero:
		return "0";

	case ButtonID::one:
		return "1";

	case ButtonID::two:
		return "2";

	case ButtonID::three:
		return "3";

	case ButtonID::four:
		return "4";

	case ButtonID::five:
		return "5";

	case ButtonID::six:
		return "6";

	case ButtonID::seven:
		return "7";

	case ButtonID::eight:
		return "8";

	case ButtonID::nine:
		return "9";

	case ButtonID::decimal:
		return ".";

	case ButtonID::clear:
		return "C";

	case ButtonID::clearEntry:
		return "CE";

	case ButtonID::plus:
		return "+";

	case ButtonID::minus:
		return "-";

	case ButtonID::asterisk:
		return "*";

	case ButtonID::slash:
		return "/";

	case ButtonID::equals:
		return "=";

	case ButtonID::percent:
		return "%";

	case ButtonID::traceON:
		return "Trace On";

	case ButtonID::traceOFF:
		return "Trace Off";

	default:
		return "Forgot to add the new button to asString()!";
	}
}

std::string Tracelog::asString(const Prescedence value) const
{
	switch (value)
	{
	case Prescedence::negative:
		return "Negation";

	case Prescedence::multiplyDivide:
		return "Multiplication/Division";

	case Prescedence::addSubtract:
		return "Addition/Subtraction";

	case Prescedence::notApplicable:
		return "Not Applicable";

	default:
		return "Unknown Prescedence!";
	}
}

std::string Tracelog::asString(const bool value) const
{
	return value ? "Yes" : "No";
}

std::string Tracelog::operation(const char symbol) const
{
	switch (symbol)
	{
	case Symbol::add:
		return "Addition";

	case Symbol::subtract:
		return "Subtraction";

	case Symbol::multiply:
		return "Multiplication";

	case Symbol::divide:
		return "Division";

	default:
		return "Unknown Operation!";
	}
}
