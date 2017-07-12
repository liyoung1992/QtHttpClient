#include "MainDialog.h"
#include "ui_MainDialog.h"
#include "HttpFun.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>
#include <QString>

#include <iostream>
#include "json/src/json.hpp"
using namespace std;
using json = nlohmann::json;

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    m_pFlushTimer = NULL;
    logLabel = new QLabel();
    //请求按钮点击信号
    connect(ui->requestBtn,SIGNAL(clicked()),this,SLOT(slot_requestBtnClicked()));
    //点击item
    connect(ui->tableWidget,SIGNAL(cellClicked(int, int)),this,SLOT(slot_clickItem(int,int)));
    loadData();
}

MainDialog::~MainDialog()
{
    delete ui;
}

//刷新界面详情
void MainDialog::updateRequestInfo()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
        ui->tableWidget->item(i,0)->setText(https[i]->getRequestTitle());
        ui->tableWidget->item(i,1)->setText(https[i]->getRequestUrl());
        ui->tableWidget->item(i,2)->setText(QString::number(https[i]->getIntervalTime()));
        ui->tableWidget->item(i,3)->setText(QString::number(https[i]->getTimeOut()));
        ui->tableWidget->item(i,4)->setText(https[i]->getStartTime() + "-" + https[i]->getEndTime());
        ui->tableWidget->item(i,5)->setText(https[i]->getRequestState());
        ui->tableWidget->item(i,6)->setText(https[i]->getRequestTimes());
    }
}


//添加到所有列表
void MainDialog::addItemToWidget(const QString &strUrl, const QString &title, const int &outTime,
                                 const int &intervalTime, const QString &start, const QString &end)
{
    int rowCount = ui->tableWidget->rowCount();
    int colCount = ui->tableWidget->columnCount();
    qDebug() << rowCount;
    qDebug() << colCount;
    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(title));
    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(strUrl));
    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(intervalTime)));
    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(QString::number(outTime)));
    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(start + "-" + end));
    //    QString status =  https[rowCount]->getRequestState();
    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem("未开始"));
    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem("0"));
    ui->tableWidget->setItem(rowCount,7,new QTableWidgetItem(QIcon(QPixmap(":/file/images/player.png")),tr("开始")));
    ui->tableWidget->setItem(rowCount,8,new QTableWidgetItem(QIcon(QPixmap(":/file/images/edit.jpg")),tr("编辑")));
    ui->tableWidget->setItem(rowCount,9,new QTableWidgetItem(QIcon(QPixmap(":/file/images/delete.png")),tr("删除")));
    ui->tableWidget->setItem(rowCount,10,new QTableWidgetItem(QIcon(QPixmap(":/file/images/show.jpg")),tr("查看日志")));
    //设置文字居中
    ui->tableWidget->item(rowCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,10)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    //设置操作项目不可编辑
    ui->tableWidget->item(rowCount,0)->setFlags(ui->tableWidget->item(rowCount,0)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,1)->setFlags(ui->    tableWidget->item(rowCount,1)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,2)->setFlags(ui->tableWidget->item(rowCount,2)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,3)->setFlags(ui->tableWidget->item(rowCount,3)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,4)->setFlags(ui->tableWidget->item(rowCount,4)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,5)->setFlags(ui->tableWidget->item(rowCount,5)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,6)->setFlags(ui->tableWidget->item(rowCount,6)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,7)->setFlags(ui->tableWidget->item(rowCount,7)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,8)->setFlags(ui->tableWidget->item(rowCount,8)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,9)->setFlags(ui->tableWidget->item(rowCount,9)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,10)->setFlags(ui->tableWidget->item(rowCount,9)->flags() & ~Qt::ItemIsEditable);

}

//删除item，释放内存
void MainDialog::deleteItem(const int &row)
{
    delete https[row];
    https.removeAt(row);
    ui->tableWidget->removeRow(row);
}

//显示log
void MainDialog::showLog(const int &row)
{

    QFile file(https[row]->getLogFileName());
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream toText(&file);
        //设置文件流编码方式
        toText.setCodec("GBK");
        logLabel->setText((toText.readAll()));
        logLabel->show();
    }
}
//开始请求
void MainDialog::startRequest(int row,int col)
{
    https[row]->sendRequest();
    ui->tableWidget->item(row,5)->setText("正在请求");
    ui->tableWidget->item(row,col)->setText("暂停");
    ui->tableWidget->item(row,col)->setIcon(QIcon(QPixmap(":/file/images/pause.png")));
    https[row]->startIntervalTimer();

    slot_flushWidget();

    //定时刷新各请求状态
    if (!m_pFlushTimer){
        m_pFlushTimer = new QTimer();
        connect(m_pFlushTimer,SIGNAL(timeout()),this,SLOT(slot_flushWidget()));
        m_pFlushTimer->start(5000);
    }
}

//编辑请求
void MainDialog::editRequest(int row,int col)
{
    EditWidget *edit = new EditWidget(this,https[row]);
    edit->show();
}

//停止请求
void MainDialog::stopRequest(int row,int col)
{
    ui->tableWidget->item(row,5)->setText("暂停中");
    ui->tableWidget->item(row,col)->setText("开始");
    ui->tableWidget->item(row,col)->setIcon(QIcon(QPixmap(":/file/images/player.png")));
    https[row]->closeIntervalTimer();
    slot_flushWidget();
}
/**
 * @brief MainDialog::delRequest
 * 删除请求
 */
