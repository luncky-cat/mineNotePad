#include "SignalRelay.h"

SignalRelay::SignalRelay(QObject *parent)
    : QObject(parent){
    fileId=QSharedPointer<QString>::create("");
    fileId->clear();
}

SignalRelay& SignalRelay::instance()
{
    static SignalRelay instance;
    return instance;
}

void SignalRelay::requestedSwitchEditor()
{
    emit switchEditorRequested();

}

void SignalRelay::requestedAddNewEditor()
{
    emit addNewEditorRequested();
}

void SignalRelay::requestedCloseEditor()
{
    emit closeEditorRequested();
}

void SignalRelay::requestAddTab()
{
    emit addTabRequested();
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

void SignalRelay::requestUpdateTitle()
{
    emit updateTitleRequested();
}


void SignalRelay::requestDelCurrentItem()
{
    emit delCurrentItemRequest();
}

void SignalRelay::requestedTopResize(const QPoint &globalPos)
{
    emit topResizeRequested(globalPos);
}

void SignalRelay::requestPerformUpdateStatusBar()
{
    emit performUpdateStatusBarRequested();
}

QSharedPointer<QString> SignalRelay::getFileId()
{
    return this->fileId;
}

