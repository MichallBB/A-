
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <array>
using namespace std;
#define WYMX 20
#define WYMY 20

class Punkt
{
    int x;
    int y;
    float wpunktu;

public:
    Punkt(int obecx, int obecy, float wpun)
    {
        x = obecx;
        y = obecy;
        wpunktu = wpun;
    };
    int obecx()
    {
        return x;
    }
    int obecy()
    {
        return y;
    }
    float wpun()
    {
        return wpunktu;
    }
    float sciezka(int x1, int y1)
    {
        float odleglosc = sqrt(((x - x1) * (x - x1)) + ((y - y1) * (y - y1)));
        return odleglosc;
    }
};

class Info
{
public:

    Punkt rodzic = Punkt(0, 0, 0);

    Info() {}
    Info( Punkt rodzic)
    {
        rodzic = rodzic;
    }
};

bool czy_pole_w_otwartej(vector<Punkt> ListaOtwarta, int x, int y)
{
    for (int i = 0; i < ListaOtwarta.size(); i++)
    {
        if (ListaOtwarta[i].obecx() == x && ListaOtwarta[i].obecy() == y)
        {
            return true;
        }
    }
    return false;
}
bool czy_pole_w_zamknietej(vector<Punkt> ListaZamknieta, int x, int y)
{
    for (int i = 0; i < ListaZamknieta.size(); i++)
    {
        if (ListaZamknieta[i].obecx() == x && ListaZamknieta[i].obecy() == y)
        {
            return true;
        }
    }
    return false;
}


void dodaj_do_listy_otwartej(Punkt pole, vector<Punkt> &ListaOtwarta)
{
    ListaOtwarta.push_back(pole);
}
void dodaj_do_listy_zamknietej(Punkt pole, vector<Punkt> &ListaZamknieta)
{
    ListaZamknieta.push_back(pole);
}
Punkt najmniejsza_war_sciezki(vector<Punkt> &ListaOtwarta)
{
    Punkt pom(0, 0, 1000);
    for (int i = 0; i < ListaOtwarta.size(); i++)
    {
        if (ListaOtwarta[i].wpun() <= pom.wpun())
        {
            pom = ListaOtwarta[i];
        }
    }
    return pom;
}

void usun_element_listy(Punkt pom, vector<Punkt> &ListaOtwarta)
{
    for (int i = 0; i < ListaOtwarta.size(); i++)
    {
        if (ListaOtwarta[i].wpun() == pom.wpun())
        {
            vector<Punkt>::iterator it;
            it = ListaOtwarta.begin() + i;
            ListaOtwarta.erase(it);
        }
    }
}

