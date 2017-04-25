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
    // Dialog dlg;
    //dlg.show();
    QFile file(":/qss/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    file.close();
    MainWindow w;
    w.show();
    // QSplitter *splitter = new QSplitter(Qt::Vertical,0);
    // QFileSystemModel *model = new QFileSystemModel;
    // model->setRootPath(QDir::currentPath());
    // QTreeView *tree = new QTreeView(splitter);
    // tree->setModel(model);
    // tree->setRootIndex(model->index(QDir::currentPath()));
    // QListView *list = new QListView(splitter);
    // list->setModel(model);
    // list->setRootIndex(model->index(QDir::currentPath()));
    //splitter->setWindowTitle("Two views onto the same file system model");
    //splitter->show();
    //table->show();
    return a.exec();
}
