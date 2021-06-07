#include <iostream>

template<typename Tip>
struct Cvor{
    Tip value;
    Cvor* next;
    Cvor(Tip value,Cvor* next):value(value),next(next){
    }
};

template<typename Tip>
class Stek{
    Cvor<Tip>* top;
    int duzina;
    public:
    Stek(){
        top = nullptr;
        duzina = 0;
    }
    Stek(const Stek& stek){
        duzina = stek.duzina;
        Cvor<Tip>* temp2 = nullptr;
        for(auto temp = stek.top;temp;temp=temp->next){
            if(temp==stek.top){
                top = new Cvor<Tip>(temp->value,nullptr);
                temp2=top;
            }else{
                temp2 -> next = new Cvor<Tip>(temp->value,nullptr);
                temp2 = temp2->next;
            }
        }
    }
    Stek(Stek&& stek){
        duzina = stek.duzina;
        top = stek.top;
        stek.top = nullptr;
        stek.duzina = 0;
    }
    Stek& operator=(const Stek& stek){
        if(&stek == this)return *this;
        brisi();
        duzina = stek.duzina;
        Cvor<Tip>* temp2 = nullptr;
        for(auto temp = stek.top;temp;temp=temp->next){
            if(temp==stek.top){
                top = new Cvor<Tip>(temp->value,nullptr);
                temp2=top;
            }else{
                temp2 -> next = new Cvor<Tip>(temp->value,nullptr);
                temp2 = temp2->next;
            }
        }
        return *this;
    }
    Stek& operator=(Stek&& stek){
        if(&stek == this)return *this;
        brisi();
        duzina = stek.duzina;
        top = stek.top;
        stek.top = nullptr;
        stek.duzina = 0;
        return *this;
    }
    inline int brojElemenata()const{
        return duzina;
    }
    void stavi(const Tip& element);
    inline Tip& vrh(){
        if(duzina==0)throw std::domain_error("Prazan stek!");
        return top->value;
    }
    Tip skini(){
        if(duzina==0)throw std::domain_error("Prazan stek!");
        auto temp = top;
        top = top->next;
        Tip result = temp -> value;
        delete temp;
        duzina--;
        return result;
    }
    void brisi(){
        auto temp = top;
        for(;temp;){
            temp = temp->next;
            delete top;
            top=temp;
        }
        top = nullptr;
        duzina=0;
    }
    ~Stek(){
        brisi();
    }

};

template<typename Tip>
inline void Stek<Tip>::stavi(const Tip& element){
    auto novi = new Cvor<Tip>(element,top);
    top = novi;
    duzina++;
}

//TESTOVI

template<typename Tip>
void TestBrojElemenata(Tip& stek) {
    stek.stavi(2);
    stek.stavi(5);
    stek.stavi(12);
    for(int i = 1; i < 10; i++)
        stek.stavi(i);
    if(stek.brojElemenata() == 12) std::cout << "\nTEST METODE brojElemenata() USPJESAN\n";
    else std::cout << "\nTEST METODE brojElemenata() NEUSPJESAN\n";
}

template<typename Tip>
void TestBrisanja(Tip& stek) {

    stek.brisi();
    if(stek.brojElemenata() == 0) std::cout << "\nTEST METODE brisi() USPJESAN\n";
    else std::cout << "\nTEST METODE brisi() NEUSPJESAN\n";
}

template<typename Tip>
void TestStavi(Tip& stek) {

    stek.stavi(23);
    for(int i = 1; i < 1000; i++)
        stek.stavi(i);
    if(stek.brojElemenata() == 1000) std::cout << "\nTEST METODE stavi() USPJESAN\n";
    else std::cout << "TEST METODE stavi() NEUSPJESAN\n";
}

template<typename Tip>
void TestSkini(Tip& stek) {
    try{
        if(stek.skini() == 999) std::cout << "\nTEST METODE stavi() USPJESAN\n";
        else std::cout << "TEST METODE stavi() NEUSPJESAN\n";
        std::cout << "\nTestiramo metodu skini() na praznom steku:\n";
        stek.brisi();
        stek.skini();
    } catch(std::domain_error e) {
        std::cout << "\nIZUZETAK metode skini(): " << e.what() << std::endl;
    }
}

template<typename Tip>
void TestVrh(Tip& stek) {
    try{
        std::cout << "\nStek je prazan pa sada na osnovu dodavanja elemenata testiramo ispravnost metode vrh(),\nodnosno bacanja izuzetaka kada ispraznimo stek\n";
        stek.stavi(5);
        stek.stavi(19);
        stek.vrh();
        if(stek.vrh() == 19 && stek.brojElemenata() == 2) std::cout << "\nTEST METODE vrh() USPJESAN\n";
        else std::cout << "\nTEST METODE vrh() NEUSPJESAN\n";
        stek.brisi();
        stek.vrh();
    } catch(std::domain_error e) {
        std::cout << "\nIZUZETAK metode vrh(): " << e.what() << std::endl;
    }
}

Stek<int> Vrati_stek() {
    Stek<int> stek;
    stek.stavi(2);
    return stek;
}

void TestKonstruktora_i_operatora(){
    Stek<int> stek;
    if(stek.brojElemenata() == 0) std::cout << "\nTEST Konstruktora_1 USPJESAN\n";
    else std::cout << "\nTEST Konstruktora_1 NEUSPJESAN\n";

    stek.stavi(2);
    stek.stavi(3);
    {
        Stek<int>test(stek);
        bool isti_su = true;
        for(int i = 0; i < stek.brojElemenata(); i++)
            if(stek.skini() != test.skini()) isti_su = false;
        if(test.brojElemenata() == stek.brojElemenata() && isti_su == true) std::cout << "\nTEST Konstruktora_2  USPJESAN\n";
        else std::cout << "\nTEST Konstruktora_2 NEUSPJESAN\n";
    }

    {
        Stek<int>test = Vrati_stek();
        bool isti_su = true;
        if(test.brojElemenata() != 1) isti_su = false;
        if(test.skini() != 2) isti_su = false;
        if(isti_su == true) std::cout << "\nTEST Konstruktora_3  USPJESAN\n";
        else std::cout << "\nTEST Konstruktora_3 NEUSPJESAN\n";
    }

    {
        Stek<int>test;
        stek.stavi(1);
        stek.stavi(2);
        test.stavi(45);
        test = stek;
        bool isti_su = true;
        for(int i = 0; i < stek.brojElemenata(); i++)
            if(stek.skini() != test.skini()) isti_su = false;
        if(test.brojElemenata() == stek.brojElemenata() && isti_su == true) std::cout << "\nTEST operator= USPJESAN\n";
        else std::cout << "\nTEST operator = NEUSPJESAN\n";
    }

    {
        Stek<int>test;
        test.stavi(45);
        test = Vrati_stek();
        bool isti_su = true;
        if(test.brojElemenata() != 1) isti_su = false;
        if(test.skini() != 2) isti_su = false;
        if(isti_su == true) std::cout << "\nTEST operator =  USPJESAN\n";
        else std::cout << "\nTEST operator = NEUSPJESAN\n";
    }
}

int main() {
  Stek<int> stek;
  TestBrojElemenata(stek);
  TestBrisanja(stek);
  TestStavi(stek);
  TestSkini(stek);
  TestVrh(stek);
  TestKonstruktora_i_operatora();
  return 0;
}
