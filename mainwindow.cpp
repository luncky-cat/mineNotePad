#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SignalRelay.h"
#include "titlebar.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    initResource();   //初始化资源
    initTitleBar();  //初始化标题栏
    initCenter();  //初始化控件
    initStatusBar();//初始化状态栏
    //initObjName();
    //titleBar->setObjectName("titleBar");

    initSignal();   //连接信号
    //on_addNewButton_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initResource(){
    appIcon = ":/Icon/application.png";
    fileId.clear();
    currentEditor=nullptr;
    defaultSize=14;
    currentSize=14;
}




void MainWindow::initTitleBar()
{
    // 自定义标题栏相关设置
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(appIcon));
}

void MainWindow::initCenter()
{
    ui->menubar->setStyleSheet("background-color:#F0F5F9");
    ui->statusBarAct->setChecked(true);    // 默认勾选（前面有✔）


    //创建一个新的 centralWidget 容器
    QWidget *centralContainer = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    titleBar * mtitleBar=new titleBar(this);
    mtitleBar->setObjectName("mtitleBar");
    //mtitleBar->setStyleSheet("background-color:#F0F5F9");
    layout->addWidget(mtitleBar);
    layout->addWidget(ui->menubar);

    //创建 QStackedWidget
    editorStack = new QStackedWidget(centralContainer);
    layout->addWidget(editorStack);  // QStackedWidget 保持在 centralWidget 里
    editorStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 让它填充整个界面

    //设置字体大小
    QFont defaultFont;
    defaultFont.setPointSize(defaultSize);  // 设置默认大小
    editorStack->setFont(defaultFont);  // 作用到整个 StackWidget

    //设置 centralWidget
    centralContainer->setLayout(layout);
    ui->centralwidget->layout()->setSpacing(0);
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(centralContainer);

    QFile file(":/styles/style.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        qDebug()<<"d";
        file.close();
    }

}

void MainWindow::initStatusBar()
{
    cursorPosLabel = new QLabel("行: 0, 列: 0", this);  // 位置
    charCountLabel = new QLabel("字符数: 0", this);     // 字符数量
    fontRateLabel = new QLabel("100%", this);     // 放缩率
    cursorPosLabel->setMinimumWidth(100);
    charCountLabel->setMinimumWidth(100);
    fontRateLabel->setMinimumWidth(80);
    statusBar()->setStyleSheet("background-color: #F0F5F9;"); // 设置浅蓝色背景
    statusBar()->addWidget(cursorPosLabel);
    statusBar()->addWidget(charCountLabel);
    statusBar()->addWidget(fontRateLabel);
    statusBar()->setFixedHeight(30);
}



void MainWindow::initSignal(){
    connect(&SignalRelay::instance(),&SignalRelay::minimizeRequested,this,&QMainWindow::showMinimized);
    connect(&SignalRelay::instance(), &SignalRelay::maximizeRequested,this,&MainWindow::showMax);
    connect(&SignalRelay::instance(), &SignalRelay::closeRequested,this, &QMainWindow::close);
    connect(ui->cutAct, &QAction::triggered, this, &MainWindow::cutText);
    connect(ui->copyAct, &QAction::triggered, this, &MainWindow::copyText);
    connect(ui->patseAct, &QAction::triggered, this, &MainWindow::pasteText);
    connect(ui->delAct, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->selectAllAct, &QAction::triggered, this, &MainWindow::selectAllText);
    connect(ui->revokeAct, &QAction::triggered, this, &MainWindow::undoText);
    //connect(ui->newAct,&QAction::triggered,&SignalRelay::instance(),&SignalRelay::addTabRequested(fileId));
}


void MainWindow::showMax(){
    this->isMaximized()?this->showNormal(): this->showMaximized();
}

// void MainWindow::updateStatusBar(){   //更新状态栏
//     QTextCursor cursor = currentEditor->textCursor();
//     int line = cursor.blockNumber() + 1;   // 获取当前行（从 0 开始，所以 +1）
//     int column = cursor.positionInBlock() + 1; // 获取列（从 0 开始，所以 +1）
//     cursorPosLabel->setText(QString("行: %1, 列: %2").arg(line).arg(column));
//     int charCount =currentEditor->toPlainText().length(); // 获取文本长度
//     charCountLabel->setText(QString("字符数: %1").arg(charCount));
//     int scale = qRound((currentSize / (double)defaultSize) * 100);
//     fontRateLabel->setText(QString("%1%").arg(scale));
// }

