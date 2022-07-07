#include "choosefiledialog.h"
#include "ui_choosefiledialog.h"

chooseFileDialog::chooseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseFileDialog)
{
    ui->setupUi(this);
}

chooseFileDialog::~chooseFileDialog()
{
    delete ui;
}
