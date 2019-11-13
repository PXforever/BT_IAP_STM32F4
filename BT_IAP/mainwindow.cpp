#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QStringList port_list;
    ui->setupUi(this);
    connect( ui->ClearDisBtn, SIGNAL(clicked(bool)), this, SLOT(OnClickedClearScr()));
    connect( ui->Connect, SIGNAL(clicked(bool)), this, SLOT(OnClickedConnect()));
    connect( ui->Disconnect, SIGNAL(clicked(bool)), this, SLOT(OnClickedDisconnect()));
    connect( ui->SendMessage, SIGNAL(clicked(bool)), this, SLOT(OnClickedSendMessage()));
    connect( ui->ChooseFile, SIGNAL(clicked(bool)), this, SLOT(OnClickedChooseFile()));
    connect( ui->UpdateCode, SIGNAL(clicked(bool)), this, SLOT(OnClickedUpdate()));
    connect( ui->UpdateCancel, SIGNAL(clicked(bool)), this, SLOT(OnClickedUpdateCancel()));
    //connect( ui->hexSend, SIGNAL(toggled(bool)), this, SLOT(OnCheckHexSend()));
    connect( ui->SearchPort, SIGNAL(clicked(bool)), this, SLOT(OnClickedSearchPort()));
    connect( &count, SIGNAL(timeout()), this, SLOT(SendAPart()));    
    port_list = GetPortList();
    if(!port_list.isEmpty())
        ui->SerialPort->removeItem(0);
    foreach (const QString &info, port_list)
    {
        //qDebug()<<"serialPortName:"<<info;
        ui->SerialPort->addItem(info);
    }
    ui->debug_winds->hide();
    ui->debug_winds->setGeometry(11,402,678,200);
    ui->debug_winds->setStyleSheet("QTextEdit{background-color: rgb(170, 170, 170);color: rgb(0, 0, 255)}");
    //ui->debug_winds->show();
//      myQdebug();
//    thread1 = new MyThread;
//    thread1->start();
//    qDebug()<<"主线程id："<<QThread::currentThreadId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnClickedClearScr()
{
    ui->TranscationScr->clear();
    qDebug() << "clear screeen!" << endl;
}

void MainWindow::OnClickedConnect()
{
    bool res;
    qDebug() << "Connect serial Port!" << endl;
    if(!QString::compare(ui->SerialPort->currentText(),"None"))
    {
        ui->TranscationScr->append("please select a port");
    }
    else
    {
        //trigger open of serial
        sem_post( &open_serial);
        res = OpenSerial(GetComNum(), GetBaudrate(), GetDataBit(), "NoFlowControl", GetVerifyBit(), GetStopBit());
        if(!res)
        {
             qDebug()<< "Port" << GetComNum() << " error" << endl;
             ui->TranscationScr->append(+" error...");
        }
        ui->BaudRate->setDisabled(1);
        ui->Connect->setDisabled(1);
        ui->SerialPort->setDisabled(1);
        ui->BitNum->setDisabled(1);
        ui->StopBit->setDisabled(1);
        ui->VerifyBit->setDisabled(1);
        SetDevStateText("设备连接成功");
        SetDevStateStyle("background-color: rgb(255, 204, 121)");
    }

}

void MainWindow::OnClickedDisconnect()
{
    qDebug() << "Disconnect serial Port..." << endl;
    ui->BaudRate->setDisabled(0);
    ui->Connect->setDisabled(0);
    ui->SerialPort->setDisabled(0);
    ui->BitNum->setDisabled(0);
    ui->StopBit->setDisabled(0);
    ui->VerifyBit->setDisabled(0);
    m_serialPort.close();
    SetDevStateText("等待设备连接");
    SetDevStateStyle("background-color: rgb(255, 255, 255)");
}

void MainWindow::OnClickedSendMessage()
{
    qDebug() << "Send a pice of message" << endl;
    SendInfo();
}

void MainWindow::OnClickedChooseFile()
{
    qDebug() << "Choose a bin file" << endl;
    ui->progressBar->setValue(0);
    QString fileName = QFileDialog::getOpenFileName( this,
                                                     tr("文件对话框！"),
                                                     "F:",
                                                     tr("烧录文件(*txt *bin *o *exe);;"
                                                        "所有文件(*)"));
     qDebug()<<"filename : "<<fileName;
     ui->FilePath->setText(fileName);
}

