#include <iostream>

//kopirano sa pripremna 8.

#include <iostream>
#include <string>
#include <iomanip>
//kopirana mapa sa Pripemne 7
using namespace std;

unsigned int f_hash(int ulaz, unsigned int max)
{
    unsigned int suma=0;
    for (int i(0); i<ulaz; i++)suma += i;
    return suma % max;
}

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
        niz = new Pair<TipKljuca, TipVrijednosti>[kapacitet] {};
    }

    ~NizMapa()
    {
        delete[] niz;
    }

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

    TipVrijednosti operator[](const TipKljuca& kljuc) const
    {
        for(int i = 0; i < brojEl; i++)
            if(niz[i].kljuc == kljuc) return niz[i].vrijednost;

        return TipVrijednosti();
    }

    TipVrijednosti& operator[](const TipKljuca& kljuc);

    int brojElemenata() const override
    {
        return brojEl;
    }

    void obrisi() override
    {
        brojEl = 0;
        kapacitet = 500;

        delete[] niz;
        niz = new Pair<TipKljuca,TipVrijednosti>[kapacitet] {};
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
inline TipVrijednosti& NizMapa<TipKljuca,TipVrijednosti>::operator[](const TipKljuca& kljuc)
{

    for(int i = 0; i < brojEl; i++)
        if(niz[i].kljuc == kljuc) return niz[i].vrijednost;

    //nije nadjen znaci napravit ce se i postavit na default vr vrijednost
    if(brojEl == kapacitet) {
        //realokacija heh
        kapacitet *= 2;
        Pair<TipKljuca,TipVrijednosti>*novi = new Pair<TipKljuca,TipVrijednosti>[kapacitet] {};
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

template<typename tKey, typename tVal>
struct Cvor {
    tKey key;
    tVal value;
    Cvor<tKey,tVal>* parent, *left, *right;
    Cvor(tKey key = tKey(), tVal value = tVal(), Cvor<tKey,tVal>* parent = nullptr, Cvor<tKey,tVal>*left = nullptr, Cvor<tKey,tVal>* right = nullptr) : key(key), value(value),parent(parent),left(left),right(right) {}
};

template<typename tKey, typename tVal>
class BinStabloMapa : public Mapa<tKey,tVal>
{
    Cvor<tKey,tVal>* root;
    int size;
    //serach delete insert copy
    Cvor<tKey,tVal>* preorderCopy(Cvor<tKey,tVal>*  &to, Cvor<tKey,tVal>* from, Cvor<tKey,tVal>*parent);    // reef ??
    Cvor<tKey,tVal>* Search(Cvor<tKey,tVal>* node, const tKey& findMe)const;
    Cvor<tKey,tVal>* insert(Cvor<tKey,tVal>* &child, Cvor<tKey,tVal>*parent, const tKey& addMe);
    void deleteTree(Cvor<tKey,tVal>* node);
    Cvor<tKey,tVal>*findNasljednik(Cvor<tKey,tVal>*y,Cvor<tKey,tVal>*node)const;

public:
    BinStabloMapa() : root(nullptr), size(0) {}
    ~BinStabloMapa()
    {
        deleteTree(root);
    }

    BinStabloMapa(const BinStabloMapa& tree)
    {
        preorderCopy(root, tree.root, nullptr);
        size = tree.size;
    }
    BinStabloMapa(BinStabloMapa&& tree)
    {
        root = tree.root;
        tree.root = nullptr;
        size = tree.size;
    }


    BinStabloMapa& operator=(const BinStabloMapa& tree);    //treba copy i deleteSve
    BinStabloMapa& operator=(BinStabloMapa&& tree);

    tVal& operator[](const tKey& key);          // treba serach i insert
    tVal operator[](const tKey& key) const;

    int brojElemenata()const override
    {
        return size;
    }

    void obrisi() override
    {
        deleteTree(root);
    }
    void obrisi(const tKey& key) override; // jos ovo
};

template<typename tKey, typename tVal>
inline Cvor<tKey,tVal>* BinStabloMapa<tKey,tVal>::preorderCopy(Cvor<tKey,tVal>* &to, Cvor<tKey,tVal>* from, Cvor<tKey,tVal>*parent)         //copy sa preorder
{
    if(from) {
        to = new Cvor<tKey,tVal>(from->key, from -> value, parent, nullptr,nullptr);
        preorderCopy(to->left, from->left, to);
        preorderCopy(to->right, from->right, to);
    }
}

template<typename tKey, typename tVal>
inline Cvor<tKey,tVal>*BinStabloMapa<tKey,tVal>::Search(Cvor<tKey,tVal>* node, const tKey& findMe) const       //serach
{
    if(node == nullptr || node->key == findMe) return node;
    else if(findMe < node->key) return Search(node->left, findMe);
    else return Search(node->right, findMe);
    // if(node != nullptr && node->key == findMe) return node;
    // while(node != nullptr && findMe != node -> key) {
    //     if(findMe < node->key)
    //         node = node->left;
    //     else node = node -> right;
    // }
    return node;
}

template<typename tKey, typename tVal>
inline Cvor<tKey,tVal>* BinStabloMapa<tKey,tVal>::insert (Cvor<tKey,tVal>* &child, Cvor<tKey,tVal>*parent, const tKey& addMe)         //insert
{
    if(child == nullptr) {
        child = new Cvor<tKey,tVal>(addMe, tVal(), parent, nullptr, nullptr);
        return child;
    } else if(child->key == addMe) return child;
    else if(addMe < child->key) return insert(child->left, child, addMe);
    else return insert(child->right, child, addMe);
}

template<typename tKey, typename tVal>
tVal& BinStabloMapa<tKey,tVal>:: operator[](const tKey& key)       //serach i insert
{
    Cvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp == nullptr) {
        temp = insert(root,nullptr,key);
        size++;
    }
    return temp -> value;
}

template<typename tKey, typename tVal>
tVal BinStabloMapa<tKey,tVal>::operator[](const tKey& key) const
{
    Cvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp) return temp->value;
    return tVal();
}

template<typename tKey, typename tVal>
inline void BinStabloMapa<tKey,tVal>::deleteTree(Cvor<tKey,tVal>* node)
{
    if(node) {

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        size = 0;
    }
    root = nullptr;
}

template<typename tKey, typename tVal>
BinStabloMapa<tKey,tVal>& BinStabloMapa<tKey,tVal>::operator=(const BinStabloMapa& tree)
{
    if(this == &tree) return *this; // destruktivna samododjela

    deleteTree(this->root);
    preorderCopy(root,tree.root, nullptr);
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
BinStabloMapa<tKey,tVal>& BinStabloMapa<tKey,tVal>::operator=(BinStabloMapa&& tree)
{
    if(this == &tree) return *this;

    deleteTree(root);
    root = tree.root;
    tree.root = nullptr;
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
Cvor<tKey,tVal>*BinStabloMapa<tKey,tVal>::findNasljednik(Cvor<tKey,tVal>*y,Cvor<tKey,tVal>*node)const
{
    if(y->left == nullptr && y->parent == node) return y;
    if(y->left != nullptr) // idi lijevo
        return findNasljednik(y->left,node);
    if(y->left == nullptr && y->right != nullptr)
        return findNasljednik(y->right, node);
    return y;
}

template<typename tKey, typename tVal>
inline void BinStabloMapa<tKey,tVal>::obrisi(const tKey& key)
{
    // prvo nadjem cvor koji ima trazeni key i ispitujem sljedece slucajeve
    Cvor<tKey,tVal>*temp = Search(root, key);
    if(temp) {
        if(temp->right == nullptr && temp->left == nullptr) {                   //case 1: nema djece nikako
            if(temp == root) root = nullptr;
            //omg niisiiii postavilllaaaa parent left i right da je nullptr
            Cvor<tKey,tVal>*roditelj = temp->parent;
            if(roditelj && roditelj->right == temp) roditelj->right = nullptr;
            if(roditelj && roditelj->left == temp) roditelj->left = nullptr;
            delete temp;
            size--;
        } else if(temp->right == nullptr) {                                     //case 1:jedno dijete ali lijevo
            Cvor<tKey,tVal>*cuvaj = temp->left;
            temp->key = temp->left->key;
            temp->value = temp->left->value;
            temp->right = temp->left->right;
            //promjena parenta
            if(cuvaj->left) {
                temp->left = temp->left->left;
                cuvaj->left->parent = temp;
            } else temp->left = nullptr;
            if(cuvaj->right) {
                // temp->right = temp->left->right;
                cuvaj->right->parent = temp;
            }//else temp->right = nullptr;
            delete cuvaj;
            size--;
        } else if(temp->left == nullptr) {                                      // case 2: ima samo jedno dijete left or right
            // sta ako temp right ima djece sad
            //prepisem od temp right u right i prespojim s njegovom djecom i obrisem temp right
            Cvor<tKey,tVal>*cuvaj = temp->right;
            temp->key = temp->right->key;
            temp->value = temp->right->value;
            temp->left = temp->right->left;
            //sad od temp left i right promijeni parenta na temp i obrisi ovog cuvaj
            if(cuvaj->right) {
                temp->right = temp->right->right;
                cuvaj->right->parent = temp;
            } else temp->right = nullptr;
            if(cuvaj->left) {
                //temp->left = temp->right->left;
                cuvaj->left->parent = temp;
            }//else temp->left = nullptr;
            delete cuvaj;
            size--;
        } else {                                                                // case 3: 2 children
            // moram nac nasljednika, napravi funkciju da ide kroz stablo

            Cvor<tKey,tVal>*nasljednik = findNasljednik(temp->right,temp);
            if(nasljednik->parent == temp) {
                //prvi desni nasljednik, a nema left child po funkciji
                nasljednik -> left = temp->left;
                nasljednik -> parent= temp->parent;
                if(temp == root) root = nasljednik;
                delete temp;
                size--;
            } else {
                //brisi kraj stabla tj na kraju je stabla - list
                temp->key = nasljednik->key;
                temp->value = nasljednik->value;
                if(nasljednik->parent->right == nasljednik)
                    nasljednik->parent->right = nullptr;
                //else if(nasljednik->parent->left == nasljednik)
                else  nasljednik->parent->left = nullptr;
                delete nasljednik;
                size--;
            }
        }
    }
}


template<typename tKey,typename tVal>
class HashMapa : public Mapa<tKey,tVal>
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

    HashMapa()
    {
        brojEl = 0;
        niz = new Pair<tKey, tVal>*[kapacitet] {};
        hash = nullptr;
        kapacitet = 10;
    }

    ~HashMapa()
    {
        obrisi();
        delete[] niz;
    }

    HashMapa(const HashMapa& mapa);

    HashMapa(HashMapa&& mapa);

    HashMapa& operator=(HashMapa&& mapa);
    HashMapa& operator=(const HashMapa& mapa);

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
int HashMapa<tKey,tVal>::dodaj(Pair<tKey,tVal>** array, unsigned int capacity, Pair<tKey,tVal>* par)
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
void HashMapa<tKey,tVal>::realociraj()
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
tVal& HashMapa<tKey,tVal>:: operator[](const tKey& kljuc)
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


// OVO NE RADDITII
// for(int i = 0; i < kapacitet; i++)
//     if(niz[i].kljuc == kljuc) return niz[i].vrijednost;

// unsigned int index = hash(kljuc,kapacitet);
// //KOLIZIJA
// //trazim linearno prvo free mjesto  i ubacujem
// // ako indeks izleti -> realokacija
// index;

// while(index < kapacitet && niz[index].kljuc != tKey())
//     index++;
// if(index >= kapacitet) {
//     Pair<tKey,tVal>* temp = new Pair<tKey,tVal>[kapacitet + 100]{};
//     for(int i = 0; i < kapacitet; i++)
//         temp[i] = niz[i];
//     delete[] niz;
//     niz = temp;
//     kapacitet += 100;
// }

// brojEl++;
// niz[index].kljuc = kljuc;
// niz[index].vrijednost = tVal();
// return niz[index].vrijednost;

}

template<typename tKey, typename tVal>
tVal HashMapa<tKey,tVal>::operator[](const tKey& kljuc)const
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

    // for(int i = 0; i  < kapacitet; i++)
    //     if(niz[i].kljuc == kljuc) return niz[i].vrijednost;
    // return tVal();
}

template<typename tKey, typename tVal>
HashMapa<tKey,tVal>& HashMapa<tKey,tVal>::operator=(HashMapa<tKey,tVal>&& mapa)
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
HashMapa<tKey,tVal>& HashMapa<tKey,tVal>::operator=(const HashMapa<tKey,tVal>& mapa)
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
HashMapa<tKey,tVal>::HashMapa(HashMapa&& mapa)
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
HashMapa<tKey,tVal>::HashMapa(const HashMapa<tKey,tVal>&mapa)
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
inline void HashMapa<tKey,tVal>::obrisi()
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
inline void HashMapa<tKey,tVal>::obrisi(const tKey& kljuc)
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
    // for(int i=0;i<kapacitet;i++){
    //     if(niz[i] && niz[i] != &deleted) {
    //         cout<<i<<". "<<niz[i]->kljuc<<" "<<niz[i]->vrijednost<<"\n";
    //     }else if(niz[i]==&deleted){
    //         cout<<i<<". deleted\n";
    //     }else{
    //         cout<<i<<". \n";
    //     }

    // }
}

