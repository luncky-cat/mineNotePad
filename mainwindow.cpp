#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initResource();   //初始化资源
    initTitleBar();  //初始化标题栏
    initCenter();  //初始化控件
    initStatusBar();//初始化状态栏
    initSignal();   //连接信号
    on_addNewButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initResource(){
    appIcon = ":/Icon/application.png";
    leftBtnIcon=":/Icon/left.png";
    rigthBtnIcon=":/Icon/rigth.png";
    fileId.clear();
    currentEditor=nullptr;
    defaultSize=14;
    currentSize=14;
    leftBtn=new QPushButton();
    rightBtn=new QPushButton();
    addNewButton = new QToolButton();
    minimizeButton = new QToolButton();
    maximizeButton = new QToolButton();
    closeButton = new QToolButton();
}

void MainWindow::initTitleBar()
{
    // 自定义标题栏相关设置
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(appIcon));

    // 创建自定义标题栏
    titleBar = new QWidget;
    titleBar->setFixedHeight(40);
    titleBar->setStyleSheet("background-color: #CEDDEA; color: #555555;");
    titleBar->installEventFilter(this);

    //定义布局
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    // 添加图标
    titleIcon = new QLabel;
    titleIcon->setPixmap(QPixmap(appIcon).scaled(30,30, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 添加标题栏列表
    titleList = new QListWidget;
    titleList->setViewMode(QListView::IconMode);
    titleList->setFlow(QListView::LeftToRight);
    titleList->setResizeMode(QListView::Adjust);
    titleList->setWrapping(false);  // 禁用换行，确保项目不会换行
    titleList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    titleList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 加载图像并调整大小
    QPixmap leftPixMap(leftBtnIcon);
    QIcon leftBtnIcon(leftPixMap.scaled(25,25, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    leftBtn->setIcon(leftBtnIcon);
    leftBtn->setIconSize(QSize(25, 25)); // 设置图标显示的实际大小
    leftBtn->setFixedSize(25,25);
    leftBtn->setStyleSheet("background-color: transparent;");

    QPixmap rigthPixMap(rigthBtnIcon);
    QIcon rigthBtnIcon(rigthPixMap.scaled(25,25, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    rightBtn->setIcon(rigthBtnIcon);
    rightBtn->setIconSize(QSize(25, 25)); // 设置图标显示的实际大小
    rightBtn->setFixedSize(25,25);
    rightBtn->setStyleSheet("background-color: transparent;");

    // 添加功能按钮
    addNewButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    addNewButton->setFixedSize(30, 30);
    addNewButton->setStyleSheet("background-color: transparent; font-size: 18px; color: #555555;");

    minimizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    minimizeButton->setFixedSize(30, 30);
    minimizeButton->setStyleSheet("background-color: transparent; font-size: 18px; color: #555555;");

    maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    maximizeButton->setFixedSize(30, 30);
    maximizeButton->setStyleSheet("background-color: transparent; font-size: 18px; color: #555555;");

    closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeButton->setFixedSize(30, 30);
    closeButton->setStyleSheet("background-color: transparent; font-size: 18px; color: #555555;");

    // 添加到标题栏布局
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(leftBtn);
    titleLayout->addWidget(titleList);
    titleLayout->addWidget(rightBtn);
    titleLayout->addWidget(addNewButton);
    titleLayout->addWidget(minimizeButton);
    titleLayout->addWidget(maximizeButton);
    titleLayout->addWidget(closeButton);
    titleBar->setLayout(titleLayout);
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
    layout->addWidget(titleBar);
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
}

void MainWindow::initStatusBar()
{
    cursorPosLabel = new QLabel("行: 0, 列: 0", this);  // 位置
    charCountLabel = new QLabel("字符数: 0", this);     // 字符数量
    fontRateLabel = new QLabel("100%", this);     // 放缩率
    // formatLabel = new QLabel("Windows(CRLF)", this); // 文件格式
    // encodeLabel = new QLabel("编码: UTF-8", this); // 编码
    cursorPosLabel->setMinimumWidth(100);
    charCountLabel->setMinimumWidth(100);
    fontRateLabel->setMinimumWidth(80);
    //formatLabel->setMinimumWidth(100);
    // encodeLabel->setMinimumWidth(100);
    statusBar()->setStyleSheet("background-color: #F0F5F9;"); // 设置浅蓝色背景
    statusBar()->addWidget(cursorPosLabel);
    statusBar()->addWidget(charCountLabel);
    statusBar()->addWidget(fontRateLabel);
    //statusBar()->addWidget(formatLabel);
    //statusBar()->addWidget(encodeLabel);
    statusBar()->setFixedHeight(30);
}

void MainWindow::initSignal(){
    connect(minimizeButton, &QToolButton::clicked, this, &QMainWindow::showMinimized);   //最小
    connect(maximizeButton, &QToolButton::clicked, this, &QMainWindow::showMaximized);   //最大
    connect(closeButton, &QToolButton::clicked, this, &QMainWindow::close);//关闭
    connect(addNewButton, &QToolButton::clicked, this,&MainWindow::on_addNewButton_clicked);//添加选项卡
    connect(ui->cutAct, &QAction::triggered, this, &MainWindow::cutText);
    connect(ui->copyAct, &QAction::triggered, this, &MainWindow::copyText);
    connect(ui->patseAct, &QAction::triggered, this, &MainWindow::pasteText);
    connect(ui->delAct, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->selectAllAct, &QAction::triggered, this, &MainWindow::selectAllText);
    connect(ui->revokeAct, &QAction::triggered, this, &MainWindow::undoText);
    // 连接按钮和滚动功能
    connect(leftBtn, &QPushButton::clicked, this, [=]() {
        int step = 100; // 设定滚动步长
        titleList->horizontalScrollBar()->setValue(titleList->horizontalScrollBar()->value() - step);
    });

    connect(rightBtn, &QPushButton::clicked, this, [=]() {
        int step = 100;
        titleList->horizontalScrollBar()->setValue(titleList->horizontalScrollBar()->value() + step);
    });
}

void MainWindow::updateStatusBar(){   //更新状态栏
    QTextCursor cursor = currentEditor->textCursor();
    int line = cursor.blockNumber() + 1;   // 获取当前行（从 0 开始，所以 +1）
    int column = cursor.positionInBlock() + 1; // 获取列（从 0 开始，所以 +1）
    cursorPosLabel->setText(QString("行: %1, 列: %2").arg(line).arg(column));
    int charCount =currentEditor->toPlainText().length(); // 获取文本长度
    charCountLabel->setText(QString("字符数: %1").arg(charCount));
    int scale = qRound((currentSize / (double)defaultSize) * 100);
    fontRateLabel->setText(QString("%1%").arg(scale));
}

void MainWindow::on_newWAct_triggered()
{
    QString program = QCoreApplication::applicationFilePath();  // 获取当前程序的可执行路径
    QProcess::startDetached(program);  // 以新进程启动
}

void MainWindow::on_openAct_triggered()   //打开文件
{
    QFileDialog Dialog(nullptr,tr("选择文件"),"","文本文档(*.txt);;所有文件(*.)");
    if(Dialog.exec()==QFileDialog::Accepted){   //选中文件并打开
        QString filePath=Dialog.selectedFiles().constFirst();
        if(textEditMap.contains(filePath)){   //已经打开
            currentEditor = textEditMap[filePath];
            editorStack->setCurrentWidget(currentEditor);
            fileId=filePath;   //记录
            return;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
            return;
        }

        //打开新文件
        fileId=filePath;   //更新当前文件id

        on_addNewButton_clicked();  //添加选项卡

        currentEditor=new QTextEdit(editorStack);

        //更新字号
        QFont currentFont = currentEditor->font();
        currentSize = currentFont.pointSize();

        //读文件
        QTextStream in(&file);
        currentEditor->setPlainText(in.readAll());
        file.close();

        //连接信号
        connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
        connect(currentEditor, &QTextEdit::cursorPositionChanged, this,  &MainWindow::updateStatusBar);

        editorStack->addWidget(currentEditor);
        editorStack->setCurrentWidget(currentEditor); //显示当前对应

        textEditMap.insert(fileId,currentEditor);

        updateStatusBar();  //更新状态栏
    }
}


void MainWindow::on_saveAct_triggered()
{
    QTextEdit *saveTextEditor=textEditMap[fileId];
    bool ok;
    if(fileId.toInt(&ok)){  //保存文件路径不存在
        QString filePath = QFileDialog::getSaveFileName(
            nullptr, "保存文件", "新建文档.txt", "文本文档(*.txt);;所有文件(*.)"
            );

        if (filePath.isEmpty()) {
            return;
        }

        //更新title   根据之前的fileid寻找item
        QListWidgetItem *item=nullptr;
        for (int i = 0; i < titleList->count(); ++i) {
            item = titleList->item(i);
            if (item->data(Qt::UserRole).toString() == fileId) {
                QWidget *itemWidget = titleList->itemWidget(item);
                item->setData(Qt::UserRole,filePath);//更新文件id
                if (itemWidget) {
                    QPushButton *titleLabel = itemWidget->findChild<QPushButton*>();
                    titleLabel->setText(QFileInfo(filePath).fileName()); // 更新文本
                }
            }
        }

        textEditMap.remove(fileId);//删除旧组合
        fileId=filePath;  //更新文本id
        textEditMap.insert(fileId,saveTextEditor);   //新组合
    }

    QSaveFile file(fileId);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
        return;
    }
    //写入数据
    QTextStream out(&file);
    out<<saveTextEditor->toPlainText();
    if (!file.commit()) {
        QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
    }
}

void MainWindow::on_closeAct_triggered(){
    QListWidgetItem* item=nullptr;
    for (int i = 0; i < titleList->count(); ++i) {
        item = titleList->item(i);
        if (item->data(Qt::UserRole).toString() == fileId) {
            delTitleItem(item);
        }
    }
}

void MainWindow::on_saveSAct_triggered()
{
    QTextEdit *saveTextEditor=textEditMap[fileId];
    QString filePath = QFileDialog::getSaveFileName(
        nullptr, "保存文件", "新建文档.txt", "文本文档(*.txt);;所有文件(*.)"
        );

    if (filePath.isEmpty()) {
        return;
    }

    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
        return;
    }

    //写入数据
    QTextStream out(&file);
    out<<saveTextEditor->toPlainText();
    if (!file.commit()) {
        QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
    }
}

void MainWindow::on_dateAct_triggered()
{
    QDateTime time = QDateTime::currentDateTime();  // 获取当前时间
    currentEditor->insertPlainText(time.toString("yyyy-MM-dd HH:mm"));
}


void MainWindow::on_delAct_triggered()
{
    QTextCursor cursor = currentEditor->textCursor();
    if (cursor.hasSelection()) {
        cursor.removeSelectedText();  // 仅删除选中内容
    }
}

void MainWindow::on_priAct_triggered()
{
    QPrinter printer;
    QPrintDialog dialog(&printer,currentEditor);
    if (dialog.exec() == QDialog::Accepted) {
        currentEditor->print(&printer);
    }
}


void MainWindow::on_enlargeAct_triggered()
{
    QFont font = currentEditor->font();
    currentSize = font.pointSize();
    font.setPointSize(currentSize + 1);  // 增加1个单位
    currentEditor->setFont(font);
    updateFontRateLabel();  // 更新放缩率
}


void MainWindow::on_reduceAct_triggered()
{
    QFont font = currentEditor->font();
    currentSize = font.pointSize();
    font.setPointSize(currentSize -1);  // 增加1个单位
    currentEditor->setFont(font);
    updateFontRateLabel();  // 更新放缩率
}

void MainWindow::updateFontRateLabel() {
    int scale = qRound((currentSize / (double)defaultSize) * 100);
    fontRateLabel->setText(QString("%1%").arg(scale));
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    // 判断是否按下 Ctrl 键
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        // 获取鼠标滚轮的旋转角度
        int delta = event->angleDelta().y();
        // 判断是滚轮向上还是向下
        if (delta > 0) {
            // 缩放字体变大
            changeFontSize(true);
        } else {
            // 缩放字体变小
            changeFontSize(false);
        }
    } else {
        // 如果没有按下 Ctrl 键，正常处理滚轮事件
        QMainWindow::wheelEvent(event);
    }
}

void MainWindow::changeFontSize(bool increase)
{
    // 获取当前字体
    QFont font = currentEditor->font();
    currentSize = font.pointSize();
    // 增加或减少字体大小
    font.setPointSize(currentSize + (increase ? 1 : -1));
    // 设置新的字体
    currentEditor->setFont(font);
    updateFontRateLabel();  // 更新放缩率
}

void MainWindow::on_statusBarAct_triggered(bool checked)
{
    statusBar()->setVisible(checked);  // 显示或隐藏状态栏
}

void MainWindow::on_addNewButton_clicked()   //每次点击添加选项卡添加新编辑器
{
    QToolButton *btn = qobject_cast<QToolButton*>(sender());
    if (btn) {
        fileId.clear();
    }

    QWidget *itemWidget=new QWidget;
    QListWidgetItem *item = new QListWidgetItem(titleList);
    QHBoxLayout * HLayout=new QHBoxLayout;

    QString titleText=fileId.isEmpty()?"无标题":QFileInfo(fileId).fileName();
    QPushButton * titleLabel=new QPushButton(titleText);
    titleLabel->setFixedSize(80,40);
    titleLabel->setStyleSheet("QPushButton {"
                              "border: none;"
                              "background-color: transparent;"
                              "outline: none;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #CAD3DD;"  // 悬停时的背景色
                              "}");

    QPushButton *closeBtn=new QPushButton();
    closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeBtn->setFixedSize(30,40);
    closeBtn->setStyleSheet("QPushButton {"
                            "border: none;"
                            "background-color: transparent;"
                            "outline: none;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #CAD3DD;"  // 悬停时的背景色
                            "}");

    item->setSizeHint(titleList->sizeHint());
    HLayout->addWidget(titleLabel);
    HLayout->addWidget(closeBtn);

    HLayout->setSpacing(0);  // 去除组件之间的间距
    HLayout->setContentsMargins(0, 0,0, 0); // 去除边距
    QFrame *frame = new QFrame(itemWidget);
    frame->setFrameShape(QFrame::VLine);  // 设置为竖直线
    frame->setFrameShadow(QFrame::Sunken); // 设置线的阴影效果
    HLayout->addWidget(frame);

    static int untitledIndex = 1;  // 记录未保存文档索引
    if(fileId.isEmpty()){    //无物理路径
        fileId = QString::number(untitledIndex++);
    }

    currentEditor=new QTextEdit(editorStack);

    QFont font = currentEditor->font();
    currentSize=font.pointSize();

    editorStack->addWidget(currentEditor);
    textEditMap.insert(fileId,currentEditor);
    editorStack->setCurrentWidget(currentEditor);

    connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    connect(currentEditor, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);

    item->setData(Qt::UserRole,fileId);
    itemWidget->setLayout(HLayout);
    item->setSizeHint(itemWidget->sizeHint()); // 让 item 适应 widget 大小
    titleList->addItem(item);
    titleList->setItemWidget(item, itemWidget);

    connect(closeBtn,&QPushButton::clicked,this,[=](){  //关联删除槽函数
        delTitleItem(item);
    });

    connect(titleLabel,&QPushButton::clicked,this,[=](){  //关联点击槽函数
        clickedTitleItem(item);
    });
}

void MainWindow::delTitleItem(QListWidgetItem *item)
{
    QString delFileId=item->data(Qt::UserRole).toString();//获得当前删除的fileId
    QTextEdit *delEditor=textEditMap.value(delFileId,nullptr);
    int index = titleList->row(item);  // 获取行号
    bool ok;

    if(!(delEditor->document()->isEmpty()&&delFileId.toInt(&ok))){   ////删除的标签项的编辑器是空的,且路径是不存在，不询问直接关闭   //其他询问保存
        bool needSave=false;    //保存意愿
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存确认","该文件尚未保存，是否保存？",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                                      );
        if (reply == QMessageBox::Yes) {
            needSave=true;
        }else if(reply == QMessageBox::Cancel){    //取消
            return;
        }

        if(needSave){   //愿意保存
            QString tempPath=fileId;
            fileId=delFileId;
            on_saveAct_triggered();
            fileId=tempPath;
        }
    }

    QListWidgetItem *itemToRemove = titleList->takeItem(index);
    delete itemToRemove;  // 删除项

    if (titleList->count() == 0) {
        qDebug() << "所有 items 已删除，关闭窗口";
        this->close();
        return;  // 确保函数在此处返回，避免后续访问空指针
    }

    editorStack->removeWidget(delEditor);
    textEditMap.remove(delFileId);

    if(currentEditor==delEditor){   //关闭的是当前的
        QListWidgetItem *nextItem = nullptr;
        if (index < titleList->count()) { // 优先选择后一项
            nextItem = titleList->item(index);
        } else if (index > 0) { // 如果没有后一项，选择前一项
            nextItem = titleList->item(index - 1);
        }
        if (nextItem) {
            fileId = nextItem->data(Qt::UserRole).toString();
            editorStack->removeWidget(currentEditor);
            currentEditor = textEditMap.value(fileId, nullptr);
            if (currentEditor) {
                editorStack->setCurrentWidget(currentEditor);
            } else {
                qDebug() << "未找到对应的编辑器";
            }
        }
    }
}

void MainWindow::clickedTitleItem(QListWidgetItem *item)
{
    fileId= item->data(Qt::UserRole).toString();//获得
    currentEditor=textEditMap[fileId];
    editorStack->setCurrentWidget(currentEditor); //切
    // 更新状态栏
    updateStatusBar();

}


void MainWindow::on_newAct_triggered()
{
    fileId.clear();
    on_addNewButton_clicked();
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


