#include "mangainfo.h"
#include "QtNetwork"
#include "mainwindow.h"

mangaInfo::mangaInfo(std::string name,std::string name2, std::string author
                     ,int year, std::string pp_url,double chapterCount,std::vector<std::string>* genres)
{
    QObject(nullptr);
    this->name = name;
    this->name2 = name2;
    this->author = author;
    this->year = year;
    this->pp_url = pp_url;
    this->chapterCount = chapterCount;
    this->genres = genres;

    QString url = MainWindow::domainLink + "/manga/" + QString::fromStdString(name);
    auto man = new QNetworkAccessManager();
    QObject::connect(man,&QNetworkAccessManager::finished,this,[=](QNetworkReply* res){
        std::string source = res->readAll().toStdString();
        size_t t1 = source.find("vm.Chapters = ") + 14;
        size_t t2 = source.find(';',t1);
        source = source.substr(t1,t2-t1);
        QJsonDocument doc = QJsonDocument::fromJson(QString(source.c_str()).toUtf8());
        setChapterCount(doc.array().size());
    });
    man->get(QNetworkRequest(QUrl(url)));


}

const std::string &mangaInfo::getName() const
{
    return name;
}

void mangaInfo::setName(const std::string &newName)
{
    name = newName;
}

const std::string &mangaInfo::getAuthor() const
{
    return author;
}

void mangaInfo::setAuthor(const std::string &newAuthor)
{
    author = newAuthor;
}

int mangaInfo::getYear() const
{
    return year;
}

void mangaInfo::setYear(int newYear)
{
    year = newYear;
}

const std::string &mangaInfo::getPp_url() const
{
    return pp_url;
}

void mangaInfo::setPp_url(const std::string &newPp_url)
{
    pp_url = newPp_url;
}

double mangaInfo::getChapterCount() const
{
    return chapterCount;
}

void mangaInfo::setChapterCount(double newChapterCount)
{
    chapterCount = newChapterCount;
}

std::vector<std::string> *mangaInfo::getGenres() const
{
    return genres;
}

void mangaInfo::setGenres(std::vector<std::string> *newGenres)
{
    genres = newGenres;
}

const std::string &mangaInfo::getName2() const
{
    return name2;
}

void mangaInfo::setName2(const std::string &newName2)
{
    name2 = newName2;
}

