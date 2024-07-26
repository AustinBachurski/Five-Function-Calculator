#include "traceTab.hpp"

TraceTab::TraceTab(wxNotebook* control)
    : wxWindow(control, wxID_ANY)
{
    m_fitToWindow->Add(m_listBox, wxSizerFlags(1).Expand().Border(wxALL, 5));
    SetSizerAndFit(m_fitToWindow);
}

void TraceTab::logMessage(const std::string& message)
{
    m_listBox->AppendText(message);
}
