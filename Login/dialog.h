#ifndef DIALOG_H
#define DIALOG_H

#include <QAbstractButton>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTextCodec>
#include <QGridLayout>
namespace Ui {
class Dialog;
}
class CationTextLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString Caption READ Caption WRITE SetCaption DESIGNABLE true)
public:
    CationTextLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        :QLabel(parent, f)
    {

    }
    CationTextLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        :QLabel(text,parent, f)
    {

    }
    void SetCaption(QString cation)
    {
        const QByteArray b = cation.toLatin1();
        QTextCodec * codec= QTextCodec::codecForName("UTF-8");
        m_cationText = codec->toUnicode(b);
        setText(Caption());
    }

    QString Caption()const
    {
        return m_cationText;
    }
private:
    QString         m_cationText;
};


class Dialog : public QDialog
{
    Q_OBJECT
protected:
    virtual void mousePressEvent(QMouseEvent* e);    // 鼠标的单击事件
    virtual void mouseMoveEvent(QMouseEvent* e);    // 鼠标的移动事件
    virtual void mouseReleaseEvent(QMouseEvent* e);  // 鼠标的单击释放事件
signals:
    void Logon(QString usr, QString pwd);
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void ShowWaitDlg();
    void FindBack();
protected:
    void InitUI();
private:
    Ui::Dialog*              ui;
    bool                     m_bIsWindowMoveable;
    QGridLayout             *lay;
    QPoint                   pt;
    QLineEdit*               m_password;
    QComboBox*               m_ComboBox;
    QPushButton*             m_FastLogin;
    QPushButton*             m_CloseButton;
    QPushButton*             m_LoginButton;
    QFrame*                  m_BkFrame;
    QFrame*                  m_BkFrame2;
    QCheckBox*               m_AutoLoginCheck;
    QCheckBox*               m_RecordCheck;
    CationTextLabel*         m_Caption1;
    CationTextLabel*         m_Caption2;
    CationTextLabel*         m_CaptionTop;
    QPushButton*             m_TroduceButton;
    QPushButton*             m_ResgisterButton;
    QPushButton*             m_FindBackButton;
    QFrame*                  m_CaptionFrame;
};

#endif // DIALOG_H
