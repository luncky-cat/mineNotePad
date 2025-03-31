#ifndef TOPRESIZEAREA_H
#define TOPRESIZEAREA_H

#include <QMouseEvent>
#include <QWidget>

class TopResizeArea : public QWidget
{
    Q_OBJECT
public:
    explicit TopResizeArea(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    const int borderWidth = 5;
    bool m_resizing = false;
    QPoint m_dragStartPos;
signals:
protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
    }
};

#endif // TOPRESIZEAREA_H