void MainWindow::OnClickedUpdate()
{
    QByteArray file_array;
    //qint64 num = -1;
    qDebug() << "update device's program" << endl;
    QFile bin_file(ui->FilePath->text());
    if(m_serialPort.isOpen())
    {
        if(bin_file.open(QIODevice::ReadOnly))
        {
            qDebug() << "open file success!" << endl;
            ui->TranscationScr->append("open file success!");
            file_array = bin_file.readAll();

            //file_string.prepend(bin_file.readAll());
        }
        else
        {
            qDebug() << "open file failed...,please check filepath" << endl;
            ui->TranscationScr->append("open file failed...,please check filepath");
            return;
        }
        //qDebug() <<"file is follow:\r\n"<< file_array.toHex();
        ui->TranscationScr->append("please wait......");
        connect( &m_serialPort, SIGNAL(bytesWritten(qint64)), this, SLOT(TimerReset()));
        SendInfo_ProgressBar(file_array);
        file_array.clear();

    }
    else
    {
        //have'nt open serial port
        ui->TranscationScr->append("please open your serial tranmister,befor update");
    }
    bin_file.close();
}

void MainWindow::OnClickedUpdateCancel()
{
    qDebug() << "cancel the update task" << endl;
    //count.start(200);
}

void MainWindow::OnClickedSearchPort()
{
    qDebug() << "search port" << endl;
    ui->SerialPort->clear();
    foreach (const QString &info, GetPortList())
    {
        //qDebug()<<"serialPortName:"<<info;
        ui->SerialPort->addItem(info);
    }
}

bool MainWindow::OnCheckHexSend()
{
    bool state = ui->hexSend->checkState();
    if(state)
    {
        qDebug() << "Check Hex Send" << endl;
    }
    else
    {
        qDebug() << "Discheck Hex Send" << endl;
    }
    return state;
}

QString MainWindow::GetComNum()
{
    QString number;
    number = this->ui->SerialPort->currentText();
    qDebug() << "ComNum:"<< number << endl;
    return number;
}

QVariant MainWindow::GetBaudrate()
{
    QString baudRate;
    baudRate = this->ui->BaudRate->currentText();
    qDebug() << "Baud rate:"<< baudRate.toInt() << endl;
    return baudRate;
}

QVariant MainWindow::GetDataBit()
{
    QString dataBit;
    dataBit = this->ui->BitNum->currentText();
    qDebug() << "Data bit is :"<< dataBit.toInt() << endl;
    return dataBit;
}

QVariant MainWindow::GetStopBit()
{
    QString stopBit;
    stopBit = this->ui->StopBit->currentText();
    qDebug() << "Stop bit is :"<< stopBit.toInt() << endl;
    return stopBit;
}

QString MainWindow::GetVerifyBit()
{
    QString verifyBit;
    verifyBit = this->ui->VerifyBit->currentText();
    qDebug() << "Verify bit :"<< verifyBit << endl;
    return verifyBit;
}

void MainWindow::AddComInf(QString num)
{
    this->ui->SerialPort->addItem( num, 0);
}

QString MainWindow::GetMessage()
{
    return this->ui->MessageBar->toPlainText();
}

QString MainWindow::GetFilePath()
{
    return this->ui->FilePath->text();
}

bool MainWindow::SetFilePathDis(QString path)
{
    this->ui->FilePath->setText(path);
    return true;
}

bool MainWindow::SetDevStateText(QString Text)
{
    this->ui->DevState->setText(Text);
    return true;
}

bool MainWindow::SetDevStateStyle(QString style)
{
    this->ui->DevState->setStyleSheet(style);
    return true;
}

void MainWindow::AppendTxt2TranscationScr(QString Text)
{
    ui->TranscationScr->append(Text);
}


QStringList MainWindow::GetPortList()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    return m_serialPortName;
}

