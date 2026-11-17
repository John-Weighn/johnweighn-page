#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcpServer(nullptr), clientSocket(nullptr)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 建立 UI 元件
    clientProgressBar = new QProgressBar(this);
    clientStatusLabel = new QLabel(tr("伺服器端就緒"), this);
    receiveButton = new QPushButton(tr("接收"), this);
    quitButton = new QPushButton(tr("退出"), this);

    // 按鈕對話框排版
    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(receiveButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    // 主版面配置
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(clientProgressBar);
    mainLayout->addWidget(clientStatusLabel);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);

    setWindowTitle(tr("接收檔案"));
    resize(350, 180);

    // 初始化 TCP 伺服器
    tcpServer = new QTcpServer(this);

    // 使用現代 Qt 函數指標連線 (Connect)
    connect(receiveButton, &QPushButton::clicked, this, &MainWindow::toggleListen);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);
}

MainWindow::~MainWindow()
{
    if (tcpServer->isListening()) {
        tcpServer->close();
    }
}

void MainWindow::toggleListen()
{
    if (!tcpServer->isListening()) {
        quint16 port = 16689; // 設定監聽連接埠
        if (tcpServer->listen(QHostAddress::Any, port)) {
            // 符合投影片畫面：點擊後狀態轉為「監聽中...」，按鈕可依需求調整狀態
            receiveButton->setEnabled(false); 
            clientStatusLabel->setText(tr("監聽中..."));
        } else {
            QMessageBox::critical(this, tr("錯誤"), tr("無法啟動監聽：%1").arg(tcpServer->errorString()));
        }
    }
}

void MainWindow::handleNewConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    clientStatusLabel->setText(tr("已成功連接客戶端"));
    connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::handleClientDisconnected);
}

void MainWindow::handleClientDisconnected()
{
    clientStatusLabel->setText(tr("監聽中..."));
    clientSocket->deleteLater();
    clientSocket = nullptr;
}