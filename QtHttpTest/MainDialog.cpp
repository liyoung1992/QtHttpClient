#include "MainDialog.h"
#include "ui_MainDialog.h"
#include "HttpFun.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    logLabel = new QLabel();
    //请求按钮点击信号
    connect(ui->requestBtn,SIGNAL(clicked()),this,SLOT(slot_requestBtnClicked()));
    //点击item
    connect(ui->tableWidget,SIGNAL(cellClicked(int, int)),this,SLOT(slot_clickItem(int,int)));
}

MainDialog::~MainDialog()
{
    delete ui;
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
    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem("正常"));
    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem("1000"));
    ui->tableWidget->setItem(rowCount,7,new QTableWidgetItem("暂停"));
    ui->tableWidget->setItem(rowCount,8,new QTableWidgetItem("编辑"));
    ui->tableWidget->setItem(rowCount,9,new QTableWidgetItem("删除"));
    ui->tableWidget->setItem(rowCount,10,new QTableWidgetItem("查看日志"));
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
    ui->tableWidget->item(rowCount,1)->setFlags(ui->tableWidget->item(rowCount,1)->flags() & ~Qt::ItemIsEditable);
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

void MainDialog::showLog(const int &row)
{

    QFile file(https[row]->getLogFileName());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString result;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        result += str;
    }
    logLabel->setText((result));
    logLabel->show();
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

//    qDebug() << "开始请求api接口:" << strUrl;
//    qDebug() << "标题" << title;
//    qDebug() << "超时时间" << outTime;
//    qDebug() << "时间间隔" << timeInterval;
//    qDebug() << "开始时间" << ui->startTimeEdit->text();
//    qDebug() << "结束时间" << ui->endTimeEdit->text();

    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_requestFinished(bool,const QString&)));
    //发送请求
    pHttpFun->init(strUrl,outTime,timeInterval,startTime,endTime,title);
    pHttpFun->sendRequest(strUrl,outTime,timeInterval,startTime,endTime);
    //添加到所有列表
    addItemToWidget(strUrl,title,outTime,timeInterval,startTime,endTime);
}

//请求结束
void MainDialog::slot_requestFinished(bool bSuccess, const QString &strResult)
{

    if(bSuccess)
    {
        qDebug()<< strResult;
//        ui->textBrowser->append(strResult);
//        ui->resultEdit->append(strResult); //纯文本显示
    }
    else
    {
        qDebug() << "请求失败！";
//        ui->textBrowser->append("请求失败！");
//        ui->resultEdit->append(tr("请求失败！"));
    }

}

void MainDialog::slot_clickItem(const int &row, const int &col)
{
    qDebug()<<row<<"-"<<col;
    QString itemText = ui->tableWidget->item(row,col)->text();
    if(col == 7){
        //开始暂停
        if(itemText == "暂停"){
            ui->tableWidget->item(row,col)->setText("开始");
            https[row]->closeIntervalTimer();
        }else{
            ui->tableWidget->item(row,col)->setText("暂停");
            https[row]->startIntervalTimer();
        }

    }else if(col == 8){
        //编辑 TODO

    }else if(col == 9){
        //删除
        deleteItem(row);
    }else if(col == 10){
        qDebug()<< "查看日志";
        showLog(row);
    }
}


//写入文件测试
//void MainDialog::on_pushButton_clicked()
//{

//     QString fileName = "1.txt";
//     QString str="0000000nnnnnnnnnnnnnnnn";
//     QFile file(fileName);
//     if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
//     {
//        QMessageBox::warning(this,"sdf","can't open",QMessageBox::Yes);
//     }
//     QTextStream in(&file);
//     in<<str<<"\n";
//     file.close();
//}

//全部暂停
void MainDialog::on_allStopPbtn_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
       QString itemText = ui->tableWidget->item(i,7)->text();
       //开始暂停
       if(itemText == "暂停"){
           ui->tableWidget->item(i,7)->setText("开始");
           https[i]->closeIntervalTimer();
       }
    }

}

//全部开始
void MainDialog::on_allStartPbtn_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
       QString itemText = ui->tableWidget->item(i,7)->text();
       //开始暂停
       if(itemText == "开始"){
           ui->tableWidget->item(i,7)->setText("暂停");
           https[i]->startIntervalTimer();
       }
    }

}



//void MainDialog::on_pushButton_2_clicked()
//{

//    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
//    QString str = time.toString("hh:mm:ss"); //设置显示格式

//    ui->textBrowser->append(str);

//    HttpFun* pHttpFun = new HttpFun();
//    ui->textBrowser->append(QString::number(pHttpFun->judgeTime("17:50:50","17:52:52")));

//}
