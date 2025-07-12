#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include<QIcon>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QTextEdit>//一个功能强大的富文本编辑控件，支持编辑和显示纯文本与富文本

#include<QVBoxLayout>
// #include<QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    QMenuBar *menubar;
    QMenu *menu1;
    QAction *newaction;
    QAction *openaction;
    QAction *saveaction;
    QAction *saveasaction;



    QMenu *menu2;
    QAction *cutaction;
    QAction *copyaction;
    QAction *pasteaction;

    QMenu *menu3;
    QAction *fontaction;
    QAction *coloraction;

    QMenu *menu4;
    QAction *helpaction;

    QTextEdit *textedit;
    QString currentfile;





private slots:
    void openFile();
    void setfont();
    void setcolor();
    void cut();
    void copy();
    void paste();
    void newfile();
    void savefile();
    void saveasfile();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void set_icon();


private:
    Ui::Widget *ui;

    QIcon *titlewindowtitle;


    int countValidCharacters(const QString &text);
};
#endif // WIDGET_H
