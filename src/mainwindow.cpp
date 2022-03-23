#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QDir>
#include<QDateTime>
#include<QDebug>
#include<QFont>
#include<QFileInfo>
#include<thumbnail.h>

#include<iostream>
#include<stack>
#include<sstream>

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Open Calib Gui");

    // 设置图标
    QPixmap icon;
    icon.loadFromData(QByteArray::fromBase64(this->iconBase64.toLatin1()));
    this->setWindowIcon(icon);

    // 设置背景
    this->setAutoFillBackground(true);
    QPixmap background;
    background.loadFromData(QByteArray::fromBase64(this->backgroundBase64.toLatin1()));
    background = background.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(background));
    this->setPalette(palette);

//    this->calibration = std::unique_ptr<Calibration>();
    this->calibration = make_unique<Calibration>();
    this->outputBasicInformation();
    this->setPath();
    connect(ui->pushButton_load_imgs,&QPushButton::clicked,this,&MainWindow::loadImgs);// 加载图像
    connect(ui->ListWidget_imgs,&QListWidget::currentRowChanged,this,&MainWindow::showImg);//点击后显示图像
    connect(ui->pushButton_find_concers,&QPushButton::clicked,this,&MainWindow::findCorners);// 寻找角点
    connect(ui->pushButton_start_calib,&QPushButton::clicked,this,&MainWindow::startCalib); // 开始标定
    connect(ui->pushButton_save,&QPushButton::clicked,this,&MainWindow::saveResult); // 保存结果

    this->isLoadFinished = false;
    this->isCornersFound = false;
    this->isCalibrated = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPath(){
    // 通过对话框选择左图像目录文件夹
    connect(ui->pushButton_left_dir,&QPushButton::clicked,this,[=](){
        QString originalPath = this->ui->lineEdit_left_dir->text();
        QString dirPath = QFileDialog::getExistingDirectory(this, "选择左图像根目录", this->ui->lineEdit_right_dir->text());
        if(dirPath.isEmpty())
            this->ui->lineEdit_left_dir->setText(originalPath);
        else
            this->ui->lineEdit_left_dir->setText(dirPath);
    });
    // 通过对话框选择右图像目录文件夹
    connect(ui->pushButton_right_dir,&QPushButton::clicked,this,[=](){
        QString originalPath = this->ui->lineEdit_right_dir->text();
        QString dirPath = QFileDialog::getExistingDirectory(this, "选择右图像根目录", this->ui->lineEdit_left_dir->text());
        if(dirPath.isEmpty())
            this->ui->lineEdit_right_dir->setText(originalPath);
        else
            this->ui->lineEdit_right_dir->setText(dirPath);
    });
    // 路径更改后的操作
    connect(ui->lineEdit_left_dir,&QLineEdit::textChanged,this,[=](const QString &text){
        this->isLoadFinished=false;
        this->isCornersFound=false;
        this->isCalibrated=false;
        this->loadImgsFistTime=true;
        calibration->clear();
    });
    // 路径更改后的操作
    connect(ui->lineEdit_right_dir,&QLineEdit::textChanged,this,[=](const QString &text){
        this->isLoadFinished=false;
        this->isCornersFound=false;
        this->isCalibrated=false;
        this->loadImgsFistTime=true;
        calibration->clear();
    });
}

