#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleListen();           // 按下「接收」按鈕進入監聽狀態
    void handleNewConnection();    // 處理客戶端連線
    void handleClientDisconnected();// 處理客戶端斷線

private:
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;

    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *receiveButton;    // 「接收」按鈕
    QPushButton *quitButton;       // 「退出」按鈕
    QDialogButtonBox *buttonBox;
};

#endif // MAINWINDOW_H