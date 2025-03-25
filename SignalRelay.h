#ifndef SIGNALRELAY_H
#define SIGNALRELAY_H

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
    //void addTabRequested();             // 请求添加一个标签页
    void removeTabRequested(int index); // 请求移除指定索引的标签页
    // 根据需要添加其他信号
    void minimizeRequested();           // 请求最小化
    void maximizeRequested();           // 请求最大化
    void closeRequested();              // 请求关闭
public slots:
    // 定义槽函数，组件可以调用这些槽来发射信号
    //void requestAddTab(QString &fileId);
    void requestRemoveTab(int index);
    void requestMinimize();
    void requestMaximize();
    void requestClose();
private:
    // 构造函数私有化，确保单例
    explicit SignalRelay(QObject *parent = nullptr);
    Q_DISABLE_COPY(SignalRelay)
};


#endif // SIGNALRELAY_H