void ubaci(Mapa<int,int> &m, int n)
{
    srand(time(NULL));
    for(int i = 0; i < n; i++)
        m[i] = rand();
}

void poredjenje()
{
    BinStabloMapa<int,int> bin_s;
    NizMapa<int,int> nm;
    HashMapa<int,int>hm;

    //POREDJENJE UBACIVANJA
    std::cout << " POREDJENJE UBACIVANJA \n";

    clock_t t1 = clock();
    ubaci(bin_s,4000);
    clock_t t2 = clock();
    double BS_vrijeme = (t2 - t1) / (CLOCKS_PER_SEC/1000);

    clock_t t3 = clock();
    ubaci(nm,4000);
    clock_t t4 = clock();
    double NM_vrijeme = (t4 - t3) / (CLOCKS_PER_SEC/1000);

    hm.definisiHashFunkciju(f_hash);
    clock_t t5 = clock();
    ubaci(hm,4000);
    clock_t t6 = clock();
    double HM_vrijeme = (t6 - t5) / (CLOCKS_PER_SEC / 1000);

    std::cout << "BinStabloMapa: " << BS_vrijeme / 4000 << std::endl            //binarno stablo dodaje elemente i  pri dodavanju ih poredi, tj.sortira
        << "HashMapa: " << HM_vrijeme / 4000 << std::endl                       //tako da pri dodavanju elemenata binarni stablo pokazuje slabije performanse
        << "NizMapa: " << NM_vrijeme / 4000;                                    //pri dodavanju u NizMapa mi iteriramo i trazimo free mjesto dok kod HashMap
                                                                                //indeks racunamo brzo pa HashMap ima bolje performanse

    //POREDJENJE PRISTUPA ELEMENTIMA
    std::cout << "\n POREDJENJE PRISTUPA ELEMENTIMA\n";
    clock_t t7 = clock();
    for(int i = 0; i < 4000; i++)
        bin_s[i];
    clock_t t8 = clock();
    BS_vrijeme = (t8 - t7) / (CLOCKS_PER_SEC/1000);

    clock_t t9 = clock();
    for(int i = 0; i < 4000; i++)
        hm[i];
    clock_t t10 = clock();
    HM_vrijeme = (t10 - t9) / (CLOCKS_PER_SEC/1000);

    clock_t t11 = clock();
    for(int i = 0; i < 4000; i++)
        nm[i];
    clock_t t12 = clock();
    NM_vrijeme = (t12 - t11) / (CLOCKS_PER_SEC/1000);

    std::cout << "BinStabloMapa: " << BS_vrijeme / 4000 << std::endl            // analogno vazi i za pritup elementima(tj. isto kao i za ubacivanje)
        << "HashMapa: " << HM_vrijeme / 4000 << std::endl
        << "NizMapa: " << NM_vrijeme / 4000;


     //BRISANJE ELEEMENATAA
    std::cout << "\n BRISANJE\n";
    clock_t t13 = clock();
    for(int i = 0; i < 4000; i++)
        bin_s.obrisi(i);
    clock_t t14 = clock();
    BS_vrijeme = (t14 - t13) / (CLOCKS_PER_SEC/1000);

    clock_t t15 = clock();
    for(int i = 0; i < 4000; i++)
        hm.obrisi(i);
    clock_t t16 = clock();
    HM_vrijeme = (t16 - t15) / (CLOCKS_PER_SEC/1000);

    clock_t t17 = clock();
    for(int i = 0; i < 4000; i++)
        nm.obrisi(i);
    clock_t t18 = clock();
    NM_vrijeme = (t18 - t17) / (CLOCKS_PER_SEC/1000);

    std::cout << "BinStabloMapa: " << BS_vrijeme / 4000 << std::endl            //NizMapa ima najlosie performanse jer treba iterirati da bi obrisala element
        << "HashMapa: " << HM_vrijeme / 4000 << std::endl                       //HashMapa ima bolje performanse zbog hash fnkcije
        << "NizMapa: " << NM_vrijeme / 4000;                                    //binStablo ima sortirane eleemente i brzo pronalazi element i uz to brisanjem manipulise sa cvorovima pa su performanse najbolje
}

int main()
{
    poredjenje();
    return 0;
}
