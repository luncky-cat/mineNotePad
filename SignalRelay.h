#ifndef SIGNALRELAY_H
#define SIGNALRELAY_H

#include <QListWidgetItem>
#include <QObject>

class SignalRelay : public QObject
{
    Q_OBJECT
public:
    static SignalRelay& instance();
    QSharedPointer<QString> getFileId();
signals:
    void addTabRequested();             // 请求添加一个标签页
    void switchEditorRequested();
    void addNewEditorRequested();   //请求添加新的编辑器
    void closeEditorRequested();
    void minimizeRequested();           // 请求最小化
    void maximizeRequested();           // 请求最大化
    void closeRequested();              // 请求关闭
    void updateTitleRequested();
    void delCurrentItemRequest();
    void performUpdateStatusBarRequested();
signals:
    void topResizeRequested(const QPoint &globalPos);   //调整检测
public slots:
    void requestedSwitchEditor();
    void requestedAddNewEditor();
    void requestedCloseEditor();
    void requestAddTab();
    void requestMinimize();
    void requestMaximize();
    void requestClose();
    void requestUpdateTitle();
    void requestDelCurrentItem();
    void requestedTopResize(const QPoint &globalPos);
    void requestPerformUpdateStatusBar();
private:
    QSharedPointer<QString>fileId;   //当前的文件id
private:
    explicit SignalRelay(QObject *parent = nullptr);
    Q_DISABLE_COPY(SignalRelay)
};


#endif // SIGNALRELAY_H
