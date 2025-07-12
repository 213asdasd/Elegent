#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include<QAudioOutput>


void Widget::on_btnOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "请选择视频文件", "E:/", "视频文件(*.mp4 *.flv);;所有文件(*.*);;");

    if (fileName.isEmpty()) {
        return;
    }

    // 创建视频输出组件
    QVideoWidget *videoWidget = new QVideoWidget(ui->video_widget);
    videoWidget->resize(ui->video_widget->size());

    QMediaPlayer *player = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);



    videoWidget->show(); // 显式调用show()
    videoWidget->setGeometry(ui->video_widget->rect()); // 调整尺寸匹配父控件

    // 关键配置
    player->setAudioOutput(audioOutput);
    player->setVideoOutput(videoWidget); // 不再需要强制转换
    player->setSource(QUrl::fromLocalFile(fileName));

    player->play();
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);







}

Widget::~Widget()
{
    delete ui;
}
