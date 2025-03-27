#include "SignalRelay.h"
#include "titlebar.h"
#include "ui_titlebar.h"
#include <QScrollBar>
#include<QListView>
#include<QFile>
#include<QListWidgetItem>
#include <QFileInfo>
#include<QListWidget>

void titleBar::initResource()
{
    titleIco = ":/Icon/application.png";
    leftBtnIco=":/Icon/left.png";
    rigthBtnIco=":/Icon/rigth.png";
    currentItem=nullptr;
    QFile file(":/styles/titleBar.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void titleBar::initContents(){
    setFixedHeight(40);

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
}


titleBar::titleBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::titleBar)
{

    ui->setupUi(this);
    initResource();
    initContents();
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
    connect(&SignalRelay::instance(),&SignalRelay::addTabRequested,this,&titleBar::on_addNewBtn_clicked);
    connect(&SignalRelay::instance(),&SignalRelay::delCurrentItemRequest,this,&titleBar::delCurrentItem);
    connect(ui->addNewBtn, &QToolButton::clicked, this,[=]{  //添加选项卡
            titleBar::on_addNewBtn_clicked(SignalRelay::instance().FileId());     //添加选项卡,发送添加编辑器的请求
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
    connect(&SignalRelay::instance(),&SignalRelay::setTitleTextRequested,this,&titleBar::setTitleText);
}


void titleBar::on_addNewBtn_clicked(QString& fileId){
    QPushButton * btn=qobject_cast<QPushButton*>(sender());
    if(btn){
        fileId.clear();
    }
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
    SignalRelay::instance().FileId()=fileId.isEmpty()?SignalRelay::instance().Index():fileId;
    item->setData(Qt::UserRole,fileId);
    itemWidget->setLayout(HLayout);
    item->setSizeHint(itemWidget->sizeHint()); // 让 item 适应 widget 大小
    ui->titleList->addItem(item);
    ui->titleList->setItemWidget(item, itemWidget);
    connect(closeBtn,&QPushButton::clicked,this,[=](){
        clickedTitleItem(item);
        QListWidgetItem *i=delTitleItem(item);
        if(!i){
            clickedTitleItem(i);   //切换下一个
            QString file=i->data(Qt::UserRole).toString();
            SignalRelay::instance().requestedSwitchEditor(file);  //切换下一个编辑器
        }
    });

    connect(titleLabel,&QPushButton::clicked,this,[=](){  //关联点击槽函数
        clickedTitleItem(item);
    });
    currentItem=item;   //更新item
    clickedTitleItem(item);
}

QListWidgetItem* titleBar::delTitleItem(QListWidgetItem *item)
{
    QString fileID=item->data(Qt::UserRole).toString();
    SignalRelay::instance().requestedCloseEditor(fileID);   //请求移除这个id对应的
    int index = ui->titleList->row(item);  // 获取行号
    QListWidgetItem *itemToRemove = ui->titleList->takeItem(index);

    delete itemToRemove;  // 删除项

    QListWidgetItem *nextItem = nullptr;
    if (index <  ui->titleList->count()) { // 优先选择后一项
        nextItem =  ui->titleList->item(index);
    } else if (index > 0) { // 如果没有后一项，选择前一项
        nextItem =  ui->titleList->item(index - 1);
    }
    if(nextItem==nullptr){
        SignalRelay::instance().requestClose();
    }
    currentItem=nextItem;
    return nextItem;
}

void titleBar::clickedTitleItem(QListWidgetItem *item)   //点击切换
{
    currentItem=item;
    QString switchFileId=item->data(Qt::UserRole).toString();
    SignalRelay::instance().updateFileId(switchFileId);
    SignalRelay::instance().requestedSwitchEditor(switchFileId);
}


void titleBar::setTitleText(QString &fileId){   //更新title
    qDebug()<<"item:"<<fileId;
    currentItem->data(Qt::UserRole).toString().clear();
    currentItem->setData(Qt::UserRole,fileId);
    QWidget *widget =ui->titleList->itemWidget(currentItem);
    if (widget) {
        QList<QPushButton *> buttons = widget->findChildren<QPushButton *>();
        buttons[0]->setText(QFileInfo(fileId).fileName());
    }
}

void titleBar::delCurrentItem(QString &fileId)
{
    QListWidgetItem* i=delTitleItem(currentItem);
    if(!i){
        clickedTitleItem(i);   //切换下一个
        QString file=i->data(Qt::UserRole).toString();
        SignalRelay::instance().requestedSwitchEditor(file);  //切换下一个编辑器
    }
    // for (int i = 0; i <ui->titleList->count(); ++i) {   //可以删除有关联的
    //     QListWidgetItem *item = ui->titleList->item(i);
    //     if (item && item->data(Qt::UserRole).toString() == fileId) {
    //         delTitleItem(item); // 删除 item 并释放内存
    //         break; // 找到并删除后退出循环
    //     }
    // }
}
