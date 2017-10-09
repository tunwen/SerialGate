#ifndef C_TCPSERVER_H
#define C_TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

// tcp 服务器端
class C_tcpServer : public QObject
{
    Q_OBJECT
public:
    explicit C_tcpServer(QObject *parent = 0);

signals:
    sig_recvData(QByteArray &array);
    sig_newConnect(QString IP,quint16 nPort);
private:
    quint64 m_sum;
    quint32 m_byte;
    quint32 m_byte_s;

public slots:
    void slot_newConnect();
    void slot_disconmnect();
    void slot_readData();
private:
    QTcpServer m_tcpServer;
    QTcpSocket *m_clientSock;
    bool m_isConnected;
public:
    bool startListen(quint16 nPort=502);
    void stopServ();
    void sendData(QByteArray &array);

    void get_count(quint64 &sum,quint32 &byte_s,quint32 &byte);
    bool isConnnected();
};

#endif // C_TCPSERVER_H
