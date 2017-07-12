#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "HttpFun.h"
#include <QQueue>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include "EditWidget.h"
namespace Ui {
class MainDialog;
}

const QString data_file_name = "data/request_data.txt";

//主窗口
class MainDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

    //刷新界面
    void updateRequestInfo();
private:
    //添加到所有列表
    void addItemToWidget(const QString &strUrl,const QString &title,const int &outTime,
                         const int &intervalTime,const QString& start,const QString& end);
    //删除请求item
    void deleteItem(const int &row);
    //查看日志
    void showLog(const int &row);
    //点击开始按钮
    void startRequest(int row,int col);
    //编辑请求
    void editRequest(int row,int col);
    //停止请求
    void stopRequest(int row,int col);
    //删除请求
    void delRequest(int row,int col);
    //加载数据
    void loadData();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    //请求按钮点击
    void slot_requestBtnClicked();
    //http请求结束
    void slot_requestFinished(bool bSuccess, const QString& strResult);
    //点击item
    void slot_clickItem(const int& row,const int& col);
    //定时刷新槽函数
    void slot_flushWidget();
    //暂停所有
    void on_allStopPbtn_clicked();
    //开始所有
    void on_allStartPbtn_clicked();



    void on_savepbtn_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::MainDialog *ui;
    QQueue<HttpFun*> https;
    QLabel *logLabel;
    QTimer *m_pFlushTimer;
};

#endif // MAINDIALOG_H
