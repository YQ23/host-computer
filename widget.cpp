#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QObject>
#include <QPainter>
#include <QEvent>
#include <QLCDNumber>
#include <QPalette>
#include <QFile>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("上位机");
    serialport = new QSerialPort;
  //  find_port();   //打开串口

    ui->close_port->setEnabled(false); //设置关闭串口不可用
  //  ui->send_button->setEnabled(false); //设置发送按钮不可用

    ui->show_p->installEventFilter(this); //安装事件过滤器

  //  startTimer(1000);  //每1s执行一次
    startTimer(500);  //每0.5s执行一次

}

Widget::~Widget()
{
    delete ui;
}

//重写过滤器事件
bool Widget::eventFilter(QObject *obj,QEvent *e)
{
   // int s[3][3]={{0,0,0},{1,0,0},{0,1,1}};
    if(obj == ui->show_p)
    {
        if(e->type() == QEvent::Paint)
        {      
          //    draw_p();
                draw_l();
          //  draw();
        }
    }
    return QWidget::eventFilter(obj,e);
}
void Widget::draw_l()
{
    QPainter painter(ui->show_p);

    painter.drawRect(QRect(0,0,300,300));

    painter.setPen(Qt::DashLine);//虚线
    painter.drawLine(100,0,100,300);
    painter.drawLine(200,0,200,300);
    painter.setPen(Qt::SolidLine);//实线

    QPen pen(QColor(255,0,0));
    pen.setWidth(3);
    painter.setPen(pen);


    int i;
    for(i=0;i<pn;i++)
    {
        painter.drawEllipse(lx[i],ly[i],10,10);
    }



  //  flag = 0;
    qDebug() << "flag=0,执行一次painter";
}


void Widget::draw_p()
{

    QPainter painter(ui->show_p);

    painter.drawRect(QRect(0,0,300,300));
   // painter.drawLine(100,0,100,300);
  //  painter.drawLine(200,0,200,300);
  //  painter.eraseRect(QRect(0,0,300,300));
 //   painter.drawRect(QRect(0,0,300,300));
    QPen pen(QColor(255,0,0));//设置颜色
    pen.setWidth(3);//设置宽度
    painter.setPen(pen);


    int col = 4;
    int row = 4;
    int x,y;
    for(int i=0;i<row*col;i++)
    {
        if(matri[i]>0)
        {
            x = (i/col) * 50;
            y = (i - x/50*col)*50;
            painter.drawEllipse(x,y,10,10);
        }
    }
  //  flag = 0;
    qDebug() << "flag=0,执行一次painter";
}

//绘图
void Widget::draw()
{
    QPainter painter(ui->show_p);

    painter.drawRect(QRect(0,0,300,300));

    QPen pen(QColor(255,0,0));
    pen.setWidth(3);
    painter.setPen(pen);
 //   painter.drawPoint(10,10);  //画点
  //  painter.drawEllipse(30,30,20,20);  //画圆

  //  posx += 20;
 //   painter.drawEllipse(10,posx,10,10);

    int col = 3; //列数
    int row = 3; //行数
    int x,y;
//    qDebug() << "mat" << mat[3];
//    for(int i=0;i<row*col;i++)
//    {
//        qDebug() << "mat["<<i<<"]"<<mat[i];
//        if(mat[i]>0)
//        {

//            x = (i/col)*100;
//            y = (i - x/100*col)*100;
//            qDebug() << x << y;
//            painter.drawEllipse(x,y,10,10);
//        }
//    }

    for(int i=0;i<row*col;i++)
    {
        qDebug() << "matr["<<i<<"]"<<matr[i];
        if(matr[i]>0)
        {
            x = (i/col)*100;
            y = (i - x/100*col)*100;
            qDebug() << x << y;
            painter.drawEllipse(x,y,10,10);
        }
    }


}



void Widget::find_port()
{
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
//        QDebug() << "Name:" << info.portName();
//        QDebug() << "Description:" << info.description();
        qDebug() << "Name:" << info.portName();
        qDebug() << "Description:" << info.description();

   //     qDebug() << "可获得的串口:" << info.availablePorts();
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(serial.portName());        //显示串口name
            serial.close();

        }
    }
}



