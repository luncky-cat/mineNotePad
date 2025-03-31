#include "topresizearea.h"
#include "qdir.h"

TopResizeArea::TopResizeArea(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setCursor(Qt::SizeVerCursor);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAutoFillBackground(false);
    setFixedHeight(borderWidth);
    QFile file(":/styles/topresizearea.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}
void TopResizeArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizing = true;
        m_dragStartPos = event->globalPos();
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void TopResizeArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_resizing) {
        QPoint diff = event->globalPos() - m_dragStartPos;
        QWidget *w = window();
        QRect geom = w->geometry();
        geom.setTop(geom.top() + diff.y());
        if (geom.height() < w->minimumHeight())
            geom.setHeight(w->minimumHeight());
        w->setGeometry(geom);
        m_dragStartPos = event->globalPos();
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void TopResizeArea::mouseReleaseEvent(QMouseEvent *event)
{
    m_resizing = false;
    QWidget::mouseReleaseEvent(event);
}
