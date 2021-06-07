#include <iostream>
#include <vector>

int index = -1;
int ispod = -1;

template<typename Tip>
struct Cvor {
    Tip value;
    Cvor* next;
    Cvor(Tip value,Cvor* next):value(value),next(next){}
};

template<typename Tip>
class Stek
{
    Cvor<Tip>* top;
    int duzina;
public:
    Stek()
    {
        top = nullptr;
        duzina = 0;
    }
    Stek(const Stek& stek)
    {
        duzina = stek.duzina;
        Cvor<Tip>* temp2 = nullptr;
        for(auto temp = stek.top; temp; temp=temp->next) {
            if(temp==stek.top) {
                top = new Cvor<Tip>(temp->value,nullptr);
                temp2=top;
            } else {
                temp2 -> next = new Cvor<Tip>(temp->value,nullptr);
                temp2 = temp2->next;
            }
        }
    }
    Stek(Stek&& stek)
    {
        duzina = stek.duzina;
        top = stek.top;
        stek.top = nullptr;
        stek.duzina = 0;
    }
    Stek& operator=(const Stek& stek)
    {
        if(&stek == this)return *this;
        brisi();
        duzina = stek.duzina;
        Cvor<Tip>* temp2 = nullptr;
        for(auto temp = stek.top; temp; temp=temp->next) {
            if(temp==stek.top) {
                top = new Cvor<Tip>(temp->value,nullptr);
                temp2=top;
            } else {
                temp2 -> next = new Cvor<Tip>(temp->value,nullptr);
                temp2 = temp2->next;
            }
        }
        return *this;
    }
    Stek& operator=(Stek&& stek)
    {
        if(&stek == this)return *this;
        brisi();
        duzina = stek.duzina;
        top = stek.top;
        stek.top = nullptr;
        stek.duzina = 0;
        return *this;
    }
    inline int brojElemenata()const
    {
        return duzina;
    }
    void stavi(const Tip& element);
    inline Tip& vrh()
    {
        if(duzina==0)throw std::domain_error("Prazan stek!");
        return top->value;
    }
    Tip skini()
    {
        if(duzina==0)throw std::domain_error("Prazan stek!");
        auto temp = top;
        top = top->next;
        Tip result = temp -> value;
        delete temp;
        duzina--;
        return result;
    }
    void brisi()
    {
        auto temp = top;
        for(; temp;) {
            temp = temp->next;
            delete top;
            top=temp;
        }
        top = nullptr;
        duzina=0;
    }
    ~Stek()
    {
        brisi();
    }

};

template<typename Tip>
inline void Stek<Tip>::stavi(const Tip& element)
{
    auto novi = new Cvor<Tip>(element,top);
    top = novi;
    duzina++;
}

void pretraga(Stek<std::vector<int>> &s, int trazeni)
{
    if(s.brojElemenata()) {
        auto temp = s.skini();

        if(temp.size()) {
            //int index = -1;
            if(temp.at(0) <= trazeni && temp.at(temp.size()-1)>=trazeni) {
                int pocetak = 0;
                int kraj = temp.size() ;
                do {
                    int sredina = (pocetak + kraj ) / 2;
                    if(trazeni < temp[sredina]) {
                        if(sredina - pocetak == 1){
                            if(trazeni > temp[pocetak])break;
                        }
                        kraj = sredina+1;
                    } else if( trazeni > temp[sredina]) {
                        pocetak = sredina;
                    } else {
                        index = sredina;
                        ispod = s.brojElemenata();
                        break;
                    }

                } while(pocetak != kraj);

                if(index != -1) std::cout << index << " " << s.brojElemenata();
                else std::cout << "Nema elementa";

            } else {
                pretraga(s,trazeni);
            }
        } else {
            pretraga(s,trazeni);
        }
        s.stavi(temp);
    } else std::cout << "Nema elementa";
}

void Test1() {
    Stek<std::vector<int>> s;
    std::vector<int> v1;
    for(int i = 1; i < 11; i++)
        v1.push_back(i);

    std::vector<int> v2;
    for(int i = 11; i < 21; i++)
        v2.push_back(i);

    s.stavi(v1);
    s.stavi(v2);
    std::cout << "Ocekivani izlaz: 9 1 | izlaz : ";
    pretraga(s, 20);
    if(index== 9 && ispod == 1) std::cout << "\nTEST_1 OK\n\n";
    else std::cout << "FAILED\n";
    index = ispod = -1;
}

void Test2() {
    Stek<std::vector<int>> s;
    std::vector<int> v1;
    for(int i = 1; i < 11; i++)
        v1.push_back(i);

    s.stavi(v1);
    std::cout << "Ocekivani izlaz: 4 0 | izlaz : ";
    pretraga(s, 5);
    if(index == 4 && ispod == 0) std::cout << "\nTEST_2 OK\n\n";
    else std::cout << "FAILED\n";
    index = ispod = -1;
}

void Test3() {

    Stek<std::vector<int>> s;
    std::cout << "Ocekivani izlaz: Nema elementa\n";
    pretraga(s, 2);
    if(index == -1 && ispod == -1) std::cout << "\nTEST_3 OK\n\n";
    else std::cout << "FAILED\n";

    std::vector<int> v1;
    for(int i = 1; i < 11; i++)
        v1.push_back(i);

    std::cout << "Ocekivani izlaz: Nema elementa\n";
    pretraga(s, 20);
    if(index == -1 && ispod == -1) std::cout << "\nTEST_3' OK\n\n";
    else std::cout << "FAILED\n";

}

int main()
{
    Test1();
    Test2();
    Test3();
    return 0;
}
