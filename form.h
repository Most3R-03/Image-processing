#ifndef FORM_H
#define FORM_H

#include <QMainWindow>

namespace Ui {
class form;
}

class form : public QMainWindow
{
    Q_OBJECT

public:
    explicit form(QWidget *parent = 0);
    ~form();

private:
    Ui::form *ui;
};

#endif // FORM_H