//延时函数
void Widget::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void Widget::on_close_port_clicked()
{
    serialport->clear();        //清空缓存区
    serialport->close();        //关闭串口
    ui->open_port->setEnabled(true); //设置打开串口可用
    ui->close_port->setEnabled(false); //设置关闭串口不可用
}

//打开串口
void Widget::on_open_port_clicked()
{
    update();
    sleep(100);
    find_port();
    //初始化串口
        serialport->setPortName(ui->comboBox->currentText()); //设置串口名
        if(serialport->open(QIODevice::ReadWrite))            //打开串口成功
        {
           // qDebug() << "串口打开成功!";
            serialport->setBaudRate(ui->baud->currentText().toInt());  //设置波特率
            switch(ui->bit->currentIndex())                   //设置数据位数
            {
                case 5:serialport->setDataBits(QSerialPort::Data5);break;
                case 6:serialport->setDataBits(QSerialPort::Data6);break;
                case 7:serialport->setDataBits(QSerialPort::Data7);break;
                case 8:serialport->setDataBits(QSerialPort::Data8);break;
                default: break;
            }
            switch(ui->cbit->currentIndex())                   //设置奇偶校验
            {
                case 0: serialport->setParity(QSerialPort::NoParity);break;
                default: break;
            }
            switch(ui->stopbit->currentIndex())                     //设置停止位
            {
                case 1: serialport->setStopBits(QSerialPort::OneStop);break;
                case 2: serialport->setStopBits(QSerialPort::TwoStop);break;
                default: break;
            }
            serialport->setFlowControl(QSerialPort::NoFlowControl);     //设置流控制

            QObject::connect(serialport, &QSerialPort::readyRead, this, &Widget::Read_data);//信号槽连接


            //设置控件是否可用
            ui->close_port->setEnabled(true);
            ui->open_port->setEnabled(false);
            ui->send_button->setEnabled(true);

        }
        else  //如果打开失败
        {
            sleep(100);
            QMessageBox::information(this,tr("Error"),tr("Open the failure"),QMessageBox::Ok);
        }
}


//读取数据
void Widget::Read_data()
{
    QByteArray buf;
    buf = serialport->readAll();
    qDebug() << "buf(直接读):" << buf;
    buf = buf.toHex();
    qDebug() << "buf:" << buf;

    QString ts = buf.mid(0,6);
    if(ts=="c0c001")
    {
        QString tes = buf.mid(8,2);
        bool ok;
        int te = tes.toInt(&ok,16);
        ui->tem->setPalette(Qt::red);
        ui->tem->display(te);

        QString ligs = buf.mid(10,4);
        int lig = ligs.toInt(&ok,16);
        ui->ray->setPalette(Qt::red);
        ui->ray->display(lig);
    }

    if(!buf.isEmpty())
    {
        if(textstate_receive == false)  //文本模式
        {
            QString str = ui->receive_window->toPlainText();
         //   str += tr(buf);
            str = tr(buf);
            ui->receive_window->clear();
            ui->receive_window->append(str);
        }
        if(textstate_receive == true)   //hex模式
        {
            QString str = ui->receive_window->toPlainText();
        //    QByteArray temp = buf.toHex().toUpper();
            QByteArray temp = buf;
      //      str += tr(temp);
            str = tr(temp);
            ui->receive_window->clear();
            ui->receive_window->append(str);
        }
    }
    buf.clear();
}


void Widget::Send_data()  //自动发送数据给单片机
{
    if(textstate_send == true) //如果是hex模式
    {
    QString str = ui->send_window->toPlainText();
    QByteArray senddata;
    StringToHex(str,senddata);//将str字符串转换为16进制的形式
    serialport->write(senddata);//发送到串口
    qDebug() << "Hex模式下发送过去的数据为：" << senddata;

    }
    else if(textstate_send == false) //如果是文本模式
    {
       // QString str = ui->send_window->toPlainText();
        QByteArray senddata = ui->send_window->toPlainText().toUtf8();
        serialport->write(senddata);//发送到串口
        qDebug() << "文本模式下发送过去的数据为:" << senddata;
    }
}

//接收窗口清零
void Widget::on_clear_button1_clicked()
{
    ui->receive_window->clear();
}

