#ifndef CALIBRATION_H
#define CALIBRATION_H

#include<opencv2/opencv.hpp>
#include<string>
class Calibration
{
private:

public:
    cv::Size boardSize;                                       // 标定板上每行、列的角点数（格子数-1）
    cv::Size squareSize;                                          // 实际测量得到的标定板上每个棋盘格的物理尺寸，单位mm
    cv::Size imageSize;                                                     // 图像的尺寸
    std::vector<cv::Mat> rvecsMat_L;                                              // 存放左图像的旋转向量，每一副图像的旋转向量为一个mat
    std::vector<cv::Mat> tvecsMat_L;                                              // 存放左图像的平移向量，每一副图像的平移向量为一个mat
    std::vector<cv::Mat> rvecsMat_R;                                              // 存放右图像的旋转向量，每一副图像的旋转向量为一个mat
    std::vector<cv::Mat> tvecsMat_R;                                              // 存放右图像的平移向量，每一副图像的平移向量为一个mat
    cv::Mat K_L,D_L; //左相机内参矩阵和畸变矩阵
    cv::Mat K_R,D_R; //右相机内参矩阵和畸变矩阵
    cv::Mat R,E,F; // 定义旋转矩阵R，本质矩阵E，基础矩阵F
    cv::Vec3d T; // 定义旋转矩阵平移向量
    double  reprojectionErrorL,reprojectionErrorR,reprojectionErrorLR;
    cv::Mat R_L, R_R, P_L, P_R, Q;

public:
    Calibration();
    std::vector<std::string> imagePathsL, imagePathsR;                            // 存放左右图像的路径
    std::vector<std::vector<cv::Point3f>> objectPointsSeq;                            // 保存标定板上角点的三维坐标
    std::vector<std::vector<cv::Point2f>> imagePointsSeqL, imagePointsSeqR;           // 分别保存左和右图像检测到的所有角点

    void setBoardSize(int row,int col){
        boardSize = cv::Size(row,col);
    }
    void setSquareSize(int mmVal){
        squareSize = cv::Size(mmVal,mmVal);
    }
    void setImageSize(cv::Size size){
        imageSize = size;
    }
    void clear()
    {
        boardSize=cv::Size(0,0);
        squareSize=cv::Size(0,0);
        imageSize=cv::Size(0,0);
        imagePathsL.clear();
        imagePathsR.clear();
        objectPointsSeq.clear();
        imagePointsSeqL.clear();
        imagePointsSeqR.clear();
    }
    bool findCorners(int idx);
    void calibCameraL();
    void calibCameraR();
    void calibCameraLR();
    cv::Mat drawChessboardCornersL(int idx);
    cv::Mat drawChessboardCornersR(int idx);
    friend std::ostream & operator<<(std::ostream &out, Calibration &calibration){
        out << "boardSize="<<calibration.boardSize
        <<"squareSize="<<calibration.squareSize<<"mm"
        <<"imageSize="<<calibration.imageSize<<std::endl;
        for(int i=0;i<calibration.imagePathsL.size();++i)
            out<<calibration.imagePathsL[i]<<std::endl;
        return out;
    }
};

#endif // CALIBRATION_H
