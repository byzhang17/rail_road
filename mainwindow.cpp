#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsOpacityEffect"
#include "top.h"
#include "qpainter.h"
#include "QRect"
#include <windows.h>
#include <qpushbutton.h>
#include <QMouseEvent>
#include <QScreen>
#include <QPixmap>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>



BOOLEAN falg_capture = false;

QString current_path = "";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    ui->setupUi(this);

    ui->tableWidget->setStyleSheet("background-color: rgba(255, 255, 255, 0);");

     setAttribute(Qt::WA_TranslucentBackground);
     setWindowFlags(Qt::FramelessWindowHint );
     manager = new QNetworkAccessManager(this);

     ui->pushButton_4->setStyleSheet("QPushButton {"
                           "border-image: url(D:/project/rail_raod/close.png);"
                           "}");

      // 设置按钮图片
      QIcon buttonIcon("D:/project/rail_raod/close.png");
      ui->pushButton_4->setIcon(buttonIcon);
      ui->pushButton_4->setIconSize(QSize(30, 30)); // 设置图标大小
     connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(close()));
     connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(slot_start_capture()));
     connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slot_start_analisys()));
     connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slot_result()));

     connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::slot_onFinished);

      ui->pushButton->setStyleSheet(
             "QPushButton {"
             "  border: 3px solid #8f8f91;"
             "  border-radius: 20px;"
             "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f0f0f3, stop: 1 #bbb);"
             "  color: #000000;"
             "  padding: 10px 10px;"
             "}"
             "QPushButton:hover {"
             "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #d9d9e1);"
             "}"
             "QPushButton:pressed {"
             "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #d7d6e2, stop: 1 #b5b8cf);"
             "}"
         );

       ui->pushButton_2->setStyleSheet(
              "QPushButton {"
              "  border: 3px solid #8f8f91;"
              "  border-radius: 20px;"
              "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f0f0f3, stop: 1 #bbb);"
              "  color: #000000;"
              "  padding: 10px 10px;"
              "}"
              "QPushButton:hover {"
              "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #d9d9e1);"
              "}"
              "QPushButton:pressed {"
              "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #d7d6e2, stop: 1 #b5b8cf);"
              "}"
          );
        ui->pushButton_3->setStyleSheet(
               "QPushButton {"
               "  border: 3px solid #8f8f91;"
               "  border-radius: 20px;"
               "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f0f0f3, stop: 1 #bbb);"
               "  color: #000000;"
               "  padding: 10px 10px;"
               "}"
               "QPushButton:hover {"
               "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #d9d9e1);"
               "}"
               "QPushButton:pressed {"
               "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #d7d6e2, stop: 1 #b5b8cf);"
               "}"
           );

}

void MainWindow::slot_result()
{

    TOP* p = new TOP();
    p->show();
}

//分析完成
void MainWindow::slot_onFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
               // 请求成功
               QByteArray responseData = reply->readAll();
               qDebug() << "Response:" << responseData;
               QMessageBox::information(nullptr, "提示", "分析完成");
           } else {
               // 请求失败
               qDebug() << "Error:" << reply->errorString();
               QMessageBox::information(nullptr, "提示", "请求失败");
           }
    reply->deleteLater();
}



//点击开始分析
void MainWindow::slot_start_analisys()
{
    // 创建一个 QFileDialog 对话框
    QString currentPath = QDir::currentPath(); //获取当前工作目录
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");



    QString selectedPath = QFileDialog::getExistingDirectory(this, "选择路径", currentPath + "/" + "imagePath");

    if (!selectedPath.isEmpty()) {
        qDebug() << "选择的路径：" << selectedPath;
        // 在这里可以使用选择的路径进行接下来的操作
        // 发送POST请求
        QUrl postUrl("http://127.0.0.1:3333/endpoint");
        QNetworkRequest postRequest(postUrl);
        QByteArray postData;
        postData.append("path=" +selectedPath);
        manager->post(postRequest, postData);
    } else {
        qDebug() << "没有选择路径";
    }
}

//点击开始分析
void MainWindow::start_analisys_now(QString path)
{
    // 发送POST请求
    QUrl postUrl("http://127.0.0.1:3333/endpoint");
    QNetworkRequest postRequest(postUrl);
    QByteArray postData;
    postData.append("path=" +path);
    manager->post(postRequest, postData);
}


