#include "mangainfodisplay.h"
#include "ui_mangainfodisplay.h"
#include <QtNetwork>
#include "downloadpopup.h"
#include "sstream"

mangaInfoDisplay::mangaInfoDisplay(QWidget *parent,mangaInfo* info) :
    QWidget(parent),
    ui(new Ui::mangaInfoDisplay)
{
    ui->setupUi(this);
    this->info = info;
    //get pp image loaded
    QNetworkAccessManager* man = new QNetworkAccessManager();
    connect(man,SIGNAL(finished(QNetworkReply *)),this,SLOT(imageLoaded(QNetworkReply *)));
    man->get(QNetworkRequest(QUrl(QString::fromStdString(info->getPp_url()))));
    std::string genres = "";
    std::ostringstream chapters_s;
    chapters_s.precision(3);
    chapters_s << info->getChapterCount();
    std::string chapters = chapters_s.str();

    for(auto i : *info->getGenres()){
        genres += i + ", ";
    }
    ui->textBrowser->setText(QString::fromStdString(
                                 info->getName() + '\n'+
                                 info->getName2() + '\n'+
                                 "Author: " + info->getAuthor() + '\n'+
                                 "Year: " + std::to_string(info->getYear()) + '\n'+
                                 "Chapters: " + chapters + '\n'+
                                 "Genres: " + genres
                                 ));
}

mangaInfoDisplay::~mangaInfoDisplay()
{
    delete ui;
}

void mangaInfoDisplay::imageLoaded(QNetworkReply *rep)
{
    QImage img;
    img.loadFromData(rep->readAll());
    ui->aimg->setPixmap(QPixmap::fromImage(img).scaled(ui->aimg->width(),ui->aimg->height()));
}

void mangaInfoDisplay::on_pushButton_clicked()
{
    DownloadPopup* popup = new DownloadPopup(this,info);
    popup->show();
}

