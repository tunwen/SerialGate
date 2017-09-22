#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_newpair.h"
#include <QHostInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initeTable();

    connect(&this->m_timer,QTimer::timeout,this,MainWindow::slot_timer);
    this->m_timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewPair_triggered()
{
    Dialog_NewPair dlg(this);
    dlg.setWindowTitle("新建服务");

    if(QDialog::Accepted ==dlg.exec())
    {
        C_PortMatch *pmatch  = new C_PortMatch;
        pmatch->setCFG(dlg.get_tcpPort()
                      ,dlg.get_com()
                      ,dlg.get_baudRate()
                      ,dlg.get_dataBits()
                      ,dlg.get_stopBits()
                      ,dlg.get_parity()
                      ,QSerialPort::NoFlowControl);
        this->m_listMatch.append(pmatch);
        int sum = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(sum);
        QTableWidgetItem*item;

        int colDex=0;

        // 编号
        item = new QTableWidgetItem(QString("%1").arg(sum));
        ui->tableWidget->setItem(sum,colDex++,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        // 本机 IP
        QString IP;
        QString localHostName = QHostInfo::localHostName();
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress address,info.addresses())
        {
            if(address.protocol() == QAbstractSocket::IPv4Protocol)
            {
                if(IP.size()>0)
                {
                    IP+="/";
                }
                IP+=address.toString();
            }
        }

        item = new QTableWidgetItem(QString("%1").arg(IP));
        ui->tableWidget->setItem(sum,colDex++,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        // TCP 端口号
        item = new QTableWidgetItem(QString("%1").arg(dlg.get_tcpPort()));
        ui->tableWidget->setItem(sum,colDex++,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        // 串口名称
        item = new QTableWidgetItem(QString("%1").arg(dlg.get_com()));
        ui->tableWidget->setItem(sum,colDex++,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        // 串口配置
         QString serialCFG;
         serialCFG = QString("%1,%2,%3,%4").arg(dlg.get_baudRate())
                                           .arg(dlg.get_dataBits())
                                           .arg(dlg.get_stopBits())
                                           .arg(C_SerialPort::parityToStr(dlg.get_parity()));

         item = new QTableWidgetItem(serialCFG);
         ui->tableWidget->setItem(sum,colDex++,item);
         item->setFlags(item->flags() & (~Qt::ItemIsEditable));

         // 上行
         item = new QTableWidgetItem(QString("%1").arg(0));
         ui->tableWidget->setItem(sum,colDex++,item);
         item->setFlags(item->flags() & (~Qt::ItemIsEditable));

         // 下行
         item = new QTableWidgetItem(QString("%1").arg(0));
         ui->tableWidget->setItem(sum,colDex++,item);
         item->setFlags(item->flags() & (~Qt::ItemIsEditable));

         // 服务状态
         item = new QTableWidgetItem(QString("%1").arg("停止"));
         ui->tableWidget->setItem(sum,colDex++,item);
         item->setFlags(item->flags() & (~Qt::ItemIsEditable));
         item->setTextColor(QColor(237,28,36));
    }
}

void MainWindow::initeTable()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(120); // 默认行宽
    ui->tableWidget->horizontalHeader()->setFixedHeight(25);
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
  //  ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);//可以设置tableview所有行的默认行高。

    QStringList headerLabels;
    headerLabels<<tr("编号")<<tr("本机IP地址")<<tr("TCP端口号")<<tr("串行端口")<<tr("串口配置")<<tr("上行传输")<<tr("下行传输")<<tr("服务状态");
    ui->tableWidget->setColumnCount(headerLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->setIconSize(QSize(28,28));

   // ui->tableWidget->setSelectionBehavior (QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行

    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,120);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,130);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setColumnWidth(6,100);
    ui->tableWidget->setColumnWidth(7,120);
}

void MainWindow::on_actionStart_triggered()
{
   int row = ui->tableWidget->currentRow();
   if(row>=0)
   {
       if(this->m_listMatch[row]->startServ())
       {
           ui->tableWidget->item(row,7)->setText("运行中...");
           ui->tableWidget->item(row,7)->setTextColor(QColor(34,177,76));
       }
   }
}

void MainWindow::slot_timer()
{
    int sum = this->m_listMatch.size();
    if(sum==ui->tableWidget->rowCount())
    {
        for(int i=0;i<sum;i++)
        {
            quint64 sum;
            quint32 byte_s;
            quint32 byte;
            this->m_listMatch[i]->get_up_count(sum,byte_s,byte);
            QString str = QString("%1/%2/%3").arg(sum).arg(byte_s).arg(byte);
            ui->tableWidget->item(i,5)->setText(str);

            this->m_listMatch[i]->get_down_count(sum,byte_s,byte);
            str = QString("%1/%2/%3").arg(sum).arg(byte_s).arg(byte);
            ui->tableWidget->item(i,6)->setText(str);
        }
    }
}

void MainWindow::on_actionStop_triggered()
{
    int row = ui->tableWidget->currentRow();
    if(row>=0)
    {
        this->m_listMatch[row]->stopServ();
        ui->tableWidget->item(row,7)->setText("停止");
        ui->tableWidget->item(row,7)->setTextColor(QColor(237,28,36));
    }
}

void MainWindow::on_actionDelPair_triggered()
{
    int row = ui->tableWidget->currentRow();
    if(row>=0)
    {
        this->m_listMatch[row]->stopServ();
        ui->tableWidget->removeRow(row);
        this->m_listMatch.removeAt(row);
    }
}
