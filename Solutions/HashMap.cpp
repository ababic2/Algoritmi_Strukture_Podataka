#include <iostream>
#include <limits>
#include <vector>
#include <list>
#include <string>
#include <queue>

//sve sto ima kopirano, koirala sam sa svojih zadaca i pripremih radi brzeg rada zadace
template<typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual TipVrijednosti& operator[](const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& kljuc)const = 0;

    virtual int brojElemenata() const
    {
        return 0;
    }

    virtual void obrisi() {}
    virtual void obrisi(const TipKljuca& kljuc) {}
    virtual ~Mapa() {}
};

template<typename TipKljuca, typename TipVrijednosti>
struct Pair {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
};

template<typename tKey,typename tVal>
class HashMapaLan : public Mapa<tKey,tVal>
{
    Pair<tKey,tVal>** niz;
    Pair<tKey,tVal> deleted;
    int brojEl;
    int kapacitet = 10;
    unsigned int (*hash)(tKey ulaz, unsigned int opseg_izlaza);
    int helpHash(tKey k, int i) const
    {
        int n = ((k % 10) + 1)%i;
        return n;
    }
    void realociraj();
    int dodaj(Pair<tKey,tVal>** array, unsigned int capacity, Pair<tKey,tVal>* par);
public:

    HashMapaLan()
    {
        brojEl = 0;
        niz = new Pair<tKey, tVal>*[kapacitet] {};
        hash = nullptr;
        kapacitet = 10;
    }

    ~HashMapaLan()
    {
        obrisi();
        delete[] niz;
    }

    HashMapaLan(const HashMapaLan& mapa);

    HashMapaLan(HashMapaLan&& mapa);

    HashMapaLan& operator=(HashMapaLan&& mapa);
    HashMapaLan& operator=(const HashMapaLan& mapa);

    tVal operator[](const tKey& kljuc) const;
    tVal& operator[](const tKey& kljuc);

    //metode kopirane iz klase NizMapa
    int brojElemenata() const override
    {
        return brojEl;
    }

    void obrisi() override;
    void obrisi(const tKey& kljuc) override;

    void definisiHashFunkciju(unsigned int (*hash)(tKey ulaz, unsigned int opseg_izlaza))
    {
        this->hash = hash;
    }
};

template<typename tKey, typename tVal>
int HashMapaLan<tKey,tVal>::dodaj(Pair<tKey,tVal>** array, unsigned int capacity, Pair<tKey,tVal>* par)
{
    int index = (*hash)(par->kljuc,capacity);
    for(int i=index;;) {
        if(array[i]==nullptr) {
            array[i] = par;
            return i;
        }
        i=(i+1)%capacity;
        if(i==index)return -1;
    }
}

template<typename tKey, typename tVal>
void HashMapaLan<tKey,tVal>::realociraj()
{
    Pair<tKey,tVal>** temp = new Pair<tKey,tVal>*[kapacitet*2] {};
    for(int i=0; i<kapacitet; i++) {
        if(niz[i] && niz[i]!=&deleted) {
            dodaj(temp,2*kapacitet,niz[i]);
        }
    }
    for(int i=0; i<kapacitet; i++) {
        niz[i]=nullptr;
    }
    delete[] niz;
    niz = temp;
    kapacitet = 2*kapacitet;
}

template<typename tKey, typename tVal>
tVal& HashMapaLan<tKey,tVal>:: operator[](const tKey& kljuc)
{
    if(hash == nullptr) throw std::domain_error("Error");
    int index = (*hash)(kljuc,kapacitet);
    // ovo je indeks na kojem bi trebao da se nalazi ovaj element
    for(int i=index;;) {
        if(niz[i]==nullptr) {
            niz[i]=new Pair<tKey,tVal> {kljuc,tVal()};
            brojEl++;
            return niz[i]->vrijednost;
        } else if(niz[i]!=&deleted && niz[i]->kljuc==kljuc) {
            return niz[i]->vrijednost;
        }
        i = (i+1)%kapacitet;// ovako cemo pretragu nastaviti od pocetka niza ako dođe do zadnjeg indeksa
        if(i==index) {//ovo se desi kad se vratimo na pocetno mjesto odakle je zapocela pretraga
            // nema vise prostora
            // realociraj
            realociraj();
            index = dodaj(niz,kapacitet,new Pair<tKey,tVal> {kljuc,tVal()});
            brojEl++;
            return niz[index]->vrijednost;
        }
    }
}


