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
private:
    Ui::MainWindow *ui;
    QMap<QString,QTextEdit*>textEditMap;//fileId-textEdit
    QStackedWidget *editorStack; //显示的编辑框，通过这个切换
    QTextEdit *currentEditor;  //当前的编辑器

    QString appIcon;   //程序运行图标    
    QString fileId;//当前打开的文件路径

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
};
#endif // MAINWINDOW_H
