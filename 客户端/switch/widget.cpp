#include "widget.h"
#include "ui_widget.h"
#include<string.h>
#include <QDebug>
#include <QTime>
#include <QMessageBox>

int led1=0;
int led2=0;
int led3=0;
int led4=0;
int pwm =0;
//int temp1;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("智能家居");//窗口标题
    this->setWindowIcon(QIcon(":/1.png"));//窗口小图标
    ui->stackedWidget->setCurrentWidget(ui->page);
    QPixmap pix =QPixmap(":/home.jpg").scaled(this->size());//窗体大背景图
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(pix));
    this->setPalette(palette);
    connect(&socket,SIGNAL(connected()),this,SLOT(myconnectSlot()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(recvSlot()));
    mark = 0;
    FLAG = 0;
}

Widget::~Widget()
{
    delete ui;
}



void Widget::myconnectSlot()
{
    ui->linkbtn->setEnabled(false);
}


void Widget::on_linkbtn_clicked()
{
    QString ip = ui->ipline->text();
    int port = ui->portline->text().toInt();
    socket.connectToHost(ip,port);
}
void Widget::recvSlot()
{
        QString strx = "pic";
        QByteArray arrx;
        arrx.append(strx);
        socket.write(arrx);

        char buf[18] = {0};
        //接收文件头

        socket.read(buf, sizeof(buf));
        //解析文件头
        QString str = QString::fromUtf8(buf);
        qDebug()<<(str);
        //size#temp#btn#pm2_%
        fileSize = str.section('#',0,0).toInt();
        tmp    = str.section('#',1,1);
        btn    = str.section('#',2,2);
        pm_2_5 = str.section('#',3,3);
        temp1 = (tmp.toDouble())*0.001;
        QString a = QString::number(temp1);
        ui->len->setText(QString::number(fileSize));
        ui->data->setText(pm_2_5);
        ui->temp->setText(a+"℃");

//        socket.flush();
//        socket.waitForReadyRead(10000);

        //只要flag不等于true，哪怕这个被调用很多次，也只会进入到这个分支
        QByteArray arrTmp = socket.readAll();
        QByteArray arr1;
        arr1.append(arrTmp);
        //qDebug()<<(QString::number(arr1.size()));
        if(arr1.size() >= fileSize)
        {
            //将文件显示在label上
            QPixmap pix;
            //将arr中的文件内容直接显示
            bool ok = pix.loadFromData(arr1);
            if(true == ok)
            {
               ui->picture->setPixmap(pix);
               ui->picture->setScaledContents(true);
            }
        }

}
void Widget::on_led1btn_clicked()
{
    QString str = "cmd";
    QByteArray arr;
    arr.append(str);
    socket.write(arr);
    led1=!led1;
    QString str1;
    if(led1)
         str1 = "f010f";
    else
         str1 = "f000f";
    QByteArray arr1;
    arr1.append(str1);
    socket.write(arr1);
}

void Widget::on_led2btn_clicked()
{
    QString str = "cmd";
    QByteArray arr;
    arr.append(str);
    socket.write(arr);
    led2=!led2;
    QString str1;
    if(led2)
        str1 = "f110f";
    else
        str1 = "f100f";
    QByteArray arr1;
    arr1.append(str1);
    socket.write(arr1);
}

void Widget::on_led3btn_clicked()
{
    QString str = "cmd";
    QByteArray arr;
    arr.append(str);
    socket.write(arr);
    led3=!led3;
    QString str1;
    if(led3)
        str1 = "f210f";
    else
        str1 = "f200f";
    QByteArray arr1;
    arr1.append(str1);
    socket.write(arr1);
}

void Widget::on_led4btn_clicked()
{
    QString str = "cmd";
    QByteArray arr;
    arr.append(str);
    socket.write(arr);
    led4=!led4;
    QString str1;
    if(led4)
        str1 = "f310f";
    else
        str1 = "f300f";
    QByteArray arr1;
    arr1.append(str1);
    socket.write(arr1);
}


void Widget::on_pwmbtn_clicked()
{
    QString str = "cmd";
    QByteArray arr;
    arr.append(str);
    socket.write(arr);
    pwm=!pwm;
    QString str1;
    if(pwm)
        str1 = "f410f";
    else
        str1 = "f400f";
    QByteArray arr1;
    arr1.append(str1);
    socket.write(arr1);
}


void Widget::on_getData_clicked()
{

    timer.start(100);

}


void Widget::on_pushButton_clicked()
{
    QString name = ui->name->text();
    QString password = ui->password->text();
    if((name == "root"&&password == "123456"))
    {
        qDebug() <<"登录成功";
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
    else
    {
        qDebug() <<"账号或密码错误";
       QMessageBox::warning(this,"警告","账号或密码错误");
    }


}

void Widget::on_pushButton_2_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->page);
}
