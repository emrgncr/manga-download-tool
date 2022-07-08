#ifndef DOWNLOADPOPUP_H
#define DOWNLOADPOPUP_H

#include <QDialog>
#include "mangainfo.h"
#include "QtNetwork"


namespace Ui {
class DownloadPopup;
}

class DownloadPopup : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadPopup(QWidget *parent = nullptr,mangaInfo* info = nullptr);
    ~DownloadPopup();

    mangaInfo *getInfo() const;
    void setInfo(mangaInfo *newInfo);

    static std::string numToChapter(int);
    static std::string numToChapter2(int);
    static std::string chapterCodeToChapter(int);
    static std::string getCoreURL(std::string);
    static std::vector<int>* getChaptersArray(std::string);
    static std::string chapterCodeToChapterSmol(int x);

    static QJsonArray arr;
    static int maxChapters;

    void enterMainLoop();

    void savePage(QNetworkReply* res,int chapter,int page,std::string seriesName,int maxPages,std::string);
    void callSinglePage(int chapter,int page,std::string seriesName,int maxPages,std::string);
    void gotChapterPage(QNetworkReply* res,int chapter);

private slots:
    void on_minspin_valueChanged(int arg1);

    void on_maxspin_valueChanged(int arg1);

    void on_pushButton_clicked();

    void gotMainPage(QNetworkReply* res);


private:
    Ui::DownloadPopup *ui;
    mangaInfo* info;

};

#endif // DOWNLOADPOPUP_H
