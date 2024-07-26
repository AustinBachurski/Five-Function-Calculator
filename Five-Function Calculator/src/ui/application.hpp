#ifndef CALCULATOR_APPLICATION_HPP
#define CALCULATOR_APPLICATION_HPP

#include "calculatorTab.hpp"
#include "traceTab.hpp"
#include "../tracelog/tracelog.hpp"

#include <wx/wx.h>
#include <wx/notebook.h>

#include <string>

class Application : public wxFrame
{
public:
    Application(const std::string& title,
        const std::filesystem::path& pathToLogFile);

private:
	// When tab changes back to page 1 (Calculator Tab)
	// set focus so keyboard inputs are captured correctly.
    void setCorrectFocus(const wxBookCtrlEvent& event);

    wxNotebook* m_tabControl;
    wxNotebookPage* m_calcTab;
    wxNotebookPage* m_traceTab;
    Tracelog m_tracelog;
};

#endif
