#include "htmlparser.h"
#include "stdio.h"
#include "iostream"
#include "algorithm"
#include "string"
#include "mangainfo.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "vector"
#include <climits>
#include <math.h>

using namespace std;
htmlParser::htmlParser(){}
vector<mangaInfo*>* htmlParser::getMangas(QJsonArray arr,string key){
    vector<mangaInfo*>* ret = new vector<mangaInfo*>();

//    cout<<arr.at(0).toObject().value("i").toString().toStdString()<<endl;
    auto vc = new vector<pair<int,QJsonObject>>();
    for (auto i : arr){
//        cout<<"qqwe"<<endl;
        QJsonObject o = i.toObject();
        int cost = dist(key,o);
        vc->push_back(* new pair<int,QJsonObject>(cost,o));
    }
    sort(vc->begin(),vc->end(),[](const pair<int,QJsonObject>& a, const pair<int,QJsonObject>& b) -> bool
    { return a.first < b.first; });
//    cout<<"first five: "<<endl;
    for(uint8_t i = 0;i<5;i++){
        cout<<vc->at(i).second.value("i").toString().toStdString()<<"/" << vc->at(i).first<<"   ";
    }
    //Take top 15 I guess
    for(uint8_t i = 0;i<15;i++){
        string name = vc->at(i).second.value("i").toString().toStdString();
        string name2 = vc->at(i).second.value("s").toString().toStdString();
        string author = vc->at(i).second.value("a").toArray().at(0).toString().toStdString();
        int year = vc->at(i).second.value("y").toString().toInt();
        string pp_url = "https://cover.nep.li/cover/"+name+".jpg";
        double chapterCount = (vc->at(i).second.value("l").toString().toDouble() - 100000) / 10;
        vector<string>* genres = new vector<string>();
        QJsonArray jar = vc->at(i).second.value("g").toArray();
        for(auto j : jar){
            genres->push_back(j.toString().toStdString());
        }
        ret->push_back(new mangaInfo(name,name2,author,year,pp_url,chapterCount,genres));
    }

    return ret;
}

int htmlParser::LevenshteinDistance(const char* s, int len_s, const char* t, int len_t)
{
  int cost = 0;
  if (len_s == 0) return len_t;
  if (len_t == 0) return len_s;

  if (s[len_s-1] == t[len_t-1]) cost = 0;
  else cost = 1;

  return min(LevenshteinDistance(s, len_s - 1, t, len_t) + 1,
                 min(LevenshteinDistance(s, len_s, t, len_t - 1) + 1,
             LevenshteinDistance(s, len_s - 1, t, len_t - 1) + cost));
}

int stringDistance(string a,string b) {
        int diff = INT_MAX;
//        char* a1 = new char[a.length()];
//        char* b1 = new char[b.length()];
//        a.copy(a1,a.length());
//        b.copy(b1,b.length());

        string t(a);
        string p(b);
        for(size_t i = 0; i < t.length(); i++){
            t[i] = tolower(t[i]);
            if(t[i] == ' ') t[i] = '-';
        }
        for(size_t i = 0; i < p.length(); i++){
            p[i] = tolower(p[i]);
            if(p[i] == ' ') p[i] = '-';
        }
        if(p.length() < t.length()) {
            swap(t,p);
        }
        int lengthdiff = p.length() - t.length();
        if(p.length() == 0) return INT_MAX;
        if(t.length() == 0) return INT_MAX;
        for(int sw = 0;sw <= lengthdiff;sw ++) {
            double tempdif = 500;
            double cons = 1;
            for(size_t i = 0;i<t.length();i++) {
                if(t[i] != p[i+sw]){ tempdif +=200; cons = 1;}else{tempdif -= 5*cons;tempdif = max(tempdif,1.0);cons *=1.7;}
//            tempdif = 1e-4*((pow(tempdif,5)*20)+0.00001);
//            tempdif *= 10;
              tempdif += 0.1*lengthdiff;
            diff = (int)min((double)diff, tempdif);
        }}
//        cout<<t<<" "<<p<<" "<<diff<<endl;
//        if(diff == 1){
//            cout<<a<< " "<<b << " "<<p << " "<<t<<endl;
//        }
        return diff;


}

int htmlParser::dist(std::string s1, std::string s2){
//    cout<<s1<< " "<< s2<<endl;
       return stringDistance(s1,s2);
}

int htmlParser::dist(std::string key, QJsonObject o)
{
    return min(dist(key,o.value("i").toString().toStdString()),
               min(dist(key,o.value("s").toString().toStdString()),dist(key,o.value("al").toString().toStdString())));
}

