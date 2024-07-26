# Five-Function Calculator

This project was a take home challenge from a company.
<br><br>
The requirements are outlined in the `Five-Function-Calculator Specification.pdf` file, the project was to be completed in one week.

Developed on a 64bit Windows 10 machine using Visual Studio Community 2022 and the wxWidgets C++ library, intended to be compiled with MSVC and C++ 20.  Development time before submission: `43 hours`.

## Build Instructions

The solution file requires a 64 bit configuration, and wxWidgets to be installed.  It references the “wxwin” environment variable to locate the wxWidgets libraries required to build the project.

*	If wxWidgets is not installed, you can download the installer at: https://wxwidgets.org/downloads/
    *	Under “Source Code” there’s a “Windows Installer (does not contain binaries)” link to download the installation executable - both your browser and Windows will likely complain about the dangerous executable file (mine did).
    *	Once installed, you will need to build the libraries with the same compiler you will compile the application with.  There are Visual Studio solution files under “$(wxwin)\build\msw\” - wx_vc17.sln is the latest.  
    *	With the solution open in Visual Studio, change to the configuration you want to build the project in (Debug or Release), and build the solution.

*	Once/If wxWidgets is installed, either update the include directories for the project, or simply add the “wxwin” environment variable pointing to your wxWidgets directory - default: “C:\wxWidgets-3.2.4” (latest version at time of writing).  You may need to restart your computer for the environment variable changes to take effect.

*	Unzip the “Five-Function-Calculator.zip” file and open the “Five-Function Calculator.sln” solution file.  With Visual Studio open, select the configuration you built the wxWidgets libraries for (Debug or Release), and build the solution.  The binary will be built as  “Five-Function-Calculator\bin\x64\(Debug or Release)\Five-Function Calculator.exe”.

## Usage Instructions

The application generates a “CalcTrace.txt” file in its current directory - this file is overwritten each time the application is opened!  Please save a copy if you wish to retain the previous output for later review.

*	The calculator accepts button input with the mouse, as well as keyboard input from either the number row or number pad.
*	The “Calc” button and second text box is a placeholder to meet visual specifications.
*	The trace tab displays the steps and output of calculations.
*	A “CalcTrace.txt” file also contains the output information.

