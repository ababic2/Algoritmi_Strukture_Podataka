#include <iostream>

template <typename Tip>
class Lista
{
public:
    Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    //Tip& obrisi();
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual void obrisi() = 0;
    virtual Tip& operator[] (int i) const = 0;                              // za citanje
    virtual Tip& operator[] (int i) = 0;                                    //izmjena
    virtual ~Lista() = default;
};

template <typename InfoTip>
struct Cvor {
    InfoTip info;
    Cvor* next;
};


template <typename Tip>
class JednostrukaLista : public Lista<Tip>
{
    int size;
    Cvor<Tip>* start;
    Cvor<Tip>* tekuci;
    Cvor<Tip>* end;
    void Unisti();
    void Inicijaliziraj();

public:
    JednostrukaLista() : size(0), start(nullptr), tekuci(nullptr), end(nullptr) {}
    int brojElemenata() const override
    {
        return size;
    }
    Tip& trenutni() override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        return tekuci -> info;
    }
    Tip trenutni() const override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        return tekuci -> info;
    }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        tekuci = start;
    }
    void kraj() override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        for(; tekuci -> next != nullptr; tekuci = tekuci -> next);
    }

    void dodajIspred(const Tip& el) override;
    void dodajIza(const Tip& el) override;

    void obrisi() override;
    Tip& operator[] (int i) const override;                                 // za citanje
    Tip& operator[] (int i) override;                                       // za izmjenu
    ~JednostrukaLista()
    {
        while(start != nullptr) {
            tekuci = start;
            start = start -> next;
            delete tekuci;
        }
        start = nullptr;
        tekuci = nullptr;
        end = nullptr;
        size = 0;
    }

    void Brisi()
    {
        Unisti();
        Inicijaliziraj();
    }
    JednostrukaLista(const JednostrukaLista<Tip>& lista);                   // za int a = b(postojece b)
    JednostrukaLista(JednostrukaLista &&lista);                             // za int a =f(b); prestajte postojat f(b)
    JednostrukaLista& operator = (const JednostrukaLista<Tip>& lista);
    JednostrukaLista& operator = (JednostrukaLista<Tip>&& lista);
};

template <typename Tip>
JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator = (const JednostrukaLista<Tip>& lista)    //duboko kopiranje
{
    if(this == &lista) return *this;
    while(start != nullptr) {
        tekuci = start;
        start = start -> next;
        delete tekuci;
    }
    //start = nullptr; tekuci = nullptr; end = nullptr;
    end = nullptr;
    if(lista.start == nullptr) {
        start = nullptr;
        tekuci = nullptr;
        end = nullptr;
        size = 0;
    } else {
        size = lista.size;
        Cvor<Tip>* klizac = lista.start;
        Cvor<Tip>*kopirac = nullptr;
        for(; klizac != lista.end; klizac = klizac -> next) {
            if(klizac == lista.start) {
                start = new Cvor<Tip> {klizac -> info, nullptr};
                kopirac = start;
                tekuci = start;
            }
            if(klizac == lista.tekuci) {
                kopirac -> next = new Cvor<Tip> {klizac -> info, nullptr};
                tekuci = kopirac;
                kopirac = kopirac -> next;
                end = kopirac;
            } else {
                kopirac -> next = new Cvor<Tip> {klizac -> info, nullptr};
                kopirac = kopirac -> next;
                end = kopirac;
            }
        }
    }
    return *this;
}

template <typename Tip>
JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator =(JednostrukaLista<Tip> && lista)        //plitko kopiranje
{
    while(start != nullptr) {
        tekuci = start;
        start = start -> next;
        delete tekuci;
    }
    start = lista.start;
    lista.start = nullptr;
    tekuci = lista.tekuci;
    lista.tekuci = nullptr;
    end = lista.end;
    lista.end = nullptr;
    this -> size = lista.size;
    lista.size = 0;
    return *this;
}

