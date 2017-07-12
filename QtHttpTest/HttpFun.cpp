#include "HttpFun.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QDir>

const int nHTTP_TIME = 10000; //10秒

HttpFun::HttpFun(QObject *parent) :
    QObject(parent)
{
    m_pNetworkManager = new QNetworkAccessManager(this);
    m_pNetworkReply = NULL;
    m_pIntervalTimer = NULL;
    m_pIntervalTime = 10000;
    m_pOutTime = 10000;
    m_pStartTime = "00:00:00";
    m_pEndTime = "23:39:59";
    m_pTitle = "标题";
    m_pRequestTimes = 0;
    m_pRequestState = "未开始";
    m_pisLogOpen = false;
    m_pIsStart = false;
    m_pOutTimer = new QTimer;
    connect(m_pOutTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));
}
//初始化参数
void HttpFun::init(const QString &strUrl, const int &outTime, const int &intervalTime,
                   const QString &startTimer, const QString &endTimer, const QString &title)
{
    m_strUrl = strUrl;
    m_pIntervalTime = intervalTime*1000;
    m_pOutTime = outTime*1000;
    m_pStartTime = startTimer;
    m_pEndTime = endTimer;
    m_pTitle = title;
}



//发起请求
void HttpFun::sendRequest()
{
    if(judgeTime(m_pStartTime,m_pEndTime) > 0){
        QNetworkRequest netRequest;
        netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netRequest.setUrl(QUrl(m_strUrl)); //地址信息
        if(m_strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
        {
            QSslConfiguration sslConfig;
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            sslConfig.setProtocol(QSsl::TlsV1_1);
            netRequest.setSslConfiguration(sslConfig);
        }
        //    QString strBody; //http body部分，可封装参数信息
        //    QByteArray contentByteArray = strBody.toLatin1();//转成二进制
        //    m_pNetworkReply = m_pNetworkManager->post(netRequest,contentByteArray);//发起post请求

        m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
        connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号

        //超时信号
        //    QTimer::singleShot(m_pOutTime,this, SLOT(slot_requestTimeout()));
        m_pOutTimer->start(m_pOutTime);
    }

    //时间间隔信号槽
    m_pIntervalTimer = new QTimer;
    connect(m_pIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
    m_pIntervalTimer->start(m_pIntervalTime);
}
//关闭时间间隔的计时器
void HttpFun::closeIntervalTimer()
{
    if(!m_pIntervalTimer){
        m_pIntervalTimer = new QTimer;
        connect(m_pIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
    }
    m_pRequestState = "暂停中";
    m_pIntervalTimer->stop();
    m_pOutTimer->stop();
}
//开始间隔时间的计时器
void HttpFun::startIntervalTimer()
{
    if(!m_pIntervalTimer){
        m_pIntervalTimer = new QTimer;
        connect(m_pIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
    }
    m_pRequestState = "正在请求";
    m_pIntervalTimer->start(m_pIntervalTime);
}

//判断时间，在时间内返回1，否则返回-1
int HttpFun::judgeTime(const QString &start, const QString &end)
{
    //获取系统现在的时间
    QTime currTime = QTime::currentTime();
    QTime startTime = QTime::fromString(start);
    QTime endTime = QTime::fromString(end);


    int startTimeStamp = (startTime.hour())*3600 + (startTime.minute())*60 + startTime.second();
    int currTimeStamp = currTime.hour()*3600 + currTime.minute()*60 + currTime.second();
    int endTimeStamp = endTime.hour()*3600 + endTime.minute()*60 + endTime.second();

    if(startTimeStamp > endTimeStamp){
        if(currTimeStamp >= startTimeStamp)
            return 1;
        if(currTimeStamp <= endTimeStamp)
            return 1;
    }

    if((startTimeStamp <= currTimeStamp) && (currTimeStamp <= endTimeStamp)){
        return 1;
    }
    return -1;
}

void HttpFun::log(const QString &log_str)
{
    if(!m_pisLogOpen)
        return ;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString currTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    QString log_result = "请求URL:" + m_strUrl + " ; " + "请求时间:" + currTime +" ; " + "请求结果:" + log_str;

    QDir *TEST = new QDir;
    bool exist = TEST->exists("log");
    if(!exist)
        bool ok = TEST->mkdir("log");

    QString fileName = "log/" + m_pTitle +"_" + time.toString("yyyy-MM-dd") + ".txt";



    m_pLogFileName = fileName;
    QString str = log_result;
    QFile file(fileName);


    QFileInfo fileinfo(fileName);
    qDebug() << QString::number(fileinfo.size());
    if (fileinfo.size() > 1024*1024){
        QTime currTime = QTime::currentTime();
        int currTimeStamp = currTime.hour()*3600 + currTime.minute()*60 + currTime.second();

        QString newfileName = "log/" + m_pTitle +"_" + time.toString("yyyy-MM-dd")+
                "_"+QString::number(currTimeStamp) + ".txt";
        file.rename(newfileName);
    }

    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"can't open";
    }

    QTextStream in(&file);
    in<<str<<"\n";
    file.close();
}

QString HttpFun::getLogFileName()
{
    return m_pLogFileName;
}

QString HttpFun::getRequestState()
{
    if(judgeTime(m_pStartTime,m_pEndTime) > 0) {
        return m_pRequestState;
    } else {
        return "不在访问时间内";
    }
}

void HttpFun::setRequestState(const QString &state)
{
    m_pRequestState = state;
}

QString HttpFun::getRequestTimes()
{
    return QString::number(m_pRequestTimes);
}

QString HttpFun::getRequestUrl()
{
    return m_strUrl;
}

QString HttpFun::getRequestTitle()
{
    return m_pTitle;
}

int HttpFun::getIntervalTime()
{
    return m_pIntervalTime/1000;
}

int HttpFun::getTimeOut()
{
    return m_pOutTime/1000;
}

QString HttpFun::getStartTime()
{
    return m_pStartTime;
}

QString HttpFun::getEndTime()
{
    return m_pEndTime;
}

void HttpFun::setLogSwitch(const bool &on_off)
{
    m_pisLogOpen = on_off;
}

//请求结束
void HttpFun::slot_requestFinished()
{
    m_pOutTimer->stop();
    QByteArray resultContent = m_pNetworkReply->readAll();
    QTextCodec* pCodec = QTextCodec::codecForName("UTF-8");
    QString strResult = pCodec->toUnicode(resultContent);
    int nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if(nHttpCode == 200)//成功
    {
        log("请求成功！");
        qDebug() << "接口："<< m_pNetworkReply->url() <<"请求成功！";
        //请求成功
        emit signal_requestFinished(true,strResult);
    }
    else
    {
        //请求失败
        log("请求失败！");
        emit signal_requestFinished(false,strResult);
    }
    m_pNetworkReply->deleteLater();
}

//请求超时
void HttpFun::slot_requestTimeout()
{
    //请求失败
    emit signal_requestFinished(false,"timeout");
    m_pNetworkReply->deleteLater();
}

//时间间隔请求处理函数
void HttpFun::slot_intervalTimeRequest(){
    if(judgeTime(m_pStartTime,m_pEndTime) > 0){
        QNetworkRequest netRequest;
        netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netRequest.setUrl(QUrl(m_strUrl)); //地址信息
        //https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
        if(m_strUrl.toLower().startsWith("https"))
        {
            QSslConfiguration sslConfig;
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            sslConfig.setProtocol(QSsl::TlsV1_1);
            netRequest.setSslConfiguration(sslConfig);
        }
        m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
        connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号

        //超时信号
        //    QTimer::singleShot(m_pOutTime,this, SLOT(slot_requestTimeout()));
        m_pOutTimer->start(m_pOutTime);
        m_pRequestTimes += 1;
    }
}