void MainWindow::loadImgs(){

    this->isLoadFinished=false;
    this->isCornersFound=false;
    this->isCalibrated=false;

    this->leftImgDir = QDir(this->ui->lineEdit_left_dir->text());
    this->rightImgDir = QDir(this->ui->lineEdit_right_dir->text());
    this->rootDir = QDir(this->leftImgDir.absoluteFilePath(".."));
    if(!leftImgDir.exists() || !rightImgDir.exists() || !rootDir.exists())
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("文件路径有错，请检查后重试");
        return;
    }

    // 获取左右图像路径
    if(loadImgsFistTime)
    {
        this->leftImgPaths = leftImgDir.entryList();
        leftImgPaths.removeFirst();
        leftImgPaths.removeFirst();

        this->rightImgPaths = rightImgDir.entryList();
        rightImgPaths.removeFirst();
        rightImgPaths.removeFirst();


        if(this->leftImgPaths.empty() || this->rightImgPaths.empty())
        {
            this->textAppendCurTime();
            this->ui->textEdit_message->append("文件路径有错，请检查后重试");
            return;
        }
        if(this->leftImgPaths.size() != this->rightImgPaths.size() )
        {
            this->textAppendCurTime();
            this->ui->textEdit_message->append("左右图像文件数量不一致，请检查后重试");
            return;
        }

        QFileInfoList leftFilesInfo = leftImgDir.entryInfoList();
        QFileInfoList rightFilesInfo = rightImgDir.entryInfoList();

        QString suffix=leftFilesInfo[2].suffix(); // 前面两个是.和..，所以从2开始
        if( suffix != "jpg" && suffix != "JPG" &&
            suffix != "png" && suffix != "PNG" &&
            suffix != "tif" && suffix != "tiff")
        {
            this->textAppendCurTime();
            this->ui->textEdit_message->append("文件不是jpg或png或tif，请检查后重试");
            return;
        }
        for(int i=2;i<leftImgPaths.size();++i) // 前面两个是.和..，所以从2开始
        {
            if( suffix !=leftFilesInfo[i].suffix() ||  suffix !=rightFilesInfo[i].suffix())
            {
                this->textAppendCurTime();
                this->ui->textEdit_message->append("文件后缀不一致，请检查后重试");
                return;
            }
        }
    }

    this->ui->pushButton_load_imgs->setEnabled(false);
    // 添加列表
    ui->ListWidget_imgs->clear();// 首先清除已经存在的列表以及标定信息
    for(int i=0;i<leftImgPaths.size();i++)
    {
        QListWidgetItem *thumbnailItem = new QListWidgetItem;
        Thumbnail *thumbail = new Thumbnail(QString::number(i, 10)+":   "+leftImgPaths[i]+"  &  "+rightImgPaths[i],leftImgDir.absoluteFilePath(leftImgPaths[i]),rightImgDir.absoluteFilePath(rightImgPaths[i]));
        thumbnailItem->setSizeHint(thumbail->sizeHint());
        ui->ListWidget_imgs->addItem(thumbnailItem);
        ui->ListWidget_imgs->setItemWidget(thumbnailItem,thumbail);
        if(loadImgsFistTime)
        {
            calibration->imagePathsL.push_back(leftImgDir.absoluteFilePath(leftImgPaths[i]).toStdString());
            calibration->imagePathsR.push_back(rightImgDir.absoluteFilePath(rightImgPaths[i]).toStdString());
        }

    }
    this->ui->pushButton_load_imgs->setEnabled(true);
    this->loadImgsFistTime=false;
    this->isLoadFinished = true;
    this->showImg(0);



    // 对话框显示信息
    this->textAppendCurTime();
    stringstream textStr;
    textStr<<"加载图像完成,共加载了"<<leftImgPaths.size()<<"张图像，请检查图像间匹配关系是否正确!";
    this->ui->textEdit_message->append(textStr.str().c_str());
}

void MainWindow::showImg(int item){
    if(isLoadFinished == false)
        return;

    if(isCornersFound)
    {
        cv::Mat cvImgL = calibration->drawChessboardCornersL(item);
        cv::Mat cvImgR = calibration->drawChessboardCornersR(item);
        cv::cvtColor(cvImgL,cvImgL,cv::COLOR_BGR2RGB);
        cv::cvtColor(cvImgR,cvImgR,cv::COLOR_BGR2RGB);
        QImage imgL=QImage(cvImgL.data,cvImgL.cols,cvImgL.rows,cvImgL.cols*cvImgL.channels(),QImage::Format_RGB888);
        QImage imgR=QImage(cvImgR.data,cvImgR.cols,cvImgR.rows,cvImgR.cols*cvImgR.channels(),QImage::Format_RGB888);
        imgL = imgL.scaled(ui->label_IMG_L->size(),Qt::KeepAspectRatio);
        imgR = imgR.scaled(ui->label_IMG_R->size(),Qt::KeepAspectRatio);
        ui->label_IMG_L->setPixmap(QPixmap::fromImage(imgL));
        ui->label_IMG_R->setPixmap(QPixmap::fromImage(imgR));

    }
    else if(isLoadFinished)
    {
        QPixmap imgL = QPixmap(leftImgDir.absoluteFilePath(leftImgPaths[item]));
        QPixmap imgR = QPixmap(rightImgDir.absoluteFilePath(rightImgPaths[item]));
        imgL = imgL.scaled(ui->label_IMG_L->size(),Qt::KeepAspectRatio);
        imgR = imgR.scaled(ui->label_IMG_R->size(),Qt::KeepAspectRatio);
        ui->label_IMG_L->setPixmap(imgL);
        ui->label_IMG_R->setPixmap(imgR);
    }
    else{
        qDebug()<<"没有图像可以显示!";
    }
}

