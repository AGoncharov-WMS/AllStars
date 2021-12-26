//---------------------------------------------------------------------------
//                   Program #0097  Made by -=WMS=-
//                  Description: Game "Again Lines"!
//              The project was started at June 26, 2004
//            The project was finished at October 22, 2005
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("main.cpp", Form1);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "All Stars!";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