template <typename Tip>
JednostrukaLista <Tip>::JednostrukaLista(JednostrukaLista<Tip>&& lista)         //posto je za inicijalizaciju nema nista u lijevom objektu pa ga ne moram prazniti
{
    size = lista.size;
    start = lista.start;
    lista.start = nullptr;

    tekuci = lista.tekuci;
    lista.tekuci = nullptr;

    end = lista.end;
    lista.end = nullptr;
}

template <typename Tip>
JednostrukaLista <Tip>::JednostrukaLista(const JednostrukaLista<Tip>& lista)
{
    if(lista.start == nullptr) {
        start = nullptr;
        tekuci = nullptr;
        end = nullptr;
        size = 0;
    } else {
        size = lista.size;
        Cvor<Tip>* klizac = lista.start;
        Cvor<Tip>*kopirac = nullptr;
        for(; klizac != lista.end; klizac = klizac -> next) {
            if(klizac == lista.start) {
                start = new Cvor<Tip> {klizac -> info, nullptr};
                kopirac = start;
                tekuci = start;
            }
            if(klizac == lista.tekuci) {
                kopirac -> next = new Cvor<Tip> {klizac -> info, nullptr};
                tekuci = kopirac;
                kopirac = kopirac -> next;
                end = kopirac;
            } else {
                kopirac -> next = new Cvor<Tip> {klizac -> info, nullptr};
                kopirac = kopirac -> next;
                end = kopirac;
            }
        }
    }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip& el)
{

    Cvor<Tip>* novi = new Cvor<Tip> {el, nullptr};
    if(size == 0) {
        start = novi;
        tekuci = novi;
        end = novi;
    } else {
        novi -> next = tekuci -> next;
        tekuci -> next = novi;
        if(tekuci == end)
            end = tekuci->next;
    }
    size++;
}

template <typename Tip>
inline void JednostrukaLista<Tip>::dodajIspred(const Tip& el)
{
    if(size == 0) {
        start =  new Cvor<Tip> {el, nullptr};
        tekuci = start;
        end = start;
    } else {
        if(tekuci == start) {
            Cvor<Tip>*novi = new Cvor<Tip> {el, tekuci};
            start = novi;
        } else {
            Cvor<Tip>* klizni = start;
            for(; klizni -> next != tekuci; klizni = klizni -> next);
            Cvor<Tip>* novi = new Cvor<Tip> {el, tekuci};
            klizni -> next = novi;
        }
    }
    size++;
}

template <typename Tip>
inline bool JednostrukaLista<Tip>:: prethodni()
{
    if(size == 0) throw std::domain_error("Lista prazna");
    if(tekuci == start) return false;
    Cvor<Tip>* klizni = start;
    for(; klizni -> next != tekuci; klizni = klizni -> next);
    tekuci = klizni;
    return true;
}

template <typename Tip>
inline bool JednostrukaLista<Tip>:: sljedeci()
{
    if(size == 0) throw std::domain_error("Lista prazna");
    if(tekuci -> next == nullptr) return false;
    tekuci = tekuci -> next;
    return true;
}

template <typename Tip>
inline void JednostrukaLista<Tip>::obrisi()
{
    if(size == 0) throw std::domain_error("Nema elemenata u listi");
    if(tekuci == start) {
        if(size == 1) {
            start = nullptr;
            delete tekuci;
            tekuci = nullptr;

            end = nullptr;
        } else {
            start = start -> next;                                              // ili pocetak = tekuci -> next
            delete tekuci;
            tekuci = start;

        }
    } else {
        Cvor<Tip>* klizac = start;
        for(; klizac -> next != tekuci; klizac = klizac -> next);
        klizac -> next = tekuci -> next;

        delete tekuci;
        tekuci = klizac -> next;
        if(tekuci == nullptr) {
            tekuci = klizac;
            end = klizac;
        }
    }
    size--;
}

template <typename Tip>
inline Tip& JednostrukaLista<Tip>::operator[] (int i) const
{
    if(i < 0 || i >= size) throw std::domain_error("Neispravan indeks");
    Cvor<Tip>*find = start;
    for(int x = 0; x < i; x++)
        find = find -> next;
    return find -> info;
}

