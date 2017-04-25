#include "login.h"

#include <QApplication>

#include  "qwinwidget.h"
#include <qmessagebox.h>

#include <QApplication>
#include <QFile>
#include "dialogwait.h"
extern QString username ;
extern QString password;
Login::Login()
{

}

Login::~Login()
{

}

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
    static bool ownApplication = FALSE;

    if ( dwReason == DLL_PROCESS_ATTACH )
    ownApplication = QMfcApp::pluginInstance( hInstance );
    if ( dwReason == DLL_PROCESS_DETACH && ownApplication )
    delete qApp;

    return TRUE;
}



extern "C" __declspec(dllexport) int showDialog( HWND parent ,TCHAR* usr, TCHAR* pwd, int* result)
{
    QFile file(("image/Logon/stylelogon.qss"));
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    file.close();
    Dialog dlg; 
    dlg.setModal(true);
    int ret = dlg.exec();
    *result = ret;;
    if(ret == QDialog::Accepted)
    {
       // usr = username.toStdWString();
       // pwd = password.toStdWString();

       _tcscpy_s(usr, 260,username.toStdWString().c_str());
       _tcscpy_s(pwd, 260,password.toStdWString().c_str());
        return true;
    }
    return false;
}
