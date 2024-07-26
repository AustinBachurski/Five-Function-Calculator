#include "calculatorTab.hpp"

#include "../enums/enums.hpp"
#include "../token/token.hpp"
#include "../tokenizer/tokenizer.hpp"
#include "../evaluator/evaluator.hpp"


CalculatorTab::CalculatorTab(wxNotebook* control, Tracelog& tracelog)
    : wxWindow(control, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS),
    m_tracelog{ tracelog },
    m_tokenizer{ tracelog },
    m_evaluator{ tracelog }
{
    Bind(wxEVT_CHAR_HOOK, &CalculatorTab::handleKeyboardInput, this);

    setButtonBindings();
    setFonts();
    setSizers();
    toggleTraceLog();
}

void CalculatorTab::clearDisplayIfClearFlagSet()
{
	if (m_clearOnNextDigit)
	{
		m_listBox->Clear();
		m_clearOnNextDigit = false;
	}
}

void CalculatorTab::clearInvalidExpressionWarning()
{
    wxString currentString{ m_listBox->GetValue() };

    if (currentString.Contains(m_invalid))
    {
		currentString.Replace(m_invalid, wxEmptyString);
		m_listBox->SetValue(currentString);
        m_tracelog.logClearInvalidWarning(currentString.ToStdString());
    }
}

void CalculatorTab::enterPressed()
{
    std::string expression{ m_listBox->GetValue().ToStdString() };

    m_tracelog.logSendEquationToTokenizer(expression);

    std::vector<Token> tokens{ m_tokenizer.tokenize(std::string_view{ expression }) };
    m_tracelog.logSendForShunting(tokens.size());

    std::queue<Token> queue{ m_evaluator.shunt(tokens) };
    m_tracelog.logShuntingComplete(queue.size());

    std::string answer{ m_evaluator.evaluate(queue) };
    m_tracelog.logCalcCheckForErrorResult(answer == Word::error
        || answer == Word::underflow
        || answer == Word::overflow);

    if (answer != Word::error)
    {
        m_tracelog.logDisplayAnswer(answer);
		m_listBox->SetValue(answer);
        return;
    }

	m_listBox->SetValue(m_invalid + expression);
	m_tracelog.logDisplayError(m_listBox->GetValue().ToStdString());
    m_tracelog.resetCounter();
}

void CalculatorTab::handleButtonPress(const wxCommandEvent& event)
{
    m_tracelog.logButtonPressed(static_cast<ButtonID>(event.GetId()));
    clearInvalidExpressionWarning();

    switch (event.GetId())
    {
    case static_cast<int>(ButtonID::zero):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::zero);
        break;

    case static_cast<int>(ButtonID::one):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::one);
        break;

    case static_cast<int>(ButtonID::two):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::two);
        break;

    case static_cast<int>(ButtonID::three):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::three);
        break;

    case static_cast<int>(ButtonID::four):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::four);
        break;

    case static_cast<int>(ButtonID::five):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::five);
        break;

    case static_cast<int>(ButtonID::six):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::six);
        break;

    case static_cast<int>(ButtonID::seven):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::seven);
        break;

    case static_cast<int>(ButtonID::eight):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::eight);
        break;

    case static_cast<int>(ButtonID::nine):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::nine);
        break;

    case static_cast<int>(ButtonID::decimal):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::decimal);
        break;

    case static_cast<int>(ButtonID::clear):
        m_listBox->Clear();
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::clearEntry):
        m_listBox->Remove(m_listBox->GetValue().size() - 1, m_listBox->GetValue().size());
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::plus):
        m_listBox->AppendText(Symbol::add);
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::minus):
        m_listBox->AppendText(Symbol::subtract);
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::asterisk):
        m_listBox->AppendText(Symbol::multiply);
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::slash):
        m_listBox->AppendText(Symbol::divide);
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::equals):
        m_clearOnNextDigit = true;
        enterPressed();
        break;

    case static_cast<int>(ButtonID::percent):
        m_listBox->AppendText(Symbol::percent);
        m_clearOnNextDigit = false;
        break;

    case static_cast<int>(ButtonID::traceON):
        toggleTraceLog();
        break;

    case static_cast<int>(ButtonID::traceOFF):
        toggleTraceLog();
        break;
    default:
        break;
    }
}

