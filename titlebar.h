#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QListWidgetItem>
#include <QWidget>
#include <QSharedPointer>
#include <QMouseEvent>

namespace Ui {
class titleBar;
}

class titleBar : public QWidget
{
    Q_OBJECT
public:
    explicit titleBar(QWidget *parent = nullptr);
    ~titleBar();
    QSharedPointer<QString>fileId;   //当前文件id
private:
    void on_addNewBtn_clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    Ui::titleBar *ui;
    QString titleIco;   //标题栏图标
    QString leftBtnIco;  //左箭头
    QString rigthBtnIco;  //右箭头
    QString addBtnIco;
    int index;
    QListWidgetItem *currentItem;//记录当前的项目
    QPoint m_dragPosition;
    void initSignal();
    void initResource();
    void initContents();
    QColor defaultColor;
   // QListWidgetItem *getNewItem();
    QListWidgetItem *getNewItem();
private slots:
    void delTitleItem(QListWidgetItem *item);
    void clickedTitleItem(QListWidgetItem *item);
    void setTitleText(QString &fileId);
    void delCurrentItem();
};

#endif // TITLEBAR_H
