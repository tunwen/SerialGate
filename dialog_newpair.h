#ifndef DIALOG_NEWPAIR_H
#define DIALOG_NEWPAIR_H

#include <QDialog>
#include <QSerialPort>


namespace Ui {
class Dialog_NewPair;
}

class Dialog_NewPair : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_NewPair(QWidget *parent = 0);
    ~Dialog_NewPair();

private:
    Ui::Dialog_NewPair *ui;

    quint16 m_tcpPort;
    QString m_com;
    qint32 m_baudRate;
    enum QSerialPort::DataBits m_dataBits;
    enum QSerialPort::StopBits m_stopBits;
    enum QSerialPort::Parity   m_parity;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public:
   quint16 get_tcpPort();
   QString get_com();
   qint32 get_baudRate();
   enum QSerialPort::DataBits get_dataBits();
   enum QSerialPort::StopBits get_stopBits();
   enum QSerialPort::Parity   get_parity();
};

#endif // DIALOG_NEWPAIR_H
