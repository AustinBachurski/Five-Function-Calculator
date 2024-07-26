#ifndef CALCULATOR_CALCULATOR_TAB_HPP
#define CALCULATOR_CALCULATOR_TAB_HPP

#include "../enums/enums.hpp"
#include "../evaluator/evaluator.hpp"
#include "../tokenizer/tokenizer.hpp"
#include "../tracelog/tracelog.hpp"

#include <wx/gbsizer.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

#include <string>

class CalculatorTab : public wxWindow
{
public:
	CalculatorTab(wxNotebook* control, Tracelog& tracelog);

private:
	void clearDisplayIfClearFlagSet();
	void clearInvalidExpressionWarning();
	void enterPressed();
	void handleButtonPress(const wxCommandEvent& event);
	void handleKeyboardInput(const wxKeyEvent& event);
	void setButtonBindings();
	void setFonts();
	void setSizers();
	void toggleTraceLog();

	Tracelog& m_tracelog;
	Tokenizer m_tokenizer;
	Evaluator m_evaluator;
	bool m_clearOnNextDigit{ false };
	std::string m_invalid{ "Invalid Expression->" };

	// Window elements.

	wxBoxSizer* m_windowStackSizer{ new wxBoxSizer(wxVERTICAL) };

	wxBoxSizer* m_traceButtonsSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxButton* m_calcButton{ new wxButton(
		this, wxID_ANY, "Calc", wxDefaultPosition, wxSize(50, 25)) }; // Placeholder, currently unbound.
	wxButton* m_traceOnButton{ new wxButton(
		this, static_cast<int>(ButtonID::traceON), "Intro Trace On", wxDefaultPosition, wxSize(85, 25)) };
	wxButton* m_traceOffButton{ new wxButton
	(this, static_cast<int>(ButtonID::traceOFF), "Intro Trace Off", wxDefaultPosition, wxSize(85, 25)) };

	wxBoxSizer* m_listBoxSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxTextCtrl* m_listBox{ new wxTextCtrl(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RIGHT) };

	wxBoxSizer* m_memoBoxSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxTextCtrl* m_memoBox{ new wxTextCtrl(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY | wxTE_RIGHT) };

	// Calculator Input Buttons

	wxBoxSizer* m_inputButtonSizer{ new wxBoxSizer(wxHORIZONTAL) };

	wxBoxSizer* m_inputColumnOneAndTwoSizer{ new wxBoxSizer(wxVERTICAL) };
	wxButton* m_zeroButton{ new wxButton(
	this, static_cast<int>(ButtonID::zero), "0", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputRowOneSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxButton* m_sevenButton{ new wxButton(
		this, static_cast<int>(ButtonID::seven), "7", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_eightButton{ new wxButton(
		this, static_cast<int>(ButtonID::eight), "8", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputRowTwoSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxButton* m_fourButton{ new wxButton(
		this, static_cast<int>(ButtonID::four), "4", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_fiveButton{ new wxButton(
		this, static_cast<int>(ButtonID::five), "5", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputRowThreeSizer{ new wxBoxSizer(wxHORIZONTAL) };
	wxButton* m_oneButton{ new wxButton(
		this, static_cast<int>(ButtonID::one), "1", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_twoButton{ new wxButton(
		this, static_cast<int>(ButtonID::two), "2", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputColumnThreeSizer{ new wxBoxSizer(wxVERTICAL) };
	wxButton* m_nineButton{ new wxButton(
		this, static_cast<int>(ButtonID::nine), "9", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_sixButton{ new wxButton(
		this, static_cast<int>(ButtonID::six), "6", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_threeButton{ new wxButton(
		this, static_cast<int>(ButtonID::three), "3", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_decimalButton{ new wxButton(
		this, static_cast<int>(ButtonID::decimal), ".", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputColumnFourSizer{ new wxBoxSizer(wxVERTICAL) };
	wxButton* m_clearButton{ new wxButton(
		this, static_cast<int>(ButtonID::clear), "C", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_plusButton{ new wxButton(
		this, static_cast<int>(ButtonID::plus), "+", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_asteriskButton{ new wxButton(
		this, static_cast<int>(ButtonID::asterisk), "*", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_equalsButton{ new wxButton(
		this, static_cast<int>(ButtonID::equals), "=", wxDefaultPosition, wxSize(50, 50)) };

	wxBoxSizer* m_inputColumnFiveSizer{ new wxBoxSizer(wxVERTICAL) };
	wxButton* m_clearEntryButton{ new wxButton(
		this, static_cast<int>(ButtonID::clearEntry), "CE", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_minusButton{ new wxButton(
		this, static_cast<int>(ButtonID::minus), "-", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_slashButton{ new wxButton(
		this, static_cast<int>(ButtonID::slash), "/", wxDefaultPosition, wxSize(50, 50)) };
	wxButton* m_percentButton{ new wxButton(
		this, static_cast<int>(ButtonID::percent), "%", wxDefaultPosition, wxSize(50, 50)) };
};

#endif
