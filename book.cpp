#include "book.h"

using namespace std;

struct Book::Pair
{
    string word;
    int cnt;
    Pair(string word)
    {
        this->word = word;
        cnt = 1;
    }
};

Book::Book() {};
Book::~Book() {};

void Book::open_file()
{
    string nazwa;
    cout << "Wprowadz nazwe pliku tekstowego: " << endl;
    getline(cin, nazwa);
    cin.clear();
    plik.open(nazwa + ".txt");
};

void Book::close_file()
{
    plik.close();
};

///parser linijki pliku HAMLET.
vector<string> Book::parser(string tekst)
{
    enum STATES { Unknown, Inside, Outside } state = Unknown; ///3 stany
    vector<string> Slowa;
    string word;

    for(string::size_type i=0; i<tekst.length(); ++i)
    {
        switch(state) ///tutaj jest implementacja przejsc w automacie stanow
        {
        case Unknown:
            if(isalpha(tekst[i]))
                state = Inside;
            else
                state = Outside;
            break;

        case Outside:
            if(isalpha(tekst[i])) state = Inside;
            break;

        case Inside:
            if(!isalpha(tekst[i]))
            {
                state = Outside;
                Slowa.push_back(word);
                word = "";
            }
            break;
        }
        if(state == Inside) word += tekst[i];
    }
    if (word.length() > 0) Slowa.push_back(word);
    return Slowa;
};

void Book::add_word(vector<string> &all_words, const string &word)//dodaje wszystkie slowa do wektora
{
    all_words.push_back(word);
};

void Book::add_uword(vector<Pair> &v, const string &word)  //dodaje slowa do struktury, gdy slowo sie powtarza zwieksza licznik ale nie dodaje tego samego slowa drugi raz
{
    for(uint32_t i = 0; i<v.size(); ++i)
        if(v.at(i).word == word)
        {
            v.at(i).cnt++;
            return;
        }
    v.push_back(Pair(word));
};


void Book::file_keywords()
{
    ifstream plik4;
    int wybor;
    plik4.open("keywords.txt");
    while(plik4.good())
    {
        getline(plik4, tekst);

        vector<string> Slowa = parser(tekst);

        for(unsigned int i = 0; i<Slowa.size(); ++i)
        keywords.push_back(Slowa.at(i));//dodawanie slow kluczowych do wektora stringow
    }
    plik4.close();

    cout << "Czy chcesz wyswietlic liste slow kluczowych?\n1. Tak\n2. Nie" << endl;
    cin >> wybor;
    cin.clear();

    if(wybor == 1)
    {
        for(uint32_t i=0; i < keywords.size(); ++i) cout << keywords.at(i) << ", ";
        cout << endl;
    };

};

void Book::initialize()
{

    while(plik.good())
    {
        getline(plik, tekst);//getline wiec string tekst bedzie pojedyncza linijka

        vector<string> Slowa = parser(tekst);

        for(unsigned int i = 0; i<Slowa.size(); ++i)
        {
            add_uword(v, Slowa.at(i));//dodawanie slow unikatowych do struktury Pair i ich zliczanie
            add_word(all_words, Slowa.at(i));//dodawanie wszystkich slow do stringa
            if(i>1 && i<6 && i%4==0) co5.push_back(Slowa.at(i));//dodawanie co 5 slowa z linijki do wektora


        }
    }
    plik.clear();
};

void Book::most_word()
{
    const vector<Pair> &tv = v;
    int maxx=0;
    for(uint32_t i = 0; i < tv.size(); ++i)
    if(tv.at(i).cnt>maxx) maxx=tv.at(i).cnt;//cout<<maxx;
    cout<<endl;
    for(uint32_t i = 0; i < tv.size(); ++i)
    if(tv.at(i).cnt==maxx)
    cout <<"slowem ktore najczesciej wystepuje jest: " << tv.at(i).word << " i wystepuje " << tv.at(i).cnt << " razy. " << endl;
};

void Book::od_tylu()
{
    const vector<string> &aw = all_words;
    vector <string> tylem;
    for(int i=aw.size()-1; i>=0; i--)
    tylem.push_back(aw.at(i));
    for(unsigned int i=0; i<aw.size(); i++)
    cout<<tylem.at(i)<<" ";
    cout<<endl;
};

void Book::co_piate()
{
    cout<<"co piate slowo z kazdej linijki: ";
    for(unsigned int i=0; i<co5.size(); i++)
    cout<<co5.at(i)<<", ";
    cout<<endl;
};

