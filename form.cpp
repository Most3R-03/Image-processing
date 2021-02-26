#include "form.h"
#include "ui_form.h"

form::form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::form)
{
    ui->setupUi(this);
}

form::~form()
{
    delete ui;
}
