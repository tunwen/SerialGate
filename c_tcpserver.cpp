#include "c_tcpserver.h"

C_tcpServer::C_tcpServer(QObject *parent) : QObject(parent)
{
    m_sum = 0;
    m_byte = 0;
    m_byte_s = 0;
    m_isConnected = false;

    this->m_clientSock = NULL;
    this->m_tcpServer.setMaxPendingConnections(50);  // 设置最大连接数
    connect(&this->m_tcpServer,&QTcpServer::newConnection,this,&C_tcpServer::slot_newConnect);
}

void C_tcpServer::slot_newConnect()
{
    QTcpSocket *sock = this->m_tcpServer.nextPendingConnection();

    // 释放销毁 sock对象
    if(this->m_clientSock!=NULL)
    {
        this->m_clientSock->close();
        this->m_clientSock->deleteLater();
    }
    this->m_clientSock = sock;
    this->m_isConnected = true;

    connect(this->m_clientSock,QTcpSocket::readyRead,this,&C_tcpServer::slot_readData);
    connect(this->m_clientSock,QTcpSocket::disconnected,this,&C_tcpServer::slot_disconmnect);

    emit sig_newConnect(this->m_clientSock->peerAddress().toString(),this->m_clientSock->peerPort());
}

void C_tcpServer::slot_disconmnect()
{
     QTcpSocket* sock = static_cast<QTcpSocket*>(this->sender());

     if(sock == this->m_clientSock)
     {
         sock->close();
         sock->deleteLater();
         this->m_clientSock = NULL;
         this->m_isConnected = false;
     }
}

void C_tcpServer::slot_readData()
{
    if(this->m_clientSock!=NULL)
    {
        QByteArray array = this->m_clientSock->readAll();
        emit sig_recvData(array);

        m_sum += 1;
        m_byte_s = array.size();
        m_byte += array.size();
    }
}

// 启动监听
// nPort：服务端口号
bool C_tcpServer::startListen(quint16 nPort)
{
    if(this->m_tcpServer.isListening())
    {
        return true;
    }
    return this->m_tcpServer.listen(QHostAddress("192.168.1.100"),nPort);
}

void C_tcpServer::stopServ()
{
    if(this->m_clientSock!=NULL)
    {
        this->m_clientSock->close();
        this->m_clientSock->deleteLater();
    }
    if(this->m_tcpServer.isListening())
    {
        this->m_tcpServer.close();
    }
}

void C_tcpServer::sendData(QByteArray &array)
{
    if(this->m_clientSock!=NULL)
    {
        this->m_clientSock->write(array);
    }
}

void C_tcpServer::get_count(quint64 &sum, quint32 &byte_s, quint32 &byte)
{
    sum = this->m_sum;
    byte_s = this->m_byte_s;
    byte = this->m_byte;
}

bool C_tcpServer::isConnnected()
{
    return this->m_isConnected;
}