//点击开始采集
void MainWindow::slot_start_capture()
{
    if(ui->pushButton_3->text() == "开始采集")
    {
        HWND hWnd = FindWindow(NULL, L"JGT-6M信息管理"); // 替换为目标窗口的标题
        if (hWnd != NULL) {
            // 向右滚动
            // 将鼠标移动到窗口中心点
            falg_capture = true;
            ui->pushButton_3->setText("停止采集");
            int centerX = 800, centerY = 400;
            SetCursorPos(centerX, centerY);

            // 模拟鼠标左键按下
            mouse_event(MOUSEEVENTF_LEFTDOWN, centerX, centerY, 0, 0);
            Sleep(100); // 延时一段时间确保左键按下生效
            // 模拟鼠标左键释放
            mouse_event(MOUSEEVENTF_LEFTUP, centerX, centerY, 0, 0);
            QPoint topLeftInParent = ui->tableWidget->mapToParent(QPoint(0, 0));

            thread_capture.setHWND(hWnd);
            thread_capture.setArea(topLeftInParent.x(),topLeftInParent.y(),ui->tableWidget->width(),ui->tableWidget->height());
            thread_capture.start();
        }
        else
        {
            QMessageBox::information(nullptr, "提示", "回放软件没有打开");
        }
    }
    else
    {
           falg_capture = false;
           ui->pushButton_3->setText("开始采集");

           QMessageBox msgBox;
               msgBox.setWindowTitle("选择对话框");
               msgBox.setText("采集完成，需要现在进行分析吗");
               msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
               msgBox.setDefaultButton(QMessageBox::No);

               int ret = msgBox.exec();

               switch (ret) {
                   case QMessageBox::Yes:
                       qDebug() << "用户选择了是";
                       start_analisys_now(current_path);
                       break;
                   case QMessageBox::No:
                       qDebug() << "用户选择了否";
                       break;
                   default:
                       qDebug() << "未识别的选择";
                       break;
               }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPoint globalPos = event->globalPos();
    // 检查点击的位置是否在透明区域或者无效点击区域
    QPoint topLeftInParent = ui->tableWidget->mapToParent(QPoint(0, 0));

    if (globalPos.x() > topLeftInParent.x() && globalPos.x() <  topLeftInParent.x() + ui->tableWidget->width() &&
            globalPos.y() > topLeftInParent.y() && globalPos.y() <  topLeftInParent.y() + ui->tableWidget->height()) {
        // 在无效点击区域，不做任何处理
        event->ignore(); // 忽略这个事件，让它继续传递到其他可能的事件接收者
    } else {
        // 在有效点击区域内，可以继续处理其他逻辑
        // 这里可以根据需要处理点击事件
        // ...
        event->accept(); // 接受并处理该事件
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
//    QPoint globalTopLeft = ui->tableWidget->mapToGlobal(tableRect.topLeft());
//    QPoint globalBottomRight = ui->tableWidget->mapToGlobal(tableRect.bottomRight());

//    int x = globalTopLeft.x();
//    int y = globalTopLeft.y();
//    int globalRightX = globalBottomRight.x(); // 屏幕上右下角的 x 坐标
//    int globalBottomY = globalBottomRight.y(); // 屏幕上右下角的 y 坐标

    QPoint topLeftInParent = ui->tableWidget->mapToParent(QPoint(0, 0));

    QRect rect(topLeftInParent.x(),topLeftInParent.y(),ui->tableWidget->width(),ui->tableWidget->height());
    int s = ui->tableWidget->width();
    int f = ui->tableWidget->height();
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // 非透明区域的颜色
    painter.setBrush(Qt::gray);
    painter.drawRoundedRect(this->rect(),5,5);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect,Qt::SolidPattern);
}

void MyThread::MoveMouseRightFromCenter(HWND hwnd,QString dirpath) {

    //SetForegroundWindow(hwnd);

    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    // 模拟按下右箭头键
    input.ki.wVk = VK_RIGHT;
    input.ki.dwFlags = 0; // 0 表示按下
    SendInput(1, &input, sizeof(INPUT));

    Sleep(100);

    // 模拟释放右箭头键
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));

    Sleep(100);

    // 模拟按下右箭头键
    input.ki.wVk = VK_RIGHT;
    input.ki.dwFlags = 0; // 0 表示按下
    SendInput(1, &input, sizeof(INPUT));

    Sleep(100);

    // 模拟释放右箭头键
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));

    Capture(x,y,width,height,dirpath);
}


void MyThread::setHWND(HWND hwnd)
{
    this->hwnd = hwnd;
}

void MyThread::Capture(int x, int y, int width, int height,QString dirPath)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap screenshot = screen->grabWindow(0, x, y, width, height);
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");

    QString fileName = dirPath + "/" + currentDateTime + ".png";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
         screenshot.save(&file, "PNG");
         file.close();
    }
}

void MyThread::setArea(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void MyThread::run()
{
    QString currentPath = QDir::currentPath(); //获取当前工作目录
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");

    QDir dir;
    // 检查文件夹是否存在，如果不存在则创建
    // 拼接目录路径
    QString  dirPath = currentPath + "/imagePath/" + currentDate + "/" + currentDateTime + "/in";
    // 检查文件夹是否存在，如果不存在则创建
    if (!dir.exists(dirPath)) {
       if (!dir.mkpath(dirPath)) {
           qDebug() << "创建目录失败：" << dirPath;
        }
    }
    while(falg_capture == true)
    {
        Sleep(1000);
        MoveMouseRightFromCenter(hwnd,dirPath);
    }
    current_path = dirPath;
}

