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

    void wheelEvent(QWheelEvent *event);
private slots:
    void cutText();
    void copyText();
    void pasteText();
    void deleteText();
    void selectAllText();
    void undoText();

    void showMax();   //最大/复原
    bool addNewEditor();
    void bindNewEditor();
    void switchEditor();
    void oncloseEditor();
    void updateStatusBar();   //状态栏防抖
    void performUpdateStatusBar();   //实际执行函数
    void updateCursorStatus();
    void updateTextStats();

    void on_newWAct_triggered();
    void on_newAct_triggered();
    void on_openAct_triggered();
    void on_saveAct_triggered();
    void on_priAct_triggered();
    void on_statusBarAct_triggered(bool checked);
    void on_dateAct_triggered();
    void on_closeAct_triggered();
    void on_saveSAct_triggered();

    void on_reduceAct_triggered();
    void on_enlargeAct_triggered();
private:
    void initResource();
    void initTitleBar();
    void initCenter();
    void initSignal();
    void initStatusBar();
    void initStyle();

    void saveFile();
    bool canCreateFile(const QString &filePath);
    void updateFontRateLabel();
    void changeFontSize(bool increase);
private:
    Ui::MainWindow *ui;
    TopResizeArea* resizeArea;//最顶部空间
    titleBar * titleBar_;  //标题栏

    QString appIcon;   //运行图标
    QMap<QString,QPair<QTextEdit*,bool>> textEditMap;
    QStackedWidget *editorStack; //显示的编辑框，通过这个切换
    QTextEdit *currentEditor;  //当前编辑器
    QTextEdit *defaultEditor;  //默认编辑器
    QSharedPointer<QString>fileId; //全局的fileId

    //状态栏控件
    QLabel * cursorPosLabel; // 位置
    QLabel * charCountLabel;     // 字符数量
    QLabel *fontRateLabel;     // 放缩率
    QLabel *formatLabel; // 文件格式
    QLabel *encodeLabel; // 编码
    QTimer* statusBarTimer;        // 防抖定时器

    int defaultSize; //默认字大小
    int currentSize;//当前字大小
    QFont fontStyle;//字体

    QPoint m_dragPosition;

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
    int m_borderWidth = 5; // 定义边缘检测宽度
    ResizeRegion detectResizeRegion(const QPoint &pos);

    void updateAllTextAndFutureFontSize();
};
#endif // MAINWINDOW_H
