#include "SignalRelay.h"

SignalRelay& SignalRelay::instance()
{
    static SignalRelay instance;
    return instance;
}

void SignalRelay::requestedSwitchEditor(QString &fileId)
{
    emit switchEditorRequested(fileId);

}

void SignalRelay::requestedAddNewEditor(QString &fileId)
{
    emit addNewEditorRequested(fileId);
}

void SignalRelay::requestedCloseEditor(QString& fileID)
{
    emit closeEditorRequested(fileID);
}

SignalRelay::SignalRelay(QObject *parent)
    : QObject(parent)
{
    index=1;
    fileId.clear();
}

void SignalRelay::requestAddTab(QString &fileId)
{
    emit addTabRequested(fileId);
}

void SignalRelay::requestRemoveTab(int index)
{
    emit removeTabRequested(index);
}

void SignalRelay::requestMinimize() {
    emit minimizeRequested();
}
void SignalRelay::requestMaximize() {
    emit maximizeRequested();
}
void SignalRelay::requestClose()    {
    emit closeRequested();
}

QString SignalRelay::Index()
{
    return QString::number(index++);
}

QString& SignalRelay::FileId()
{
    return fileId;
}
