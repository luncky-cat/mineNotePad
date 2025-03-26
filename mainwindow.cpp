#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SignalRelay.h"
#include "titlebar.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initResource();   //初始化资源
    initTitleBar();  //初始化标题栏
    initCenter();  //初始化中心界面
    initStatusBar();//初始化状态栏
    initSignal();   //连接信号
    initStyle();  //加载qss
    //qDebug()<<"index::"<<SignalRelay::instance().Index();
    qDebug()<<"fileid:"<<SignalRelay::instance().FileId();
    SignalRelay::instance().requestAddTab(SignalRelay::instance().FileId());
    addNewEditor(SignalRelay::instance().FileId());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initResource(){
    appIcon = ":/Icon/application.png";
    currentEditor=nullptr;
    defaultSize=14;
    currentSize=14;
}

void MainWindow::initTitleBar()
{
    setWindowFlags(Qt::FramelessWindowHint);  //隐藏
    titleBar_=new titleBar(this); //自定义的标题栏
    for (QToolBar *toolbar : findChildren<QToolBar *>()) {
        removeToolBar(toolbar);
        delete toolbar;
    }
}

void MainWindow::initCenter()
{
    ui->statusBarAct->setChecked(true);    // 默认勾选（前面有✔）

    QWidget *centralContainer = new QWidget(this);

    editorStack = new QStackedWidget(centralContainer);
    editorStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 让它填充整个界面
    QFont defaultFont;
    defaultFont.setPointSize(defaultSize);  // 设置默认大小
    editorStack->setFont(defaultFont);  // 作用到整个 StackWidget

    QVBoxLayout *layout = new QVBoxLayout(centralContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(titleBar_);
    layout->addWidget(ui->menubar);
    layout->addWidget(editorStack);  // QStackedWidget 保持在 centralWidget 里

    centralContainer->setLayout(layout);
    setCentralWidget(centralContainer);

    nullptrEditor=new QTextEdit(editorStack);
    editorStack->addWidget(nullptrEditor);
    setWindowIcon(QIcon(appIcon));//设置运行图标
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
    statusBar()->setFixedHeight(40);
    ui->menubar->setStyleSheet("background-color:#F0F5F9");
}


void MainWindow::initSignal(){
    connect(&SignalRelay::instance(),&SignalRelay::minimizeRequested,this,&QMainWindow::showMinimized);  //最小
    connect(&SignalRelay::instance(), &SignalRelay::maximizeRequested,this,&MainWindow::showMax);//最大/还原
    connect(&SignalRelay::instance(), &SignalRelay::closeRequested,this, &QMainWindow::close); //关闭
    connect(nullptrEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //对生成新的并进行绑定
    connect(&SignalRelay::instance(),&SignalRelay::addNewEditorRequested,this,&MainWindow::addNewEditor);//新建编辑器
    //connect(&SignalRelay::instance(),&SignalRelay::closeEditorRequested,this,&MainWindow::closeEditor); //关闭编辑器
    connect(ui->cutAct, &QAction::triggered, this, &MainWindow::cutText);
    connect(ui->copyAct, &QAction::triggered, this, &MainWindow::copyText);
    connect(ui->patseAct, &QAction::triggered, this, &MainWindow::pasteText);
    connect(ui->delAct, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->selectAllAct, &QAction::triggered, this, &MainWindow::selectAllText);
    connect(ui->revokeAct, &QAction::triggered, this, &MainWindow::undoText);
    //connect(ui->newAct,&QAction::triggered,&SignalRelay::instance(),&SignalRelay::requestAddTab);
    connect(&SignalRelay::instance(), &SignalRelay::closeEditorRequested, this, &::MainWindow::oncloseEditor);
    connect(&SignalRelay::instance(),&SignalRelay::switchEditorRequested,this,&MainWindow::switchEditor);

}

void MainWindow::switchEditor(QString& fileID){
    qDebug()<<"cc";
    if(fileID==SignalRelay::instance().FileId()){
        return;
    }
    bool ok;
    if(fileID.toInt(&ok)){   //属于无路径，数字的   也无内容
        currentEditor=nullptrEditor;
    }else{
        currentEditor=textEditMap[fileID];
    }
    editorStack->setCurrentWidget(nullptrEditor);
}


void MainWindow::initStyle()
{
    QFile file(":/styles/style.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void MainWindow::bindNewEditor()
{
    QString fileId=SignalRelay::instance().FileId();
    qDebug()<<"绑定新编辑器"<<fileId;
    disconnect(nullptrEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //对生成新的并进行绑定
    QTextEdit* newEditor=new QTextEdit(editorStack);
    newEditor->setText(nullptrEditor->toPlainText()+fileId);  //复制文本
    currentEditor=newEditor;
    editorStack->addWidget(currentEditor);
    editorStack->setCurrentWidget(currentEditor);
    nullptrEditor->clear();
    textEditMap.insert(fileId,currentEditor);
    qDebug()<<"组合大小:"<<textEditMap.size();
    qDebug()<<"容器大小:"<<editorStack->count();
    connect(nullptrEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //对生成新的并进行绑定
}


void MainWindow::showMax(){
    this->isMaximized()?this->showNormal(): this->showMaximized();
}

void MainWindow::addNewEditor(QString &fileID)    //
{
    bool ok;
    qDebug()<<"addneweditor:"<<fileID;
    if(fileID.toInt(&ok)){    //空的为新建按钮点击
        currentEditor=nullptrEditor;
        editorStack->setCurrentWidget(currentEditor);
        return;
    }

    if(textEditMap.contains(fileID)){   //已经打开
        currentEditor = textEditMap[fileID];
        editorStack->setCurrentWidget(currentEditor);
        SignalRelay::instance().FileId()=fileID;   //更新记录
        return;
    }

    QFile file(fileID);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
        return;
    }

    //打开新文件
    SignalRelay::instance().FileId()=fileID;   //更新当前文件id
    currentEditor=new QTextEdit(editorStack);

    QTextStream in(&file); //读文件
    currentEditor->setPlainText(in.readAll());
    file.close();

    //连接信号
    // connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    // connect(currentEditor, &QTextEdit::cursorPositionChanged, this,  &MainWindow::updateStatusBar);

    editorStack->addWidget(currentEditor);
    editorStack->setCurrentWidget(currentEditor); //显示当前对应
    textEditMap.insert(SignalRelay::instance().FileId(),currentEditor);
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
    SignalRelay::instance().FileId().clear();
    SignalRelay::instance().requestAddTab(SignalRelay::instance().FileId());
}

void MainWindow::on_openAct_triggered()   //打开文件
{
    QFileDialog Dialog(nullptr,tr("选择文件"),"","文本文档(*.txt);;所有文件(*.)");
    if(Dialog.exec()==QFileDialog::Accepted){   //选中文件并打开
        QString filePath=Dialog.selectedFiles().constFirst();
        addNewEditor(filePath);   //添加编辑器
        SignalRelay::instance().addTabRequested(SignalRelay::instance().FileId());   //添加选项卡
    }
}


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
//     //QTextEdit *saveTextEditor=textEditMap[fileId];
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
//     //out<<saveTextEditor->toPlainText();
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

void MainWindow::oncloseEditor(QString & fileId){
    if(textEditMap.contains(fileId)){    //存在
        bool needSave=false;    //保存意愿
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存确认","该文件尚未保存，是否保存？",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            needSave=true;
        }else if(reply == QMessageBox::Cancel){    //取消
            return;
        }
        if(needSave){   //愿意保存
            //on_saveSAct_triggered();
        }
        editorStack->removeWidget(textEditMap.value(fileId,nullptr));   //移出容器
        textEditMap.remove(fileId);  //移除组合
    }
}

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