bool MainWindow::OpenSerial(QString com, QVariant Sbaud, QVariant SDataBit, QString SFlowCtrl, QString SParuty, QVariant  SStopbit )
{
    //m_serialPort = new QSerialPort();//实例化串口类一个对象

    qint32 baud;
    QSerialPort::DataBits dataBit;
    QSerialPort::FlowControl FlowControl;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbit;

    QMap<QString,int> SparutyMap;
    SparutyMap[QString("NONE")] = 1;
    SparutyMap[QString("SPACE")] = 2;
    SparutyMap[QString("MARK")] = 3;
    SparutyMap[QString("EVEN")] = 4;
    SparutyMap[QString("ODD")] = 5;

    QMap<QString,QSerialPort::FlowControl> FCMap;
    FCMap[QString("NoFlowControl")] = QSerialPort::NoFlowControl;
    FCMap[QString("NoFlowControl")] = QSerialPort::NoFlowControl;
    FCMap[QString("NoFlowControl")] = QSerialPort::NoFlowControl;
    FCMap[QString("NoFlowControl")] = QSerialPort::NoFlowControl;
    FCMap[QString("NoFlowControl")] = QSerialPort::NoFlowControl;



    //translate data type
    switch (Sbaud.toInt()) {
    case 1200:baud = QSerialPort::Baud1200;break;
    case 2400:baud = QSerialPort::Baud2400;break;
    case 4800:baud = QSerialPort::Baud4800;break;
    case 9600:baud = QSerialPort::Baud9600;break;
    case 19200:baud = QSerialPort::Baud19200;break;
    case 38400:baud = QSerialPort::Baud38400;break;
    case 57600:baud = QSerialPort::Baud57600;break;
    case 115200:baud = QSerialPort::Baud115200;break;
    default:baud = QSerialPort::Baud115200;
        break;
    }
    switch (SDataBit.toInt()) {
    case 5:dataBit = QSerialPort::Data5;break;
    case 6:dataBit = QSerialPort::Data6;break;
    case 7:dataBit = QSerialPort::Data7;break;
    case 8:dataBit = QSerialPort::Data8;break;
    default:dataBit = QSerialPort::Data8;
        break;
    }
    //qDebug()<<"ddd"<<SparutyMap.value(SParuty)<<endl;
    switch (SparutyMap.value(SParuty)) {
    case 1:parity = QSerialPort::NoParity;break;
    case 2:parity = QSerialPort::SpaceParity;break;
    case 3:parity = QSerialPort::MarkParity;break;
    case 4:parity = QSerialPort::EvenParity;break;
    case 5:parity = QSerialPort::OddParity;break;
    default:parity = QSerialPort::NoParity;
        break;
    }
    switch (SStopbit.toInt()*10) {
    case 10:stopbit = QSerialPort::OneStop;break;
    case 15:stopbit = QSerialPort::OneAndHalfStop;break;
    case 20:stopbit = QSerialPort::TwoStop;break;
    case 100:stopbit = QSerialPort::UnknownStopBits;break;
    default:stopbit = QSerialPort::OneStop;
        break;
    }
    if(m_serialPort.isOpen())//如果串口已经打开了 先给他关闭了
    {
        m_serialPort.clear();
        m_serialPort.close();
    }

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        if(!QString::compare( info.portName(), com))
        {
            m_serialPort.setPort(info);
            m_serialPort.setPortName(info.portName());
        }
    }
    qDebug()<<baud<<","<< dataBit<< ","<<SDataBit<<","<<parity<<","<<stopbit<< endl;
    m_serialPort.setBaudRate(baud,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort.setDataBits(dataBit);		//数据位为8位
    m_serialPort.setFlowControl(FlowControl);//无流控制
    m_serialPort.setParity(parity);	//无校验位
    m_serialPort.setStopBits(stopbit); //一位停止位

    if(!m_serialPort.open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        qDebug()<<m_serialPort.portName()<<"打开失败...";
        return false;
    }
    qDebug()<<m_serialPort.portName()<<"打开成功!";

    if(m_serialPort.isReadable())
    {
        qDebug() << "readble";
    }

    //connect( &m_serialPort, SIGNAL(readyRead()), this, SLOT(receiveInfo()));
    QObject::connect( &m_serialPort, &QSerialPort::readyRead, this, &MainWindow::receiveInfo);
    return true;
}