void CalculatorTab::handleKeyboardInput(const wxKeyEvent& event)
{
    m_tracelog.logKeyPressed(event);
    clearInvalidExpressionWarning();

    switch (event.GetKeyCode())
    {
    case WXK_NUMPAD0:
        [[fallthrough]];
    case static_cast<int>(ASCII::zero):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::zero);
        break;

    case WXK_NUMPAD1:
        [[fallthrough]];
    case static_cast<int>(ASCII::one):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::one);
        break;

    case WXK_NUMPAD2:
        [[fallthrough]];
    case static_cast<int>(ASCII::two):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::two);
        break;

    case WXK_NUMPAD3:
        [[fallthrough]];
    case static_cast<int>(ASCII::three):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::three);
        break;

    case WXK_NUMPAD4:
        [[fallthrough]];
    case static_cast<int>(ASCII::four):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::four);
        break;

    case WXK_NUMPAD5:
        [[fallthrough]];
    case static_cast<int>(ASCII::five):
        if (event.GetModifiers() == wxMOD_SHIFT)
        {
            m_listBox->AppendText(Symbol::percent);
			m_clearOnNextDigit = false;
            break;
        }
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::five);
        break;

    case WXK_NUMPAD6:
        [[fallthrough]];
    case static_cast<int>(ASCII::six):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::six);
        break;

    case WXK_NUMPAD7:
        [[fallthrough]];
    case static_cast<int>(ASCII::seven):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::seven);
        break;

    case WXK_NUMPAD8:
        [[fallthrough]];
    case static_cast<int>(ASCII::eight):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::eight);
        break;

    case WXK_NUMPAD9:
        [[fallthrough]];
    case static_cast<int>(ASCII::nine):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::nine);
        break;

    case WXK_NUMPAD_DECIMAL:
        [[fallthrough]];
    case WXK_DECIMAL:
        [[fallthrough]];
    case static_cast<int>(ASCII::decimal):
        clearDisplayIfClearFlagSet();
        m_listBox->AppendText(Symbol::decimal);
        break;

    case WXK_ESCAPE:
        m_listBox->Clear();
        m_clearOnNextDigit = false;
        break;

    case WXK_BACK:
        m_listBox->Remove(m_listBox->GetValue().size() - 1, m_listBox->GetValue().size());
        m_clearOnNextDigit = false;
        break;

    case WXK_NUMPAD_ADD:
        [[fallthrough]];
    case WXK_ADD:
        [[fallthrough]];
    case static_cast<int>(ASCII::plus):
        m_listBox->AppendText(Symbol::add);
        m_clearOnNextDigit = false;
        break;

    case WXK_NUMPAD_SUBTRACT:
        [[fallthrough]];
    case WXK_SUBTRACT:
        [[fallthrough]];
    case static_cast<int>(ASCII::minus):
        m_listBox->AppendText(Symbol::subtract);
        m_clearOnNextDigit = false;
        break;

    case WXK_NUMPAD_MULTIPLY:
        [[fallthrough]];
    case WXK_MULTIPLY:
        [[fallthrough]];
    case static_cast<int>(ASCII::asterisk):
        m_listBox->AppendText(Symbol::multiply);
        m_clearOnNextDigit = false;
        break;

    case WXK_NUMPAD_DIVIDE:
        [[fallthrough]];
    case WXK_DIVIDE:
        [[fallthrough]];
    case static_cast<int>(ASCII::slash):
        m_listBox->AppendText(Symbol::divide);
        m_clearOnNextDigit = false;
        break;

    case WXK_NUMPAD_EQUAL:
        [[fallthrough]];
    case WXK_RETURN:
        m_clearOnNextDigit = true;
        enterPressed();
        break;

    default:
        break;
    }
}

void CalculatorTab::setButtonBindings()
{
    m_zeroButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_oneButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_twoButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_threeButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_fourButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_fiveButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_sixButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_sevenButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_eightButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_nineButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_decimalButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_clearButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_clearEntryButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_plusButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_minusButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_asteriskButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_slashButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_equalsButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_percentButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_traceOnButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });

    m_traceOffButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event) { handleButtonPress(event); });
}

