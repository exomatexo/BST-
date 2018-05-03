#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
// struktura elementu
struct Element {
    string slowo;
    Element *lewo;
    Element *prawo;
    Element *odpowiednik;
};

Element *min_wart_wezla(Element *pElement);

void dodawanie_polskiego(Element *&BST_pol, string pol, Element *&temp){
    // jezeli puste
    if (BST_pol==NULL){
        BST_pol = new Element;
        BST_pol->slowo=pol;
        BST_pol->lewo=NULL;
        BST_pol->prawo=NULL;
        BST_pol->odpowiednik=NULL;
        temp=BST_pol;
        return ;
    }
    else{   //jesli nie puste
        if (pol.compare(BST_pol->slowo)==0)
        {
            temp= nullptr;
            return;
        }
        if(pol.compare(BST_pol->slowo)>0)
            dodawanie_polskiego(BST_pol->prawo,pol,temp);
        if(pol.compare(BST_pol->slowo)<0)
            dodawanie_polskiego(BST_pol->lewo,pol,temp);
    }
}
void dodawanie_angielskiego(Element *&BST_ang, string ang,Element *&temp){
    // jezeli puste
    if (BST_ang==NULL){
        BST_ang = new Element;
        BST_ang->slowo=ang;
        BST_ang->lewo=NULL;
        BST_ang->prawo=NULL;
        BST_ang->odpowiednik=NULL;
        temp=BST_ang;
        return ;
    }
    else{
        //jesli nie puste
        if(ang.compare(BST_ang->slowo)>0)
            dodawanie_polskiego(BST_ang->prawo,ang,temp);
        if(ang.compare(BST_ang->slowo)<0)
            dodawanie_polskiego(BST_ang->lewo,ang,temp);
    }
}
void szukaj(string n, Element *BST,Element *&temp){
    while(BST!= nullptr){
        if(BST->slowo.compare(n)==0) {
            temp = BST;
            return;
        }
        if(n.compare(BST->slowo)>0)
            BST=BST->prawo;
        else
            BST=BST->lewo;
    }
    temp = NULL;
    return ;
}
void KLP_zapis(Element *BST_ang,fstream &plik)  //korzen-lewy-prawy
{
    if(BST_ang!= nullptr)
    {
        plik << BST_ang->slowo <<" "<<BST_ang->odpowiednik->slowo<<endl;
        if(BST_ang->lewo!= nullptr)
            KLP_zapis(BST_ang->lewo,plik);
        if(BST_ang->prawo!= nullptr)
            KLP_zapis(BST_ang->prawo,plik);
    }
}
void wczytaj(string nazwa,Element *&BST_pol,Element *&BST_ang){
    string a,b ;
    fstream plik;
    plik.open(nazwa);
    if(!plik.good()){
        cout<<"Blad otwarcia pliku"<<endl;
        return;
    }
    Element *temp1= nullptr,*temp2= nullptr;
    while(!plik.eof()){
        plik>>a>>b;
        dodawanie_polskiego(BST_pol,b, temp1);
        if(temp1!= nullptr){
            dodawanie_angielskiego(BST_ang,a,temp2);
            if(temp2!= nullptr){
                temp1->odpowiednik=temp2;
                temp2->odpowiednik=temp1;
            }
        }
    }
    plik.close();
    return;
}
Element* usuwanie_Elementu(Element *&korzen, string n){
    if (korzen == NULL) return korzen;
    if (n.compare(korzen->slowo)<0)
        korzen->lewo = usuwanie_Elementu(korzen->lewo, n);
    else if (n.compare(korzen->slowo)>0)
        korzen->prawo = usuwanie_Elementu(korzen->prawo, n);
    else
    {
        if (korzen->lewo == NULL)
        {
            Element *temp = korzen->prawo;
            delete (korzen);
            return temp;
        }
        else if (korzen->prawo == NULL)
        {
            Element *temp = korzen->lewo;
            delete(korzen);
            return temp;
        }
        Element *temp = min_wart_wezla(korzen->prawo);
        korzen->slowo = temp->slowo;
        korzen->prawo = usuwanie_Elementu(korzen->prawo, temp->slowo);
    }
    return korzen;
}
Element *min_wart_wezla(Element *pElement) {
    Element *obecny=pElement;
    while(obecny->lewo!= nullptr)
        obecny=obecny->lewo;
    return obecny;
}