void Book::to_html()
{
    ofstream plik3;
    string html = "<html><body>";
    string html_end =  "</body></html>";

    plik3.open("przeksztalcone.html");
    plik3 << html;
    plik.seekg(0, plik.beg);
    while(plik.good())
    {
        getline(plik, tekst);//getline wiec string tekst bedzie pojedyncza linijka
        vector<string> Slowa = parser(tekst);
        plik3 <<  tekst << "<br />" << endl;//dodawanie linijek tekstu w petli do pliku html
    }
    plik3 << html_end;//dodawanie na koniec pliku html
    plik3.close();
    plik.clear();

};

void Book::to_htmlplus(Book &b)
{
    ofstream plik3;
    string html = "<html><body>";
    string html_end =  "</body></html>";

    plik3.open("podkreslone.html");
    plik3 << html;
    b.file_keywords();
    plik.seekg(0, plik.beg);
    while(plik.good())
    {
        getline(plik, tekst);//getline wiec string tekst bedzie pojedyncza linijka
        vector<string> Slowa = parser(tekst);
        for(unsigned int i = 0; i < keywords.size(); ++i)//podkreslanie slow kluczowych w tekscie ksiazki
        {
            size_t start = tekst.find(" " + keywords.at(i) + " ");
            if(start != string::npos)
            {
                tekst.replace(start, string(keywords.at(i)).length()+1, " <font color=fuchsia><b><u>"+string(keywords.at(i))+"</u></b></font> ");
            }
        }
        plik3 <<  tekst << "<br />" << endl;//dodawanie linijek tekstu w petli do pliku html
    }
    plik3 << html_end;//dodawanie na koniec pliku html
    plik3.close();
    plik.clear();
};

void Book::ile_wszystkich_slow()
{
    cout << "Ilosc wszystkich slow ogolem: " << all_words.size() << endl;//ilosc slow
    cout << "Ilosc roznych slow: " << v.size() << endl;
};

void Book::unikat()
{
    const vector <Pair> &copy_v = v;
    vector <string> unikaty;
    int wybor;

    for(uint32_t i = 0; i < copy_v.size(); ++i)
    if(copy_v.at(i).cnt==1)
    unikaty.push_back(copy_v.at(i).word);//kopiowanie slow unikatowych do wektora
    cout << "Slow unikatowych jest: " << unikaty.size() << endl;
    cout << "Czy chcesz wyswietlic ich liste?\n1. Tak\n2. Nie" << endl;
    cin >> wybor;
    cin.clear();

    if(wybor == 1)
    {for( unsigned int i = 0; i < unikaty.size()-1; i++ )//sortowanie slow unikatowych
        for( unsigned int j = 0; j < unikaty.size()-1; j++ )
            if( unikaty.at(j) > unikaty.at(j+1) )
                swap( unikaty.at(j), unikaty.at(j+1) );

    for(uint32_t i = 0; i < unikaty.size(); ++i)
    cout << i+1 << "." << unikaty.at(i) << endl ;
    };
};

//void zamiana(vector<string> &vvv,vector<string> &vvv2,vector<string> &vvv3)
void Book::zamiana()
{

ifstream plik1;
ifstream plik2;
    plik1.open("h.txt");
    plik2.open("m.txt");
    string tekst;
    vector <string> vvv3;
    vector <string> vvv2;
    vector <string> vvv;

    while(plik1.good())
    {
        getline(plik1, tekst);
        vector<string> Slowa=parser(tekst);
          for(unsigned int i = 0; i<Slowa.size(); ++i)
        vvv.push_back(Slowa.at(i));

    }
    plik1.close();
   while(plik2.good())
    {
        getline(plik2, tekst);
        vector<string> Slowa = parser(tekst);
        for(unsigned int i = 0; i<Slowa.size(); ++i)
            vvv2.push_back(Slowa.at(i));
    }
  //  plik2.close();



    //cout<<"co 2 slowo zamiana: "<<endl;
    for(int i=1; i<vvv2.size(); i+=2)
       swap(vvv.at(i),vvv2.at(i));
   // for(int i=1; i<vvv2.size(); i+=2)
      //  swap(vvv2.at(i),vvv3.at(i));
  //  cout<<"-----------------------------------------------------------"<<endl;
    for(int i=0; i<vvv.size(); i++)
        cout<<vvv.at(i)<<" ";
  /*  cout<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
    for(int i=0; i<vvv2.size(); i++)
        cout<<vvv2.at(i)<<" ";
    cout<<endl;
    cout<<"------------------------------------------------------------"<<endl;
    cout<<endl;
*/

    plik2.close();
}


