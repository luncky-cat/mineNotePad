#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SignalRelay.h"
#include "titlebar.h"
#include "topresizearea.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initResource();   //初始化资源
    initTitleBar();  //初始化标题栏
    initCenter();  //初始化中心界面
    initStatusBar();//初始化状态栏
    initSignal();   //连接信号
    initStyle();  //加载qss

    SignalRelay::instance().requestAddTab();
    addNewEditor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initResource(){

    appIcon = ":/Icon/application.png";
    currentEditor=nullptr;
    defaultEditor=new QTextEdit();   //添加默认
    newEditor=nullptr;
    defaultSize=14;
    currentSize=14;
    statusBarTimer=new QTimer(this);
    statusBarTimer->setInterval(100);
    statusBarTimer->setSingleShot(true);
}

void MainWindow::initTitleBar()
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window); //隐藏
    resizeArea=new TopResizeArea(this);  //最顶上
    titleBar_=new titleBar(this); //自定义标题栏
    fileId =titleBar_->fileId;   //初始化共享指针
}

void MainWindow::initCenter()
{
    ui->statusBarAct->setChecked(true);    // 默认勾选

    delete ui->toolBar;

    QWidget *centralContainer = new QWidget(this);

    editorStack = new QStackedWidget(centralContainer);
    editorStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 让它填充整个界面
    QFont defaultFont;
    defaultFont.setPointSize(defaultSize);  // 设置默认大小
    editorStack->setFont(defaultFont);
    editorStack->addWidget(defaultEditor);

    QVBoxLayout *layout = new QVBoxLayout(centralContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(resizeArea);
    layout->addWidget(titleBar_);
    layout->addWidget(ui->menubar);
    layout->addWidget(editorStack);  // QStackedWidget 保持在 centralWidget 里
    centralContainer->setLayout(layout);
    setCentralWidget(centralContainer);

    setWindowIcon(QIcon(appIcon));//设置运行图标
}

void MainWindow::initStatusBar()
{
    cursorPosLabel = new QLabel("行: 0, 列: 0", this);  // 位置
    charCountLabel = new QLabel("字符数: 0", this);     // 字符数量
    fontRateLabel = new QLabel("100%", this);     // 放缩率
    cursorPosLabel->setMinimumWidth(100);
    charCountLabel->setMinimumWidth(100);
    fontRateLabel->setMinimumWidth(100);
    statusBar()->setStyleSheet("background-color: #F0F5F9;"); // 设置浅蓝色背景
    statusBar()->addWidget(cursorPosLabel);
    statusBar()->addWidget(charCountLabel);
    statusBar()->addWidget(fontRateLabel);
    statusBar()->setFixedHeight(30);
    ui->menubar->setStyleSheet("background-color:#F0F5F9");
}


void MainWindow::initSignal(){
    connect(&SignalRelay::instance(),&SignalRelay::minimizeRequested,this,&QMainWindow::showMinimized);  //最小
    connect(&SignalRelay::instance(), &SignalRelay::maximizeRequested,this,&MainWindow::showMax);//最大/还原
    connect(&SignalRelay::instance(), &SignalRelay::closeRequested,this, &QMainWindow::close); //关闭
    connect(&SignalRelay::instance(),&SignalRelay::switchEditorRequested,this,&MainWindow::switchEditor);  //切换
    connect(&SignalRelay::instance(), &SignalRelay::closeEditorRequested, this, &::MainWindow::oncloseEditor);  //关闭
    connect(&SignalRelay::instance(),&SignalRelay::addNewEditorRequested,this,&MainWindow::addNewEditor);//新建编辑器

    connect(ui->cutAct, &QAction::triggered, this, &MainWindow::cutText);
    connect(ui->copyAct, &QAction::triggered, this, &MainWindow::copyText);
    connect(ui->patseAct, &QAction::triggered, this, &MainWindow::pasteText);
    connect(ui->delAct, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->selectAllAct, &QAction::triggered, this, &MainWindow::selectAllText);
    connect(ui->revokeAct, &QAction::triggered, this, &MainWindow::undoText);

    connect(defaultEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //文本改变绑定新编辑器
    connect(statusBarTimer, &QTimer::timeout, this, &MainWindow::performUpdateStatusBar);
    connect(defaultEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    connect(defaultEditor, &QTextEdit::cursorPositionChanged,this, &MainWindow::updateStatusBar);

}

void MainWindow::switchEditor(){    //切换id对应编辑器
    qDebug()<<"切换编辑器:"<<*fileId;
    currentEditor=defaultEditor;  //默认
    if(textEditMap.contains(*fileId)){
        bool changed;
        std::tie(currentEditor,changed) = textEditMap[*fileId];
    }
    editorStack->setCurrentWidget(currentEditor);
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
    disconnect(defaultEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //断开共享页连接
    currentEditor=new QTextEdit(editorStack);
    currentEditor->setText(defaultEditor->toPlainText());  //复制文本
    editorStack->addWidget(currentEditor);
    editorStack->setCurrentWidget(currentEditor);
    textEditMap.insert(*fileId,qMakePair(currentEditor,true));
    connect(statusBarTimer, &QTimer::timeout, this, &MainWindow::performUpdateStatusBar);
    connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    connect(currentEditor, &QTextEdit::cursorPositionChanged,this, &MainWindow::updateStatusBar);
    defaultEditor->clear();
    connect(defaultEditor,&QTextEdit::textChanged,this,&MainWindow::bindNewEditor);   //重新绑定
    qDebug()<<"绑定新编辑器:"<<*fileId<<"对应地址:"<<currentEditor;
}

void MainWindow::showMax(){
    this->isMaximized()?this->showNormal(): this->showMaximized();
}

bool MainWindow::addNewEditor()
{
    qDebug()<<"新建编辑器:"<<*fileId;
    currentEditor=defaultEditor;
    bool ok;
    if(fileId->toInt(&ok)){    //空的为新建按钮点击
        editorStack->setCurrentWidget(currentEditor);
         qDebug()<<"空的为新建按钮";
        return true;
    }

    if(textEditMap.contains(*fileId)){   //已经打开
        switchEditor();
        qDebug()<<"已经存在，切换";
        //更新title  点击
        return false;
    }

    QFile file(*fileId);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
        return false;
    }

    currentEditor=new QTextEdit(editorStack);

    QTextStream in(&file); //读文件
    currentEditor->setPlainText(in.readAll());
    file.close();

    //连接信号
    connect(statusBarTimer, &QTimer::timeout, this, &MainWindow::performUpdateStatusBar);
    connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    connect(currentEditor, &QTextEdit::cursorPositionChanged,this, &MainWindow::updateStatusBar);
    connect(currentEditor, &QTextEdit::textChanged, this, [=]() {textEditMap[*fileId].second = true;});

    editorStack->addWidget(currentEditor);
    editorStack->setCurrentWidget(currentEditor); //显示当前对应
    textEditMap.insert(*fileId,qMakePair(currentEditor,false));  //插入新组合

    return true;
}



void MainWindow::updateStatusBar() {
    statusBarTimer->start();
}

void MainWindow::performUpdateStatusBar() {
    updateCursorStatus();
    updateTextStats();
}

void MainWindow::updateCursorStatus() {
    QTextCursor cursor = currentEditor->textCursor();
    int line = cursor.blockNumber() + 1;      // 行号（从0开始，加1）
    int column = cursor.positionInBlock();  // 列号（从0开始，加1）
    cursorPosLabel->setText(QString("行: %1, 列: %2").arg(line).arg(column));
}

void MainWindow::updateTextStats() {
    int charCount = currentEditor->toPlainText().length(); // 文本长度
    charCountLabel->setText(QString("字符数: %1").arg(charCount));
    int scale = qRound((currentSize / (double)defaultSize) * 100);
    fontRateLabel->setText(QString("%1%").arg(scale));
}


void MainWindow::on_newWAct_triggered()
{
    QString program = QCoreApplication::applicationFilePath();  // 获取当前程序的可执行路径
    QProcess::startDetached(program);  // 以新进程启动
}


void MainWindow::on_newAct_triggered()  //新建选项卡
{
    fileId->clear();
    SignalRelay::instance().requestAddTab();
}

void MainWindow::on_openAct_triggered()   //打开文件
{
    QFileDialog Dialog(nullptr,tr("选择文件"),"","文本文档(*.txt);;所有文件(*.)");
    if(Dialog.exec()==QFileDialog::Accepted){   //选中文件并打开
        QString filePath = Dialog.selectedFiles().constFirst();
        QFileInfo fileInfo(filePath);  // 只创建一次 QFileInfo，提高效率
        if (fileInfo.exists() && fileInfo.isFile()) {
            *fileId = filePath;
        }
        else{
            return;
        }
        bool flag=addNewEditor();   //是否需要添加编辑器
        if(flag){
            SignalRelay::instance().addTabRequested();   //添加选项卡
        }
    }
}


void MainWindow::on_saveAct_triggered()
{
    bool ok;
    if(fileId->toInt(&ok)){  //保存文件路径不存在
        QString filePath = QFileDialog::getSaveFileName(
            nullptr, "保存文件", "新建文档.txt", "文本文档(*.txt);;所有文件(*.)"
            );

        QFileInfo fileInfo(filePath);  // 只创建一次 QFileInfo，提高效率
        if (fileInfo.exists() && fileInfo.isFile()) {
            qDebug()<<"filepath:"<<filePath;
            textEditMap.remove(*fileId);//删除旧组合
            *fileId=filePath;
            textEditMap.insert(*fileId,qMakePair(currentEditor,false));   //新组合

            SignalRelay::instance().requestSetTitleText(filePath);  //更新title
        }else{
            return;
        }
    }

    QSaveFile file(*fileId);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "文件打开失败", "无法打开文件：" + file.errorString());
        return;
    }

    //写入数据
    qDebug()<<"写入到:"<<*fileId;
    QTextStream out(&file);
    out<<currentEditor->toPlainText();
    if (!file.commit()) {
        QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
    }
}


void MainWindow::oncloseEditor(){
    if(textEditMap.contains(*fileId)){    //存在
        qDebug()<<"关闭对应已经编辑的编辑器:"<<*fileId;
        if(textEditMap.value(*fileId).second){   //改变了
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "保存确认","该文件尚未保存，是否保存？",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes) {
                switchEditor();  //切换编辑器
                on_saveAct_triggered();//保持
            }else if(reply == QMessageBox::Cancel){    //取消
                return;
            }
        }
        editorStack->removeWidget(textEditMap.value(*fileId).first);   //移出容器
        textEditMap.remove(*fileId);  //移除组合
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

void MainWindow::on_closeAct_triggered(){
    SignalRelay::instance().requestDelCurrentItem();
}

void MainWindow::on_saveSAct_triggered()   //优化
{
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
    out<<currentEditor->toPlainText();
    if (!file.commit()) {
        QMessageBox::critical(this, "文件保存失败", "无法保存文件：" + file.errorString());
    }
    SignalRelay::instance().requestSetTitleText(filePath);
}


void MainWindow::on_priAct_triggered()
{
    QPrinter printer;
    QPrintDialog dialog(&printer,currentEditor);
    if (dialog.exec() == QDialog::Accepted) {
        currentEditor->print(&printer);
    }
}

void MainWindow::on_dateAct_triggered()
{
    QDateTime time = QDateTime::currentDateTime();  // 获取当前时间
    currentEditor->insertPlainText(time.toString("yyyy-MM-dd HH:mm"));
}

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

void MainWindow::on_statusBarAct_triggered(bool checked)
{
    statusBar()->setVisible(checked);  // 显示或隐藏状态栏
}

MainWindow::ResizeRegion MainWindow::detectResizeRegion(const QPoint &pos)
{
    ResizeRegion region = None;
    QRect rect = this->rect();
    bool left = pos.x() <= m_borderWidth;
    bool right = pos.x() >= rect.width() - m_borderWidth;
    bool top = pos.y() <= m_borderWidth;
    bool bottom = pos.y() >= rect.height() - m_borderWidth;

    if (top && left)
        region = TopLeft;
    else if (top && right)
        region = TopRight;
    else if (bottom && left)
        region = BottomLeft;
    else if (bottom && right)
        region = BottomRight;
    else if (left)
        region = Left;
    else if (right)
        region = Right;
    else if (top)
        region = Top;
    else if (bottom)
        region = Bottom;
    return region;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_resizeRegion = detectResizeRegion(event->pos());
        if (m_resizeRegion != None) {
            m_resizing = true;
            m_dragPos = event->globalPos();
            event->accept();
            return;
        }
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 如果正在调整大小，则计算鼠标位移更新窗口尺寸
    if (m_resizing) {
        QPoint diff = event->globalPos() - m_dragPos;
        QRect geom = geometry();

        switch (m_resizeRegion) {
        case Left:
            geom.setLeft(geom.left() + diff.x());
            break;
        case Right:
            geom.setRight(geom.right() + diff.x());
            break;
        case Top:
            geom.setTop(geom.top() + diff.y());
            break;
        case Bottom:
            geom.setBottom(geom.bottom() + diff.y());
            break;
        case TopLeft:
            geom.setTopLeft(geom.topLeft() + diff);
            break;
        case TopRight:
            geom.setTopRight(geom.topRight() + QPoint(diff.x(), diff.y()));
            break;
        case BottomLeft:
            geom.setBottomLeft(geom.bottomLeft() + QPoint(diff.x(), diff.y()));
            break;
        case BottomRight:
            geom.setBottomRight(geom.bottomRight() + diff);
            break;
        default:
            break;
        }

        // 限制最小尺寸
        if (geom.width() < minimumWidth())
            geom.setWidth(minimumWidth());
        if (geom.height() < minimumHeight())
            geom.setHeight(minimumHeight());

        setGeometry(geom);
        m_dragPos = event->globalPos();
        event->accept();
        return;
    }
    else {
        // 非调整大小状态，根据鼠标位置改变光标形状
        ResizeRegion region = detectResizeRegion(event->pos());
        Qt::CursorShape cursorShape = Qt::ArrowCursor;

        switch (region) {
        case Left:
        case Right:
            cursorShape = Qt::SizeHorCursor;
            break;
        case Top:
        case Bottom:
            cursorShape = Qt::SizeVerCursor;
            break;
        case TopLeft:
        case BottomRight:
            cursorShape = Qt::SizeFDiagCursor;
            break;
        case TopRight:
        case BottomLeft:
            cursorShape = Qt::SizeBDiagCursor;
            break;
        default:
            cursorShape = Qt::ArrowCursor;
            break;
        }
        setCursor(cursorShape);
    }

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_resizing = false;
    m_resizeRegion = None;
    setCursor(Qt::ArrowCursor);
    QMainWindow::mouseReleaseEvent(event);
}

