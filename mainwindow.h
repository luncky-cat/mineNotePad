#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "titlebar.h"
#include "topresizearea.h"

#include <QMainWindow>
#include<QProcess>
#include <QFileDialog>
#include<QDebug>
#include <QSaveFile>
#include<QMessageBox>
#include<QDateTime>

#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextEdit>
#include <QWheelEvent>
#include<QLabel>
#include<QPushButton>
#include<QToolButton>
#include<QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include<QFont>
#include <QScrollBar>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void cutText();
    void copyText();
    void pasteText();
    void deleteText();
    void selectAllText();
    void undoText();
    void showMax();   //最大/复原
    void on_newWAct_triggered();
    void on_newAct_triggered();
    bool addNewEditor();
    void on_openAct_triggered();
    void switchEditor();
    void oncloseEditor();
    void on_saveAct_triggered();
    void on_priAct_triggered();
    void on_statusBarAct_triggered(bool checked);
    void on_dateAct_triggered();
    void on_closeAct_triggered();
    void on_saveSAct_triggered();
    void updateStatusBar();   //状态栏防抖
    void performUpdateStatusBar();   //实际执行函数
    void updateCursorStatus();
    void updateTextStats();
private:
    Ui::MainWindow *ui;
    //QMap<QString,QTextEdit*>textEditMap;
    QMap<QString,QPair<QTextEdit*,bool>> textEditMap;//fileId-textEdit
    QStackedWidget *editorStack; //显示的编辑框，通过这个切换
    QTextEdit *currentEditor;  //当前的编辑器
    QTextEdit *defaultEditor;  //当前的编辑器
    QSharedPointer<QString>fileId;
    QPoint m_dragPosition;

    titleBar * titleBar_;
    QString appIcon;   //程序运行图标    
    //QString fileId;//当前打开的文件路径
    QTextEdit* newEditor;
    QLabel * cursorPosLabel; // 位置
    QLabel * charCountLabel;     // 字符数量
    QLabel *fontRateLabel;     // 放缩率
    QLabel *formatLabel; // 文件格式
    QLabel *encodeLabel; // 编码
    int defaultSize; //默认字大小
    int currentSize;//当前字大小
    QFont fontStyle;//字体

    void initResource();
    void initTitleBar();
    void initCenter();
    void initSignal();
    void initStatusBar();
    void initStyle();

    void bindNewEditor();

    TopResizeArea* resizeArea;


    enum ResizeRegion {
        None,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };
    ResizeRegion m_resizeRegion = None;
    bool m_resizing = false;
    QPoint m_dragPos;
    int m_borderWidth = 10; // 定义边缘检测宽度
    ResizeRegion detectResizeRegion(const QPoint &pos);

    QTimer* statusBarTimer;        // 防抖定时器


};
#endif // MAINWINDOW_H