template<typename tKey, typename tVal>
tVal HashMapaLan<tKey,tVal>::operator[](const tKey& kljuc)const
{
    if(hash == nullptr) throw std::domain_error("Error");
    int index = (*hash)(kljuc,kapacitet);

    for(int i = index;;) {

        if(niz[i]==nullptr) {
            return tVal();
        } else if(niz[i]!=&deleted && niz[i]->kljuc == kljuc) {
            return niz[i]->vrijednost;
        }
        i=(i+1)%kapacitet;
        if(i==index) {
            return tVal();
        }

    }
}
template<typename tKey, typename tVal>
HashMapaLan<tKey,tVal>& HashMapaLan<tKey,tVal>::operator=(HashMapaLan<tKey,tVal>&& mapa)
{
    if(this == &mapa) return *this;
    obrisi();
    delete[] niz;
    kapacitet = mapa.kapacitet;
    niz = mapa.niz;
    mapa.niz = nullptr;
    brojEl = mapa.brojEl;
    hash = mapa.hash;
    for(int i=0; i<kapacitet; i++) {
        if(niz[i]==&(mapa.deleted)) {
            niz[i]=&deleted;
        }
    }
    return *this;
}

template<typename tKey, typename tVal>
HashMapaLan<tKey,tVal>& HashMapaLan<tKey,tVal>::operator=(const HashMapaLan<tKey,tVal>& mapa)
{
    if(this == &mapa) return *this;
    obrisi();
    delete[] niz;
    kapacitet = mapa.kapacitet;
    brojEl = mapa.brojEl;
    hash = mapa.hash;
    niz = new Pair<tKey,tVal>*[kapacitet] {};
    for(int i = 0; i < kapacitet; i++) {
        if(mapa.niz[i]) {
            if(mapa.niz[i]==&(mapa.deleted)) {
                niz[i] = &deleted;
            } else {
                niz[i]  = new Pair<tKey,tVal> {mapa.niz[i]->kljuc,mapa.niz[i]->vrijednost};
            }
        }
    }

    return *this;
}

template<typename tKey, typename tVal>
HashMapaLan<tKey,tVal>::HashMapaLan(HashMapaLan&& mapa)
{
    kapacitet = mapa.kapacitet;
    brojEl = mapa.brojEl;
    niz = mapa.niz;
    mapa.niz = nullptr;
    hash = mapa.hash;
    for(int i=0; i<kapacitet; i++) {
        if(niz[i]==&(mapa.deleted)) {
            niz[i]=&deleted;
        }
    }
}

template<typename tKey, typename tVal>
HashMapaLan<tKey,tVal>::HashMapaLan(const HashMapaLan<tKey,tVal>&mapa)
{
    kapacitet = mapa.kapacitet;
    niz = new Pair<tKey,tVal>*[kapacitet] {};
    brojEl = mapa.brojEl;
    hash = mapa.hash;
    for(int i = 0; i < kapacitet; i++)
        if(mapa.niz[i]) {
            if(mapa.niz[i]==&(mapa.deleted)) {
                niz[i] = &deleted;
            } else {
                niz[i]  = new Pair<tKey,tVal> {mapa.niz[i]->kljuc,mapa.niz[i]->vrijednost};
            }
        }
}

template<typename tKey, typename tVal>
inline void HashMapaLan<tKey,tVal>::obrisi()
{
    brojEl = 0;
    for(int i=0; i<kapacitet; i++) {
        if(niz[i]) {
            if(niz[i]!=&deleted) {
                delete niz[i];
            }
            niz[i]=nullptr;
        }
    }
}

template<typename tKey, typename tVal>
inline void HashMapaLan<tKey,tVal>::obrisi(const tKey& kljuc)
{
    int index = (*hash)(kljuc,kapacitet);
    if(niz[index]) {
        if(niz[index] != &deleted && niz[index]->kljuc==kljuc) {
            delete niz[index];
            niz[index] = &deleted;
            brojEl--;
        } else {
            int start = (index==kapacitet-1)?0:index+1;
            // krecemo sa pretragom za prvo slobodno mjesto

            for(int i=start; ; i=(i+1)%kapacitet) {
                if(i==index) {
                    // nema ga
                    break;
                }
                if(niz[i] && niz[i] != &deleted && niz[i]->kljuc==kljuc) {
                    delete niz[i];
                    niz[i] = &deleted;
                    brojEl--;
                    break;
                }
            }
        }
    }
}

