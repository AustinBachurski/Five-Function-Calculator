#ifndef CALCULATOR_TRACE_TAB_HPP
#define CALCULATOR_TRACE_TAB_HPP

#include <wx/msgdlg.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include <string>

class TraceTab : public wxNotebookPage
{
public:
    TraceTab(wxNotebook* control);

    void logMessage(const std::string& message);

private:
    wxBoxSizer* m_fitToWindow{ new wxBoxSizer(wxVERTICAL) };
    wxTextCtrl* m_listBox{ new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY) };
};

#endif
