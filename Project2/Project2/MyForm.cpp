#include "MyForm.h"
#include<GL\freeglut.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ arg)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Project2::MyForm form;
	Application::Run(%form);

}