template <typename Tip>
inline Tip& JednostrukaLista<Tip>::operator[] (int i)
{
    if(i < 0 || i >= size) throw std::domain_error("Neispravan indeks");
    Cvor<Tip>*find = start;
    for(int x = 0; x < i; x++)
        find = find -> next;
    return find -> info;
}

template <typename Tip>
class NizLista : public Lista<Tip>
{
private:
    int kapacitet;
    int duzina;
    int tekuci; // pozicija tekuceg

    Tip** lista; //niz za elemente liste

public:

    NizLista()
    {
        try {
            kapacitet = 100;
            duzina = 0;
            tekuci = 0;
            lista = new Tip* [kapacitet] {};
        } catch(...) {
            delete [] lista;
        }
    }
    int brojElemenata() const override
    {
        return duzina;
    }
    Tip& trenutni() override
    {
        if(duzina == 0) throw std::domain_error("Prazan nizLista");
        return *lista[tekuci];
    }
    Tip trenutni() const override
    {
        if(duzina == 0) throw std::domain_error("Prazna nizLista");
        return *lista[tekuci];
    }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override
    {
        if(duzina == 0) throw std::domain_error("Prazna lista");
        tekuci = 0;
    }
    void kraj() override
    {
        if(duzina == 0) throw std::domain_error("Prazna lista");
        tekuci = duzina - 1;
    }

    void dodajIspred(const Tip& el) override;
    void dodajIza(const Tip& el) override;

    void obrisi() override;
    Tip& operator[] (int i) const override;
    Tip& operator[] (int i) override;
    ~NizLista()
    {
        for(int i = 0; i < duzina; i++) delete lista[i];
        delete [] lista;
        lista = nullptr;
    }

    NizLista(const NizLista<Tip>& niz);                                     // za int a = b(postojece b)
    NizLista(NizLista<Tip> &&niz);                                          // za int a =f(b); prestajte postojat f(b)
    NizLista& operator = (const NizLista<Tip>& niz);
    NizLista& operator = (NizLista<Tip>&& niz);
};

template<typename Tip>
inline bool NizLista<Tip>::sljedeci()
{
    if(tekuci == duzina - 1) return false;
    tekuci++;
    return true;
}

template <typename Tip>
inline bool NizLista<Tip>::prethodni()
{
    if(tekuci == 0) return false;
    tekuci--;
    return true;
}

template <typename Tip>
inline void NizLista<Tip>::dodajIspred(const Tip& el)
{
    if(duzina == 0) {
        lista[0] = new Tip(el);
        tekuci = 0;
    } else {
        //1 2 3 4 5 6. 7 8 9 10
        //1 2 3 4 5 -1 6. 7 8 9 10
        if(duzina==kapacitet) {
            Tip** temp = new Tip*[2*kapacitet] {};
            for(int i=0; i<duzina; i++) {
                temp[i]=lista[i];
                lista[i]=nullptr;
            }
            kapacitet*=2;//jao
            delete[] lista;
            lista = temp;
        }

        for(int i=duzina; i>tekuci; i--) {
            lista[i]=lista[i-1];
        }
        lista[tekuci] = nullptr;
        lista[tekuci] = new Tip(el);
        tekuci++;
    }
    duzina++;
}

template <typename Tip>
inline void NizLista<Tip>::dodajIza(const Tip& el)
{
    if(duzina == 0) {
        lista[0] = new Tip(el);
        tekuci = 0;
    } else {
        if(duzina==kapacitet) {
            Tip** temp = new Tip*[2*kapacitet] {};
            for(int i=0; i<duzina; i++) {
                temp[i]=lista[i];
                lista[i]=nullptr;
            }
            kapacitet*=2;
            delete[] lista;
            lista = temp;
        }
        for(int i=duzina; i>tekuci+1; i--) {
            lista[i]=lista[i-1];
        }
        lista[tekuci+1] = new Tip(el);
    }
    duzina++;
}

