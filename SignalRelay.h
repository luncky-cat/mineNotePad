#ifndef SIGNALRELAY_H
#define SIGNALRELAY_H

#include <QListWidgetItem>
#include <QObject>

// 定义一个全局信号转发类，采用单例模式
class SignalRelay : public QObject
{
    Q_OBJECT
public:
    // 获取全局唯一实例
    static SignalRelay& instance();
signals:
    // 定义需要转发的信号
    void addTabRequested();             // 请求添加一个标签页
    void switchEditorRequested();
    void addNewEditorRequested();   //请求添加新的编辑器
    void closeEditorRequested();
    void removeTabRequested(int index); // 请求移除指定索引的标签页
    void minimizeRequested();           // 请求最小化
    void maximizeRequested();           // 请求最大化
    void closeRequested();              // 请求关闭
    void setTitleTextRequested(QString &fileId);
     void delCurrentItemRequest();

signals:
    void topResizeRequested(const QPoint &globalPos);   //调整检测
public slots:
    // 定义槽函数，组件可以调用这些槽来发射信号
    void requestedSwitchEditor();
    void requestedAddNewEditor();
    void requestedCloseEditor();
    void requestAddTab();
    void requestRemoveTab(int index);
    void requestMinimize();
    void requestMaximize();
    void requestClose();
    void requestSetTitleText(QString &fileId);
    void requestDelCurrentItem();
    void requestedTopResize(const QPoint &globalPos);
private:
    // 构造函数私有化，确保单例
    explicit SignalRelay(QObject *parent = nullptr);
    Q_DISABLE_COPY(SignalRelay)
};


#endif // SIGNALRELAY_H
