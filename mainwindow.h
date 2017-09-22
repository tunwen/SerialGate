#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "c_portmatch.h"
#include <QTimer>
/*
 * modebus 串口转网口网关
 * */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNewPair_triggered();

    void on_actionStart_triggered();

    void slot_timer();
    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
    void initeTable();
    QTimer m_timer;

    QList<C_PortMatch*> m_listMatch;
};

#endif // MAINWINDOW_H
