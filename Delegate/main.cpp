#include "mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QDir>
#include <QTreeView>
#include <QListView>
#include <dialog.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/qss/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    file.close();
    MainWindow w;
    w.show();
    return a.exec();
}
