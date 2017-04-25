#ifndef MYMODEL_H
#define MYMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QMap>

class MyModel: public QStandardItemModel
{
    Q_OBJECT
public:
  QMap<QModelIndex,qint64 > mymap;
  QMap<int,QString > mymapf;
public:
    MyModel(QObject * parent = 0) :
    QStandardItemModel(parent)
    {}
    MyModel(int rows, int columns, QObject * parent = 0):
    QStandardItemModel(rows,columns,parent)
    {}
};


#endif // MYMODEL_H