void algorytm_caly(Punkt start, Punkt pocz, Punkt konc, vector<vector<int>> G, int wymx, int wymy, vector<Punkt> &ListaOtwarta, vector<Punkt> &ListaZamknieta, array<array<Info, WYMX>, WYMY> info)
{
    ListaOtwarta.push_back(pocz);
    int krok = 0;

    while (!ListaOtwarta.empty())
    {

        pocz = najmniejsza_war_sciezki(ListaOtwarta);
        usun_element_listy(pocz, ListaOtwarta);
        dodaj_do_listy_zamknietej(pocz, ListaZamknieta);

        if (((pocz.obecy() + 1) >= 0) && ((pocz.obecy() + 1) < wymy))
        {
            if (G[pocz.obecx()][pocz.obecy() + 1] != 5)
            {
                if (!czy_pole_w_zamknietej(ListaZamknieta, pocz.obecx(), pocz.obecy() + 1))
                {
                    if (!czy_pole_w_otwartej(ListaOtwarta, pocz.obecx(), pocz.obecy() + 1))
                    {
                        float w_sciezki1 = konc.sciezka(pocz.obecx(), pocz.obecy() + 1);
                        int krok_sasiada = krok + 1; // krok_sasiada = krok1
                        w_sciezki1 += krok_sasiada;
                        Punkt pomocniczy1(pocz.obecx(), pocz.obecy() + 1, w_sciezki1);
                        //                        cout<<"x=" <<pocz.obecx()<<"y="<<pocz.obecy()+1<< " dol " << G[pocz.obecx()][pocz.obecy()-1]  << "  "<<pomocniczy1.wpun()<<endl;
                        dodaj_do_listy_otwartej(pomocniczy1, ListaOtwarta);

                        info[pocz.obecx()][pocz.obecy() + 1].rodzic = pocz;
                        //                        cout<< info[pocz.obecx()][pocz.obecy()+1].rodzic.obecx()<<"-----------" <<endl;
                    }
                }
            }
        }
        if (((pocz.obecy() - 1) >= 0) && ((pocz.obecy() - 1) < wymy))
        { //sprawdzam czy zalicza sie do grida
            if (G[pocz.obecx()][pocz.obecy() - 1] != 5)
            { //czy to nie przeszkoda
                if (!czy_pole_w_zamknietej(ListaZamknieta, pocz.obecx(), pocz.obecy() - 1))
                { // czy nie jest w otwartej
                    if (!czy_pole_w_otwartej(ListaOtwarta, pocz.obecx(), pocz.obecy() - 1))
                    {                                                                   // czy nie jest w otwartej
                        float w_sciezki = konc.sciezka(pocz.obecx(), pocz.obecy() - 1); //wartoœæ sciezki od punktu do mety
                        int krok_sasiada = krok + 1;
                        w_sciezki += krok_sasiada;
                        Punkt pomocniczy(pocz.obecx(), pocz.obecy() - 1, w_sciezki);
                        //                        cout<<"x=" <<pocz.obecx()<<"y="<<pocz.obecy()-1<< " gora  " << G[pocz.obecx()][pocz.obecy()-1] << "  "<<pomocniczy.wpun()<<endl;
                        dodaj_do_listy_otwartej(pomocniczy, ListaOtwarta);

                        info[pocz.obecx()][pocz.obecy() - 1].rodzic = pocz;
                        //cout<< info[pocz.obecx()][pocz.obecy()].rodzic.obecx()<<"-----------" <<endl;
                    }
                }
            }
        }

        if (((pocz.obecx() + 1) >= 0) && ((pocz.obecx() + 1) < wymx))
        {
            if (G[pocz.obecx() + 1][pocz.obecy()] != 5)
            {
                if (!czy_pole_w_zamknietej(ListaZamknieta, pocz.obecx() + 1, pocz.obecy()))
                {
                    if (!czy_pole_w_otwartej(ListaOtwarta, pocz.obecx() + 1, pocz.obecy()))
                    {
                        float w_sciezki3 = konc.sciezka(pocz.obecx() + 1, pocz.obecy());
                        int krok_sasiada = krok + 1;
                        w_sciezki3 += krok_sasiada;
                        Punkt pomocniczy3(pocz.obecx() + 1, pocz.obecy(), w_sciezki3);
                        //                        cout<<"x=" <<pocz.obecx()+1<<"y="<<pocz.obecy()<< " prawo " << G[pocz.obecx()][pocz.obecy()-1] << "  "<<pomocniczy3.wpun()<<endl;
                        dodaj_do_listy_otwartej(pomocniczy3, ListaOtwarta);

                        info[pocz.obecx() + 1][pocz.obecy()].rodzic = pocz;
                        //                        cout<< info[pocz.obecx()+1][pocz.obecy()].rodzic.obecx()<<"-----------" <<endl;
                    }
                }
            }
        }
        if (((pocz.obecx() - 1) >= 0) && ((pocz.obecx() - 1) < wymx))
        {
            if (G[pocz.obecx() - 1][pocz.obecy()] != 5)
            {
                if (!czy_pole_w_zamknietej(ListaZamknieta, pocz.obecx() - 1, pocz.obecy()))
                {
                    if (!czy_pole_w_otwartej(ListaOtwarta, pocz.obecx() - 1, pocz.obecy()))
                    {
                        float w_sciezki2 = konc.sciezka(pocz.obecx() - 1, pocz.obecy());
                        int krok_sasiada = krok + 1;
                        w_sciezki2 += krok_sasiada;
                        Punkt pomocniczy2(pocz.obecx() - 1, pocz.obecy(), w_sciezki2);
                        //                        cout<<"x=" <<pocz.obecx()-1<<"y="<<pocz.obecy()<< " lewo " << G[pocz.obecx()][pocz.obecy()-1]  << "  "<<pomocniczy2.wpun()<<endl;
                        dodaj_do_listy_otwartej(pomocniczy2, ListaOtwarta);

                        info[pocz.obecx() - 1][pocz.obecy()].rodzic = pocz;
                        //                        cout<< info[pocz.obecx()-1][pocz.obecy()].rodzic.obecx()<<"-----------" <<endl;
                    }
                }
            }
        }

        if ((pocz.obecx() == konc.obecx()) && (pocz.obecy() == konc.obecy()))
        {

            //cout << info[19][19].rodzic.obecy() << "------------------------------------";
            vector<Punkt> lista_punktow;
            vector<Punkt> wynik;

            int x = konc.obecx();
            int y = konc.obecy();
            lista_punktow.push_back(konc);
            Punkt pom = info[x][y].rodzic;

//            cout<< "x=" << pom.obecx() <<"y=" << pom.obecy() << "Pierwszy rodzic" << endl;
//            cout<< "x=" << pocz.obecx() <<"y=" << pocz.obecy() << endl;
            while ((pom.obecx() != start.obecx()) && (pom.obecy() != start.obecy()))
            {
                lista_punktow.push_back(pom);
                x = pom.obecx();
                y = pom.obecy();
                pom = info[x][y].rodzic;

//                cout << "X " << x << " Y " << y << endl;

            }
            lista_punktow.push_back(pom);

//            cout<< "x=" << pom.obecx() <<"y=" << pom.obecy() << endl;
            lista_punktow.push_back(start);
            while (!lista_punktow.empty())
            {
                Punkt pom1 = lista_punktow.back();
                lista_punktow.pop_back();

                G[pom1.obecx()][pom1.obecy()] = 1;

            }
            cout << "grid ukonczony" << endl;
            for (int i = 0; i < WYMX; i++)
            {
                for (int j = 0; j < WYMY; j++)
                {
                    cout << G[i][j] ;
                }
                cout << endl;
            }
            break;
        }
        krok++;
    }

}

int main()
{
    string mapa = "grid.txt";
    int wymx = 20;
    int wymy = 20;

    vector<Punkt> list_otw;
    vector<Punkt> list_zamkn;
    vector<Punkt> scierzka;
    array<array<Info, WYMX>, WYMY> info;

    Punkt start(19,19, 1000);
    Punkt meta(0,0, 0);

    //tablica dla grida
    std::ifstream file(mapa.c_str());
    vector<vector<int>> grid;
    int value;

    for (unsigned int i = 0; i < wymx; i++)
    {
        vector<int> wiersz;
        for (unsigned int j = 0; j < wymy; j++)
        {
            file >> value;
            wiersz.push_back(value);
        }
        grid.push_back(wiersz);
    }
    file.close();

    for (int i = 0; i < wymx; i++)
    {
        for (int j = 0; j < wymy; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }

    algorytm_caly(start, start, meta, grid, wymx, wymy, list_otw, list_zamkn, info);

    cout << "\n\nNacisnij ENTER aby zakonczyc";
    getchar();

    return 0;
}
