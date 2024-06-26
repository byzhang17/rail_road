#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <windows.h>
#include <QSystemTrayIcon>
#include <QMenu>

//配置软件样式列表
void OnConfigurationStyleSheet()
{
    QFile file("D:/project/rail_raod/blue_ew.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream fileText(&file);
        QString styleSheet = fileText.readAll();
        if (!styleSheet.isEmpty())
        {
            qApp->setStyleSheet(styleSheet);
        }
    }
    file.close();
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.showMaximized();
    w.show();

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon("D:/project/rail_raod/图标.ico"), &a);

        // 创建托盘菜单
        QMenu *trayMenu = new QMenu();
        QAction *restoreAction = trayMenu->addAction("还原");
        QAction *quitAction = trayMenu->addAction("退出");

        // 将菜单与托盘图标关联
        trayIcon->setContextMenu(trayMenu);

        // 显示托盘图标
        trayIcon->show();

        // 连接菜单动作
        QObject::connect(restoreAction, &QAction::triggered, &a, [&]() {
            // 在此处添加将窗口从托盘还原的代码
            w.show();
        });
        QObject::connect(quitAction, &QAction::triggered, &a, &QApplication::quit);

    return a.exec();
}

