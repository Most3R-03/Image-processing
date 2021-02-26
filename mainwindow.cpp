#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QPixmap"
#include"QBitmap"
#include"QPainter"
#include"QImage"
#include"Qcolor"
#include"QDebug"
#include"QFont"
#include"QFontDialog"
#include"QFileDialog"
#include"QRect"
#include"form.h"
#include <QAxObject>//这里是引入的是excel读取的库/////////////////////////////
#include <QSqlDatabase>
#include <QSqlQuery>//aa////////////////////////////////////////////////////
#include "QVector"
#include"QVariant"
//上面是引入的文件//////////////////////////////////////////////////////////////
//下面是定义的全局变量//////////////////////////////////////////////////////////
static bool adebug=false;
QFont font_author( "苹方 粗体,28,-1,5,50,0,0,0,0,0,Regular" );//一个全局函数的字体
QFont font_title( "苹方 粗体,38,-1,5,50,0,0,0,0,0,Regular" );//一个全局函数的字体
QString title;  //文章的标题
QString author;  //文章的作者
QString filepath;//excel的地址
/*
QPixmap image_head;//这里因为全局变量是早于main定义的，所以如果QPixmap作为全局定义的话，会因为没有定义QAppilcation而报错
QPixmap image_erweima;
QPixmap image_model;*/
//aa//////////////////////////////////////////////////////////////////////////////////////////

//这里提前声明函数///////////////////////////////////////////////////////////////////////////
void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);
////////////////////////////////////////////////////////////////////////////////////////////
//重载的MainWindow函数/////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ainit();


    //ui->model->installEventFilter(this); //标签过滤事件，使可以在标签上绘图

}
//aa///////////////////////////////////////////////////////////////////////////////////////////
//MainWindow的析构函数//////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}
//aa////////////////////////////////////////////////////////////////////////////////////////////
//将图片的color（1，1，1）替换成white，之后进行图片替换->model  图片大小230*230//////////////////////////////
QPixmap chang_white(QPixmap image,int x=230,int y=230){
    QImage towhite=image.toImage();
    qDebug()<<towhite.pixelColor(1,1);
    QColor touming(0,0,0,0);//设置要替换的颜色，不知道为什么刚开始是1，1，1，1 后来莫名变成0，0，0，0了
    for(int i=0;i<x;i++){

        for(int j=0;j<y;j++){

            if(towhite.pixelColor(i,j)==touming){
                towhite.setPixelColor(i,j,QColor("white"));
            }

        }

    }
    qDebug()<<towhite.pixelColor(1,1);
    QPixmap return_image=QPixmap::fromImage(towhite);
    return return_image;


}
//这里是MainWindow初始化函数////////////////////////////////////////////////////////////////////////////////////
void MainWindow::ainit(){

    QPixmap image_head;
    QPixmap image_erweima;
    QPixmap image_model;
    image_model.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\qqq.png");  //预加载模板  用于对测试和预览  1242*3421大小的
    image_erweima.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\81m.png");//预加载二维码，230*230
    image_head.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\81t.JPEG");//预加载头像  230*230
    ui->head->setPixmap(image_head);
    ui->erweima->setPixmap(image_erweima);
    ui->model->setPixmap(image_model);
    ui->head->setScaledContents(true);
    ui->erweima->setScaledContents(true);
    ui->model->setScaledContents(true);
    ui->model->show();


}
//aa/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//对图片进行裁剪的函数  两个参数，一个QPixmao的图片，一个半径///////////////////////////////////////////////////////////////////////////////
static QPixmap PixmapToRound(QPixmap &src, int radius)   //第二个函数为半径
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));//将画笔设置为透明
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);


    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

//aa////////////////////////////////////////////////////////////////////////

//替换指定位置的图片,返回一个Qpixmap的类型的。q为替换位置的x，w为替换位置的y，  x  y分别为要替换图形的大小//////////////
QPixmap replace_image(QPixmap imagemodel,QPixmap image_head_real_cut,int q=506,int w=1006,int x=230,int y=230)
{

    QImage head;
    head=image_head_real_cut.toImage();
    QColor color[x][y];//image_head_cut颜色的数组，x y形式  //创建一个220*220的数组

    for(int i=0;i<x;i++)
    {

        for(int j=0;j<x;j++)
        {
            color[i][j]=head.pixelColor(i,j);}}
    QImage model;
    model=imagemodel.toImage();
    for(int a=0;a<x;a++){
        for(int b=0;b<y;b++)  //
        {
            model.setPixelColor(q+a,w+b,color[a][b]);


        }}
    return QPixmap::fromImage(model);

}
//aa////////////////////////////////////////////////////////////////////////////////////////////////////////




