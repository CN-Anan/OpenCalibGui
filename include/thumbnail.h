#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>

namespace Ui {
class Thumbnail;
}

class Thumbnail : public QWidget
{
    Q_OBJECT

public:
    Thumbnail(QWidget *parent=nullptr);
    Thumbnail(QString filename,QString pathImgL,QString pathImgR,QWidget *parent=nullptr);
    ~Thumbnail();

private:
    Ui::Thumbnail *ui;
};

#endif // THUMBNAIL_H