void MainWindow::findCorners(){
    if(isCalibrated == true)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("标定已完成，无需提取角点");
        return;
    }
    if(isCornersFound == true)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("提取角点已完成，无需重复提取角点");
        return;
    }
    if(isLoadFinished==false)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请先加载图像后再提取角点");
        return;
    }
    if(ui->lineEdit_size->text().toInt()<=0)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请正确设置棋盘格尺寸!");
        return;
    }
    if(ui->lineEdit_rows->text().toInt()<=0 || ui->lineEdit_cols->text().toInt()<=0)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请正确设置棋盘格角点数目!");
        return;
    }

    calibration->setSquareSize(ui->lineEdit_size->text().toInt());
    calibration->setBoardSize(ui->lineEdit_rows->text().toInt(),ui->lineEdit_cols->text().toInt());
    calibration->setImageSize(cv::imread(calibration->imagePathsL[0]).size());

    try{
        this->ui->pushButton_find_concers->setEnabled(false);
        std::stack<int> stackNotFindCorners;
        stringstream textStr;
        textStr<<"图像 ";
        for(int i=0;i<this->leftImgPaths.size();++i)
        {
            bool findSuccess =  calibration->findCorners(i);
            if(findSuccess==false)
            {
                textStr<<i<<" ";
                stackNotFindCorners.push(i);
            }
            qDebug()<<i;
        }

        if(stackNotFindCorners.size()*2 >= leftImgPaths.size())
        {
            this->textAppendCurTime();
            this->ui->textEdit_message->append("成功提取角点的图像不超过一半，请检查配置参数！");
            return;
        }

        textStr<<"寻找角点失败！已自动删除这些图像";
        this->textAppendCurTime();
        this->ui->textEdit_message->append(textStr.str().c_str());

        // 删除无法找到角点的图像
        while(!stackNotFindCorners.empty())
        {
            int pos=stackNotFindCorners.top();
            cout<<leftImgPaths.size()<<" "
               <<rightImgPaths.size()<<" "
              <<calibration->imagePathsL.size()<<" "
             <<calibration->imagePointsSeqL.size()<<endl;

            leftImgPaths.erase(leftImgPaths.begin()+pos);
            rightImgPaths.erase(rightImgPaths.begin()+pos);
            calibration->imagePathsL.erase(calibration->imagePathsL.begin()+pos);
            calibration->imagePathsR.erase(calibration->imagePathsR.begin()+pos);
            stackNotFindCorners.pop();
        }

        this->loadImgs(); // 重新加载图像
        this->isCornersFound=true;
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请点击开始以启动标定程序");
        this->ui->pushButton_find_concers->setEnabled(true);
    }catch(...)
    {

        this->textAppendCurTime();
        this->ui->textEdit_message->append("角点提取出错，请检查参数配置！");
        this->ui->pushButton_find_concers->setEnabled(true);
    }

}

void MainWindow::startCalib(){
    if(this->isCalibrated == true)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("标定已完成，无需重复标定");
        return;
    }
    if(this->isCornersFound == false)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请先提取角点后再开始标定！");
        return;
    }
    this->ui->pushButton_start_calib->setEnabled(false);
    calibration->calibCameraL();
    calibration->calibCameraR();
    calibration->calibCameraLR();
    this->ui->pushButton_start_calib->setEnabled(true);
    if(isnan(calibration->reprojectionErrorL))
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("标定结果全为nan,标定失败!");
        return;
    }

    this->isCalibrated = true;

    stringstream textStr;
    textStr<<"标定完成"<<endl;
    textStr<<"左相机的重投影误差为:"<<calibration->reprojectionErrorL<<endl;
    textStr<<"左相机内参矩阵K2="<<calibration->K_L<<endl;
    textStr<<"左相机畸变系数D2="<<calibration->D_L<<endl;
    textStr<<endl;
    textStr<<"右相机的重投影误差为:"<<calibration->reprojectionErrorR<<endl;
    textStr<<"右相机内参矩阵K2="<<calibration->K_R<<endl;
    textStr<<"右相机畸变系数D2="<<calibration->D_R<<endl;
    textStr<<endl;
    textStr<<"立体标定重投影误差为:"<<calibration->reprojectionErrorLR<<endl;
    textStr<<"旋转矩阵R="<<calibration->R<<endl;
    textStr<<"平移向量T="<<calibration->T<<endl;
    textStr<<"其它参数请保存yml文件以查看结果！"<<endl;
    this->textAppendCurTime();
    this->ui->textEdit_message->append(textStr.str().c_str());
}