//在图片中加入文本，需要原图片，title，字体，颜色，auther，字体，颜色////////////////////////////////////////////////////////////////////
QPixmap image_add_text(QPixmap image,QString title,QFont t_font,QColor t_color,QString author,QFont a_font,QColor a_color,int t_y=1382,int t_h=87.5,int a_y=1270,int a_h=39,int pix_max=1050,int center_x=620){
    QPainter painter(&image);    


    painter.setPen(t_color);//设置画笔颜色
    painter.setFont(t_font); //设置auther字体
    qDebug()<<t_font;
    qDebug()<<a_font;
    int widthOftitle=painter.fontMetrics().width(title);//字符串显示title的像素大小  中心轴x=620
    qDebug()<<widthOftitle;
    int left_title_x; //这里自动获取文本框的x
    int index;
    index=title.length();  //获取title
    if(widthOftitle>pix_max){
        for(int i=0;i<index;i++)  //这里如果title的像素长大于1050的话  就把文字分开，用两个文本框显示。
        {
            QString index18=title.mid(0,index-i);
            int widthaaa=painter.fontMetrics().width(index18);  //逐个把字符串分开找像素，
            if(widthaaa<pix_max)
            {
                left_title_x=center_x-pix_max/2;//title这个方框的左边x
                painter.setFont(t_font); //设置auther字体
                painter.drawText(left_title_x,t_y,pix_max,t_h,Qt::AlignHCenter,index18);//加入的title文本
                QString title2=title.mid(index-i,index);
                int widthoftemp;
                widthoftemp=painter.fontMetrics().width(title2);//获取第二个文本的像素宽度
                left_title_x=center_x-widthoftemp/2;//自动设置第二个文本的x
                painter.setFont(t_font);
                painter.drawText(left_title_x,t_y+t_h,widthoftemp,t_h,Qt::AlignHCenter,title2);//绘制第二个文本


            }


        }
    }else {

        painter.setFont(t_font);
        painter.drawText(left_title_x,t_y,widthOftitle,t_h,Qt::AlignHCenter,title);//如果像素小于pixmax，直接按文本的数量绘制文本框
    }
//aa///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//author绘制//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    painter.setPen(a_color);//设置画笔颜色
    painter.setFont(a_font);
    int widthOfauthor = painter.fontMetrics().width(author);//字符串显示author的像素大小  中心轴x=620
    int left_author_x=0;
    left_author_x=center_x-widthOfauthor/2;//根据文字的长短来设置drawtext的小方框
    painter.drawText(left_author_x,a_y,widthOfauthor,a_h,Qt::AlignCenter,author);  //加入author文本
    //font_author.setPointSize(38);  //设置title字号为38


    //QPixmap pixmap_temp(1243,3421); //1242*3421;
    //pixmap_temp
    //painter.drawPixmap(0,0,1243,3421,pixmap_temp);
    if(adebug==true){
        image.save(QString("debug\\aaaimage_add_text.png"));

    }

    return image;


    
    
    
    
    
    
    
    
    
    
}
//aa//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打开author文字选择框///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_6_clicked()
{
    bool ok;
    font_author=QFontDialog::getFont(&ok,QFont("苹方 粗体",28,QFont::Bold),this,"自定义文字属性");  //打开字体对话框，获取所用字体
    ui->label_font_author->setText(QString(font_author.family())+" "+QString(font_author.pixelSize()));
}
//aa///////////////////////////////////////////////////////////////////////////////////////////////
//打开excl文件///////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_7_clicked()
{
    filepath = QFileDialog::getOpenFileName(NULL, "请选择数据文件", "", "xlsx(*.xlsx)");
    ui->lineEdit->setText(filepath);

}
//aa//////////////////////////////////////////////////////////////////////////////////////////////////////
//title文字选择框//////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_8_clicked()
{
    bool ok;
    font_title=QFontDialog::getFont(&ok,QFont("苹方 粗体",38,QFont::Bold),this,"自定义文字属性");  //打开字体对话框，获取所用字体
    ui->label_font_title->setText(QString(font_title.family())+" "+QString(font_title.pixelSize()));
}
//aa/////////////////////////////////////////////////////////////////////////////////////////////////////
QPixmap handl_pic(QPixmap image_model,QPixmap image_erweima,QPixmap image_head,QString author,QString title){


    image_head=image_head.scaled(230,230);//修改头像图片的分辨率，全部设置成220*220的
    image_erweima=image_erweima.scaled(230,230);//修改二维码的分辨率，全部设置230*230的
    image_head=PixmapToRound(image_head,115);//头像裁圆
    image_head=chang_white(image_head);//将color1，1，1设置为白色
    image_model=replace_image(image_model,image_head);//替换头像，默认参数直接替换头像  头像左上角506，1006  二维码 475，2635
    image_model=replace_image(image_model,image_erweima,475,2635); //替换二维码图片
    QColor color(78,78,78);

    image_model=image_add_text(image_model,title,font_title,color,author,font_author,color);//在图片上增加文本
    qDebug()<<"添加文本";
    return image_model;
}

