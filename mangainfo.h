#ifndef MANGAINFO_H
#define MANGAINFO_H
#include "stdio.h"
#include "iostream"
#include "algorithm"
#include "vector"
#include "string"
#include <QObject>

class mangaInfo : public QObject
{
    Q_OBJECT

public:
    mangaInfo(std::string name,std::string name2 ,std::string author,int year, std::string pp_url,double chapterCount,std::vector<std::string>* genres );

    const std::string &getName() const;
    void setName(const std::string &newName);
    const std::string &getAuthor() const;
    void setAuthor(const std::string &newAuthor);
    int getYear() const;
    void setYear(int newYear);
    const std::string &getPp_url() const;
    void setPp_url(const std::string &newPp_url);
    double getChapterCount() const;
    void setChapterCount(double newChapterCount);


    std::vector<std::string> *getGenres() const;
    void setGenres(std::vector<std::string> *newGenres);

    const std::string &getName2() const;
    void setName2(const std::string &newName2);

private:
    std::string name;
    std::string name2;
    std::string author;
    int year;
    std::string pp_url;
    double chapterCount;
    std::vector<std::string>* genres;
};

#endif // MANGAINFO_H
