#include "EditWidget.h"
#include "ui_EditWidget.h"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);
}

EditWidget::EditWidget(MainDialog *mainDialog,HttpFun *http):
    QWidget(),
    ui(new Ui::EditWidget)
{
   m_pHttp = http;
   m_pMainDialog = mainDialog;

   ui->setupUi(this);
   showHttpValue();
}

EditWidget::~EditWidget()
{
    delete ui;
}

void EditWidget::showHttpValue()
{
    if (m_pHttp) {
        ui->requestUrl->setText(m_pHttp->getRequestUrl());
        ui->requestTitle->setText(m_pHttp->getRequestTitle());
        ui->intervalTime->setText(QString::number(m_pHttp->getIntervalTime()));
        ui->timeOut->setText(QString::number(m_pHttp->getTimeOut()));
        QTime startTime = QTime::fromString(m_pHttp->getStartTime());
        QTime endTime = QTime::fromString(m_pHttp->getEndTime());
        ui->endTime->setTime(endTime);
        ui->startTime->setTime(startTime);
    }
}

//点击确定
void EditWidget::on_sureBtn_clicked()
{
    QString url = ui->requestUrl->text();
    QString title = ui->requestTitle->text();
    int intervalTime = ui->intervalTime->text().toInt();
    int timeOut = ui->timeOut->text().toInt();
    QString startTime = ui->startTime->text();
    QString endTime = ui->endTime->text();
    m_pHttp->init(url,timeOut,intervalTime,startTime,endTime,title);
    m_pMainDialog->updateRequestInfo();
    this->close();
    this->deleteLater();
}

//点击取消
void EditWidget::on_cancleBtn_clicked()
{
    this->close();
    this->deleteLater();
}
