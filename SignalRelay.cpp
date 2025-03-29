#include "SignalRelay.h"

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

SignalRelay::SignalRelay(QObject *parent)
    : QObject(parent)
{

}

void SignalRelay::requestAddTab()
{
    emit addTabRequested();
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

void SignalRelay::requestSetTitleText(QString &fileId)
{
    emit setTitleTextRequested(fileId);
}

void SignalRelay::requestDelCurrentItem()
{
    emit delCurrentItemRequest();
}

void SignalRelay::requestedTopResize(const QPoint &globalPos)
{
    emit topResizeRequested(globalPos);
}

