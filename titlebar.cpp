#include "global.h"
#include "SignalRelay.h"
#include "titlebar.h"
#include "ui_titlebar.h"
#include <QScrollBar>
#include<QListView>
#include<QFile>
#include<QListWidgetItem>
#include <QFileInfo>

void titleBar::initResource()
{  // this->setStyleSheet("background-color: #3498db;");
    titleIco = ":/Icon/application.png";
    leftBtnIco=":/Icon/left.png";
    rigthBtnIco=":/Icon/rigth.png";
    QFile file(":/styles/titleBar.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}


titleBar::titleBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::titleBar)
{
    initResource();
    ui->setupUi(this);
    setFixedHeight(40);
    installEventFilter(this);

    ui->titleLayout->setSpacing(0);
    ui->titleLayout->setContentsMargins(0, 0, 0, 0);

    //添加图标
    ui->titleIcon->setPixmap(QPixmap(titleIco).scaled(30,30, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 添加标题栏列表
     ui->titleList->setViewMode(QListView::IconMode);
     ui->titleList->setFlow(QListView::LeftToRight);
     ui->titleList->setResizeMode(QListView::Adjust);
     ui->titleList->setWrapping(false);  // 禁用换行，确保项目不会换行
     ui->titleList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     ui->titleList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     ui->titleList->setFrameShape(QFrame::NoFrame); //无边

    // 加载图像并调整大小
    QPixmap leftPixMap(leftBtnIco);
    QIcon leftBtnIcon(leftPixMap.scaled(25,25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->leftBtn->setIcon(leftBtnIcon);

    QPixmap rigthPixMap(rigthBtnIco);
    QIcon rigthBtnIcon(rigthPixMap.scaled(25,25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->rightBtn->setIcon(rigthBtnIcon);

    // 添加功能按钮
    ui->addNewBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));

    // 添加到标题栏布局
    ui->titleLayout->addWidget(ui->titleIcon);
    ui->titleLayout->addWidget(ui->leftBtn);
    ui->titleLayout->addWidget(ui->titleList);
    ui->titleLayout->addWidget(ui->rightBtn);
    ui->titleLayout->addWidget(ui->addNewBtn);
    ui->titleLayout->addWidget(ui->minimizeBtn);
    ui->titleLayout->addWidget(ui->maximizeBtn);
    ui->titleLayout->addWidget(ui->closeBtn);

    setLayout(ui->titleLayout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->titleLayout->setSpacing(0);
    setContentsMargins(0, 0, 0, 0);
    ui->titleLayout->setContentsMargins(0, 0, 0, 0);
    initSignal();
}

titleBar::~titleBar()
{
    delete ui;
}

void titleBar::initSignal(){
    connect(ui->minimizeBtn, &QToolButton::clicked,&SignalRelay::instance(),&SignalRelay::requestMinimize);
    connect(ui->maximizeBtn, &QToolButton::clicked,&SignalRelay::instance(),&SignalRelay::requestMaximize);
    connect(ui->closeBtn, &QToolButton::clicked,&SignalRelay::instance(), &SignalRelay::requestClose);

    connect(ui->addNewBtn, &QToolButton::clicked, this,[=]{  //添加选项卡
            titleBar::on_addNewBtn_clicked(fileId);
    });
    // 连接按钮和滚动功能
    connect(ui->leftBtn, &QPushButton::clicked, this, [=]() {
        int step = 100; // 设定滚动步长
        ui->titleList->horizontalScrollBar()->setValue(ui->titleList->horizontalScrollBar()->value() - step);
    });

    connect(ui->rightBtn, &QPushButton::clicked, this, [=]() {
        int step = 100;
        ui->titleList->horizontalScrollBar()->setValue(ui->titleList->horizontalScrollBar()->value() + step);
    });
}


void titleBar::on_addNewBtn_clicked(QString& fileId){
    QWidget *itemWidget=new QWidget;
    QListWidgetItem *item = new QListWidgetItem();
    QHBoxLayout * HLayout=new QHBoxLayout;
    QString titleText=fileId.isEmpty()?"无标题":QFileInfo(fileId).fileName();   //空则无标题或获取文件名
    QPushButton * titleLabel=new QPushButton(titleText);
    titleLabel->setFixedSize(80,40);
    QPushButton *closeBtn=new QPushButton();
    closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeBtn->setFixedSize(30,40);

    item->setSizeHint(ui->titleList->sizeHint());
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

    // currentEditor=new QTextEdit(editorStack);
    // QFont font = currentEditor->font();
    // currentSize=font.pointSize();

    // editorStack->addWidget(currentEditor);
    // textEditMap.insert(fileId,currentEditor);
    // editorStack->setCurrentWidget(currentEditor);

    // connect(currentEditor, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    // connect(currentEditor, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);

    item->setData(Qt::UserRole,fileId);
    itemWidget->setLayout(HLayout);
    item->setSizeHint(itemWidget->sizeHint()); // 让 item 适应 widget 大小
    ui->titleList->addItem(item);
    ui->titleList->setItemWidget(item, itemWidget);

    // connect(closeBtn,&QPushButton::clicked,this,[=](){  //关联删除槽函数
    //     delTitleItem(item);
    // });

    // connect(titleLabel,&QPushButton::clicked,this,[=](){  //关联点击槽函数
    //     clickedTitleItem(item);
    // });
}

// void titleBar::delTitleItem(QListWidgetItem *item)
// {
//     QString delFileId=item->data(Qt::UserRole).toString();//获得当前删除的fileId
//     QTextEdit *delEditor=textEditMap.value(delFileId,nullptr);
//     int index = ui->titleList->row(item);  // 获取行号
//     bool ok;
//     //delEditor->document()->modificationChanged();
//     if(!(delEditor->document()->isEmpty()&&delFileId.toInt(&ok))){   //删除的标签项的编辑器是空的,且路径是不存在，不询问直接关闭   //其他询问保存
//         bool needSave=false;    //保存意愿
//         QMessageBox::StandardButton reply;
//         reply = QMessageBox::question(this, "保存确认","该文件尚未保存，是否保存？",
//                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
//                                       );
//         if (reply == QMessageBox::Yes) {
//             needSave=true;
//         }else if(reply == QMessageBox::Cancel){    //取消
//             return;
//         }

//         if(needSave){   //愿意保存
//             QString tempPath=fileId;
//             fileId=delFileId;
//             on_saveAct_triggered();
//             fileId=tempPath;
//         }
//     }

//     QListWidgetItem *itemToRemove = titleList->takeItem(index);
//     delete itemToRemove;  // 删除项

//     if (titleList->count() == 0) {
//         qDebug() << "所有 items 已删除，关闭窗口";
//         this->close();
//         return;  // 确保函数在此处返回，避免后续访问空指针
//     }

//     editorStack->removeWidget(delEditor);
//     textEditMap.remove(delFileId);

//     if(currentEditor==delEditor){   //关闭的是当前的
//         QListWidgetItem *nextItem = nullptr;
//         if (index < titleList->count()) { // 优先选择后一项
//             nextItem = titleList->item(index);
//         } else if (index > 0) { // 如果没有后一项，选择前一项
//             nextItem = titleList->item(index - 1);
//         }
//         if (nextItem) {
//             fileId = nextItem->data(Qt::UserRole).toString();
//             editorStack->removeWidget(currentEditor);
//             currentEditor = textEditMap.value(fileId, nullptr);
//             if (currentEditor) {
//                 editorStack->setCurrentWidget(currentEditor);
//             } else {
//                 qDebug() << "未找到对应的编辑器";
//             }
//         }
//     }
// }