void MainDialog::delRequest(int row,int col)
{
    QMessageBox box(QMessageBox::Warning,"提示","是否确认删除？");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    box.setButtonText (QMessageBox::Cancel,QString("取 消"));
    if(box.exec() == QMessageBox::Ok){
        deleteItem(row);
    }
}
/**
 * @brief MainDialog::loadData
 */
void MainDialog::loadData()
{
    QFile file(data_file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd()) {
//        QByteArray line = file.readLine();

        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QString line = codec->toUnicode(file.readLine());
        QStringList strlist=line.split(",");
        qDebug()<< strlist;


        QString strUrl = strlist.at(0).trimmed();
        int outTime = strlist.at(3).trimmed().toInt();
        QString title = strlist.at(1).trimmed();
        int timeInterval = strlist.at(2).trimmed().toInt();

        QString startTime = strlist.at(4).trimmed();
        QString endTime = strlist.at(5).trimmed();

        HttpFun* pHttpFun = new HttpFun();
        https.enqueue(pHttpFun);
        //http请求结束信号
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)),
                this,SLOT(slot_requestFinished(bool,const QString&)));
        //初始化参数
        pHttpFun->init(strUrl,outTime,timeInterval,startTime,endTime,title);

        //添加到所有列表
        addItemToWidget(strUrl,title,outTime,timeInterval,startTime,endTime);
    }
}

void MainDialog::closeEvent(QCloseEvent *)
{
    on_savepbtn_clicked();
}



//请求按钮点击
void MainDialog::slot_requestBtnClicked()
{
    HttpFun* pHttpFun = new HttpFun();
    https.enqueue(pHttpFun);

    QString strUrl = ui->addressEdit->text().trimmed();
    int outTime = ui->timeoutEdit->text().toInt();
    QString title = ui->titleEdit->text();
    int timeInterval = ui->timeIntervalEdit->text().toInt();

    QString startTime = ui->startTimeEdit->text();
    QString endTime = ui->endTimeEdit->text();

    //http请求结束信号
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)),
            this,SLOT(slot_requestFinished(bool,const QString&)));
    //初始化参数
    pHttpFun->init(strUrl,outTime,timeInterval,startTime,endTime,title);

    //添加到所有列表
    addItemToWidget(strUrl,title,outTime,timeInterval,startTime,endTime);
}

//请求结束
void MainDialog::slot_requestFinished(bool bSuccess, const QString &strResult)
{

    if(bSuccess)
    {
        qDebug()<< strResult;
    }
    else
    {
        qDebug() << "请求失败！";
    }

}

//点击table中item
void MainDialog::slot_clickItem(const int &row, const int &col)
{
    qDebug()<<row<<"-"<<col;
    QString itemText = ui->tableWidget->item(row,col)->text();
    if(col == 7){
        //开始暂停
        if(itemText == "暂停"){
            stopRequest(row,col);
        }else{
            startRequest(row,col);
        }

    }else if(col == 8){
        editRequest(row,col);

    }else if(col == 9){
        delRequest(row,col);
    }else if(col == 10){
        qDebug()<< "查看日志";
        showLog(row);
    }
}

//刷新请求状态和请求数据的变化
void MainDialog::slot_flushWidget()
{
    updateRequestInfo();
}



//全部暂停
void MainDialog::on_allStopPbtn_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
        ui->tableWidget->item(i,5)->setText("暂停中");

        QString itemText = ui->tableWidget->item(i,7)->text();
        //开始暂停
        if(itemText == "暂停"){
            stopRequest(i,7);
        }
    }

}

//全部开始
void MainDialog::on_allStartPbtn_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
        ui->tableWidget->item(i,5)->setText("正在请求");

        QString itemText = ui->tableWidget->item(i,7)->text();
        //开始暂停
        if(itemText == "开始"){
            startRequest(i,7);
        }
    }

}
//请求保存为json配置
void MainDialog::on_savepbtn_clicked()
{
    QDir *fileHander = new QDir;
    bool exist = fileHander->exists("data");
    if(!exist)
        bool ok = fileHander->mkdir("data");

    fileHander->remove(data_file_name);


    QString fileName = data_file_name;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"can't open";
    }
     QQueue<HttpFun*>::iterator item;
     for (item = https.begin(); item != https.end(); ++item) {
         QString data_line = (*item)->getRequestUrl().trimmed() + ","
                  + (*item)->getRequestTitle().trimmed() +","
                 + QString::number((*item)->getIntervalTime()).trimmed() +","
                 + QString::number((*item)->getTimeOut()).trimmed() +","
                 + (*item)->getStartTime().trimmed() +","
                 + (*item)->getEndTime().trimmed();
        QTextStream in(&file);
        in<<data_line<<"\n";
     }
     file.close();
}

/**
 * @brief MainDialog::on_checkBox_clicked
 * @param checked
 */
void MainDialog::on_checkBox_clicked(bool checked)
{
    QQueue<HttpFun*>::iterator item;
    for (item = https.begin(); item != https.end(); ++item) {
      (*item)->setLogSwitch(checked);
    }
}