template <typename Tip>
inline Tip& NizLista<Tip>::operator [] (int i) const
{
    if(i < 0 || i >= duzina) throw std::domain_error("Neispravan indeks");
    return *lista[i];
}

template <typename Tip>
inline Tip& NizLista<Tip>::operator [] (int i)
{
    if(i < 0 || i >= duzina) throw std::domain_error("Neispravan indeks");
    return *lista[i];
}

template <typename Tip>
inline void NizLista<Tip>::obrisi()
{
    auto temp = lista[tekuci];
    for(int i=tekuci; i<duzina-1; i++) {
        lista[i] = lista[i+1];
    }
    lista[duzina-1]=nullptr;
    delete temp;
    duzina--;
    if(tekuci==duzina)tekuci--;
}

template <typename Tip>
NizLista<Tip>::NizLista(const NizLista<Tip> &niz)
{
    lista = new Tip*[niz.kapacitet] {};
    kapacitet = niz.kapacitet;
    duzina = niz.duzina;
    tekuci = niz.tekuci;
    for(int i = 0; i < duzina; i++) {
        lista[i] = new Tip(*niz.lista[i]);
    }
}

template <typename Tip>
NizLista<Tip>::NizLista(NizLista<Tip> &&niz)
{
    duzina = niz.duzina;
    kapacitet = niz.kapacitet;
    tekuci = niz.tekuci;
    niz.tekuci = 0;
    lista = niz.lista;
    niz.lista = nullptr;
}

template <typename Tip>
NizLista<Tip>& NizLista<Tip>::operator =(const NizLista<Tip> &niz)
{
    if(&niz == this)return *this;
    for(int i=0; i<duzina; i++) {
        delete lista[i];
    }
    delete[] lista;
    lista = new Tip*[niz.kapacitet] {};
    kapacitet = niz.kapacitet;
    duzina = niz.duzina;
    tekuci = niz.tekuci;
    for(int i = 0; i < duzina; i++) {
        lista[i] = new Tip(*niz.lista[i]);
    }
    return *this;
}

template <typename Tip>
NizLista<Tip>& NizLista<Tip>::operator =(NizLista<Tip> && niz)
{
    if(&niz == this)return *this;
    for(int i=0; i<duzina; i++) {
        delete lista[i];
    }
    delete[] lista;
    duzina = niz.duzina;
    kapacitet = niz.kapacitet;
    lista = niz.lista;
    niz.lista = nullptr;
    return *this;
}

void TestKonstruktora() {
    JednostrukaLista<int> lista;
    if(lista.brojElemenata() == 0) std::cout << "\nTEST KONSTRUKTORA BEZ PARAMETARA USPJESAN\n";
    else std::cout << "\nTEST KONSTRUKTORA BEZ PARAMETARA NEUSPJESAN\n";
}

template <typename Tip>
void Test_dodajIspred(const Tip& l) {
    Tip lista;
    lista.dodajIspred(1);
    for(int i = 1; i < 15; i++) lista.dodajIspred(i);
    lista.prethodni();
    if(lista.trenutni() == 14 && lista.brojElemenata() == 15) std::cout << "\nTEST METODE dodajIspred() USPJESAN\n ";
    else  std::cout << "\nTEST METODE dodajIspred() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_dodajIza(const Tip& l) {
    Tip lista;
    lista.dodajIza(1);
    for(int i = 1; i < 15; i++) lista.dodajIza(i);
    lista.sljedeci();
    if(lista.trenutni() == 14 && lista.brojElemenata() == 15) std::cout << "\nTEST METODE dodajIza() USPJESAN\n ";
    else  std::cout << "\nTEST METODE dodajIza() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_brojElemenata(const Tip& l) {
    Tip lista;
    lista.dodajIza(1);
    lista.dodajIspred(2);
    lista.dodajIspred(2);
    if(lista.brojElemenata() == 3) std::cout << "\nTEST METODE brojElemenata() USPJESAN\n ";
    else std::cout << "\nTEST METODE brojElemenata() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_obrisi(const Tip& l) {
    Tip lista;
    lista.dodajIspred(1);
    lista.dodajIspred(2);
    lista.obrisi();
    if(lista.trenutni() == 2)  std::cout << "\nTEST METODE obrisi() USPJESAN\n ";
    else std::cout << "\nTEST METODE obrisi() NEUSPJESAN\n ";

}

