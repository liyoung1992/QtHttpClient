#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "HttpFun.h"
#include <QQueue>
#include <QDateTime>
#include <QLabel>
namespace Ui {
class MainDialog;
}

//主窗口
class MainDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    //添加到所有列表
    void addItemToWidget(const QString &strUrl,const QString &title,const int &outTime,
                         const int &intervalTime,const QString& start,const QString& end);
    //删除请求item
    void deleteItem(const int &row);
    //查看日志
    void showLog(const int &row);

private slots:
    void slot_requestBtnClicked();//请求按钮点击
    void slot_requestFinished(bool bSuccess, const QString& strResult); //http请求结束
    //点击item
    void slot_clickItem(const int& row,const int& col);
//    void on_pushButton_clicked();


    void on_allStopPbtn_clicked();

    void on_allStartPbtn_clicked();

//    void on_pushButton_2_clicked();

private:
    Ui::MainDialog *ui;
    QQueue<HttpFun*> https;
    QLabel *logLabel;
};

#endif // MAINDIALOG_H
