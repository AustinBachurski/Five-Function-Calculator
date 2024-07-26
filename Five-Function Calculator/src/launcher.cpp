#include "ui/application.hpp"

#include <wx/wx.h>

class Launcher : public wxApp
{
public:
    bool OnInit() override;
};

bool Launcher::OnInit()
{
    Application* appWindow{ new Application(
        "Five-Function Calculator", "./CalcTrace.txt")};

    appWindow->Show();
    return true;
}

wxIMPLEMENT_APP(Launcher); // int main()
