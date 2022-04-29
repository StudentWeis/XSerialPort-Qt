#ifndef XSERIALPORTWIDGET_H
#define XSERIALPORTWIDGET_H

#include <string>
#include <stdio.h>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include "SerialPortList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class XSerialPortWidget; }
QT_END_NAMESPACE

class XSerialPortWidget : public QWidget
{
    Q_OBJECT

public:
    XSerialPortWidget(QWidget *parent = nullptr);
    ~XSerialPortWidget();


    QSerialPort *serialPort;
    SerialPortList *portList;
    bool opened = false;

private slots:
    void on_OpenClose_Bt_clicked();
    void on_Send_Bt_clicked();
    void on_Send_Bt_2_clicked();
    void on_ClearRec_Bt_clicked();
    void on_ClearSend_Bt_clicked();
    void on_HexSend_Ck_clicked();
    void on_NLineSend_Ck_clicked();
    void serialPortReadReady();
    void onNewPortList(QStringList portName);
    void on_HexDisplay_Ck_clicked();

    void on_Help_Bt_clicked();

private:
    Ui::XSerialPortWidget *ui;
    void ASCIItoHex(char * dataASCII);
};

#endif // XSERIALPORTWIDGET_H
