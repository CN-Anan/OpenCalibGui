#include "thumbnail.h"
#include "ui_thumbnail.h"
#include<QDebug>
Thumbnail::Thumbnail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Thumbnail)
{
    ui->setupUi(this);
}
Thumbnail::Thumbnail(QString filename,QString pathImgL,QString pathImgR, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Thumbnail)
{
    ui->setupUi(this);

    QPixmap imgL = QPixmap(pathImgL);
    imgL = imgL.scaled(ui->label_img_left->size(),Qt::KeepAspectRatio);

    QPixmap imgR = QPixmap(pathImgR);
    imgR = imgR.scaled(ui->label_img_right->size(),Qt::KeepAspectRatio);

    ui->label_img_left->setPixmap(imgL);
    ui->label_img_right->setPixmap(imgR);
    ui->label_filename->setText(filename);
}

Thumbnail::~Thumbnail()
{
    delete ui;
}
