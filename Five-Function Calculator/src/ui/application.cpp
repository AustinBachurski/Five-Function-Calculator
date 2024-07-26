#include "application.hpp"

Application::Application(const std::string& title, 
    const std::filesystem::path& pathToLogFile)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition,
        wxSize(316, 410), wxDEFAULT_FRAME_STYLE),
    m_tabControl{ new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(320, 380)) },
    m_traceTab{ new TraceTab(m_tabControl) },
    m_tracelog{ pathToLogFile, m_traceTab },
    m_calcTab{ new CalculatorTab(m_tabControl, m_tracelog) }
{
    wxBoxSizer* sizer{ new wxBoxSizer(wxVERTICAL) };
    sizer->Add(m_tabControl, 1, wxEXPAND);
    SetSizerAndFit(sizer);

    m_tabControl->AddPage(m_calcTab, "Calculator");
    m_tabControl->AddPage(m_traceTab, "Trace Logic");

    // Set initial focus so that keyboard inputs are captured correctly.
    m_calcTab->SetFocus();
    m_tabControl->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Application::setCorrectFocus, this);
}

void Application::setCorrectFocus(const wxBookCtrlEvent& event)
{
    if (event.GetSelection() == 0)
    {
        m_calcTab->SetFocus();
    }
}
