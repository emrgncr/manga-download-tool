#ifndef HTMLPARSER_H
#define HTMLPARSER_H
#include "stdio.h"
#include "iostream"
#include "algorithm"
#include "string"
#include "mangainfo.h"
#include "vector"
#include <QJsonObject>


class htmlParser
{
public:
    htmlParser();
    static std::vector<mangaInfo*>* getMangas(QJsonArray arr,std::string key,std::string pp_source);
    static int dist(std::string s1, std::string s2);
    static int LevenshteinDistance(const char* s, int len_s, const char* t, int len_t);
    static int dist(std::string key,QJsonObject o);

};

#endif // HTMLPARSER_H
