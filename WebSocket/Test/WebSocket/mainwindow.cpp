#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _pdataRecvWS = new QWebSocket();
    _ptimer = new QTimer(this);
    connect(_pdataRecvWS,SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::AutoConnection);
    connect(_pdataRecvWS,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextReceived(QString)),Qt::AutoConnection);
    connect(_pdataRecvWS,SIGNAL(connected()),this,SLOT(onConnected()),Qt::AutoConnection);
    connect(_ptimer,SIGNAL(timeout()),this,SLOT(reconnect()),Qt::AutoConnection);
    _pdataRecvWS->open(QUrl("ws://localhost:26001"));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    _pdataRecvWS->close();
   _ptimer->stop();
    qDebug() << "websocket is close";
    delete ui;
}

//断开连接会触发这个槽函数
void MainWindow::onDisconnected()
{
    _ptimer->start(2000);
    qDebug() << "websocket is disconnected";
}
//连接成功会触发这个槽函数
void MainWindow::onConnected()
{
    _ptimer->stop();
    qDebug() << "connect successful";
}
//收到服务发来的消息会触发这个槽函数
void MainWindow::onTextReceived(QString msg)
{
    ui->textEdit_2->setText(msg);
    QByteArray byteArryay;
    QJsonObject jsonObject;
    QJsonDocument document;
    document.setObject(jsonObject);
    byteArryay = QByteArray(document.toJson());

    QJsonObject jsonResp = QJsonDocument::fromJson(msg.toUtf8()).object();

    QString strMode = jsonResp.value("Mode").toString();
    QString strOpType = jsonResp.value("OpType").toString();
    qDebug() << strMode << strOpType;

    //qDebug() << "textReceiveString" + msg;
}
// 断开连接会启动定时器，触发这个槽函数重新连接
void MainWindow::reconnect()
{
    qDebug() << "websocket reconnected";
    _pdataRecvWS->abort();
    _pdataRecvWS->open(QUrl("ws://localhost:26001"));
}

void MainWindow::on_pushButton_clicked()
{
    QString strSend = ui->textEdit->toPlainText();
    _pdataRecvWS->sendTextMessage(strSend);
}
