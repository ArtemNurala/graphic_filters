#include "Form1.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Threading::Tasks;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ arg) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew Form1::Form1);
}