//发送窗口清零
void Widget::on_clear_button2_clicked()
{
    ui->send_window->clear();
}


//接收窗口模式切换
void Widget::on_txt_mod1_clicked()
{
    if(ui->txt_mod1->text() == "文本模式")
    {
        textstate_receive = true;
        ui->txt_mod1->setText("hex模式");
    }
    else
    {
        ui->txt_mod1->setText("文本模式");
        textstate_receive = false;
    }
}

//发送窗口模式切换
void Widget::on_txt_mod2_clicked()
{
    if(ui->txt_mod2->text() == "文本模式")
    {
        textstate_send = true;
        ui->txt_mod2->setText("hex模式");
    }
    else
    {
        textstate_send = false;
        ui->txt_mod2->setText("文本模式");
    }
}

//发送窗口的发送数据按钮
void Widget::on_send_button_clicked()
{
    if(textstate_send == false)  //文本模式
    {
        serialport->write(ui->send_window->toPlainText().toLatin1());
    }

    if(textstate_send == true)     //16进制 hex模式
    {

        QString str = ui->send_window->toPlainText();
        QByteArray senddata;
        StringToHex(str,senddata);//将str字符串转换为16进制的形式
        serialport->write(senddata);//发送到串口
        qDebug() << "发送过去的数据为：" << senddata;
    }

}

//将字符转为十六进制格式
void Widget::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
  
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char Widget::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}


//更新数据按钮
void Widget::on_updat_clicked()
{
    qDebug() << "接收窗口内容为:" << ui->receive_window->toPlainText().toLatin1();
    QString str;
    int n;
    str = ui->receive_window->toPlainText().toLatin1();
    qDebug() << str;
    bool ok;
    n = str.toInt(&ok,16);
    qDebug() << n;

    float t = 26.5;
    float r = 10.7;
    int p = 3;
 //   QLCDNumber *te = new QLCDNumber(this);
    ui->tem->setPalette(Qt::red);
    ui->tem->display(t);
    ui->ray->setPalette(Qt::red);
    ui->ray->display(r);
    ui->person->setPalette(Qt::red);
    ui->person->display(p);

}


//读取接收串口数据按钮 测试用
void Widget::on_read_com_clicked()
{
    qDebug() << "接收窗口内容为:" << ui->receive_window->toPlainText().toLatin1();
    QByteArray byte_data = serialport->readAll();
    QString str;
    QString s1,s2,s3;
    int i;
    QString tem,ray,s;
    float t,r;
    int p=0;
 //   QChar ts[4];

    qDebug() << "串口接收数据为:" << byte_data;

    if(textstate_send == false)  //文本模式
    {
        str = ui->receive_window->toPlainText().toLatin1();
        qDebug() << "str1:" << str.mid(0,4); //直接用str.mid更加快速简便
//        for(i=0;i<100;i++)
//        {
//    //        if(str[i]=='null')
//    //            break;
//      //      else
//            {
//                qDebug() << str[i];
//                if(i<4)   //str[0-3]为温度,字符显示传输过来
//                {
//                    tem.append(str[i]);
//        //            ts[i] = str[i];
//                }
//                if(i>4 && i<9) //str[5-8]为光强
//                {
//                    ray.append(str[i]);
//                }
//                if(i>9)
//                {
//                    if(str[i]=='1' || str[i]=='0')
//                    {
//                        s.append(str[i]);

//                    }
//                }
//            }
//        }
        tem = str.mid(0,4);
        ray = str.mid(5,4);
        s = str.mid(9,-1);
     //   qDebug() << "tem" << tem;
     //  qDebug() << "s" << s;
    //    qDebug() << "s.length()" << s.length();
        s = s.replace(QString("\n"),QString(""));
   //     qDebug() << "s(去除换行符)" << s;
   //     qDebug() << "s.length()(去除换行符)" << s.length();

        t = tem.toInt();
        t = t/100;
        ui->tem->setPalette(Qt::red);
        ui->tem->display(t);

        r = ray.toInt();
        r = r/100;
        ui->ray->setPalette(Qt::red);
        ui->ray->display(r);

        int x,y,col;
        col = 3;
        for(i=0;(i<s.length()&&i<ma_len);i++)
        {
            if(s[i]=='1')
            {
                x = (i/col)*100;
                y = (i - x/100*col)*100;
                qDebug() << "x,y" << x << y;
                matr[i] = 1;
                p = p + 1; //人数计数
            }
        }
        ui->person->setPalette(Qt::red);
        ui->person->display(p);
    }

    else if(textstate_send == true) //hex模式
       {
        str = ui->receive_window->toPlainText().toLatin1().toUpper();
        s1 = str.mid(0,2);
        s2 = str.mid(2,2);
        s3 = str.mid(4,-1);
        int h1,h2,h3;
        h1 = Hextoi(s1,2);  //将字符串转化为int整型
        t = h1/10.0;
        h2 = Hextoi(s2,2);
        r = h2/10.0;
        h3 = Hextoi(s3,s3.length());

        qDebug() << "s1" << s1;
        qDebug() << "s2" << s2;
        qDebug() << "s3" << s3;
        qDebug() << "h1" << h1;
        qDebug() << "h2" << h2;

        ui->tem->setPalette(Qt::red);
        ui->tem->display(t);
        ui->ray->setPalette(Qt::red);
        ui->ray->display(r);


        QString key;
     //   key =  QString("%1").arg(h3,4,16,QLatin1Char('0'));//h3为int型或char型都可
        key = QString::number(h3,2);  //将int整型数据转化为二进制字符串
        qDebug() << "key" << key;


        int x,y,col;
        col = 3;
        for(i=0;i<9;i++)
        {
            if(key[i]=='1')
            {
                x = (i/col)*100;
                y = (i - x/100*col)*100;
                qDebug() << "x,y" << x << y;
                matr[i] = 1;
                p = p + 1; //人数计数
            }
        }
        ui->person->setPalette(Qt::red);
        ui->person->display(p);
       }
    update();
}


