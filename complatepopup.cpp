#include "complatepopup.h"
#include "ui_complatepopup.h"

ComplatePopup::ComplatePopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComplatePopup)
{
    ui->setupUi(this);
}

ComplatePopup::~ComplatePopup()
{
    delete ui;
}
