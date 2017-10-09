#ifndef C_PORTMATCH_H
#define C_PORTMATCH_H

#include <QObject>
#include "c_tcpserver.h"
#include "c_serialport.h"

/*
 * TCP_1 <-> COM_1 <=>[...]
 * TCP_2 <-> COM_2 <=>[...]
 * TCP_3 <-> COM_3 <=>[...]
 *
 * */
class C_PortMatch : public QObject
{
    Q_OBJECT
public:
    explicit C_PortMatch(QObject *parent = 0);

public slots:
     slot_newConnect(QString IP,quint16 Port);
private:
    C_tcpServer m_tcpServer;    //
    C_SerialPort m_serialPort;  //
    quint16 m_nPort;
    QString m_comName;
    quint16 m_baudRate;
    enum QSerialPort::DataBits m_dataBits;
    enum QSerialPort::StopBits m_stopBits;
    enum QSerialPort::Parity m_parity;
    enum QSerialPort::FlowControl m_flowControl;

    QString m_remoteSockIP;
    quint16 m_remoteSockPort;

public:
    void setCFG(quint16 nPort
                ,QString comName
                ,qint32  baudRate
                ,enum QSerialPort::DataBits  dataBits
                ,enum QSerialPort::StopBits stopBits
                ,enum QSerialPort::Parity   parity
                ,enum QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    bool startServ();
    void stopServ();

    void get_up_count(quint64 &sum,quint32 &byte_s,quint32 &byte);
    void get_down_count(quint64 &sum,quint32 &byte_s,quint32 &byte);

    void getRemoteInfo(QString &IP,quint16 &nPort);
    bool isConnnected();
};


#endif // C_PORTMATCH_H
