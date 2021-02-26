#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QDebug"
#include"QThread"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    //QPixmap imagemodel;
//    void paint();
    //QPixmap image;
    void ainit();

    //void paintEvent(QPaintEvent* );
    void Init();


    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;


/*protected:

    bool eventFilter(QObject *watched, QEvent *event);*/
};

class mythread:public QThread{

    Q_OBJECT
public:



private:






};






#endif // MAINWINDOW_H
