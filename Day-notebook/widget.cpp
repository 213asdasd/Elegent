#include "widget.h"
#include "ui_widget.h"

#include<QVBoxLayout>
#include<QFileDialog>
#include<QFontDialog>
#include<QMessageBox>
#include<QColorDialog>
#include<QStandardPaths>
#include<QHBoxLayout>
#include<QGridLayout>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    set_icon();


    menubar=new QMenuBar(this);


    menu1=new QMenu("文件");
    menu2=new QMenu("编辑");
    menu3=new QMenu("格式");
    menu4=new QMenu("帮助");

    menubar->addMenu(menu1);
    menubar->addMenu(menu2);
    menubar->addMenu(menu3);
    menubar->addMenu(menu4);
    menubar->addSeparator();

    newaction=new QAction("新建");
    openaction=new QAction("打开");
    saveaction=new QAction("保存");
    saveasaction=new QAction("另存为");

    newaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_1));
    openaction->setShortcut(tr("Ctrl+2"));
    saveaction->setShortcut(tr("Ctrl+3"));
    saveasaction->setShortcut(tr("Ctrl+4"));

    menu1->addAction(newaction);
    menu1->addAction(openaction);
    menu1->addSeparator();
    menu1->addAction(saveaction);
    menu1->addAction(saveasaction);




    cutaction=new QAction("剪切");
    copyaction=new QAction("复制");
    pasteaction=new QAction("粘贴");

    cutaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_5));
    copyaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_6));
    pasteaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_7));

    menu2->addAction(cutaction);
    menu2->addAction(copyaction);
    menu2->addAction(pasteaction);


    fontaction=new QAction("字体");
    coloraction=new QAction("格式");

    fontaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_8));
    coloraction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_9));

    menu3->addAction(fontaction);
    menu3->addAction(coloraction);

    helpaction=new QAction("帮助");

    helpaction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));

    menu4->addAction(helpaction);

    textedit=new QTextEdit(this);
    QVBoxLayout *layout= new QVBoxLayout(this);
    layout->setMenuBar(menubar);
    layout->addWidget(textedit);

    // 在构造函数中优化布局层次
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5); // 减少窗口边距[7](@ref)
    mainLayout->setSpacing(3);

    // 添加状态栏布局
    QHBoxLayout *statusLayout = new QHBoxLayout;
    statusLayout->addStretch(); // 左侧弹簧
    statusLayout->addWidget(ui->labelCount); // 字数统计右对齐
    mainLayout->addLayout(statusLayout);


    textedit->setFrameShape(QFrame::NoFrame); // 去除默认边框[5](@ref)
    textedit->setStyleSheet("QTextEdit { background: #F8F9FA; }");



    // 在构造函数中添加
    QFile styleFile(":/qss/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    qApp->setStyleSheet(styleSheet); // 全局生效[1](@ref)



    // QHBoxLayout *hlayout=new QHBoxLayout;
    // hlayout->addStretch();
    // hlayout->addWidget(textedit);

    // 底部布局
    // QHBoxLayout *bottomLayout = new QHBoxLayout;
    // bottomLayout->addStretch();// 左侧弹簧

    // 创建字数标签
    // ui->labelCount = new QLabel("字数: 0", this);
    // ui->labelCount->setAlignment(Qt::AlignRight | Qt::AlignVCenter); // 右对齐[4](@ref)
    // bottomLayout->addWidget(ui->labelCount);

    // 设置边距（右/下各留5像素）
    // bottomLayout->setContentsMargins(100,100, 50, 50);
    // mainLayout->addLayout(bottomLayout);



    connect(openaction,&QAction::triggered,this,&Widget::openFile);
    connect(fontaction,&QAction::triggered,this,&Widget::setfont);
    connect(coloraction,&QAction::triggered,this,&Widget::setcolor);
    connect(cutaction,&QAction::triggered,this,&Widget::cut);
    connect(copyaction,&QAction::triggered,this,&Widget::copy);
    connect(pasteaction,&QAction::triggered,this,&Widget::paste);
    connect(newaction,&QAction::triggered,this,&Widget::newfile);
    connect(saveaction,&QAction::triggered,this,&Widget::savefile);
    connect(saveasaction,&QAction::triggered,this,&Widget::saveasfile);

    connect(textedit, &QTextEdit::textChanged, [=](){
        int count = countValidCharacters(textedit->toPlainText());
        ui->labelCount->setText(QString("字数: %1").arg(count));
    });

}



