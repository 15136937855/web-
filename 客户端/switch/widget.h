#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void myconnectSlot();
    void recvSlot();

    //void on_sendbtn_clicked();

    void on_linkbtn_clicked();

    void on_led1btn_clicked();

    void on_led2btn_clicked();

    void on_led3btn_clicked();

    void on_led4btn_clicked();

    void on_pwmbtn_clicked();


    void on_getData_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket socket;
    QTimer timer;
    char bufALL[10000];
    int mark;
    int FLAG;
    int fileSize;
    double temp1;
    QString tmp;
    QString btn;
    QString pm_2_5;
     QString a;
    //QString  tmp1;

};

#endif // WIDGET_H
