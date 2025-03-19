#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void undoText();
private slots:
    void on_newWAct_triggered();

    void on_openAct_triggered();

    void on_saveAct_triggered();

    void on_saveSAct_triggered();

    void on_dateAct_triggered();

    void on_delAct_triggered();

    void on_priAct_triggered();

    void on_enlargeAct_triggered();

    void on_reduceAct_triggered();

    void on_statusBarAct_triggered(bool checked);

    void on_addNewButton_clicked();

    void delTitleItem(QListWidgetItem *item);

    void clickedTitleItem(QListWidgetItem *item);

    void on_newAct_triggered();

    void cutText();

    void copyText();

    void pasteText();

    void deleteText();

    void selectAllText();

    void updateFontRateLabel();

    void on_closeAct_triggered();

    void wheelEvent(QWheelEvent *event) override;

    void updateStatusBar();

private:
    void changeFontSize(bool increase);

    void initResource();

    void initSignal();

    void initCenter();

    void initTitleBar();

    void initStatusBar();

private:
    Ui::MainWindow *ui;
    QMap<QString,QTextEdit*>textEditMap;//fileId-textEdit
    QStackedWidget *editorStack; //显示的编辑框
    QTextEdit *currentEditor;  //当前编辑器
    QString appIcon;   //程序运行图标
    QString leftBtnIcon;
    QString rigthBtnIcon;
    QPushButton *leftBtn;//左箭头
    QPushButton *rightBtn;//右箭头
    QLabel *titleIcon;//图标
    QString fileId;//当前打开的文件路径
    QLabel * cursorPosLabel; // 位置
    QLabel * charCountLabel;     // 字符数量
    QLabel *fontRateLabel;     // 放缩率
    QLabel *formatLabel; // 文件格式
    QLabel *encodeLabel; // 编码
    QToolButton *addNewButton;//添加按钮
    QListWidget* titleList;   //标签项列表
    QToolButton *minimizeButton;//最小
    QToolButton *maximizeButton;//最大
    QToolButton *closeButton;  //关闭
    QWidget *titleBar;  //标题
    int defaultSize; //默认字大小
    int currentSize;//当前字大小
    QFont fontStyle;//字体

};
#endif // MAINWINDOW_H
