#include "SignalRelay.h"

SignalRelay& SignalRelay::instance()
{
    static SignalRelay instance;
    return instance;
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