void MainWindow::saveResult()
{
    if(this->isCalibrated == false)
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("请先完成标定后再保存结果");
        return;
    }

    QString savePath = QFileDialog::getExistingDirectory(this, "选择保存文件目录",this->rootDir.absolutePath());

    if(savePath.isEmpty())
    {
        this->textAppendCurTime();
        this->ui->textEdit_message->append("保存失败！请选择正确的保存路径并点击确认");
        return;
    }

    QDir saveDir = QDir(savePath);
    cv::FileStorage ymlFileL(saveDir.absoluteFilePath("calib_l.yml").toStdString(), cv::FileStorage::WRITE);
    ymlFileL << "reprojectionError" << calibration->reprojectionErrorL;
    ymlFileL << "imageSize" << calibration->imageSize; // 图像尺寸
    ymlFileL << "K" << calibration->K_L;
    ymlFileL << "D" << calibration->D_L;
    ymlFileL << "board_width" << calibration->boardSize.width;
    ymlFileL << "board_height" << calibration->boardSize.height;
    ymlFileL << "squareSize" << calibration->squareSize;


    cv::FileStorage ymlFileR(saveDir.absoluteFilePath("calib_r.yml").toStdString(), cv::FileStorage::WRITE);
    ymlFileR << "reprojectionError" << calibration->reprojectionErrorR;
    ymlFileR << "imageSize" << calibration->imageSize; // 图像尺寸
    ymlFileR << "K" << calibration->K_R;
    ymlFileR << "D" << calibration->D_R;
    ymlFileR << "board_width" << calibration->boardSize.width;
    ymlFileR << "board_height" << calibration->boardSize.height;
    ymlFileR << "squareSize" << calibration->squareSize;

    cv::FileStorage ymlFileLR(saveDir.absoluteFilePath("calib_stereo.yml").toStdString(), cv::FileStorage::WRITE);
    ymlFileLR << "reprojectionErrorLR" << calibration->reprojectionErrorLR; // 立体标定的重投影误差
    ymlFileLR << "imageSize" << calibration->imageSize; // 图像尺寸
    ymlFileLR << "K1" << calibration->K_L;    // 相机1的内参
    ymlFileLR << "K2" << calibration->K_R;    // 相机2的内参
    ymlFileLR << "D1" << calibration->D_L;    // 相机1的畸变系数
    ymlFileLR << "D2" << calibration->D_R;    // 相机2的畸变系数
    ymlFileLR << "R" << calibration->R;      // 12相机坐标系之间的旋转矩阵
    ymlFileLR << "T" << calibration->T;      // 12相机坐标系之间的平移向量
    ymlFileLR << "E" << calibration->E;      // 本质矩阵E
    ymlFileLR << "F" << calibration->F;      // 基础矩阵F
    ymlFileLR << "R1" << calibration->R_L;    // 第一个摄像机的校正变换矩阵（旋转变换）
    ymlFileLR << "R2" << calibration->R_R;    // 第二个摄像机的校正变换矩阵（旋转变换）
    ymlFileLR << "P1" << calibration->P_L;    // 第一个摄像机在新坐标系下的投影矩阵
    ymlFileLR << "P2" << calibration->P_R;    // 第二个摄像机在想坐标系下的投影矩阵
    ymlFileLR << "Q" << calibration->Q;      // 4*4的深度差异映射矩阵
    ymlFileLR << "board_width"  << calibration->boardSize.width;     // 角点行数
    ymlFileLR << "board_height" << calibration->boardSize.height;    //角点列数
    ymlFileLR << "squareSize"   << calibration->squareSize;          //方格尺寸(mm)

    this->textAppendCurTime();
    this->ui->textEdit_message->append("保存成功!保存路径如下");
    this->ui->textEdit_message->append(saveDir.absoluteFilePath("calib_l.yml"));
    this->ui->textEdit_message->append(saveDir.absoluteFilePath("calib_r.yml"));
    this->ui->textEdit_message->append(saveDir.absoluteFilePath("calib_stereo.yml"));
}

void MainWindow::outputBasicInformation(){
    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(QColor(255,0,0));// 前景色(即字体色)设为color色
    fmt.setFontWeight(QFont::Bold);
    fmt.setFontPointSize(15);

    QTextCursor cursor = ui->textEdit_message->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit_message->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式

    stringstream textStr;
    textStr<<"欢迎使用Open Calib Gui!"<<endl;
    textStr<<"作者：Anan   软件发开时间: 2022.03.22"<<endl;
    textStr<<"Github:   https://github.com/CN-Anan"<<endl;
    textStr<<"CSDN:     https://blog.csdn.net/qq_39638989"<<endl;
    textStr<<"Bilibili: https://space.bilibili.com/27673468"<<endl;
    this->ui->textEdit_message->append(textStr.str().c_str());

    fmt.setForeground(QColor(0,0,0));// 前景色(即字体色)设为color色
    fmt.setFontWeight(QFont::Normal);
    fmt.setFontPointSize(15);
    cursor = ui->textEdit_message->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit_message->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void MainWindow::textAppendCurTime()
{
    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(QColor(255,0,0));// 前景色(即字体色)设为color色
    QTextCursor cursor = ui->textEdit_message->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit_message->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式

    this->ui->textEdit_message->append("\n"+QDateTime::currentDateTime().toString(("时间: hh:mm:ss")));

    fmt.setForeground(QColor(0,0,0));// 前景色(即字体色)设为color色
    cursor = ui->textEdit_message->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit_message->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式

}
