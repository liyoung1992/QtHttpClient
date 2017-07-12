#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>
#include "HttpFun.h"
#include "MainDialog.h"

namespace Ui {
class EditWidget;
}
class MainDialog;

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = 0);
    EditWidget(MainDialog *mainDialog, HttpFun *http);
    ~EditWidget();
    void showHttpValue();

private slots:
    void on_sureBtn_clicked();

    void on_cancleBtn_clicked();

private:
    Ui::EditWidget *ui;
    HttpFun* m_pHttp;
    MainDialog* m_pMainDialog;
};

#endif // EDITWIDGET_H
