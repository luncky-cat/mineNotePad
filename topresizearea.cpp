#include "topresizearea.h"
#include "qdir.h"

TopResizeArea::TopResizeArea(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setCursor(Qt::SizeVerCursor);
    setAttribute(Qt::WA_TranslucentBackground, true);
    // 禁止系统绘制背景
    setAttribute(Qt::WA_NoSystemBackground, true);
    // 禁止自动填充背景
    setAutoFillBackground(false);
    setFixedHeight(borderWidth);
    QFile file(":/styles/topresizearea.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        qDebug()<<"生成";
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
    qDebug()<<styleSheet();

}
void TopResizeArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizing = true;
        m_dragStartPos = event->globalPos();
        event->accept();
        qDebug() << "TopResizeArea: mouse press" << m_dragStartPos;
    } else {
        QWidget::mousePressEvent(event);
    }
}

void TopResizeArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_resizing) {
        QPoint diff = event->globalPos() - m_dragStartPos;
        // 获取主窗口（顶层窗口）
        QWidget *w = window();
        QRect geom = w->geometry();
        // 调整上边缘：移动窗口顶部并减小高度
        geom.setTop(geom.top() + diff.y());
        // 限制最小高度（假设 minimumHeight() 已经设置）
        if (geom.height() < w->minimumHeight())
            geom.setHeight(w->minimumHeight());
        w->setGeometry(geom);
        m_dragStartPos = event->globalPos();
        event->accept();
        qDebug() << "TopResizeArea: mouse move, new geometry:" << geom;
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void TopResizeArea::mouseReleaseEvent(QMouseEvent *event)
{
    m_resizing = false;
    QWidget::mouseReleaseEvent(event);
    qDebug() << "TopResizeArea: mouse release";
}
