#ifndef HTTPFUN_H
#define HTTPFUN_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QSslConfiguration>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

//http请求类
class HttpFun : public QObject
{
    Q_OBJECT
public:
    explicit HttpFun(QObject *parent = 0);

    void init(const QString& strUrl,const int& outTime,const int& intervalTimer,
               const QString& startTimer,const QString& endTimer,const QString& title);
    //根据url发起http请求
    void sendRequest(const QString& strUrl,const int& outTime,const int& intervalTimer,
                     const QString& startTimer,const QString& endTimer);
    //点击暂停，关闭时间间隔的定时器
    void closeIntervalTimer();
    //点击开始，时间间隔定时器
    void startIntervalTimer();
    //判断时间
    int judgeTime(const QString& start,const QString& end);
    void log(const QString& log_str);
    QString  getLogFileName();
    QString  getRequestState();
    void setRequestState(const QString &state);
    QString  getRequestTimes();
signals:
    //http请求结束
    void signal_requestFinished(bool bSuccess,const QString& strResult);
public slots:
    //http请求结束
    void slot_requestFinished();
    //请求超时
    void slot_requestTimeout();
    //时间间隔请求处理函数
    void slot_intervalTimeRequest();

private:
    //网络管理类
    QNetworkAccessManager* m_pNetworkManager;
     //封装请求返回信息
    QNetworkReply* m_pNetworkReply;
    //请求超时计时器
    QTimer* m_pOutTimer;
    //时间间隔计时器
    QTimer* m_pIntervalTimer;
   //间隔时间
    int m_pIntervalTime;
    //超时时间
    int m_pOutTime;
    //记录当前请求的url
    QString m_strUrl;
    //开始时间
    QString m_pStartTime;
    //结束时间
    QString m_pEndTime;
    //请求标题
    QString m_pTitle;
    //log文件名
    QString m_pLogFileName;
    //访问次数
    long m_pRequestTimes;
    //访问状态
    QString m_pRequestState;

};

#endif // HTTPFUN_H
