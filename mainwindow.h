#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


// 自定义线程类
class MyThread : public QThread {
public:
    void MoveMouseRightFromCenter(HWND hwnd,QString);
    void Capture(int x, int y, int width, int height,QString);
    void run() override;
    void setHWND(HWND hwnd);
    void setArea(int x, int y, int width, int height);

    HWND hwnd;
    int x,y,width,height;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event) override;
    void start_analisys_now(QString path);
    void mousePressEvent(QMouseEvent *event) override;
    MyThread thread_capture;
//    void Capture(int,int,int,int);
//    void MoveMouseRightFromCenter(HWND hwnd);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;

private slots:
    void slot_start_analisys();
    void slot_start_capture();
    void slot_result();
    void slot_onFinished(QNetworkReply *reply);

};
#endif // MAINWINDOW_H
