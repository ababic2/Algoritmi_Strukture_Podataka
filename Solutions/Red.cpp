#include <iostream>

template<typename T>
struct Cvor{
    T value;
    Cvor* next;
    Cvor(const T& value, Cvor* next):value(value),next(next){

    }
};

template<typename T>
class Red{
    Cvor<T>* start;
    Cvor<T>* end;
    int size;
    public:
    Red(){
        start = end = nullptr;
        size=0;
    }
    Red(const Red& red){
        start = end = nullptr;
        size= red.size;
        auto temp = red.start;
        auto klizni = start;
        for(;temp;temp = temp->next){
            if(!start){
                start = new Cvor<T>(temp->value,0);
                klizni = start;
            }else{
                klizni -> next = new Cvor<T>(temp->value,0);
                klizni = klizni->next;
            }
        }
        end = klizni;
    }
    Red(Red&& red){
        size = red.size;
        start = red.start;
        end = red.end;
        red.end = red.start = red.size = 0;
    }
    Red& operator=(const Red& red){
        if(&red == this )return *this;
        brisi();
        size= red.size;
        auto temp = red.start;
        auto klizni = start;;
        for(;temp;temp = temp->next){
            if(!start){
                start = new Cvor<T>(temp->value,0);
                klizni = start;
            }else{
                klizni -> next = new Cvor<T>(temp->value,0);
                klizni = klizni->next;
            }
        }
        end = klizni;
        return *this;
    }
    Red& operator=(Red&& red){
        if(&red == this )return *this;
        brisi();
        size = red.size;
        start = red.start;
        end = red.end;
        red.end = red.start = red.size = 0;
        return *this;
    }
    int brojElemenata() const{
        return size;
    }
    T& celo(){
        if(size==0)throw std::domain_error("Red prazan!");
        return start -> value;
    }
    T skini(){
        if(size==0)throw std::domain_error("Red prazan!");
        auto temp = start;
        start = start -> next;
        T result = temp -> value;
        delete temp;
        size--;
        return result;
    }
    void stavi(const T& element){
        if(size==0){
            start = new Cvor<T>(element,0);
            end=start;
        }else{
            end -> next = new Cvor<T>(element,0);
            end = end->next;
        }
        size++;
    }
    void brisi(){
        auto temp = start;
        for(;temp;){
            start = temp->next;
            delete temp;
            temp=start;
        }
        end = start = nullptr;
        size=0;
    }
    ~Red(){
        brisi();
    }
};

void TestBrElemenata() {
    Red<int> red;
    {
        if(red.brojElemenata() == 0) std::cout << "TEST METODE brojElemenata() USPJESAN - prazan red\n";
        else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
    }

    {
        for(int i = 1; i < 6; i++) red.stavi(i);
        if(red.brojElemenata() == 5) std::cout << "TEST METODE brojElemenata() USPJESAN\n";
        else std::cout << "TEST METODE brojElemenata() NEUSPJESAN\n";
    }
}

void TestBrisi() {
    Red<int> red;
    {
        for(int i = 0; i < 5; i++)
            red.stavi(i);
        red.brisi();
        if(red.brojElemenata() == 0) std::cout << "\nTEST METODE brisi() USPJESAN\n";
        else std::cout << "TEST METODE brisi() NEUSPJESAN\n";
    }
}

void TestCelo() {
    Red<int> red;
    {
        try{
            red.celo();
        } catch(std::domain_error e) {
            std::cout << "\nTEST METODE celo() USPJESAN -izuzetak-" << e.what() << std::endl;
        }
    }

    {
        for(int i = 0; i < 5; i++)
            red.stavi(i);
        if(red.celo() == 0) std::cout << "TEST METODE celo() USPJESAN\n";
        else std::cout << "TEST METODE celo() NEUSPJESAN";
    }
}

void TestSkini() {
    Red<int> red;
    {
       try {
           red.skini();
       } catch(std::domain_error e) {
            std::cout << "\nTEST METODE skini() USPJESAN -izuzetak-" << e.what() << std::endl;
        }
    }

    {
        for(int i = 1; i < 8; i++) red.stavi(i);
        if(red.skini() == 1) std::cout << "TEST METODE skini() USPJESAN\n";
        else std::cout << "TEST METODE skini() NEUSPJESAN\n";
    }
}

void TestStavi() {
     Red<int> red;
     bool prosao = true;
     for(int i = 1; i < 4; i++) red.stavi(i);
     for(int i = 1; i < 4; i++) {
         if(red.skini() == i) prosao = true;
         else {
             prosao = false;
             break;
         }
     }
     if(prosao) std::cout << "\nTEST ZA METODU stavi() USPJESAN";
     else std::cout << "TEST ZA METODU stavi() NEUSPJESAN";
}

int main() {
    TestBrElemenata();
    TestBrisi();
    TestCelo();
    TestSkini();
    TestStavi();

    return 0;
}
