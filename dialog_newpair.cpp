#include "dialog_newpair.h"
#include "ui_dialog_newpair.h"
#include "c_serialport.h"
Dialog_NewPair::Dialog_NewPair(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_NewPair)
{
    ui->setupUi(this);

    ui->lineEdit_tcpPort->setText("9100");
    ui->comboBox_COM->addItems(C_SerialPort::comList());
    ui->comboBox_baudRate->addItems(C_SerialPort::baudList());
    ui->comboBox_dataBits->addItems(C_SerialPort::dataBList());
    ui->comboBox_stopBits->addItems(C_SerialPort::stopBList());
    ui->comboBox_parity->addItems(C_SerialPort::parityList());

}

Dialog_NewPair::~Dialog_NewPair()
{
    delete ui;
}

void Dialog_NewPair::on_pushButton_clicked()
{
    this->m_tcpPort = ui->lineEdit_tcpPort->text().toInt();
    this->m_com = ui->comboBox_COM->currentText();
    this->m_baudRate = ui->comboBox_baudRate->currentText().toInt();
    this->m_dataBits = (enum QSerialPort::DataBits)ui->comboBox_dataBits->currentText().toInt();
    this->m_stopBits = (enum QSerialPort::StopBits)ui->comboBox_stopBits->currentText().toInt();
    this->m_parity = C_SerialPort::strToParity(ui->comboBox_parity->currentText());

    QDialog::accept();
}

void Dialog_NewPair::on_pushButton_2_clicked()
{
    QDialog::reject();
}

quint16 Dialog_NewPair::get_tcpPort()
{
    return this->m_tcpPort;
}

QString Dialog_NewPair::get_com()
{
    return this->m_com;
}

qint32 Dialog_NewPair::get_baudRate()
{
    return this->m_baudRate;
}

QSerialPort::DataBits Dialog_NewPair::get_dataBits()
{
    return this->m_dataBits;
}

QSerialPort::StopBits Dialog_NewPair::get_stopBits()
{
    return this->m_stopBits;
}

QSerialPort::Parity Dialog_NewPair::get_parity()
{
    return this->m_parity;
}
