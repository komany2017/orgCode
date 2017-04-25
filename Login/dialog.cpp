#include "dialog.h"
#include "ui_dialog.h"
#include <QDesktopServices>
#include <QMouseEvent>
#include "dialogwait.h"
#include "windows.h"
QString username ;
QString password;

#define CLOSEBUTTONSIZE 30

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
   //setWindowFlags(Qt::FramelessWindowHint);
  // setAttribute(Qt::WA_TranslucentBackground);
   m_bIsWindowMoveable = false;
   ui->setupUi(this);
   InitUI();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::InitUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setWindowOpacity(0.8);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(363,212);
    m_BkFrame2    = new QFrame(this);
    m_BkFrame2->setGeometry(this->rect());
    m_BkFrame2->setObjectName("BkWall_Logon");
    m_BkFrame    = new QFrame(m_BkFrame2);
    m_BkFrame->setGeometry(0,0,345,212);
    m_BkFrame->setObjectName("Frame_Logon");
    m_CaptionFrame = new QFrame(m_BkFrame);
    m_CaptionFrame->setGeometry(0,0,m_BkFrame->width(),CLOSEBUTTONSIZE);
    m_CaptionFrame->setObjectName("CationFrame_Logon");
    m_CaptionTop  = new CationTextLabel(m_CaptionFrame);
    m_CaptionTop->setGeometry(0,0,m_BkFrame->width() / 2,CLOSEBUTTONSIZE);
    m_CaptionTop->setObjectName("CationTop_Logon");
    m_ComboBox = new QComboBox(m_BkFrame);
    m_ComboBox->setGeometry(22, 67, 238, 27);
    m_ComboBox->setObjectName("username_Logon");
    m_ComboBox->setEditable(true);
    QTextCodec * codec= QTextCodec::codecForName("UTF-8");
    m_ComboBox->insertItem(0,(codec->toUnicode("张三")));
    m_ComboBox->insertItem(1,(codec->toUnicode("李四")));
    m_ComboBox->insertItem(2,(codec->toUnicode("王二")));
    m_password   = new QLineEdit(m_BkFrame);
    m_password->setGeometry(22, 115, 238, 25);
    m_password->setObjectName("password_Logon");
    m_password->setContextMenuPolicy(Qt::NoContextMenu);
    m_password->setEchoMode(QLineEdit::Password);
    m_password->setMaxLength(40);
    m_Caption1 = new CationTextLabel(m_BkFrame);
    m_Caption1->setObjectName("Cation1_Logon");
    m_Caption1->setGeometry(22, 42, 238, 27);
    m_Caption2 = new CationTextLabel(m_BkFrame);
    m_Caption2->setObjectName("Cation2_Logon");
    m_Caption2->setGeometry(22, 96, 238, 17);
    m_AutoLoginCheck = new QCheckBox(m_BkFrame);
    m_AutoLoginCheck->setObjectName("AutoLogonCheck_Logon");
    m_AutoLoginCheck->setGeometry(22, 155, 70, 17);
    m_RecordCheck = new QCheckBox(m_BkFrame);
    m_RecordCheck->setObjectName("RecordCheck_Logon");
    m_RecordCheck->setGeometry(117, 155, 70, 17);
    m_FastLogin = new QPushButton(m_BkFrame);
    m_FastLogin->setObjectName("FastLogon_Logon");
    m_FastLogin->setGeometry(22, 180, 108, 24);
    m_LoginButton = new QPushButton(m_BkFrame);
    m_LoginButton->setObjectName("Logon_Logon");
    m_LoginButton->setGeometry(267, 180, 59, 24);
    connect(m_LoginButton, SIGNAL(clicked(bool)), this, SLOT(ShowWaitDlg()));
    m_CloseButton = new QPushButton(m_CaptionFrame);
    m_CloseButton->setObjectName("LogonClose");
    m_CloseButton->setGeometry(345 - CLOSEBUTTONSIZE, 0, CLOSEBUTTONSIZE, CLOSEBUTTONSIZE);
    connect(m_CloseButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    m_TroduceButton = new QPushButton(m_BkFrame2);
    m_TroduceButton->setObjectName("TroduceButton1_Logon");
    m_TroduceButton->setGeometry(m_BkFrame->width()-1, 31, 17, 59);
    m_ResgisterButton = new QPushButton(m_BkFrame);
    m_ResgisterButton->setObjectName("ResgisterButton_Logon");
    m_ResgisterButton->setGeometry(275, 72, 51, 14);
    m_FindBackButton = new QPushButton(m_BkFrame);
    m_FindBackButton->setObjectName("FindBackButton_Logon");
    m_FindBackButton->setGeometry(275, 122, 51, 14);
    connect(m_FindBackButton, SIGNAL(clicked(bool)), this, SLOT(FindBack()));
}

void Dialog::FindBack()
{
    QDesktopServices::openUrl(QString("http://www.pyc.com.cn/getPwd0.aspx"));
}

void Dialog::mousePressEvent(QMouseEvent* e)
{
    if(e->button() ==  Qt::LeftButton)
    {
        m_bIsWindowMoveable = true;
        pt = e->pos();
    }
}

void Dialog::mouseMoveEvent(QMouseEvent* e)
{
    if(m_bIsWindowMoveable)
    {
        move(e->globalPos() - pt);
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent* e)
{
    m_bIsWindowMoveable = false;
}

void Dialog::ShowWaitDlg()
{
   this->showMinimized();
   DialogWait dlg;
   dlg.exec();
   this->showNormal();;
}