template <typename Tip>
void Test_prethdoni(const Tip& l) {
    bool proslo = false;
    Tip lista;
    lista.dodajIspred(1);

    if(lista.prethodni() == false) proslo = true;

    lista.dodajIza(3);
    lista.dodajIspred(2);
    lista.prethodni();
    if(lista.trenutni() == 2 && proslo)  std::cout << "\nTEST METODE prethodni() USPJESAN\n ";
    else std::cout << "\nTEST METODE prethodni() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_sljedeci(const Tip& l) {
    bool proslo = false;
    Tip lista;
    lista.dodajIspred(1);
    if(lista.sljedeci() == false) proslo = true;
    lista.dodajIspred(2);
    lista.dodajIza(3);
    lista.sljedeci();
    if(lista.trenutni() == 3 && proslo)  std::cout << "\nTEST METODE sljedeci() USPJESAN\n ";
    else std::cout << "\nTEST METODE sljedeci() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_trenutni(const Tip& l) {
    Tip lista;
    for(int i = 11; i < 20; i++)
        lista.dodajIspred(i);

    if(lista.trenutni() == 11)  std::cout << "\nTEST METODE trenutni() USPJESAN\n ";
    else std::cout << "\nTEST METODE trenutni() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_pocetak(const Tip& l) {
    Tip lista;
    for(int i = 11; i < 20; i++)
        lista.dodajIspred(i);

    lista.pocetak();
    if(lista.trenutni() == 12)  std::cout << "\nTEST METODE pocetak() USPJESAN\n ";
    else std::cout << "\nTEST METODE pocetak() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_kraj(const Tip& l) {
    Tip lista;
    for(int i = 11; i < 20; i++)
        lista.dodajIza(i);

    lista.kraj();
    if(lista.trenutni() == 12)  std::cout << "\nTEST METODE kraj() USPJESAN\n ";
    else std::cout << "\nTEST METODE kraj() NEUSPJESAN\n ";
}

template <typename Tip>
void Test_operator_izdvoji(const Tip& l) {
    Tip lista;
    lista.dodajIza(1);
    for(int i = 1; i < 5; i++) lista.dodajIza(i);
    if(lista[3] == 2) std::cout << "\nTEST METODE operator[] USPJESAN\n ";
    else  std::cout << "\nTEST METODE operator [] NEUSPJESAN\n ";
}

int main()
{
    std::cout << "\nTESTOVI METODA JEDNOSTRUKE LISTE\n";

    JednostrukaLista<int> lista;  //samo radi template prepoznavanja tipa
    TestKonstruktora();
    Test_dodajIspred(lista);
    Test_dodajIza(lista);
    Test_brojElemenata(lista);
    Test_obrisi(lista);
    Test_prethdoni(lista);
    Test_sljedeci(lista);
    Test_trenutni(lista);
    Test_pocetak(lista);
    Test_kraj(lista);
    Test_operator_izdvoji(lista);


    std::cout << "\nTESTOVI METODA NIZLISTA\n";

    NizLista<int> niz_lista;  //samo radi template prepoznavanja tipa

    Test_dodajIspred(niz_lista);
    Test_dodajIza(niz_lista);
    Test_brojElemenata(niz_lista);
    Test_obrisi(niz_lista);
    Test_prethdoni(niz_lista);
    Test_sljedeci(niz_lista);
    Test_trenutni(niz_lista);
    Test_pocetak(niz_lista);
    Test_kraj(niz_lista);
    Test_operator_izdvoji(niz_lista);
    return 0;
}
