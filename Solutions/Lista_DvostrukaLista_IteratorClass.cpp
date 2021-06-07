#include <iostream>

template<typename Tip>
class Iterator;


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
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual void obrisi() = 0;
    virtual Tip& operator[] (int i) const = 0;
    virtual Tip& operator[] (int i) = 0;
    virtual ~Lista() = default;
};

template <typename Tip>
struct Cvor {
    Tip info;
    Cvor* previous;
    Cvor* next;

    Cvor(Cvor* previous = nullptr, const Tip &info = 0, Cvor* next = nullptr) : info(info), previous(previous), next(next) {}
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip>
{

    Cvor<Tip>* start;
    Cvor<Tip>* tekuci;
    Cvor<Tip>* end;
    int size;
    void Unisti();

public:
    DvostrukaLista() : start(nullptr), tekuci(nullptr), end(nullptr), size(0) {}
    int brojElemenata() const override
    {
        return size;
    }

    Tip& trenutni() override
    {
        return tekuci -> info;
    }
    Tip trenutni() const override
    {
        return tekuci -> info;
    }

    bool prethodni() override
    {
        if(size == 0) throw std::domain_error("Lista prazna");
        else if(tekuci == start) return false;
        tekuci = tekuci -> previous;
        return true;
    }
    bool sljedeci() override
    {
        if(size == 0) throw std::domain_error("Lista prazna");
        if(tekuci == end) return false;
        tekuci = tekuci -> next;
        return true;
    }

    void pocetak() override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        tekuci = start;
    }
    void kraj() override
    {
        if(size == 0) throw std::domain_error("Prazna lista");
        tekuci = end;
    }

    Tip& operator[] (int i) const override
    {
        if(i < 0 || i >= size) throw std::range_error("Invalid index");
        Cvor<Tip>* klizac = start;
        for(int j = 0; j < i; j++)
            klizac = klizac -> next;

        return klizac -> info;
    }
    Tip& operator[] (int i) override
    {
        if(i < 0 || i >= size) throw std::range_error("Invalid index");
        Cvor<Tip>* klizac = start;
        for(int j = 1; j <= i; j++)
            klizac = klizac -> next;

        return klizac -> info;
    }

    ~DvostrukaLista()
    {
        Unisti();
    }

    void obrisi() override;

    DvostrukaLista(const DvostrukaLista<Tip>& lista);
    DvostrukaLista(DvostrukaLista &&lista);
    DvostrukaLista& operator = (const DvostrukaLista<Tip>& lista);
    DvostrukaLista& operator = (DvostrukaLista<Tip>&& lista);

    void dodajIspred(const Tip& el) override;
    void dodajIza(const Tip& el) override;

