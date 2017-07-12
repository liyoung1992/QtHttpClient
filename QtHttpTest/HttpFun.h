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
    void sendRequest();
    //点击暂停，关闭时间间隔的定时器
    void closeIntervalTimer();
    //点击开始，时间间隔定时器
    void startIntervalTimer();
    //判断时间
    int judgeTime(const QString& start,const QString& end);
    //写log
    void log(const QString& log_str);
    //获取log文件名
    QString  getLogFileName();
    //获取请求状态
    QString  getRequestState();
    //设置请求状态
    void setRequestState(const QString &state);
    //获取请求次数
    QString  getRequestTimes();
    //获取请求url
    QString getRequestUrl();
    //获取请求标题
    QString getRequestTitle();
    //获取访问时间间隔
    int getIntervalTime();
    //获取超时时间
    int getTimeOut();
    //获取开始时间
    QString getStartTime();
    //获取结束时间
    QString getEndTime();
    //设置log开关
    void setLogSwitch(const bool &on_off);


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
    int64_t m_pRequestTimes;
    //访问状态
    QString m_pRequestState;
    //记录是否开始
    bool m_pIsStart;
    //是否开启日志
    bool m_pisLogOpen;

};

#endif // HTTPFUN_H
