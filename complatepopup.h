#ifndef COMPLATEPOPUP_H
#define COMPLATEPOPUP_H

#include <QDialog>

namespace Ui {
class ComplatePopup;
}

class ComplatePopup : public QDialog
{
    Q_OBJECT

public:
    explicit ComplatePopup(QWidget *parent = nullptr);
    ~ComplatePopup();

private:
    Ui::ComplatePopup *ui;
};

#endif // COMPLATEPOPUP_H
