#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QListWidgetItem>
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
    QListWidgetItem *currentItem;//记录当前的项目
    void initSignal();
    void initResource();
    void initContents();

private slots:
    QListWidgetItem* delTitleItem(QListWidgetItem *item);
    void clickedTitleItem(QListWidgetItem *item);
    void setTitleText(QString &fileId);
    void delCurrentItem(QString &fileId);
};

#endif // TITLEBAR_H
