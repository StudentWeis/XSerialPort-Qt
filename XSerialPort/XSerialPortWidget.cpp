#include "XSerialPortWidget.h"
#include "ui_XSerialPortWidget.h"

XSerialPortWidget::XSerialPortWidget(QWidget *parent) : QWidget(parent), ui(new Ui::XSerialPortWidget)
{
    ui->setupUi(this);

    // 串口读数据准备好的信号与槽函数的连接；
    serialPort = new QSerialPort(this);
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadReady()));

    // 自动更新端口列表槽函数和信号连接；
    portList = new SerialPortList(200);
    portList->ScanStart();
    connect(portList, SIGNAL(onNewSerialPort(QStringList)), this, SLOT(onNewPortList(QStringList)));
}

XSerialPortWidget::~XSerialPortWidget()
{
    delete ui;
}

//===========================================================
// 发送数据

/**
 * @brief 判断是否发送十六进制数据
 */
bool HexSend = 0;
void XSerialPortWidget::on_HexSend_Ck_clicked()
{
    HexSend = 1 - HexSend;
    if(HexSend)
    {
        ui->Sent_Ed->setInputMask("HHHHHHHHHHHHHHHH");
        ui->Sent_Ed_2->setInputMask("HHHHHHHHHHHHHHHH");
    }
    else
    {
        ui->Sent_Ed->clearMask();
        ui->Sent_Ed_2->clearMask();
    }
}
/**
 * @brief 判断是否发送新行
 */
bool NewLine = 0;
void XSerialPortWidget::on_NLineSend_Ck_clicked()
{
    NewLine = 1 - NewLine;
}
/**
 * @brief 发送数据
 */
void XSerialPortWidget::on_Send_Bt_clicked()
{
    if (HexSend) // 十六进制发送
    {
        ASCIItoHex(ui->Sent_Ed->text().toLocal8Bit().data());
    }
    else // 字符发送
    {
        serialPort->write(ui->Sent_Ed->text().toLocal8Bit().data());
    }
    if (NewLine)
    {
        const char *newLineChar = "\r\n";
        serialPort->write(newLineChar, 2);
    }
}
void XSerialPortWidget::on_Send_Bt_2_clicked()
{
    if (HexSend) // 十六进制发送
    {
        ASCIItoHex(ui->Sent_Ed_2->text().toLocal8Bit().data());
    }
    else // 字符发送
    {
        serialPort->write(ui->Sent_Ed_2->text().toLocal8Bit().data());
    }
    if (NewLine)
    {
        const char *newLineChar = "\r\n";
        serialPort->write(newLineChar, 2);
    }
}
/**
 * @brief ASCII 转十六进制
 * @param dataASCII
 */
void XSerialPortWidget::ASCIItoHex(char * dataASCII)
{
    int dataNumber = strlen(dataASCII);
    if (dataNumber%2 || dataNumber == 0)
    {
        QMessageBox::information(this, "提示", "十六进制没输够");
        return;
    }
    char dataHex[dataNumber/2];
    for (int i = 0; i < dataNumber/2; i++) {
        dataHex[i] = 0;
        sscanf(&dataASCII[2*i], "%x", &dataHex[i]);
    }
    serialPort->write(dataHex);
}
/**
 * @brief 清除发送
 */
void XSerialPortWidget::on_ClearSend_Bt_clicked()
{
    ui->Sent_Ed->clear();
    ui->Sent_Ed_2->clear();
}

// 发送数据
//===========================================================

//===========================================================
// 接收数据

bool HexDisplay = 0;
/**
 * @brief 判断是否以十六进制显示数据
 */
void XSerialPortWidget::on_HexDisplay_Ck_clicked()
{
    HexDisplay = 1 - HexDisplay;
}
/**
 * @brief 读取接受缓存
 */
void XSerialPortWidget::serialPortReadReady()
{
    QString buf = QString(serialPort->readAll());
    if (HexDisplay) {
        char bufbuf[100];
        for (int i = 0; i < (int)strlen(buf.toLocal8Bit().data()); i++) {
            sprintf(bufbuf, "%x", buf.toLocal8Bit().data()[i]);
            ui->Receieve_Ed->insertPlainText(bufbuf);
        }
    }
    else {
        ui->Receieve_Ed->insertPlainText(buf);
    }
}
/**
 * @brief 清空接受区
 */
void XSerialPortWidget::on_ClearRec_Bt_clicked()
{
    ui->Receieve_Ed->clear();
}
// 接收数据
//===========================================================


//===========================================================
// 串口配置

/**
 * @brief 切换串口开关
 */
void XSerialPortWidget::on_OpenClose_Bt_clicked()
{
    if (!opened)
    {
        // 设置串口号、数据位、停止位
        QSerialPort::DataBits myDataBits;
        QSerialPort::StopBits myStopBits;
        int myBaudRate;
        sscanf(ui->Baud_Cb->currentText().toLatin1(),"%d",&myBaudRate); // 转换波特率字符串为整型数字
        switch (ui->Date_Cb->currentIndex()) {
        case 0:
            myDataBits = QSerialPort::Data8;
            break;
        case 1:
            myDataBits = QSerialPort::Data7;
            break;
        }
        switch (ui->Stop_Cb->currentIndex()) {
        case 0:
            myStopBits = QSerialPort::OneStop;
            break;
        case 1:
            myStopBits = QSerialPort::OneAndHalfStop;
            break;
        case 2:
            myStopBits = QSerialPort::TwoStop;
            break;
        }
        serialPort->setPortName(ui->Serial_Cb->currentText());
        serialPort->setBaudRate(myBaudRate);
        serialPort->setDataBits(myDataBits);
        serialPort->setStopBits(myStopBits);

        // 开启串口
        if (serialPort->open(QIODevice::ReadWrite))
        {
            ui->OpenClose_Bt->setText("关闭串口");
            opened = true;
        }
        else
        {
            QMessageBox::information(this, "提示", "打开串口失败");
        }
    }
    else
    {
        serialPort->close();
        ui->OpenClose_Bt->setText("打开串口");
        opened = false;
    }
}

/**
 * @brief 更新端口列表；
 * @param portName：端口列表;
 */
void XSerialPortWidget::onNewPortList(QStringList portName)
{
    ui->Serial_Cb->clear();
    ui->Serial_Cb->addItems(portName);
}

// 串口配置
//===========================================================


void XSerialPortWidget::on_Help_Bt_clicked()
{
    QMessageBox::information(this, "帮助", "发邮件至 studentweis@gmail.com");
}