template<typename Tip>
class Cvor;

template<typename Tip>
class Grana;

template<typename Tip>
class GranaIterator;

template<typename Tip>
class UsmjereniGraf {

    public:
    UsmjereniGraf(int n){} // tsdgdsgsdg
    UsmjereniGraf(){}

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int n) = 0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina = 0) =0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const = 0;
    virtual bool postojiGrana(int polazni, int dolazni) const = 0;
    virtual void postaviOznakuCvora(int cvor, Tip oznaka) = 0;
    virtual Tip dajOznakuCvora(int cvor) const = 0;
    virtual void postaviOznakuGrane(int polazni, int dolazni,Tip oznaka) = 0;
    virtual Tip dajOznakuGrane(int polazni,int dolazni) const = 0;
    virtual Grana<Tip> dajGranu(int polazni, int dolazni) = 0;
    virtual Cvor<Tip>dajCvor(int cvor)  = 0; // kkkj
    GranaIterator<Tip> dajGranePocetak() {
        return ++GranaIterator<Tip>(this,0 ,-1);
    }


    GranaIterator<Tip> dajGraneKraj() {
        return GranaIterator<Tip>(this,dajBrojCvorova(),0);
    }


    virtual ~UsmjereniGraf(){};
};

template<typename Tip>
class Grana {

private:
    UsmjereniGraf<Tip>* graf;
    int polazni, dolazni;

public:
    Grana(UsmjereniGraf<Tip>*graf, int polazni, int dolazni) : graf(graf), polazni(polazni),dolazni(dolazni){}
    float dajTezinu() const { return graf->dajTezinuGrane(polazni, dolazni); }
    void postaviTezinu(float tezina) {
        graf->postaviTezinuGrane(polazni,dolazni,tezina);
    }
    Tip dajOznaku()const { return graf->dajOznakuGrane(polazni,dolazni); }
    void postaviOznaku(Tip oznaka) { graf -> postaviOznakuGrane(polazni,dolazni,oznaka);}
    Cvor<Tip> dajPolazniCvor() const { return graf->dajCvor(polazni); }
    Cvor<Tip> dajDolazniCvor() const { return graf->dajCvor(dolazni); }
};

template<typename Tip>
class Cvor {
private:
    UsmjereniGraf<Tip>* graf;
    int broj;
public:
    Cvor(UsmjereniGraf<Tip>*graf, int redni) : graf(graf), broj(redni){}
    Tip dajOznaku() const { return graf->dajOznakuCvora(broj); }
    void postaviOznaku(Tip oznaka) { graf -> postaviOznakuCvora(broj,oznaka); }
    int dajRedniBroj() const { return broj;}
};

template<typename Tip>
class GranaIterator {
private:
    UsmjereniGraf<Tip>*graf;
    int i, j;
public:
    GranaIterator(UsmjereniGraf<Tip>*graf,int i, int j):graf(graf),i(i),j(j){}
    Grana<Tip> operator*() {return graf -> dajGranu(i,j);}
    bool operator==(const GranaIterator &iter) const  {
        return (i == iter.i && j == iter.j && graf == iter.graf);
    }
    bool operator!=(const GranaIterator &iter) const {
        return !(iter == *this);
    }
    GranaIterator &operator++() {
        do {
            j+1 >= graf->dajBrojCvorova() ? j = 0,i++ : j++;
        }while(i < graf->dajBrojCvorova() && !(graf -> postojiGrana(i,j)));
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator<Tip> temp(*this);
        ++(*this);
        return temp;
    }
};

template<typename Tip>
class MatricaGraf : public UsmjereniGraf<Tip>{

private:
    std::vector<std::vector<float>>tezina;
    std::vector<Tip> oznakaCvora;
    std::vector<std::vector<Tip>>oznakaGrane;
    bool okejCvor(int broj) const {
        if(broj < 0 || broj >= dajBrojCvorova()) return false;
        return true;
    }

public:
    MatricaGraf(int vel) {
        if(vel < 0) throw std::domain_error("Not valid size");
        tezina = std::vector<std::vector<float>>(vel, std::vector<float>(vel,-1));
        oznakaCvora = std::vector<Tip>(vel);
        oznakaGrane = std::vector<std::vector<Tip>>(vel,std::vector<Tip>(vel));
    }

