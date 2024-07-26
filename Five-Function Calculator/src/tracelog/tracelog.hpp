#ifndef CALCULATOR_TRACELOG_HPP
#define CALCULATOR_TRACELOG_HPP

#include "../enums/enums.hpp"
#include "../token/token.hpp"
#include "../ui/traceTab.hpp"

#include <wx/notebook.h>
#include <wx/wx.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <string>

class Tracelog
{
public:
	Tracelog(const std::filesystem::path& filePath, wxNotebookPage* traceTab);

	void disableLogging();
	void enableLogging();
	bool getLogState() const;
	void log(const std::string& message) const;
	void resetCounter();


private:
	void checkForExistingFile();

	TraceTab* m_traceTab;
	std::filesystem::path m_filePath;
	bool m_enabled{ true };

// Trace Log Methods
public:
	void logButtonPressed(const ButtonID button);
	void logKeyPressed(const wxKeyEvent& event);
	void logClearInvalidWarning(const std::string displayed); // Pass by value intentional - wxString conversion.
	void logSendEquationToTokenizer(const std::string& equation);
	void logIsCharacterOperator(const bool result);
	void logGenerateOperatorToken(const char symbol);
	void logFoundNumberComponent(const char component);
	void logGenerateNumberToken(const std::string_view number);
	void logInvalidNumber(const std::string_view number);
	void logTokenizerGeneratedCount(const size_t count);
	void logDetectedPercentSymbol(const long double consumed, const long double percentage);
	void logDetectedNegativeSymbol(const long double consumed);
	void logNoAnalysisNeeded(const Token& token);
	void logLexerGeneratedCount(const size_t count);
	void logSendForShunting(const size_t count);
	void logMoveToOutputQueue(const long double value);
	void logMoveOperatorToOperatorStack(const char symbol);
	void logHigherPrescedence(const Prescedence lower, const Prescedence higher);
	void logPrescedenceOK(const char symbol);
	void logAllTokensAnalyzed();
	void logOpStackToOuptutQueue(const char symbol);
	void logShuntingComplete(const size_t count);
	void logNumberToOperandStack(const long double value);
	void logOperatorFound(const char symbol);
	void logCheckingAvailableOperands(const int count);
	void logErrorFound(const size_t available);
	void logFoundSufficientOperands(const size_t available);
	void logPullingOperandsFromStack(const long double value);
	void logCallingArithmeticOperation(const char symbol);
	void logCheckForPercentOperator(const bool result);
	void logPercentArithmetic(const long double percentage,
		const long double value, const long double result);
	void logCheckForOverflow(const bool result);
	void logCheckForUnderflow(const bool result);
	void logCheckForOverflowFlagSet(const bool result);
	void logCheckForUnderflowFlagSet(const bool result);
	void logCheckForDivideByZero(const bool result);
	void logPerformArithmetic(const char symbol,
		const long double left, const long double right, const long double result);
	void logCheckForWholeNumber(const bool result);
	void logExpectOneToken(const bool result);
	void logRemovingDecimal(const std::string& result);
	void logTrimExtraZeroes(const std::string& result);
	void logCalcCheckForErrorResult(const bool result);
	void logEvalCheckForErrorResult(const bool result);
	void logDisplayError(const std::string error); // Pass by value intentional - wxString conversion.
	void logDisplayAnswer(const std::string& answer);
	void logTrimDecimal(const std::string& result);

private:
	std::string asString(const wxKeyEvent& event) const;
	std::string asString(const ButtonID value) const;
	std::string asString(const Prescedence value) const;
	std::string asString(const bool value) const;
	std::string operation(const char symbol) const;

	// Log Counters
	std::array<int, 46> counter; // enum Log Counter Indexes count + 1

	// Log Counter Indexes
	enum Index
	{
		buttonPressed,
		keyPressed,
		clearWarning,
		sendEquationToTokenizer,
		isCharacterOperator,
		generateOperatorToken,
		foundNumberComponent,
		generateNumberToken,
		invalidNumber,
		tokenizerGeneratedCount,
		detectedPercentSymbol,
		dectedNegativeSymbol,
		noAnalysisNeeded,
		lexerGeneratedCount,
		sendForShunting,
		moveToOutputQueue,
		moveOperatorToOperatorStack,
		higherPrescedence,
		prescedenceOK,
		allTokensAnalyzed,
		opStackToOutputQueue,
		shuntingComplete,
		numberToOperandStack,
		operatorFound,
		checkingAvailableOperands,
		errorFound,
		foundSufficientOperands,
		pullingOperandsFromStack,
		callingArithmeticOperation,
		checkForPercentOperator,
		percentArithmetic,
		checkForOverflow,
		checkForUnderflow,
		checkForOverflowFlag,
		checkForUnderflowFlag,
		checkForDivideByZero,
		performArithmetic,
		checkForWholeNumber,
		expectOneToken,
		removingDecimal,
		trimExtraZeroes,
		calcCheckForErrorResult,
		evalCheckForErrorResult,
		displayError,
		displayAnswer,
		trimDecimal,
	};
};

#endif
