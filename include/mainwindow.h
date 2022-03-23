#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<iostream>
#include<memory>
#include <QMainWindow>
#include<QDir>
#include<calibration.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 槽函数
    void loadImgs();
    void showImg(int item);
    void findCorners();
    void startCalib();
    void saveResult();

private:
    bool isLoadFinished = false;
    bool isCornersFound = false;
    bool isCalibrated=false;
    bool loadImgsFistTime = true;

    Ui::MainWindow *ui;
    QStringList leftImgPaths;
    QStringList rightImgPaths;
    QDir rootDir, leftImgDir, rightImgDir;
    std::unique_ptr<Calibration> calibration;

    void setPath();
    void textAppendCurTime();
    void outputBasicInformation();

    const QString iconBase64 = "iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAIAAAD/gAIDAAAAAXNSR0IArs4c6QAAAARnQU1B"
                               "AACxjwv8YQUAAAAJcEhZcwAAEnQAABJ0Ad5mH3gAAAFnSURBVHhe7duhjYAwAEZhegqDYBQs"
                               "IRgS5sExBZNgSTCECRgAg2AAECxwZzrAPVfxPvdXNU/UNYzjeBxHlpIQwjAMeZ7HnYbv+7Ku"
                               "6+JKyfu+v4nZtu0n3k7/YCzAWICxAGMBxgKMBRgLMBZgLMBYgLEAYwHGAowFGAswFmAswFiA"
                               "sQBjAcYCjAUYCzAWYCzAWICxAGMBxgKMBRgLMBZgLMBYgLEAYwHGAowFGAswFmAswFiAsQBj"
                               "AcYCjAWEvu/3fY8rGeu6FkURRxrO8wzLstR1HQ/ScN93VVVxJKNpmmzbtvhVMxnXdcULpqRt"
                               "W98swFiAsQBjAcYCjAUYCzAWYCzAWICxAGMBxgKMBRgLMBZgLMBYgLEAYwHGAowFGAswFmAs"
                               "wFiAsQBjAcYCjAUYCzAWYCzAWICxAGMBxgKMBRgLMBZgLMBYgLEAYwHGAowFGAsI8zyXZRlX"
                               "Gp7nmaYphBB3Gqqq+gPg+UjSL5VcAwAAAABJRU5ErkJggg==";
    const QString backgroundBase64 = "/9j/4AAQSkZJRgABAQEAYABgAAD/4QZGRXhpZgAATU0AKgAAAAgABwESAAMAAAABAA"
                                     "EAAAEaAAUAAAABAAAAYgEbAAUAAAABAAAAagEoAAMAAAABAAIAAAExAAIAAAAcAAAA"
                                     "cgEyAAIAAAAUAAAAjodpAAQAAAABAAAAogAAAM4ADqYAAAAnEAAOpgAAACcQQWRvYm"
                                     "UgUGhvdG9zaG9wIENTMyBXaW5kb3dzADIwMTY6MDY6MjIgMTc6MjM6MjYAAAOgAQAD"
                                     "AAAAAf//AACgAgAEAAAAAQAABaCgAwAEAAAAAQAAA+gAAAAAAAAABgEDAAMAAAABAA"
                                     "YAAAEaAAUAAAABAAABHAEbAAUAAAABAAABJAEoAAMAAAABAAIAAAIBAAQAAAABAAAB"
                                     "LAICAAQAAAABAAAFEQAAAAAAAABIAAAAAQAAAEgAAAAB/9j/4AAQSkZJRgABAgAASA"
                                     "BIAAD/7QAMQWRvYmVfQ00AAv/uAA5BZG9iZQBkgAAAAAH/2wCEAAwICAgJCAwJCQwR"
                                     "CwoLERUPDAwPFRgTExUTExgRDAwMDAwMEQwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDA"
                                     "wMDAwBDQsLDQ4NEA4OEBQODg4UFA4ODg4UEQwMDAwMEREMDAwMDAwRDAwMDAwMDAwM"
                                     "DAwMDAwMDAwMDAwMDAwMDAwMDP/AABEIAG8AoAMBIgACEQEDEQH/3QAEAAr/xAE/AA"
                                     "ABBQEBAQEBAQAAAAAAAAADAAECBAUGBwgJCgsBAAEFAQEBAQEBAAAAAAAAAAEAAgME"
                                     "BQYHCAkKCxAAAQQBAwIEAgUHBggFAwwzAQACEQMEIRIxBUFRYRMicYEyBhSRobFCIy"
                                     "QVUsFiMzRygtFDByWSU/Dh8WNzNRaisoMmRJNUZEXCo3Q2F9JV4mXys4TD03Xj80Yn"
                                     "lKSFtJXE1OT0pbXF1eX1VmZ2hpamtsbW5vY3R1dnd4eXp7fH1+f3EQACAgECBAQDBA"
                                     "UGBwcGBTUBAAIRAyExEgRBUWFxIhMFMoGRFKGxQiPBUtHwMyRi4XKCkkNTFWNzNPEl"
                                     "BhaisoMHJjXC0kSTVKMXZEVVNnRl4vKzhMPTdePzRpSkhbSVxNTk9KW1xdXl9VZmdo"
                                     "aWprbG1ub2JzdHV2d3h5ent8f/2gAMAwEAAhEDEQA/ANiE8KUJwFJbd4WMJ4UgE8JW"
                                     "rhYgKQCcBOAlauFaE8KQCeErRTGE8J4TwhaKWhKFKEoStVMYShShKErVTGEoTwlCVo"
                                     "pjCYhThNCNqphCaESE0JWin//Q39qcNRNqfYm8TpUj2p9qIGJ9qXEqke1PtRNifYjx"
                                     "IpHtT7UTan2JcSKR7UtqJtT7UuJFI4S2omxLalxKpHtShE2pbUuJFItqUIu1LajaqR"
                                     "QltRNqW1K0Ui2pbUXaltS4kU//0er2JwxWBWn9NVuN0uJBsT7Ef00/ppca3iQbEtis"
                                     "emlsR41cSDYn2I+xLYjxI4kGxLYj7EtiXEi0GxLYjbEtqXEq0O1NsR9qbajxKtDtS2"
                                     "o21LalxKtDsS2I21NtR4lWi2pbEXan2pcSrf/S7wMUgxTAUgFn22jJGGJ9iKAlCVre"
                                     "JFsS2IsJoRsp4ke1NtRITJwKuJHtS2qcJQjabR7UoU4TIpthtS2qaZJVsYS2qSSKrY"
                                     "bUtqmmRtVsdqUKSSWqrf/T9BDk4cq4sUg9U+BnbAcn3IAen3pcCEsptyFvTb0RBSXc"
                                     "m3IW9NvREVJdybch7029O4VWllNKFvTb0eBVppTbkHelvR4EcSbcluQd6belwq4025"
                                     "Lcg+om9RHhRxp9ybcg+om9RHgV7j//1OyD1IPVYOUg5N4Ee6GyHp96rhyfem8CfdCf"
                                     "em3oW5LcjwJ90JdybcoSlKPAr3Az3JtyjKaUeFHuMtybcoymJR4VpyMi5MXKBKYlHh"
                                     "WnIz3pt6GXJtyPCtORJvS3oRcolyPCtOVN6ib1EHcm3JcKPdf/1emDlIOQxKdT8Llj"
                                     "Mk3JeohklQLneBQ4F3vNj1E4eq253gVIOd4FDhSMzZD0+9Vw53gU4c7wKXCn3k+9RL"
                                     "0Iud4FRLneB+5HhQcybemL0Aud4H7lHc7wP3I8C052xvUd6Dud4H7ktzvApcKPeSly"
                                     "iXoRcfAqJc7wKPCtOZLuTFyFuPgUpPgUeFb7yQuTbkOT4FKT5pcKPdf/2QD/2wBDAA"
                                     "MCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMU"
                                     "FRUVDA8XGBYUGBIUFRT/2wBDAQMEBAUEBQkFBQkUDQsNFBQUFBQUFBQUFBQUFBQUFB"
                                     "QUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBT/wAARCABkAGQDASIAAhEB"
                                     "AxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAw"
                                     "UFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcY"
                                     "GRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhI"
                                     "WGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ"
                                     "2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBg"
                                     "cICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKR"
                                     "obHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWV"
                                     "pjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3"
                                     "uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAx"
                                     "EAPwD28RmpljqwsB9KkW3PtXZ7RH2zpMgWMVKseasLD04qRYTR7REOkyusXvUoj9RU"
                                     "yw1IsRo9oR7MgEdHlVZ8k0vl+1L2gnArbT60bTVnyaTyzR7RE8jK+DSFfarHl+1BiF"
                                     "NSRPKVfLoqx5NFVzeZPINW29sVItqf8itdbM+lSLZN6V4ftz6BxRkrbH0qRbb2zWst"
                                     "nT1sT6VXtjNpGStr7U9bXjpWr9h9qetn7UvbGdjJFuR0FL5J71r/AGT/AGaPsvtT9s"
                                     "TymP5Jo8itZrQHtSfYx6VXtkTyoyPs49qX7P6DFan2MelJ9l9qpViXBGX9nPpRWn9l"
                                     "HpRV+0I5Ea4072p/9n10g0/2p32D2r5NYjzH9YRzg08DtUi2P+zW99j20fZP9mtVXZ"
                                     "PtzC+wn0FH2H2rc+ye1N+y1ftmT7W5hmzx2oNoPStr7N2xSG3p+2ZftDE+x+gpDZn0"
                                     "rZa39qT7OPSrVYftDF+x+1N+x1ufZx6YpDbitFWDnMT7GKK2vs/tRV+2DnOoNuOwoM"
                                     "PtUnnL60xphXzMYSPBU2xjQdTgVGYx6U8z1G0w9a6YxkXzsY8YqPy6c0oppmrdRZaq"
                                     "DWjNM8qlaYetMaYetXysr2onl/jTWj9qPO96b5wrRQY/bB5ftRs9qPtA+n40n2j3q1"
                                     "TkHthdn0opnnUVfs2L26LH273NMa+96wftnvSG6Prj8apYc4FVibbXx9aja+GPvVjG"
                                     "696T7R71vHDoPaxNZrz3pv2z3rJaf3ppuRW6oeRm6y7mq18OmaY1771ktc1E10RWio"
                                     "GUsQjXN96Gmm+96xGuj61E10fWtlhzCWKRvG/96adQH96uea896Y18R3rRYdGDxaOi"
                                     "/tL/AGv1ormvtx/yaKv6ujP655lr7Z70q3Qbvmud/tD3p8eoD1rv+rI+ZjmvmdGtx7"
                                     "07zvesKPUB61Mt8PWj6uaf2mu5sed70xph61m/bhj/AOvUTXw9cVaoEvM13NRpveoX"
                                     "mHrWU+oj+9UD6iPWrWHMJZmu5qvP6frUDzZ5rNbUM+9Ma9461oqJyyzJPqXpJ9veoW"
                                     "ufes6S9GPvVWa+HrWio+RyyzJdzVN0fWisc3lFX7B9jD+0l3I5Jn3HmkS4f1oorc8G"
                                     "7uWo7hz3qwkz+tFFSdCbHec/rUEs77Sc0UVUTObZTkuJOOahkuHH8VFFdCSOScn3Gi"
                                     "d/Wn+cxHWiinZGPM+5DNK23rVQ3D+tFFaI5pyfcPNdv4jRRRVmHM+5/9k=";
};
#endif // MAINWINDOW_H
