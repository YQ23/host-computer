#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <QEvent>
#include <QObject>
#define ma_len 9

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

   // void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *,QEvent *);
    void draw();
    void draw_p();
    void draw_l();
    int Hextoi(QString s,char len);
  //  int posx;

    void check_auto();  //检查自动发送是否打开

    int matr[ma_len]={0};

    void r_txt(QString txtname);
    int matri[16]={0};

    void r_loc(QString txtname);
    int  lx[100] = {0};
    int  ly[100] = {0};
    int  pn = 0; //人数
    int index = 0;

    void r_uart();
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
    void Send_data();


    void timerEvent(QTimerEvent *);
 //   int flag = 0;

private slots:
    void Read_data();   //读取数据
    void find_port();   //查找串口
    void sleep(int msec);  //延时函数

//    void on_open_port_clicked();      //打开串口
//    void on_close_port_clicked();     //关闭串口
//    void on_clear_button1_clicked();  //接收区清空数据
//    void on_txt_mod1_clicked();       //接收区文本模式
//    void on_clear_button2_clicked();  //发送区清空数据
//    void on_send_button_clicked();    //发送区发送数据
//    void on_txt_mod2_clicked();       //发送区文本模式

    void on_close_port_clicked();
    void on_open_port_clicked();
    void on_clear_button1_clicked();
    void on_clear_button2_clicked();
    void on_txt_mod1_clicked();
    void on_txt_mod2_clicked();
    void on_send_button_clicked();



    void on_updat_clicked();

    void on_read_com_clicked();

    void on_checkBox_stateChanged(int arg1);



private:
    Ui::Widget *ui;
    QSerialPort *serialport;
    bool textstate_receive;
    bool textstate_send;
};
#endif // WIDGET_H
