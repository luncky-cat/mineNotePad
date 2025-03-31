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

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void initSignal();
    void initResource();
    void initContents();
    QListWidgetItem *getNewItem();
private slots:
    void on_addNewBtn_clicked();
    void delTitleItem(QListWidgetItem *item);
    void clickedTitleItem(QListWidgetItem *item);
    void delCurrentItem();
    void updateTitle();
private:
    Ui::titleBar *ui;
    QSharedPointer<QString>fileId;   //当前文件id
    QString titleIco;   //标题栏图标
    QString leftBtnIco;  //左箭头
    QString rigthBtnIco;  //右箭头
    QString addBtnIco;  //添加
    int index;  //未保存的分配id
    QListWidgetItem *currentItem;//记录当前的项目
    QPoint m_dragPosition;
    QColor defaultColor;
};

#endif // TITLEBAR_H