    ~MatricaGraf(){}


    int dajBrojCvorova() const { return tezina.size();}

    void postaviBrojCvorova(int n) {
        if(n < 0 || n < dajBrojCvorova()) throw std::domain_error("Not valid number of nodes");
        tezina.resize(n);
        for(int i = 0; i < n; i++)
            tezina[i].resize(n, -1);

        oznakaCvora.resize(n);

        oznakaGrane.resize(n);
        for(int i = 0; i < n; i++)
            oznakaGrane[i].resize(n);
    }

    void dodajGranu(int polazni, int dolazni, float tezinaa = 0) {
        if(okejCvor(polazni) && okejCvor(dolazni))
        tezina[polazni][dolazni] = tezinaa;
    }

    void obrisiGranu(int polazni, int dolazni) {
        if(okejCvor(polazni) && okejCvor(dolazni))
            tezina[polazni][dolazni] = -1;
    }

    void postaviTezinuGrane(int polazni, int dolazni, float tezinaa = 0) {
        if(okejCvor(polazni) && okejCvor(dolazni))
          tezina[polazni][dolazni] = tezinaa;
    }

    float dajTezinuGrane(int polazni, int dolazni) const {
        //if(okejCvor(polazni) && okejCvor(dolazni) && tezina[polazni][dolazni] != -1)
          if(postojiGrana(polazni,dolazni))
            return tezina[polazni][dolazni];
    }

    bool postojiGrana(int polazni, int dolazni) const {
        if(okejCvor(polazni) && okejCvor(dolazni) && tezina[polazni][dolazni] != -1) return true; // provjeri ba
        return false;
    }

    void postaviOznakuCvora(int cvor, Tip oznaka) {
        if(okejCvor(cvor))
            oznakaCvora[cvor] = oznaka;
    }

    Tip dajOznakuCvora(int cvor) const {
        if(okejCvor(cvor)) return oznakaCvora[cvor];
    }

    void postaviOznakuGrane(int polazni, int dolazni,Tip oznaka) {
        if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
        oznakaGrane[polazni][dolazni] = oznaka;
    }

    Tip dajOznakuGrane(int polazni,int dolazni) const {
        if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
        return oznakaGrane[polazni][dolazni];

    }

    Grana<Tip> dajGranu(int polazni, int dolazni){
        if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
            return Grana<Tip>(this,polazni,dolazni);
    }

    Cvor<Tip>dajCvor(int cvor) {
        if(okejCvor(cvor)) return Cvor<Tip>(this, cvor);
    }

    GranaIterator<Tip> dajGranePocetak();
    GranaIterator<Tip> dajGraneKraj();
};

template<typename Tip>
void dfs(UsmjereniGraf<Tip>*graf, std::vector<Cvor<Tip>> &discovered, Cvor<Tip>cvor) {
    /// polazni kec dobije i idemo next susjede
    cvor.postaviOznaku(1);
    discovered.push_back(cvor);
    for(GranaIterator<Tip> i = graf->dajGranePocetak(); i != graf->dajGraneKraj(); ++i) {
        Cvor<Tip> susjed = (*i).dajDolazniCvor();
        if((*i).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() && susjed.dajOznaku() != 1)
            dfs(graf,discovered,susjed);
    }
}

template<typename Tip>
void bfs(UsmjereniGraf<Tip>*graf, std::vector<Cvor<Tip>>&cvorovi, Cvor<Tip>cvor) {
    int trenutni = cvor.dajRedniBroj();
    bool ima = 0;
    cvorovi.push_back(cvor);
    recursive_bfs(graf,cvorovi,trenutni);
}