    friend class Iterator<Tip>;
};

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>:: operator= (const DvostrukaLista<Tip> &lista)
{

    if(this == &lista) return *this;

    Unisti();

    size = lista.size;

    Cvor<Tip>*klizni, *copy;
    for(klizni = lista.start; klizni != lista.end; klizni = klizni -> next) {
        if(klizni == lista.start) {
            start = new Cvor<Tip>(nullptr, klizni -> info, nullptr);
            copy = start;
            if(klizni == lista.tekuci)tekuci = start;
        } else {
            copy -> next = new Cvor<Tip>(copy, klizni -> info, nullptr);
            if(klizni == lista.tekuci) tekuci = copy->next;
            copy = copy -> next;
        }
    }
    end = copy;
    return *this;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::operator =(DvostrukaLista<Tip> &&lista)
{

    if(this == &lista) return *this;

    Unisti();

    size = lista.size;
    start = lista.start;
    lista.start = nullptr;
    lista.size=0;
    tekuci = lista.tekuci;
    lista.tekuci = nullptr;

    end = lista.end;
    lista.end = nullptr;

    return *this;
}


template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista<Tip> &lista)
{

    size = lista.size;

    Cvor<Tip>*klizni, *copy;
    for(klizni = lista.start; klizni != lista.end; klizni = klizni -> next) {
        if(klizni == lista.start) {
            start = new Cvor<Tip>(nullptr, klizni -> info, nullptr);
            copy = start;
            if(klizni == lista.tekuci)tekuci = start;
        } else {
            copy -> next = new Cvor<Tip>(copy, klizni -> info, nullptr);
            if(klizni == lista.tekuci) tekuci = copy->next;
            copy = copy -> next;
        }
    }
    end = copy;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(DvostrukaLista<Tip> &&lista)
{


    size = lista.size;
    start = lista.start;
    lista.start = nullptr;

    tekuci = lista.tekuci;
    lista.tekuci = nullptr;
    lista.size=0;
    end = lista.end;
    lista.end = nullptr;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el)
{
    if(size == 0) {
        start = new Cvor<Tip>(nullptr, el, nullptr);
        tekuci = start;
        end = start;
    }

    else if(tekuci == start) {
        Cvor<Tip>* novi = new Cvor<Tip>(nullptr, el, tekuci);
        tekuci -> previous = novi;
        start = novi; /// AMINA !!!!!
    }

    else {
        Cvor<Tip>* novi = new Cvor<Tip>(tekuci -> previous, el, tekuci);
        tekuci -> previous -> next = novi;   // ovdje ti je bitan poredak, crtaj na papir
        tekuci ->previous = novi;
    }
    size++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &el)
{

    if(size == 0) {
        start = new Cvor<Tip>(nullptr, el, nullptr);
        tekuci = start;
        end = start;
    }

    else if(tekuci == end) {
        Cvor<Tip>* novi = new Cvor<Tip>(tekuci, el, nullptr);
        tekuci -> next = novi;
        end = novi;
    }

    else {
        Cvor<Tip>* novi = new Cvor<Tip>(tekuci, el, tekuci -> next);
        tekuci -> next ->previous = novi;
        tekuci -> next = novi;
    }
    size++;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi()
{

    if(size == 0) throw std::domain_error("Prazna lista");

    if(tekuci == start) {
        if(size == 1) {
            start = nullptr;
            delete tekuci;
            tekuci = end = nullptr;
        } else {
            start = start -> next;
            delete tekuci;
            tekuci = start;
        }
    }

    else if(tekuci == end) {
        auto predzadnji = tekuci -> previous;
        predzadnji -> next = nullptr;
        end = tekuci -> previous;
        delete tekuci;
        tekuci = end;
    } else {
        Cvor<Tip>* sljedeci = tekuci -> next;
        auto prethodni = tekuci -> previous;
        tekuci -> next = tekuci -> previous = 0;
        delete tekuci;
        tekuci = sljedeci;
        prethodni->next = tekuci;
        sljedeci -> previous = prethodni;
    }
    size--;
}

template <typename Tip>
void DvostrukaLista<Tip>::Unisti()
{
    for(; start; ) {
        start -> previous = nullptr;
        tekuci = start;
        start = start->next;
        delete tekuci;
    }
    start = end = tekuci= nullptr;
    size = 0;
}

template<typename tip>

class Iterator
{
    const DvostrukaLista<tip>* dvostruka = nullptr;
    Cvor<tip>* start, *end, *place;

public:
    Iterator(const DvostrukaLista<tip>&l)
    {
        dvostruka = &l;
        start = l.start;
        end = l.end;
        place = start;
    }

    void pocetak()
    {
        place = start;
    }

    void kraj()
    {
        place = end;
    }

    bool sljedeci()
    {
        if(place == end) return false;
        place = place -> next;
        return true;
    }

    bool prethodni()
    {
        if(place == start) return false;
        place = place -> previous;
        return true;
    }

    const tip& trenutni() const
    {
        return place -> info;
    }

    int brojElemenata() const
    {
        return dvostruka -> brojElemenata();
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n)
{
    const DvostrukaLista<Tip>* x = (const DvostrukaLista<Tip>*) &n;
    Iterator<Tip>it(*x);
    it.pocetak();
    Tip max = it.trenutni();
    for(int i = 0; i < it.brojElemenata(); i++) {
        if(it.trenutni() > max) max = it.trenutni();
        it.sljedeci();
    }
    return max;
}

void TestBrElemenata1() {
    DvostrukaLista<int> d_List;
    if(d_List.brojElemenata() == 0)
        std::cout << "TEST METODE brojElemenata() USPJESAN\n";
    else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
}

void TestBrElemenata2() {
    DvostrukaLista<int> d_List;
    d_List.dodajIspred(2);
    d_List.dodajIspred(4);
    for(int i = 1; i < 4; i++) d_List.dodajIspred(i);
    if(d_List.brojElemenata() == 5)
        std::cout << "TEST METODE brojElemenata() USPJESAN\n";
    else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
}

void TestTrenutni1() {
    DvostrukaLista<int>lista;
    lista.dodajIspred(3);
    lista.dodajIspred(5);
    lista.dodajIspred(12);
    if(lista.trenutni() == 3) std::cout << "\nTEST METODE trenutni() USPJESAN\n";
    else std::cout << "TEST METODE trenutni() NEUSPJESAN\n";
}

void TestTrenutni2() {
    DvostrukaLista<int>lista;
    lista.dodajIspred(3);
    lista.dodajIspred(5);
    lista.dodajIspred(12);
    lista.trenutni() = 6;
    if(lista.trenutni() == 6) std::cout << "TEST METODE trenutni() -za izmjenu/ nonconst- USPJESAN\n";
    else std::cout << "TEST METODE trenutni() -za izmjenu/ nononst- NEUSPJESAN\n";
}

void TestPrethodni() {
    DvostrukaLista<int> lista;

    {
        try{
            lista.prethodni();
        } catch(std:: domain_error e) {
            std::cout << "\nTEST USPJESAN/OCEKIVAN JE IZUZETAK I GLASI: " << e.what() << std::endl;
        }
    }

    {
        lista.dodajIza(1);
        lista.dodajIza(2);
        lista.dodajIza(3);
        if(!lista.prethodni()) std::cout << "TEST METODE prethodni() USPJESAN - nema prethodnog -\n";
        else std::cout << "TEST METODE prethodni() NEUSPJESAN\n";
    }

    {
        lista.dodajIspred(12);
        lista.dodajIspred(11);
        if(lista.prethodni() && lista.trenutni() == 11) std::cout << "TEST METODE prethodni() USPJESAN - ima prethodnog -\n";
        else std::cout << "TEST METODE prethodni() NEUSPJESAN\n";
    }
}

void TestSljedeci() {
    DvostrukaLista<int> lista;

    {
        try{
            lista.sljedeci();
        } catch(std:: domain_error e) {
            std::cout << "\nTEST metode slejdeci() USPJESAN/OCEKIVAN JE IZUZETAK I GLASI: " << e.what() << std::endl;
        }
    }

    {
        lista.dodajIspred(1);
        lista.dodajIspred(2);
        lista.dodajIspred(3);
        if(!lista.sljedeci()) std::cout << "TEST METODE sljedeci() USPJESAN - nema sljedeceg -\n";
        else std::cout << "TEST METODE sljedeci() NEUSPJESAN\n";
    }

    {
        lista.dodajIza(12);
        lista.dodajIza(11);
        if(lista.sljedeci() && lista.trenutni() == 11) std::cout << "TEST METODE sljedeci() USPJESAN - ima sljedeci -\n";
        else std::cout << "TEST METODE sljedeci() NEUSPJESAN\n";
    }
}

void TestPocetak() {
    DvostrukaLista<int> lista;
    {
        try {
            lista.pocetak();
        } catch(std::domain_error e) {
            std::cout << "\nTEST METODE pocetak() USPJESAN -izuzetak-"<< e.what() << std::endl;
        }
    }
    {
        lista.dodajIspred(3);
        lista.dodajIspred(4);
        lista.dodajIza(4);
        lista.dodajIza(5);
        lista.pocetak();
        if(lista.trenutni() == 4) std::cout << "TEST METODE pocetak() USPJESAN\n";
        else std::cout << "TEST METODE pocetak() NEUSPJESAN\n";
    }
}

void TestKraj() {
    DvostrukaLista<int> lista;
    {
        try {
            lista.kraj();
        } catch(std::domain_error e) {
            std::cout << "\nTEST METODE kraj() USPJESAN -izuzetak-"<< e.what() << std::endl;
        }
    }
    {
        lista.dodajIspred(3);
        lista.dodajIspred(4);
        lista.dodajIza(4);
        lista.dodajIza(5);
        lista.kraj();
        if(lista.trenutni() == 4) std::cout << "TEST METODE kraj() USPJESAN\n";
        else std::cout << "TEST METODE kraj() NEUSPJESAN\n";
    }
}

void TestPristupa() {
    DvostrukaLista<int> lista;
    {
        lista.dodajIspred(3);
        lista.dodajIspred(4);
        lista.dodajIza(4);
        lista.dodajIza(5);
        if(lista[0] == 4) std::cout << "\nTEST operator[] USPJESAN -samo citanje-\n";
            else std::cout << "TEST operator[] NEUSPJESAN\n";
    }

    {
        lista[1] = 54;
        if(lista.trenutni() == 54) std::cout << "TEST operator[] USPJESAN -promjena elementa-\n";
            else std::cout << "TEST operator[] NEUSPJESAN\n";
    }
}

void TestObrisi() {
    DvostrukaLista<int> lista;

    {
        try {
            lista.obrisi();
        }catch(std::domain_error e) {
            std::cout << "TEST METODE obrisi() USPJESAN - izuzetak glasi: " << e.what() << std::endl;
        }
    }

    {
        lista.dodajIspred(3);
        lista.dodajIspred(4);
        lista.dodajIza(4);
        lista.dodajIza(5);
        lista.obrisi();
        if(lista.brojElemenata() == 3 && lista.trenutni() == 5) std::cout << "TEST METODE obrisi() USPJESAN\n";
        else std::cout << "TEST METODE obrisi() NIJE USPJESAN\n";
    }
}

void TestDodajIspred() {
    DvostrukaLista<int> lista;
    lista.dodajIspred(3);
    lista.dodajIspred(4);
    if(lista.brojElemenata() == 2 && lista[0] == 4 && lista[1] == 3) std::cout << "\nTEST METODE dodajIspred() USPJESAN\n";
    else std::cout << "TEST METODE obrisi() NIJE USPJESAN\n";
}

void TestDodajIza() {
    DvostrukaLista<int> lista;
    lista.dodajIza(3);
    lista.dodajIza(11);
    if(lista.brojElemenata() == 2 && lista[0] == 3 && lista[1] == 11) std::cout << "\nTEST METODE dodajIza() USPJESAN\n";
    else std::cout << "TEST METODE dodajIza() NEUSPJESAN\n";
}

void TestdajMaksimum() {
    DvostrukaLista<int> lista;
    for(int i = 0; i < 100; i++)
        lista.dodajIspred(i);
    if(dajMaksimum(lista) == 99) std::cout << "\nTEST METODE dajMaksimum() USPJESAN\n";
    else std::cout << "TEST METODE dajMaksimum() NEUSPJESAN\n";
}

int main()
{
   TestBrElemenata1();
   TestBrElemenata2();
   TestTrenutni1();
   TestTrenutni2();
   TestPrethodni();
   TestSljedeci();
   TestPocetak();
   TestKraj();
   TestPristupa();
   TestObrisi();
   TestDodajIspred();
   TestDodajIza();
   TestdajMaksimum();

   return 0;
}
