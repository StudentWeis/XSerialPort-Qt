#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QTimer>
#include <QDebug>
#include <QSerialPort>
#include <QStringList>
#include <QSerialPortInfo>

/* @交流：studentweis@gmail.com
 *
 * @功能：
 * SerialPortList 类实现当前可用的串口进行实时扫描，当发现有新的串口
 * 或是现有串口消失时，SerialPortList类将发出一个QStringList类型的
 * 信号onNewSerialPort，QStringList类型对象中存放当前可用的串口设备。
 *
 * @使用方法：
 * 1、Qt版本需要高于5.1以上；
 *
 * 2、在新建Qt工程文件中时需要加入QSerialPort模块；即：在xxx.pro文件
 *    中加入 Qt += serialport
 *
 * 3、在应用程序中定义一个槽，用于响应SerialPortList信号，获取串口列表
 *    void onNewPortList(QStringList portName){
 *     ui->comboBoxComList->clear();
 *     ui->comboBoxComList->addItems(portName);
 *    }
 *
 * 4、在应用程序中构建SerialProt对象，连接onNewSerialPort信号：
 *   portList = new SerialPortList(200);
 *   connect(portList, SIGNAL(onNewSerialPort(QStringList)),\
 *           this,     SLOT(onNewPortList(QStringList)));
 *
 * 5、开启扫描
 *   portList->ScanStart();
 */

#define DEBUG_INFOR_EN 0

class SerialPortList : public QObject
{
    Q_OBJECT

  public:
    SerialPortList();
    SerialPortList(quint16);
    ~SerialPortList();

    quint16 scanCycleMs;
    QTimer *timer;
    QStringList oldPortStringList;

    void ScanStart();
    void ScanStop();

  public slots:
    void onTimeOut();

  signals:
    void onNewSerialPort(QStringList);
};

#endif // SERIALPORTLIST_H
