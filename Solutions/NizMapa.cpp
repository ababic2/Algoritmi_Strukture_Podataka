#include <iostream>
#include <string>

template<typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    Mapa() {}
    virtual TipVrijednosti& operator[](TipKljuca kljuc) = 0;
    virtual TipVrijednosti operator[](TipKljuca kljuc)const = 0;

    virtual int brojElemenata() const
    {
        return 0;
    }

    virtual void obrisi() {}
    virtual void obrisi(const TipKljuca& kljuc) {}
    virtual ~Mapa(){}
};

template<typename TipKljuca, typename TipVrijednosti>
struct Pair {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca,TipVrijednosti>
{
    Pair<TipKljuca,TipVrijednosti>* niz;
    int brojEl;
    int kapacitet = 1000;
public:

    NizMapa()
    {
        brojEl = 0;
        niz = new Pair<TipKljuca, TipVrijednosti>[kapacitet]{};
    }

    ~NizMapa() { delete[] niz;}

    NizMapa<TipKljuca,TipVrijednosti>(const NizMapa& mapa)
    {
        kapacitet = mapa.kapacitet;
        brojEl = mapa.brojEl;
        niz = new Pair<TipKljuca, TipVrijednosti>[kapacitet] {};

        for(int i = 0; i < mapa.brojElemenata(); i++)
            niz[i] = mapa.niz[i];
    }

    NizMapa<TipKljuca,TipVrijednosti>(NizMapa&& mapa)
    {
        brojEl = mapa.brojEl;
        kapacitet = mapa.kapacitet;
        niz = mapa.niz;
        mapa.niz = nullptr;
    }

    NizMapa& operator=(NizMapa&& mapa)
    {
        if(this == &mapa) return *this;

        delete[] niz;

        brojEl = mapa.brojEl;
        kapacitet = mapa.kapacitet;
        niz = mapa.niz;
        mapa.niz = nullptr;
        return *this;
    }

    NizMapa& operator=(const NizMapa& mapa)
    {
        //duboka kopija

        if(this == &mapa) return *this;

        delete[] niz;

        kapacitet = mapa.kapacitet;
        brojEl = mapa.brojEl;
        niz = new Pair<TipKljuca, TipVrijednosti>[kapacitet] {};

        for(int i = 0; i < mapa.brojElemenata(); i++)
            niz[i] = mapa.niz[i];

        return *this;
    }

    TipVrijednosti operator[](TipKljuca kljuc) const
    {
        for(int i = 0; i < brojEl; i++)
            if(niz[i].kljuc == kljuc) return niz[i].vrijednost;

        return TipVrijednosti();
    }

    TipVrijednosti& operator[](TipKljuca kljuc);

    int brojElemenata() const override
    {
        return brojEl;
    }

    void obrisi() override
    {
        brojEl = 0;
        kapacitet = 500;

        delete[] niz;
        niz = new Pair<TipKljuca,TipVrijednosti>[kapacitet]{};
    }

    void obrisi(const TipKljuca& kljuc) override
    {
        for(int i = 0; i < brojEl; i++)
            if(niz[i].kljuc == kljuc) {
                for(int j = i; j < brojEl - 1; j++)
                    niz[j] = niz[j + 1];
                brojEl--;
            }
    }

};

template<typename TipKljuca, typename TipVrijednosti>
inline TipVrijednosti& NizMapa<TipKljuca,TipVrijednosti>::operator[](TipKljuca kljuc)
{

    for(int i = 0; i < brojEl; i++)
        if(niz[i].kljuc == kljuc) return niz[i].vrijednost;

    //nije nadjen znaci napravit ce se i postavit na default vr vrijednost
    if(brojEl == kapacitet) {
        //realokacija heh
        kapacitet *= 2;
        Pair<TipKljuca,TipVrijednosti>*novi = new Pair<TipKljuca,TipVrijednosti>[kapacitet]{};
        for(int i = 0; i < brojEl; i++)
            novi[i] = niz[i];
        delete[] niz;
        niz = novi;
        novi = nullptr;
    }

   Pair<TipKljuca,TipVrijednosti> hold;
   hold.kljuc = kljuc;
   hold.vrijednost = TipVrijednosti();
   niz[brojEl] = hold;
   brojEl++;
   return niz[brojEl - 1].vrijednost;
}

int main()
{
    {
        //TEST 1. OPERATORI
        NizMapa<std::string,std::string>test;
        test["Bosna"] = "Sarajevo";
        test["Rusija"] = "Moskva";

        if(test["Bosna"] == "Sarajevo" && test["Rusija"] == "Moskva" && test.brojElemenata() == 2) std::cout << "TEST 1. OK\n";
        else std::cout << "test 1 not ok";

        //TEST 2.
        test.obrisi("Rusija");
        if(test.brojElemenata() == 1) std::cout << "TEST 2. OBRISI OK\n";
        else std::cout << "TEST 2. NOT OK";

        //TEST 3.
        NizMapa<std::string,std::string>test2(test);
        if(test2.brojElemenata() == 1) std::cout << "TEST 3. KONSTRUKTOR OK\n";
        else std::cout << "TEST 3. NOT OK";

        //TEST 4.
        test2["Rusija"] = "Moskva";
        test2["Bosna"] = "Mostar";                                              //promjena vrijednosti vec postojecem kljucu
        test2["Turska"] = "Istanbul";
        if(test2.brojElemenata() == 3 && test2["Bosna"] == "Mostar") std::cout << "TEST 4. OK";
        else std::cout << "TEST 4. NOT OK";
    }

     {
        //TEST 5. OPERATORI
        NizMapa<int,std::string>test;
        test[1] = "Sarajevo";
        test[2] = "Moskva";

        if(test[1] == "Sarajevo" && test[2] == "Moskva" && test.brojElemenata() == 2) std::cout << "\nTEST 5. OK\n";
        else std::cout << "test 5 not ok";

        //TEST 6.
        test.obrisi(2);
        if(test.brojElemenata() == 1) std::cout << "TEST 6. OBRISI OK\n";
        else std::cout << "TEST 6. NOT OK";

        //TEST 7.
        NizMapa<int,std::string>test2(test);
        if(test2.brojElemenata() == 1) std::cout << "TEST 7. KONSTRUKTOR OK\n";
        else std::cout << "TEST 7. NOT OK";

        //TEST 8.
        test2[2] = "Moskva";
        test2[1] = "Mostar";                                              //promjena vrijednosti vec postojecem kljucu
        test2[3] = "Istanbul";
        if(test2.brojElemenata() == 3 && test2[1] == "Mostar") std::cout << "TEST 8. OK";
        else std::cout << "TEST 8. NOT OK";
    }
    return 0;
}
