#include "calibration.h"

using namespace cv;
using namespace std;
Calibration::Calibration()
{

}

void Calibration::calibCameraL(){
    reprojectionErrorL = calibrateCamera(objectPointsSeq, imagePointsSeqL, imageSize, K_L, D_L, rvecsMat_L, tvecsMat_L, CV_CALIB_FIX_K3);
}
void Calibration::calibCameraR(){
    reprojectionErrorR = calibrateCamera(objectPointsSeq, imagePointsSeqR, imageSize, K_R, D_R, rvecsMat_R, tvecsMat_R, CV_CALIB_FIX_K3);
}
void Calibration::calibCameraLR(){
    reprojectionErrorLR = stereoCalibrate(objectPointsSeq, imagePointsSeqL, imagePointsSeqR,K_L,D_L,K_R,D_R,imageSize,R,T,E,F);// 立体标定
    stereoRectify(K_L, D_L, K_R, D_R, imageSize, R, T, R_L, R_R, P_L, P_R, Q); // 获取立体矫正参数
}

bool Calibration::findCorners(int idx)
{
    Mat grayImageL = imread(this->imagePathsL[idx], IMREAD_GRAYSCALE);  //灰度图用于检测角点以及角点优化
    Mat grayImageR = imread(this->imagePathsR[idx], IMREAD_GRAYSCALE);  //灰度图用于检测角点以及角点优化
    //分别对左右图像寻找角点并进行亚像素优化，只有当左右图像都找到角点才继续，否则跳过本次循环
    vector< Point2f > cornersL, cornersR;   //分别存储当前左右图像的角点
    bool isCornersLFound = findChessboardCorners(grayImageL, boardSize, cornersL,
                                                CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);//寻找角点
    bool isCornersRFound = findChessboardCorners(grayImageR, boardSize, cornersR,
                                                CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);//寻找角点
    if(isCornersLFound==false || isCornersRFound == false)
        return false;
    cornerSubPix(grayImageL, cornersL, cv::Size(5,5), cv::Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));//亚像素检测。提升角点的准确度
    cornerSubPix(grayImageR, cornersR, cv::Size(5,5), cv::Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));//亚像素检测。提升角点的准确度
    imagePointsSeqL.push_back(cornersL);    // 将当前图像的角点存入imagePointsSeq中
    imagePointsSeqR.push_back(cornersR);    // 将当前图像的角点存入imagePointsSeq中

    vector<Point3f> objectPoints;
    for (int i = 0; i < boardSize.height;i++)
        for (int j = 0; j < boardSize.width;j++)
            objectPoints.push_back(Point3f(i*squareSize.width, j*squareSize.height, 0)); // 假设标定板放在世界坐标系中z=0的平面上
    objectPointsSeq.push_back(objectPoints); // 保存标定板上角点的三维坐标

    return true;
}

Mat Calibration::drawChessboardCornersL(int idx){
    Mat colorImageL=imread(this->imagePathsL[idx], IMREAD_COLOR);
    drawChessboardCorners(colorImageL, boardSize, imagePointsSeqL[idx],true);//画图，为了显示
    return colorImageL;
}
Mat Calibration::drawChessboardCornersR(int idx){
    Mat colorImageR=imread(this->imagePathsR[idx], IMREAD_COLOR);
    drawChessboardCorners(colorImageR, boardSize, imagePointsSeqR[idx],true);//画图，为了显示
    return colorImageR;
}