void CalculatorTab::setFonts()
{
    wxFont font = m_zeroButton->GetFont();
    font.SetPointSize(16);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    m_zeroButton->SetFont(font);
    m_oneButton->SetFont(font);
    m_twoButton->SetFont(font);
    m_threeButton->SetFont(font);
    m_fourButton->SetFont(font);
    m_fiveButton->SetFont(font);
    m_sixButton->SetFont(font);
    m_sevenButton->SetFont(font);
    m_eightButton->SetFont(font);
    m_nineButton->SetFont(font);
    m_decimalButton->SetFont(font);
    m_clearButton->SetFont(font);
    m_clearEntryButton->SetFont(font);
    m_plusButton->SetFont(font);
    m_minusButton->SetFont(font);
    m_asteriskButton->SetFont(font);
    m_slashButton->SetFont(font);
    m_equalsButton->SetFont(font);
    m_percentButton->SetFont(font);

    font.SetPointSize(12);
    m_listBox->SetFont(font);
    m_memoBox->SetFont(font);
}

void CalculatorTab::setSizers()
{
    m_traceButtonsSizer->Add(m_calcButton, wxSizerFlags(0).Expand().Border(wxALL, 2));
    m_traceButtonsSizer->Add(5, 0, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_traceButtonsSizer->Add(m_traceOnButton, wxSizerFlags(0).Expand().Border(wxALL, 2));
    m_traceButtonsSizer->Add(m_traceOffButton, wxSizerFlags(0).Expand().Border(wxALL, 2));

    m_listBoxSizer->Add(m_listBox, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_memoBoxSizer->Add(m_memoBox,  wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputRowOneSizer->Add(m_sevenButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputRowOneSizer->Add(m_eightButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputRowTwoSizer->Add(m_fourButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputRowTwoSizer->Add(m_fiveButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputRowThreeSizer->Add(m_oneButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputRowThreeSizer->Add(m_twoButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputColumnOneAndTwoSizer->Add(m_inputRowOneSizer, wxSizerFlags(1).Expand());
    m_inputColumnOneAndTwoSizer->Add(m_inputRowTwoSizer, wxSizerFlags(1).Expand());
    m_inputColumnOneAndTwoSizer->Add(m_inputRowThreeSizer, wxSizerFlags(1).Expand());
    m_inputColumnOneAndTwoSizer->Add(m_zeroButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputColumnThreeSizer->Add(m_nineButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnThreeSizer->Add(m_sixButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnThreeSizer->Add(m_threeButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnThreeSizer->Add(m_decimalButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputColumnFourSizer->Add(m_clearButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFourSizer->Add(m_plusButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFourSizer->Add(m_asteriskButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFourSizer->Add(m_equalsButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputColumnFiveSizer->Add(m_clearEntryButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFiveSizer->Add(m_minusButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFiveSizer->Add(m_slashButton, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputColumnFiveSizer->Add(m_percentButton, wxSizerFlags(1).Expand().Border(wxALL, 2));

    m_inputButtonSizer->Add(m_inputColumnOneAndTwoSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputButtonSizer->Add(m_inputColumnThreeSizer, wxSizerFlags(1).Expand().Border(wxTOP | wxBOTTOM, 2));
	m_inputButtonSizer->Add(5, 0, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputButtonSizer->Add(m_inputColumnFourSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_inputButtonSizer->Add(m_inputColumnFiveSizer, wxSizerFlags(1).Expand().Border(wxTOP | wxBOTTOM | wxRIGHT, 2));

    m_windowStackSizer->Add(m_traceButtonsSizer, wxSizerFlags(0).Expand().Border(wxALL, 2));
    m_windowStackSizer->Add(m_listBoxSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_windowStackSizer->Add(m_memoBoxSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    m_windowStackSizer->Add(m_inputButtonSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));

    SetSizerAndFit(m_windowStackSizer);
}

void CalculatorTab::toggleTraceLog()
{
    if (m_tracelog.getLogState())
    {
        m_traceOnButton->Enable();
        m_traceOffButton->Disable();
        m_tracelog.disableLogging();
    }
    else
    {
        m_traceOnButton->Disable();
        m_traceOffButton->Enable();
        m_tracelog.enableLogging();
    }
}
