#include "top.h"
#include "ui_top.h"
#include "QDir"
#include "QDateTime"
#include <QFileDialog>
#include <QDebug>

QString select_path = "";

TOP::TOP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOP)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slot_select_rote()));
    resize(2534, 1393);
    move(13, 99);
    // 连接点击信号到槽函数
    //connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *item)), this, SLOT(close()));

    connect(ui->listWidget,&QListWidget::itemClicked,this,[=](QListWidgetItem *item)
        {
            onItemClicked(item);
        });
}



TOP::~TOP()
{
    delete ui;
}

void TOP::slot_select_rote()
{
    QString currentPath = QDir::currentPath(); //获取当前工作目录
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");



    QString selectedPath = QFileDialog::getExistingDirectory(this, "选择路径", currentPath + "/" + "imagePath");

    if (!selectedPath.isEmpty()) {
        qDebug() << "选择的路径：" << selectedPath;
        select_path = selectedPath;
        // 在这里可以使用选择的路径进行接下来的操作
        QDir directory(selectedPath);
            QFileInfoList fileList = directory.entryInfoList(QDir::Files);

            // 添加文件到列表中
            for (const QFileInfo &fileInfo : fileList) {
                new QListWidgetItem(fileInfo.fileName(), ui->listWidget);
            }

    } else {
        qDebug() << "没有选择路径";
    }
}

void TOP::onItemClicked(QListWidgetItem* item) {
    QString path = select_path + "/" + item->text();

    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        ui->label->setPixmap(pixmap.scaled( ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->label->setText("Failed to load image");
    }
}