template<typename Tip>
void recursive_bfs(UsmjereniGraf<Tip>*graf, std::vector<Cvor<Tip>>&cvorovi,int trenutni) {
    bool ciklus = 0;
    int size = cvorovi.size();
    for(GranaIterator<Tip>it = graf->dajGranePocetak(); it != graf->dajGraneKraj(); ++it) {
        ciklus = 0;
        if((*it).dajPolazniCvor().dajRedniBroj() == trenutni) {
            int sljedeci = (*it).dajPolazniCvor().dajRedniBroj();
            for(Cvor<Tip>x:cvorovi) {
                if(x.dajRedniBroj() == sljedeci){
                    ciklus = 1;
                    break;
                }
            }
            if(ciklus) ciklus = 0;
            else cvorovi.push_back((*it).dajDolazniCvor());
        }
    }
    int max = cvorovi.size();
    for(int i = size; i < max; i++)
        recursive_bfs(graf,cvorovi,cvorovi[i].dajRedniBroj());

}

template<typename Tip>
struct Element {
    int cvor;
    float tezina;
    Tip oznaka;
};

// template<typename Tip>
// class ListaGraf : public UsmjereniGraf<Tip> {

//     private:
//     std::vector<std::list<Element<Tip>>>l;
//     std::vector<Tip> oznakaCvora;
//     //std::vector<std::vector<Tip>>oznakaGrane;
//     bool okejCvor(int broj) const {
//         if(broj < 0 || broj >= dajBrojCvorova()) return false;
//         return true;
//     }
//     typename std::list<Element<Tip>>::iterator traziELement(int polazni,int dolazni) {
//         auto it = l[polazni].begin();
//         while(it != l[polazni].end()) {
//             if(dolazni == it->cvor) return it;
//             else if(dolazni < it->cvor)return l[polazni].end();
//             it++;
//         }
//         return it;
//     }

// public:
//     ListaGraf(int n) {
//         if(n < 0) throw std::domain_error("Not valid size");
//         l.resize(n);
//         oznakaCvora.resize(n);
//     }


//     int dajBrojCvorova() const { return l.size();}

//     void postaviBrojCvorova(int n) {
//         if(n < 0 || n < dajBrojCvorova()) throw std::domain_error("Not valid number of nodes");
//         l.resize(n);
//         oznakaCvora.resize(n);
//     }


//     void obrisiGranu(int polazni, int dolazni) {
//         if(okejCvor(polazni) && okejCvor(dolazni))
//             l[polazni].erase(traziELement(polazni,dolazni));
//     }


//     void postaviOznakuCvora(int cvor, Tip oznaka) {
//         if(okejCvor(cvor))
//             oznakaCvora[cvor] = oznaka;
//     }

//     Tip dajOznakuCvora(int cvor) const {
//         if(okejCvor(cvor)) return oznakaCvora[cvor];
//     }



//     Grana<Tip> dajGranu(int polazni, int dolazni){
//         if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
//             return Grana<Tip>(this,polazni,dolazni);
//     }

//     Cvor<Tip>dajCvor(int cvor) {
//         if(okejCvor(cvor)) return Cvor<Tip>(this, cvor);
//     }

//     GranaIterator<Tip> dajGranePocetak();
//     GranaIterator<Tip> dajGraneKraj();





//     // void postaviOznakuGrane(int polazni, int dolazni,Tip oznaka) {
//     //     if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
//     //     oznakaGrane[polazni][dolazni] = oznaka;
//     // }

//     // Tip dajOznakuGrane(int polazni,int dolazni) const {
//     //     if(okejCvor(polazni) && okejCvor(dolazni) && postojiGrana(polazni,dolazni))
//     //     return oznakaGrane[polazni][dolazni];

//     // }

//     //  void postaviTezinuGrane(int polazni, int dolazni, float la = 0) {
//     //     if(okejCvor(polazni) && okejCvor(dolazni))
//     //       l[polazni][dolazni] = la;
//     // }

//     // float dajTezinuGrane(int polazni, int dolazni) const {
//     //     //if(okejCvor(polazni) && okejCvor(dolazni) && l[polazni][dolazni] != -1)
//     //       if(postojiGrana(polazni,dolazni))
//     //         return l[polazni][dolazni];
//     // }

//     // bool postojiGrana(int polazni, int dolazni) const {
//     //     if(okejCvor(polazni) && okejCvor(dolazni) && l[polazni][dolazni] != -1) return true; // provjeri ba
//     //     return false;
//     // }
//     // void dodajGranu(int polazni, int dolazni, float la = 0) {
//     //     if(okejCvor(polazni) && okejCvor(dolazni))
//     //     l[polazni][dolazni] = la;
//     // }
// };

int main() {
    std::cout << "Zadaća 5, Zadatak 1";
    return 0;
}