void MainWindow::receiveInfo()
{
    QByteArray info = m_serialPort.readAll();
    ui->TranscationScr->insertPlainText(info);
}

void MainWindow::SendInfo()
{
    QString info = ui->MessageBar->toPlainText();
    QByteArray info_arry =info.toLatin1();
    QByteArray infos ;
    int length;
    if(info.length() == 0 && !m_serialPort.isOpen()) return;
    if(ui->hexSend->checkState())
    {
        infos = info_arry.toHex();
        length = infos.length();
        for(int i=0 ; i < length+(length/2)+1; i++)
        {
            if(i%3 == 0)
            {
                qDebug() <<"enter insert zero" << infos;
                 infos.insert(i , ' ');
            }
            qDebug() << i <<":" << infos[i];

        }
        infos = info_arry.toHex();
        length = infos.length();
        for(int i=0 ; i < length+(length/2)+1; i++)
        {
            if(i%3 == 0)
            {
                qDebug() <<"enter insert zero" << infos;
                 infos.insert(i , ' ');
            }
            qDebug() << i <<":" << infos[i];

        }
        if(ui->newLine->checkState())
        {

            infos.insert( infos.length(), "0a 0d");
            qDebug() << "hex:" << infos;

        }
        else
        {


        }
         qDebug() << "hex:" << infos;
         m_serialPort.write(infos);
    }
    else
    {
        if(ui->newLine->checkState())
        {
            info_arry.insert(info_arry.length(), "\r\n");
            m_serialPort.write(info_arry);
        }
        else
        {
            m_serialPort.write(info_arry);

        }
    }
}


void MainWindow::SendAPart()
{
    static int current_num = 1;
    //qDebug() <<"send a part";
    count.stop();
    ui->progressBar->setValue(progress_value+=each_part_bar);
    //qDebug()<< "current part" << current_num << "part length" <<part_num ;
    if(current_num < part_num){
        m_serialPort.write(part_list[current_num]);
        current_num++;
    }
    else
    {
        count.stop();
        part_num = 0;
        each_part_bar = 0;
        part_list.clear();
        progress_value = 0.0;
        current_num = 1;
        ui->progressBar->setValue(100);
        disconnect(&m_serialPort, SIGNAL(bytesWritten(qint64)), this, SLOT(TimerReset()));
        ui->TranscationScr->append("finished!");
    }
}

void MainWindow::SendInfo_ProgressBar(QByteArray info_arry)
{

    part_num = info_arry.length()/a_part_size + 1;
    each_part_bar = 100.0/(double)part_num;
    int num_ready_send;
    qDebug()<<"this data has :" << info_arry.length() << "could divide:" << part_num << "each part" << each_part_bar;
    if(!part_list.isEmpty())
    {
        qDebug()<< "not empty," << part_list;
    }
    for(int part = 0; part < part_num; part++)
    {
        part_list << info_arry.mid( part*a_part_size, a_part_size);
        //qDebug()<< "part"<<part;
    }
    //qDebug()<<"list" << part_list;
    m_serialPort.write(part_list[0]);
    num_ready_send = m_serialPort.bytesToWrite();
    info_arry.clear();
    qDebug() <<"num:"<< num_ready_send;
}

void MainWindow::myQdebug(QString text)
{
    ui->debug_winds->append(text);
}

void MainWindow::ChangeProgressBar(qint64 num)
{
    qDebug() <<"ChangeProgressBar" ;
     ui->progressBar->setValue(num);
}

void MainWindow::TimerReset()
{
    //qDebug() <<"timer reset"  ;
    count.start(200);
}



void MainWindow::on_CheckOnline_clicked()
{
    QByteArray comd = "C|C|ON?";
    QByteArray ack;
    m_serialPort.write(comd);
    if(!m_serialPort.isOpen())
    {
        return;
    }
    if(!m_serialPort.waitForReadyRead(500))
    {
        qDebug() << "time out" << "no device";
    }
    else
    {
        ack = m_serialPort.readAll();
        if(ack == "A|C|ON!")
        {
            qDebug() << "Device ok!";
            ui->CheckOnline->setText("IAP Online");
            ui->CheckOnline->setStyleSheet("QTextEdit{background-color: rgb(170, 85, 0)}");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{

}
