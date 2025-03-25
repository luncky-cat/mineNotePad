#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

namespace Ui {
class titleBar;
}

class titleBar : public QWidget
{
    Q_OBJECT
public:
    explicit titleBar(QWidget *parent = nullptr);
    ~titleBar();
private:
    void on_addNewBtn_clicked(QString& fileId);
private:
    Ui::titleBar *ui;
    QString titleIco;   //标题栏图标
    QString leftBtnIco;  //左箭头
    QString rigthBtnIco;  //右箭头
    void initSignal();
    void initResource();
   // void requestMinimize(QToolButton *, void (*)(bool), SignalRelay *, );
   // void requestMinimize(QToolButton *, void (*)(bool), SignalRelay *, );
};

#endif // TITLEBAR_H
