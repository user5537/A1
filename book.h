#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Book
{
private:
    vector <string> Slowa;
    vector<string> all_words;
    struct Pair;
    vector<Pair> v;
    vector<string> keywords;
    string tekst;//pojedyncza linijka wczytywana z getline
    vector<string> co5;
    ifstream plik, plik2, plik1;
    vector<string> parser(string tekst);
//    string tekst;
    vector <string> vvv3;
    vector <string> vvv2;
    vector <string> vvv;

public:
    Book();
    ~Book();
    void zamiana();
    void open_file();
    void close_file();
    void add_word(vector<string> &all_words, const string &word);
    void add_uword(vector<Pair> &v, const string &word);//dodawanie slow do pary i zliczanie kolejnych wystapien
    void file_keywords();
    void initialize();
    void to_html();
    void to_htmlplus(Book &b);
    void ile_wszystkich_slow();
    void most_word();
    void od_tylu();
    void co_piate();
    void unikat();
    //void zamiana();
};
