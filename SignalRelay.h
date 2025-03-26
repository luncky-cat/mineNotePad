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
    void addTabRequested(QString &fileId);             // 请求添加一个标签页
    void switchEditorRequested(QString &fileId);
    void addNewEditorRequested(QString &fileId);   //请求添加新的编辑器
    void closeEditorRequested(QString &fileId);

    void removeTabRequested(int index); // 请求移除指定索引的标签页
    void minimizeRequested();           // 请求最小化
    void maximizeRequested();           // 请求最大化
    void closeRequested();              // 请求关闭
public slots:
    // 定义槽函数，组件可以调用这些槽来发射信号
    void requestedSwitchEditor(QString &fileId);
    void requestedAddNewEditor(QString &fileId);
    void requestedCloseEditor(QString &fileID);
    void requestAddTab(QString &fileId);
    void requestRemoveTab(int index);
    void requestMinimize();
    void requestMaximize();
    void requestClose();
public:
    QString Index();
    QString& FileId();
private:
    // 构造函数私有化，确保单例
    explicit SignalRelay(QObject *parent = nullptr);

    Q_DISABLE_COPY(SignalRelay)
    QString fileId; //当前文件的id
    int index;   //未保存的 分配数字id
};


#endif // SIGNALRELAY_H