void MainWindow::on_newWAct_triggered()
{
    QString program = QCoreApplication::applicationFilePath();  // 获取当前程序的可执行路径
    QProcess::startDetached(program);  // 以新进程启动
}


void MainWindow::on_newAct_triggered()
{
    fileId.clear();

}

// void MainWindow::on_openAct_triggered()   //打开文件
// {
//     QFileDialog Dialog(nullptr,tr("选择文件"),"","文本文档(*.txt);;所有文件(*.)");
//     if(Dialog.exec()==QFileDialog::Accepted){   //选中文件并打开
//         QString filePath=Dialog.selectedFiles().constFirst();
//         if(textEditMap.contains(filePath)){   //已经打开
//             currentEditor = textEditMap[filePath];
//             editorStack->setCurrentWidget(currentEditor);
//             fileId=filePath;   //记录
//             return;
//         }

//         QFile file(filePath);
//         if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
//             QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
//             return;
//         }

//         //打开新文件
//         fileId=filePath;   //更新当前文件id

//         on_addNewButton_clicked();  //添加选项卡

//         currentEditor=new QTextEdit(editorStack);

//         //更新字号
//         QFont currentFont = currentEditor->font();
//         currentSize = currentFont.pointSize();

//         //读文件
//         QTextStream in(&file);
//         currentEditor->setPlainText(in.readAll());
//         file.close();

//         //连接信号
//         connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
//         connect(currentEditor, &QTextEdit::cursorPositionChanged, this,  &MainWindow::updateStatusBar);

//         editorStack->addWidget(currentEditor);
//         editorStack->setCurrentWidget(currentEditor); //显示当前对应

//         textEditMap.insert(fileId,currentEditor);

//         updateStatusBar();  //更新状态栏
//     }
// }


// void MainWindow::on_saveAct_triggered()
// {
//     QTextEdit *saveTextEditor=textEditMap[fileId];
//     bool ok;
//     if(fileId.toInt(&ok)){  //保存文件路径不存在
//         QString filePath = QFileDialog::getSaveFileName(
//             nullptr, "保存文件", "新建文档.txt", "文本文档(*.txt);;所有文件(*.)"
//             );

//         if (filePath.isEmpty()) {
//             return;
//         }

//         //更新title   根据之前的fileid寻找item
//         // QListWidgetItem *item=nullptr;
//         // for (int i = 0; i < titleList->count(); ++i) {
//         //     item = titleList->item(i);
//         //     if (item->data(Qt::UserRole).toString() == fileId) {
//         //         QWidget *itemWidget = titleList->itemWidget(item);
//         //         item->setData(Qt::UserRole,filePath);//更新文件id
//         //         if (itemWidget) {
//         //             QPushButton *titleLabel = itemWidget->findChild<QPushButton*>();
//         //             titleLabel->setText(QFileInfo(filePath).fileName()); // 更新文本
//         //         }
//         //     }
//         // }

//         textEditMap.remove(fileId);//删除旧组合
//         fileId=filePath;  //更新文本id
//         textEditMap.insert(fileId,saveTextEditor);   //新组合
//     }

//     QSaveFile file(fileId);
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
//         return;
//     }
//     //写入数据
//     QTextStream out(&file);
//     out<<saveTextEditor->toPlainText();
//     if (!file.commit()) {
//         QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
//     }
// }

// void MainWindow::on_closeAct_triggered(){
//     QListWidgetItem* item=nullptr;
//     // for (int i = 0; i < titleList->count(); ++i) {
//     //     item = titleList->item(i);
//     //     if (item->data(Qt::UserRole).toString() == fileId) {
//     //         delTitleItem(item);
//     //     }
//     // }
// }

