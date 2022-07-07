#ifndef CHOOSEFILEDIALOG_H
#define CHOOSEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class chooseFileDialog;
}

class chooseFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chooseFileDialog(QWidget *parent = nullptr);
    ~chooseFileDialog();

private:
    Ui::chooseFileDialog *ui;
};

#endif // CHOOSEFILEDIALOG_H
