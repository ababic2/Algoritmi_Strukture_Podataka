#include <iostream>

template <typename Tip>
struct Cvor {
    Tip info;
    Cvor* previous;
    Cvor* next;

    Cvor(Cvor* previous = nullptr, const Tip &info = 0, Cvor* next = nullptr) : info(info), previous(previous), next(next) {}
};

template<typename Tip>
class DvostraniRed {

    Cvor<Tip>*pocetak;
    Cvor<Tip>*kraj;
    int size;
    void kopiraj(const DvostraniRed& red) {
        pocetak = nullptr;
        Cvor<Tip>* klizni = red.pocetak;
        kraj = pocetak;
        for(; klizni; klizni=klizni->next){
            if(!pocetak){
                pocetak = new Cvor<Tip>(nullptr,klizni->info,nullptr);
                kraj = pocetak;
            }else{
                kraj->next = new Cvor<Tip>(kraj,klizni->info,nullptr);
                kraj = kraj->next;
            }
        }
        size = red.size;
    }

    public:

    DvostraniRed();
    DvostraniRed(const DvostraniRed &);
    DvostraniRed& operator=(const DvostraniRed &);
    int brojElemenata() const {
        return size;
    }
    void brisi();
    void staviNaVrh(const Tip& element){
        auto temp = new Cvor<Tip>(nullptr,element,pocetak );
        if(size)pocetak->previous = temp;
        pocetak = temp;
        size++;
        if(size==1)kraj=pocetak;
    }
    Tip skiniSaVrha(){
        if(size==0)throw std::domain_error("Nema elemenata");
        Tip temp = pocetak->info;
        size--;
        auto x = pocetak;
        pocetak = pocetak->next;
        if(pocetak)pocetak->previous = nullptr;
        if(!pocetak)kraj = nullptr;
        delete x;
        return temp;
    }
    void staviNaCelo(const Tip& element){
        auto temp = new Cvor<Tip>(kraj,element,nullptr);
        if(size)kraj->next = temp;
        kraj = temp;
        size++;
        if(size==1)pocetak=kraj;
    }
    Tip skiniSaCela(){
        if(size==0)throw std::domain_error("Nema elemenata");
        Tip temp = kraj->info;
        size--;
        auto x = kraj;
        kraj = kraj->previous;
        if(kraj)kraj->next=nullptr;
        if(!kraj)pocetak = nullptr;
        delete x;
        return temp;
    }
    Tip vrh() const{
        if(size==0)throw std::domain_error("Nema elemenata");
        return pocetak->info;
    }
    Tip celo() const{
        if(size==0)throw std::domain_error("Nema elemenata");
        return kraj->info;
    }
    ~DvostraniRed(){
        brisi();
    }

};

template<typename Tip>
DvostraniRed<Tip>::DvostraniRed() : pocetak(nullptr), kraj(nullptr), size(0){}
template<typename Tip>
DvostraniRed<Tip>::DvostraniRed(const DvostraniRed<Tip> &red) {
    kopiraj(red);
}

template<typename Tip>
DvostraniRed<Tip>& DvostraniRed<Tip>::operator=(const DvostraniRed<Tip>& red){
    if(this== &red)return *this;
    brisi();
    kopiraj(red);
    return *this;
}

template<typename Tip>
void DvostraniRed<Tip>::brisi(){
    auto klizni = pocetak;
    for(;klizni;){
        pocetak = klizni;
        klizni = klizni -> next;
        delete pocetak;
    }
    kraj = pocetak = nullptr;
    size=0;
}

//TESTOVI ZA red

void TestBrElemenata() {
    DvostraniRed<int> red;
    {
        if(red.brojElemenata() == 0) std::cout << "TEST METODE brojElemenata() USPJESAN - prazan red\n";
        else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
    }

    {
        for(int i = 1; i < 6; i++) red.staviNaVrh(i);

        if(red.brojElemenata() == 5) std::cout << "TEST METODE brojElemenata() USPJESAN\n";
        else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";

        red.brisi();
        for(int i = 1; i < 6; i++) red.staviNaCelo(i);

        if(red.brojElemenata() == 5) std::cout << "TEST METODE brojElemenata() USPJESAN\n";
        else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
    }
}

void TestBrisi() {
    DvostraniRed<int> red;
    {
        for(int i = 0; i < 5; i++)
            red.staviNaCelo(i);
        red.brisi();
        if(red.brojElemenata() == 0) std::cout << "\nTEST METODE brisi() USPJESAN\n";
        else std::cout << "TEST METODE brisi() NEUSPJESAN\n";
    }
}

void TestStavi() {
    DvostraniRed<int> red;
    bool tacno = false;

    for(int i = 1; i < 10; i++)
        red.staviNaVrh(i);

    for(int i = 11; i < 13; i++)
        red.staviNaCelo(i);

    if(red.skiniSaVrha() == 9 && red.skiniSaCela() == 12) tacno = true;
    if(red.skiniSaVrha() == 8 && red.skiniSaCela() == 11) tacno = true;
    else tacno = false;

    if(tacno)  std::cout << "\nTEST METODA staviNaVrh() i staviNaCelo() USPJESAN\n";
        else std::cout << "TEST METODA skiniSaCela() i staviNaVrh() NEUSPJESAN\n";
}

void TestSkini() {
    DvostraniRed<int> red;

    try{
        red.skiniSaCela();
    } catch(std::domain_error e) {
        std::cout << "Test za skiniSaCela() OK : " << e.what() << std::endl;
    }

    try{
        red.skiniSaVrha();
    } catch(std::domain_error e) {
        std::cout << "Test za skiniSaVrha() OK : " << e.what() << std::endl;
    }

    red.staviNaVrh(5);
    red.staviNaVrh(8);
    red.staviNaVrh(15);
    red.staviNaVrh(23);

    if(red.skiniSaVrha() == 23 && red.skiniSaVrha() != 23) std::cout << "\nTEST METODA skiniSaVrha() USPJESAN\n";
        else std::cout << "TEST METODA skiniSaVrha() NEUSPJESAN\n";

    if(red.skiniSaCela() == 5 && red.skiniSaCela() != 5) std::cout << "\nTEST METODA skiniSaCela() USPJESAN\n";
        else std::cout << "TEST METODA skiniSaCela() NEUSPJESAN\n";
}

void TestVrh() {
    DvostraniRed<int> red;

    try{
        red.vrh();
    } catch(std::domain_error e) {
        std::cout << "\nTest za vrh() OK : " << e.what() << std::endl;
    }

    red.staviNaVrh(5);
    red.staviNaVrh(8);
    red.staviNaVrh(15);
    red.staviNaVrh(23);

    if(red.vrh() == 23 && red.skiniSaVrha() == 23) std::cout << "\nTEST METODA vrh() USPJESAN\n";
        else std::cout << "TEST METODA vrh() NEUSPJESAN\n";
}

void TestCelo(){
    DvostraniRed<int> red;

    try{
        red.celo();
    } catch(std::domain_error e) {
        std::cout << "\nTest za celo() OK : " << e.what() << std::endl;
    }

    red.staviNaVrh(5);
    red.staviNaVrh(8);
    red.staviNaVrh(15);
    red.staviNaVrh(23);

    if(red.celo() == 5 && red.skiniSaCela() == 5) std::cout << "\nTEST METODA celo() USPJESAN\n";
        else std::cout << "TEST METODA celo() NEUSPJESAN\n";
}

int main() {
    TestBrElemenata();
    TestBrisi();
    TestStavi();
    TestSkini();
    TestVrh();
    TestCelo();

    return 0;
}
