#ifndef LOGIN_H
#define LOGIN_H
#define _AFXDLL
#include <afx.h>
#include "login_global.h"
#include "dialog.h"
class LOGINSHARED_EXPORT Login
{

public:
    Login();
    virtual ~Login();


private:
    Dialog*   LoginDlg;

};

extern "C" __declspec(dllexport) int showDialog( HWND parent,TCHAR* usr, TCHAR* pwd, int* result );

#endif // LOGIN_H
