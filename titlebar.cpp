#include "SignalRelay.h"
#include "titlebar.h"
#include "ui_titlebar.h"
#include <QScrollBar>
#include<QListView>
#include<QFile>
#include<QListWidgetItem>
#include <QFileInfo>
#include<QListWidget>



/*      初始化      */
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

void titleBar::initResource()
{
    titleIco = ":/Icon/application.png";
    leftBtnIco=":/Icon/left.png";
    rigthBtnIco=":/Icon/rigth.png";
    currentItem=nullptr;
    addBtnIco=":/Icon/add.png";
    index=1;  //未保存文件生成索引
    fileId = SignalRelay::instance().getFileId();
    QFile file(":/styles/titleBar.qss"); // QSS文件路径
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }

}

void titleBar::initContents(){
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
    QPixmap addPixMap(addBtnIco);
    QIcon addBtnIcon(addPixMap.scaled(25,25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->addNewBtn->setIcon(addBtnIcon);

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
    ui->titleLayout->setSpacing(0);
    ui->titleLayout->setContentsMargins(0, 0, 0, 0);
}


void titleBar::initSignal(){
    connect(ui->minimizeBtn, &QToolButton::clicked,&SignalRelay::instance(),&SignalRelay::requestMinimize);  //最小
    connect(ui->maximizeBtn, &QToolButton::clicked,&SignalRelay::instance(),&SignalRelay::requestMaximize);//最大/还原
    connect(ui->closeBtn, &QToolButton::clicked,&SignalRelay::instance(), &SignalRelay::requestClose); //关闭
    // 连接按钮和滚动功能
    connect(ui->leftBtn, &QPushButton::clicked, this, [=]() {
        int step = ui->titleList->visualItemRect(currentItem).width(); // 动态获取 item 宽度
        ui->titleList->horizontalScrollBar()->setValue(ui->titleList->horizontalScrollBar()->value() - step);
    });

    connect(ui->rightBtn, &QPushButton::clicked, this, [=]() {
        int step = ui->titleList->visualItemRect(currentItem).width(); // 动态获取 item 宽度
        ui->titleList->horizontalScrollBar()->setValue(ui->titleList->horizontalScrollBar()->value()+step);
    });

    connect(&SignalRelay::instance(),&SignalRelay::delCurrentItemRequest,this,&titleBar::delCurrentItem);  //关闭当前标签页
    connect(&SignalRelay::instance(),&SignalRelay::addTabRequested,this,&titleBar::on_addNewBtn_clicked); //添加标签项
    connect(&SignalRelay::instance(),&SignalRelay::updateTitleRequested,this,&titleBar::updateTitle);  //更新标签状态
}




QListWidgetItem* titleBar::getNewItem(){
    QWidget *itemWidget=new QWidget;
    QListWidgetItem *item = new QListWidgetItem();
    QHBoxLayout * HLayout=new QHBoxLayout;
    QPushButton * titleLabel=new QPushButton(tr("无标题"));
    QPushButton *closeBtn=new QPushButton();
    QFrame *frame = new QFrame();

    titleLabel->setFixedSize(80,40);
    closeBtn->setFixedSize(30,40);
    closeBtn->setObjectName("closeBtn");
    item->setSizeHint(ui->titleList->sizeHint());
    frame->setFrameShape(QFrame::VLine);  // 设置为竖直线
    frame->setFrameShadow(QFrame::Sunken); // 设置线的阴影效果

    if(!fileId->isEmpty()){
        titleLabel->setText(QFileInfo(*fileId).fileName());
    }else{
        *fileId=QString::number(index++);
    }

    item->setData(Qt::UserRole,*fileId);
    item->setData(Qt::UserRole+1,false);

    connect(closeBtn,&QPushButton::clicked,this,[=](){delTitleItem(item);});

    connect(titleLabel,&QPushButton::clicked,this,[=](){clickedTitleItem(item);});

    HLayout->addWidget(titleLabel);
    HLayout->addWidget(closeBtn);
    HLayout->setSpacing(0);  // 去除组件之间的间距
    HLayout->setContentsMargins(0, 0,0, 0); // 去除边距
    HLayout->addWidget(frame);

    itemWidget->setLayout(HLayout);
    item->setSizeHint(itemWidget->sizeHint()); // 让 item 适应 widget 大小
    ui->titleList->addItem(item);
    ui->titleList->setItemWidget(item, itemWidget);

    return item;
}



/*      槽函数实现      */
void titleBar::on_addNewBtn_clicked(){

    QPushButton * btn=qobject_cast<QPushButton*>(sender());
    if(btn){
        fileId->clear();
    }
    currentItem=getNewItem();     //添加并更新当前的item
    SignalRelay::instance().requestedSwitchEditor();
    SignalRelay::instance().requestPerformUpdateStatusBar();
}

void titleBar::delTitleItem(QListWidgetItem *item)
{
    clickedTitleItem(item);   //先切换到对应的标签
    *fileId=item->data(Qt::UserRole).toString();   //获得标签中的数据作为新的fileid
    SignalRelay::instance().requestedSwitchEditor();  //切换当前的id的编辑器
    SignalRelay::instance().requestedCloseEditor();   //请求移除这个id对应的编辑器

    int index = ui->titleList->row(item);  // 获取行号
    QListWidgetItem *itemToRemove = ui->titleList->takeItem(index);
    delete itemToRemove;  // 删除项

    //找下一项目
    QListWidgetItem *nextItem = nullptr;
    if (index < ui->titleList->count()) {
        nextItem = ui->titleList->item(index);
    } else if (index > 0) { // 如果没有后一项，选择前一项
        nextItem =ui->titleList->item(index - 1);
    }

    if(nextItem){   //找到新标签
        clickedTitleItem(nextItem);   //切换下一个
    }else{   //没有找到新标签
        SignalRelay::instance().requestedCloseEditor();   //请求移除这个id对应的编辑器
        SignalRelay::instance().requestClose();
    }
    currentItem=nextItem;
}

void titleBar::clickedTitleItem(QListWidgetItem *item)   //点击切换
{
    currentItem=item;
    *fileId=currentItem->data(Qt::UserRole).toString();//点击的标签
    SignalRelay::instance().requestedSwitchEditor();
     SignalRelay::instance().requestPerformUpdateStatusBar();
}

void titleBar::updateTitle(){   //更新title
    currentItem->data(Qt::UserRole).toString().clear();
    currentItem->setData(Qt::UserRole,*fileId);
    QWidget *widget =ui->titleList->itemWidget(currentItem);
    if (widget) {
        QList<QPushButton *> buttons = widget->findChildren<QPushButton *>();
        buttons[0]->setText(QFileInfo(*fileId).fileName());
    }
}

void titleBar::delCurrentItem()   //关闭当前选项卡
{
    delTitleItem(currentItem);
}




/*      拖拽，双击放大      */
void titleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - window()->frameGeometry().topLeft();
        event->accept();
    }
}

void titleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        window()->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void titleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        SignalRelay::instance().requestMaximize();
        event->accept();
    }
}
