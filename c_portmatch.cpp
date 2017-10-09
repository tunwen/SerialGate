#include "c_portmatch.h"

C_PortMatch::C_PortMatch(QObject *parent) : QObject(parent)
{
    connect(&this->m_serialPort,&C_SerialPort::sig_readData,&this->m_tcpServer,&C_tcpServer::sendData);
    connect(&this->m_tcpServer,&C_tcpServer::sig_recvData,&this->m_serialPort,&C_SerialPort::writeData);

    connect(&this->m_tcpServer,&C_tcpServer::sig_newConnect,this,C_PortMatch::slot_newConnect);

    this->m_remoteSockIP.clear();
    this->m_nPort =0;
}

C_PortMatch::slot_newConnect(QString IP, quint16 Port)
{
    this->m_remoteSockIP = IP;
    this->m_remoteSockPort = Port;
}

void C_PortMatch::setCFG(quint16 nPort, QString comName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    this->m_nPort = nPort;
    this->m_comName = comName;
    this->m_baudRate = baudRate;
    this->m_dataBits = dataBits;
    this->m_stopBits = stopBits;
    this->m_parity = parity;
    this->m_flowControl = flowControl;
}

bool C_PortMatch::startServ()
{
    bool res = this->m_serialPort.openCOM(this->m_comName
                                         ,this->m_baudRate
                                         ,this->m_dataBits
                                         ,this->m_stopBits
                                         ,this->m_parity
                                         ,this->m_flowControl);

    res &= this->m_tcpServer.startListen(this->m_nPort);

    return res;
}


void C_PortMatch::stopServ()
{
    this->m_tcpServer.stopServ();
    this->m_serialPort.closeCOM();
}

void C_PortMatch::get_up_count(quint64 &sum, quint32 &byte_s, quint32 &byte)
{
    this->m_tcpServer.get_count(sum,byte_s,byte);
}

void C_PortMatch::get_down_count(quint64 &sum, quint32 &byte_s, quint32 &byte)
{
    this->m_serialPort.get_count(sum,byte_s,byte);
}

void C_PortMatch::getRemoteInfo(QString &IP, quint16 &nPort)
{
   IP =  this->m_remoteSockIP;
   nPort = this->m_remoteSockPort;
}

bool C_PortMatch::isConnnected()
{
    return this->m_tcpServer.isConnnected();
}

