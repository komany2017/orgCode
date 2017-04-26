#include "settingpanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("settingpanel.qm",":/");
    a.installTranslator(&translator);

    SettingPanel w;
    w.show();

    return a.exec();
}