int main() {
    int wybor;
    fstream zapis ;
    Element *BST_pol= nullptr;
    Element *BST_ang=nullptr;
    Element *temp_pol= NULL, *temp_ang= nullptr;
    while (true){
        cout<<"-------------------------"<<endl;
        cout<<"Wybierz co chcesz zrobic" <<endl;
        cout<<"-------------------------"<<endl;
        cout<<"1.Dodaj slowo polskie"<<endl;
        cout<<"2.Usun slowo polskie"<<endl;
        cout<<"3.Dodaj slowo angielskie"<<endl;
        cout<<"4.Usun slowo angielskie"<<endl;
        cout<<"5.Dodaj z pliku"<<endl;
        cout<<"6.Zapis do pliku"<<endl;
        cout<<"7.Szukaj tlumaczenia pol->ang"<<endl;
        cout<<"8.Szukaj tlumaczenia ang->pol"<<endl;
        cout<<"9.Wyjscie"<<endl;
        cin>>wybor;
        string temp,del,szukana;
        switch(wybor){
            case 1:
                system("cls");
                cout<<"Podaj slowo polskie: ";
                cin>>temp;
                temp_ang= nullptr;
                temp_pol= nullptr;
                dodawanie_polskiego(BST_pol,temp, temp_pol);
                if(temp_pol== nullptr){
                    cout<<"Podane slowo juz sie znajduje"<<endl;
                    break;
                }
                cout<<"Podaj angielski odpowiednik:  ";
                cin>>temp;
                dodawanie_angielskiego(BST_ang,temp,temp_ang);
                temp_pol->odpowiednik=temp_ang;
                temp_ang->odpowiednik=temp_pol;
                break;
            case 2:
                system("cls");
                if(BST_pol==NULL) {
                    cout << "Drzewo puste nie ma co usunac" << endl;
                    continue;
                }
                cout << "Podaj slowo do usuniecia: ";
                cin >> del;
                szukaj(del,BST_pol,temp_ang);

                if(temp_ang!= nullptr){
                    string temp2 = temp_ang->odpowiednik->slowo;
                    usuwanie_Elementu(BST_pol, del);
                    usuwanie_Elementu(BST_ang, temp2);
                }
                break;
            case 3:
                system("cls");
                cout<<"Podaj slowo angielskie: ";
                cin>>temp;
                dodawanie_angielskiego(BST_ang,temp,temp_ang);
                if(temp_ang== nullptr){
                    cout<<"Podane slowo juz sie znajduje"<<endl;
                    break;
                }
                cout<<"Podaj polski odpowiednik:  ";
                cin>>temp;
                dodawanie_polskiego(BST_pol,temp,temp_pol);
                temp_ang->odpowiednik=temp_pol;
                temp_pol->odpowiednik=temp_ang;
                break;
            case 4:
                system("cls");
                if(BST_ang==NULL) {
                    cout << "Drzewo puste nie ma co usunac" << endl;
                    continue;
                }
                cout << "Podaj slowo do usuniecia: ";
                cin >> del;
                szukaj(del,BST_ang,temp_ang);

                if(temp_ang!= nullptr){
                    string temp2 = temp_ang->odpowiednik->slowo;
                    BST_ang=usuwanie_Elementu(BST_ang, del);
                    BST_pol=usuwanie_Elementu(BST_pol, temp2);
                }
                break;
            case 5://odczyt
                system("cls");
                cout<<"Podaj nazwe pliku z ktorego chcesz wczytac: ";
                cin>>temp;
                wczytaj(temp,BST_pol,BST_ang);
                break;
            case 6://zapis
                system("cls");
                cout<<"Podaj nazwe pliku: ";
                cin>>temp;
                zapis.open(temp,ios::out);
                if(zapis.good())
                    KLP_zapis(BST_ang,zapis);
                else cout<<"Blad otwarcia pliku"<<endl;
                zapis.close();
                break;
            case 7: //pol-ang
                system("cls");
                cout<<"Podaj slowo do wyszukania: ";
                cin>>szukana;
                szukaj(szukana,BST_pol,temp_pol);
                if(temp_pol!= nullptr)
                    cout<<"Tlumaczenie:  "<<temp_pol->odpowiednik->slowo<<endl;
                else cout<<"Slownik nie zawiera tego slowa"<<endl;
                break;
            case 8://ang-pol
                system("cls");
                cout<<"Podaj slowo do wyszukania: ";
                cin>>szukana;
                szukaj(szukana,BST_ang,temp_ang);
                if(temp_ang!= nullptr)
                    cout<<"Tlumaczenie:  "<<temp_ang->odpowiednik->slowo<<endl;
                else cout<<"Slownik nie zawiera tego slowa"<<endl;
                break;
            case 9://wyjsice
                return 0;

            default:
                cout<<"Nie ma takiej opcji!!!!"<<endl;
                break;
        }
    }



}