// void MainWindow::on_saveSAct_triggered()
// {
//     QTextEdit *saveTextEditor=textEditMap[fileId];
//     QString filePath = QFileDialog::getSaveFileName(
//         nullptr, "保存文件", "新建文档.txt", "文本文档(*.txt);;所有文件(*.)"
//         );

//     if (filePath.isEmpty()) {
//         return;
//     }

//     QSaveFile file(filePath);
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
//         return;
//     }

//     //写入数据
//     QTextStream out(&file);
//     out<<saveTextEditor->toPlainText();
//     if (!file.commit()) {
//         QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
//     }
// }

// void MainWindow::on_dateAct_triggered()
// {
//     QDateTime time = QDateTime::currentDateTime();  // 获取当前时间
//     currentEditor->insertPlainText(time.toString("yyyy-MM-dd HH:mm"));
// }


// void MainWindow::on_delAct_triggered()
// {
//     QTextCursor cursor = currentEditor->textCursor();
//     if (cursor.hasSelection()) {
//         cursor.removeSelectedText();  // 仅删除选中内容
//     }
// }

// void MainWindow::on_priAct_triggered()
// {
//     QPrinter printer;
//     QPrintDialog dialog(&printer,currentEditor);
//     if (dialog.exec() == QDialog::Accepted) {
//         currentEditor->print(&printer);
//     }
// }


// void MainWindow::on_enlargeAct_triggered()
// {
//     QFont font = currentEditor->font();
//     currentSize = font.pointSize();
//     font.setPointSize(currentSize + 1);  // 增加1个单位
//     currentEditor->setFont(font);
//     updateFontRateLabel();  // 更新放缩率
// }


// void MainWindow::on_reduceAct_triggered()
// {
//     QFont font = currentEditor->font();
//     currentSize = font.pointSize();
//     font.setPointSize(currentSize -1);  // 增加1个单位
//     currentEditor->setFont(font);
//     updateFontRateLabel();  // 更新放缩率
// }

// void MainWindow::updateFontRateLabel() {
//     int scale = qRound((currentSize / (double)defaultSize) * 100);
//     fontRateLabel->setText(QString("%1%").arg(scale));
// }


// void MainWindow::wheelEvent(QWheelEvent *event)
// {
//     // 判断是否按下 Ctrl 键
//     if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
//         // 获取鼠标滚轮的旋转角度
//         int delta = event->angleDelta().y();
//         // 判断是滚轮向上还是向下
//         if (delta > 0) {
//             // 缩放字体变大
//             changeFontSize(true);
//         } else {
//             // 缩放字体变小
//             changeFontSize(false);
//         }
//     } else {
//         // 如果没有按下 Ctrl 键，正常处理滚轮事件
//         QMainWindow::wheelEvent(event);
//     }
// }

// void MainWindow::changeFontSize(bool increase)
// {
//     // 获取当前字体
//     QFont font = currentEditor->font();
//     currentSize = font.pointSize();
//     // 增加或减少字体大小
//     font.setPointSize(currentSize + (increase ? 1 : -1));
//     // 设置新的字体
//     currentEditor->setFont(font);
//     updateFontRateLabel();  // 更新放缩率
// }

// void MainWindow::on_statusBarAct_triggered(bool checked)
// {
//     statusBar()->setVisible(checked);  // 显示或隐藏状态栏
// }





// void MainWindow::clickedTitleItem(QListWidgetItem *item)
// {
//     fileId= item->data(Qt::UserRole).toString();//获得
//     currentEditor=textEditMap[fileId];
//     editorStack->setCurrentWidget(currentEditor); //切
//     // 更新状态栏
//     updateStatusBar();

// }



void MainWindow::cutText()
{
    currentEditor->cut();
}

void MainWindow::copyText()
{

    QTextCursor cursor = currentEditor->textCursor();
    if (cursor.hasSelection()) {  // 只有选中了文本才执行复制
        currentEditor->copy();
    }
}

void MainWindow::pasteText()
{
    currentEditor->paste();
}

void MainWindow::deleteText()
{
    currentEditor->textCursor().removeSelectedText();
}

void MainWindow::selectAllText()
{
    currentEditor->selectAll();
}

void MainWindow::undoText()
{
    currentEditor->undo();
}