//对excel进行读取//////////////////////////////////////////////////////////////////////////////////////////////
void Read_Excel(const QString PATH,const QString FILENAME,const int SHEETNUM,const QString RANGE,const int INVALIDROW,const int TOTALCOLNUM,QVector<QString>& RESULT)
//参数解释：路径，文件名，第几个sheet表，读取范围（格式为A1:B），无效的行数（比如不想要的title等），读取范围的总列数，返回一个QString的vector。
{
    QString pathandfilename=PATH+FILENAME;
    QAxObject excel("ket.Application");
    excel.setProperty("Visible", true); //隐藏打开的excel文件界面
    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    QAxObject *workbook = workbooks->querySubObject("Open(QString, QVariant)", pathandfilename); //打开文件
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", SHEETNUM); //访问第SHEETNUM个工作表
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");
    QVariant allEnvDataQVariant = usedrange->property("Value");//读取所有的值
    qDebug()<<allEnvDataQVariant;
    QList<QList<QVariant> > result;

    castVariant2ListListVariant(allEnvDataQVariant,result);
    qDebug()<<"++++++++++++++++++"<<result;}
    /*
    if(NULL == usedrange || usedrange->isNull())

    {

        qDebug()<<"excel内是空的。"
        return

    }
    QAxObject * rows = usedrange->querySubObject("Rows");
    int rownum = rows->property("Count").toInt(); //获取行数
    qDebug()<<rownum<<"-------------------------------------";

    QString Range = RANGE +QString::number(rownum);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range); //读取范围
    //QVariant allEnvDataQVariant = allEnvData->property("Value");//读取所有的值
    qDebug()<<allEnvDataQVariant<<"------------------------------------------";
    QVariantList allEnvDataList = allEnvDataQVariant.toList();//转换为list
    qDebug()<<allEnvDataList<<"------------------------------------------";
    if(allEnvDataList.isEmpty()){
        qDebug()<<"这个标是空的。。";

        return ;
    }

    for(int i=0; i<rownum-INVALIDROW; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList();//第i行的数据
        for(int j=0;j<TOTALCOLNUM;j++)
        {
            QString tempvalue=allEnvDataList_i[j].toString();
            RESULT.push_back(tempvalue);
        }
    }
    workbooks->dynamicCall("Close()");
    excel.dynamicCall("Quit()");
}*/
//aa/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//对
void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{

    QVariantList varRows = var.toList();
    if(varRows.isEmpty()){
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i){

        rowData = varRows[i].toList();
        res.push_back(rowData);

    }

 }




//aa//////////////////////////////////////////////////////////////////////////////////////////
//第一个输出按钮//////////////////////////////////////////////////////////////////////////
void MainWindow::on_pushButton_5_clicked()
{

    QPixmap image_head;
    QPixmap image_erweima;
    QPixmap image_model;
    QString author="作者:你看这么多字够不够多呢啊啊啊啊";  //字号28
    QString title="《糖油饼，一二三四五六七八九十一二三四五六七八九十";   //字号38
    image_model.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\qqq.png");  //预加载模板  用于对测试和预览  1242*3421大小的
    image_erweima.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\81m.png");//预加载二维码，230*230
    image_head.load("C:\\Users\\bijiben\\Desktop\\aaa\\aaa\\81t.JPEG");//预加载头像  230*230
    QPixmap success_pic=handl_pic(image_model,image_erweima,image_head,author,title);
    ui->model->setPixmap(success_pic);
    /* ui->head_2->setScaledContents(false);
    ui->head_2->setScaledContents(false);
    ui->head_2->setPixmap(image_head);//展示到label里
    ui->erweima->setPixmap(image_erweima);//展示二维码*/
}
//aa///////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{

    QVector<QString> excl;
    Read_Excel("C:\\Users\\bijiben\\Desktop\\aaa\\","a11.xlsx",1,"F1:F",1,1,excl);
    qDebug()<<excl;
}