//大写
int Widget::Hextoi(QString s,char len)
{
    int i;
    int n = 0;

    if(s.length()<len)
        return 0; //解决崩毁，指针溢出问题

    for (i=0;i<len;++i)
        {
            if (s.at(i) > '9')
            {
                n = 16 * n + (10 + s.at(i).toLatin1() - 'A');
            }
            else
            {
                n = 16 * n +( s.at(i).toLatin1() - '0');
            }
        }
        return n;
}

void Widget::on_checkBox_stateChanged(int arg1)//测试用
{
    qDebug() << "点击checkBox";
    if(arg1 == 2)
    {
        qDebug() << "选中checkBox";
     //   r_txt("F:/design/test1/build-test1-Desktop_Qt_5_14_2_MinGW_64_bit-Debug/debug/out2.txt");
     //   startTimer(1000);  //每1s执行一次



    }
    if(arg1 == 0)
        qDebug() << "未选中checkBox";
}
void Widget::r_loc(QString txtname)  //读取位置文本文件
{
    QString dis_s;
    QString a;
    QString send;
    QString loc;
    QFile rf(txtname);
   // int len;
    if(!rf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!rf.atEnd())
    {
        QByteArray line = rf.readLine();
        QString str(line);
        qDebug() << str;
        dis_s.append(str);
    }
    a = dis_s.replace(QString("\n"),QString(""));
    send = a.mid(0,3);
    loc = a.mid(3,-1);
   // ui->send_window->setPlainText(send);
    qDebug() << "send:" << send;
    qDebug() << "loc:" << loc;
    pn = loc.length() / 8 ;
    ui->person->setPalette(Qt::red);
    ui->person->display(pn);
    qDebug() << "人数:" << pn;
    int i;
    QString x;
    QString y;
    float xf;
    float yf;
    for(i=0;i<pn;i++)
    {
        x = loc.mid(i*8,4);
        y = loc.mid(i*8+4,4);
        xf = x.toFloat();
        yf = y.toFloat();
        lx[i] = int(xf*300);
        ly[i] = int(300-yf*300);
    }
//    QString te,ra;
//    te = ui->tem->value();
//    ra = ui->ray->value();
//    qDebug() << "tem:" << te;
//    qDebug() << "ray:" << ra;
//    int t,r;
//    t = te.toInt();
//    r = ra.toInt();
    int t,r;
    t = ui->tem->intValue();
    r = ui->ray->intValue();
//    qDebug() << "处理后的t:" << t;
//    qDebug() << "处理后的r:" << r;
    if((a[0]=="1")&&(t<20)&&(r<50))  //如果A区有人且光照小于50lx且温度低于20摄氏度,则A区亮暖灯
    {
        ui->al->setText("A区暖灯");
    }
    else if((a[0]=="1")&&(t>20)&&(r<50))  //如果A区有人且光照小于50lx且温度高于20摄氏度,则A区亮冷灯
    {
        ui->al->setText("A区冷灯");
    }
    else
    {
        ui->al->setText("A区不亮");
    }

    if((a[1]=="1")&&(t<20)&&(r<50))  //如果B区有人且光照小于50lx且温度低于20摄氏度,则B区亮暖灯
    {
        ui->bl->setText("B区暖灯");
    }
    else if((a[1]=="1")&&(t>20)&&(r<50))  //如果B区有人且光照小于50lx且温度高于20摄氏度,则B区亮冷灯
    {
        ui->bl->setText("B区冷灯");
    }
    else
    {
        ui->bl->setText("B区不亮");
    }

    if((a[2]=="1")&&(t<20)&&(r<50))  //如果C区有人且光照小于50lx且温度低于20摄氏度,则C区亮暖灯
    {
        ui->cl->setText("C区暖灯");
    }
    else if((a[2]=="1")&&(t>20)&&(r<50))  //如果C区有人且光照小于50lx且温度高于20摄氏度,则C区亮冷灯
    {
        ui->cl->setText("C区冷灯");
    }
    else
    {
        ui->cl->setText("C区不亮");
    }


    QString sw;
    if(index==0) //将A区信息发送过去
    {
        if(a[0]=="1") //如果A区有人,发送0xC1 0xC1 A
        {
            sw = "C1C141";
            ui->send_window->setPlainText(sw);
        }
        if(a[0]=="0") //如果A区无人,发送0xC1 0xC1 a
        {
            sw = "C1C161";
            ui->send_window->setPlainText(sw);
        }

      //  index = index + 1;
    }
    if(index == 1)  //将B区信息发送过去
    {
        if(a[1]=="1") //如果B区有人,发送0xC1 0xC1 B
        {
            sw = "C1C142";
            ui->send_window->setPlainText(sw);
        }
        if(a[1]=="0") //如果B区无人,发送0xC1 0xC1 b
        {
            sw = "C1C162";
            ui->send_window->setPlainText(sw);
        }
      //  index = index + 1;
    }
    if(index == 2)  //将C区信息发送过去
    {
        if(a[2]=="1") //如果A区有人,发送0xC1 0xC1 C
        {
            sw = "C1C143";
            ui->send_window->setPlainText(sw);
        }
        if(a[2]=="0") //如果A区无人,发送0xC1 0xC1 c
        {
            sw = "C1C163";
            ui->send_window->setPlainText(sw);
        }
    //    index = 0;
    }

    if(index<2)
        index = index + 1;
    else
        index = 0;

    update();
}

void Widget::r_txt(QString txtname)
{
    QString dis_s;
    QString a;
    QFile rf(txtname);
    if(!rf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!rf.atEnd())
    {
        QByteArray line = rf.readLine();
        QString str(line);
        qDebug() << str;
        dis_s.append(str);
    }
    ui->send_window->setPlainText(dis_s);
    qDebug() << "dis_s" << dis_s;
    a = dis_s.replace(QString("\n"),QString(""));
    a = a.mid(22,-1);

    int i;
    for(i=0;i<a.length();i++)
    {
        if(a[i]=='1')
        {
            matri[i] = 1;
        }
        else
        {
            matri[i] = 0;
        }
    }
 //   flag = 1;
    qDebug() << "flag=1,执行一次r_txt";
 //   update();
}

void Widget::r_uart() //获取端口数据
{
    QByteArray byte_data = serialport->readAll();

}

void Widget::timerEvent(QTimerEvent *)//事件过滤器
{
    if(ui->checkBox->isChecked()==true)
    {

        r_loc("E:/yolov3/yolov3/location.txt"); //读取位置文本文件

        Send_data();//发送数据

        update();//更新

    }
}