int Widget::countValidCharacters(const QString &text)//统计数字
{
    // 方法一：使用字符分类（推荐）
    int count = 0;
    for(const QChar &c : text) {
        if(c.isLetter()) count++; // 统计所有字母和汉字[6,7](@ref)
    }
    return count;

    /* 方法二：正则表达式（需包含<QRegExp>头文件）
    QRegExp chnEngRegex("[\\u4E00-\\u9FA5a-zA-Z]");
    return text.count(chnEngRegex); */
}
void Widget::openFile()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename=QFileDialog::getOpenFileName(this,"打开文件");
    currentfile=filename;

    // if(!filename.isEmpty()){

    // }else{
    //     QMessageBox::warning(this,"提示","请取消操作");

    // }

    if(!filename.isEmpty()){
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly|QFile::Text)){
            QMessageBox::warning(this,"警告","无法打开"+file.errorString());
            return;
        }
        setWindowTitle(filename);//打开文件并改变title为文件名字
        QTextStream in(&file);//文本流
        QString contentstr=in.readAll();

        textedit->setText(contentstr);


        file.close();



    }




}

void Widget::setfont()
{
    bool isOk;
    QFont font=QFontDialog::getFont(&isOk,this->textedit->font(),this,"选择字体");
    if(isOk){
        this->textedit->setFont(font);

    }else{
        QString dlgtitle="提示";
        QString strinfo="没有选择字体";
        QMessageBox::warning(this,dlgtitle,strinfo);

    }

}

void Widget::setcolor()
{

    QColor color=QColorDialog::getColor(Qt::white,this,"选择颜色");
    if(color.isValid()){
        this->textedit->setTextColor(color);

    }else{
        QString dlgtitle="提示";
        QString strinfo="没有选择颜色";
        QMessageBox::warning(this,dlgtitle,strinfo);

    }

}

void Widget::cut()
{



}

void Widget::copy()
{



}

void Widget::paste()
{




}

void Widget::newfile()
{
    currentfile.clear();

    setWindowTitle("未命名.txt");
    textedit->setText(QString());


}

void Widget::savefile()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename;
    if(currentfile.isEmpty()){
        // filename=QFileDialog::getOpenFileName(this,"保存");
        filename = QFileDialog::getSaveFileName(this, "保存", desktopPath+"/未命名.txt", "文本文档 (*.txt);;所有文件 (*)");
        currentfile=filename;
    }
    else{
        filename=currentfile;
    }


    QFile file(filename);
    // if(!file.open(QIODevice::ReadOnly|QFile::Text)){
    //     QMessageBox::warning(this,"警告","无法打开"+file.errorString());
    //     return;
    // }
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return;
    }

    setWindowTitle(filename);

    QTextStream out(&file);
    QString text=textedit->toPlainText();

    out<<text;
    file.close();
}

void Widget::saveasfile()
{
    QString filename;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    // filename=QFileDialog::getOpenFileName(this,"另存为");
    filename = QFileDialog::getSaveFileName(this, "另存为",
                                                    desktopPath+ "/未命名.txt",
                                                    "文本文档 (*.txt);;所有文件 (*)");
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"警告","无法打开文件"+file.errorString());
        return;
    }
    if (!filename.endsWith(".txt", Qt::CaseInsensitive)) {
        filename += ".txt";
    }
    currentfile=filename;


    setWindowTitle(filename);

    QTextStream out(&file);

    QString text=textedit->toPlainText();

    out<<text;
    file.close();
}



Widget::~Widget()
{
    delete ui;
}


void Widget::set_icon()
{
    this->setGeometry(400,400,400,400);
    titlewindowtitle=new QIcon(":/resourse/icon_notebook.png");
    this->setWindowIcon(*titlewindowtitle);
    this->setWindowTitle("记事本");


    // 设置标签样式
    ui->labelCount->setStyleSheet(
        "QLabel {"
        "   color: #666666;"      // 灰色文字
        "   font-size: 12px;"     // 小号字体
        "   padding-right: 5px;" // 右内边距
        "}"
        );

}
