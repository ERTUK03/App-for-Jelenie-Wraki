#include "App.h"

bool App::OnInit()
{
	Main* MainWindow = new Main();
	MainWindow->Show();
	return true;
}

wxIMPLEMENT_APP(App);