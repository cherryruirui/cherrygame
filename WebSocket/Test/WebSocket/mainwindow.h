#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void onDisconnected();

    void onConnected();

    void onTextReceived(QString msg);

    void reconnect();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QWebSocket *_pdataRecvWS;
    QTimer *_ptimer;
};

#endif // MAINWINDOW_H
