#ifndef MANGAINFODISPLAY_H
#define MANGAINFODISPLAY_H

#include <QWidget>
#include <mangainfo.h>
#include <QtNetwork>

namespace Ui {
class mangaInfoDisplay;
}

class mangaInfoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit mangaInfoDisplay(QWidget *parent = nullptr,mangaInfo* info = nullptr);
    ~mangaInfoDisplay();

private:
    Ui::mangaInfoDisplay *ui;
    mangaInfo* info;
private slots:
    void imageLoaded(QNetworkReply* rep);
    void on_pushButton_clicked();
};

#endif // MANGAINFODISPLAY_H
