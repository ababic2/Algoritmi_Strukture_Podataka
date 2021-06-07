#include <iostream>
#include <string>
#include <iomanip>
//kopirana mapa sa Pripemne 7
using namespace std;

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
    BinStabloMapa() : root(nullptr), size(0){}
    ~BinStabloMapa() {
        deleteTree(root);
    }

    BinStabloMapa(const BinStabloMapa& tree){
        preorderCopy(root, tree.root, nullptr);
        size = tree.size;
    }
    BinStabloMapa(BinStabloMapa&& tree) {
        root = tree.root;
        tree.root = nullptr;
        size = tree.size;
    }


    BinStabloMapa& operator=(const BinStabloMapa& tree);    //treba copy i deleteSve
    BinStabloMapa& operator=(BinStabloMapa&& tree);

    tVal& operator[](const tKey& key);          // treba serach i insert
    tVal operator[](const tKey& key) const;

    int brojElemenata()const override { return size; }

    void obrisi() override {
        deleteTree(root);
    }
    void obrisi(const tKey& key) override; // jos ovo
};

template<typename tKey, typename tVal>
inline Cvor<tKey,tVal>* BinStabloMapa<tKey,tVal>::preorderCopy(Cvor<tKey,tVal>* &to, Cvor<tKey,tVal>* from, Cvor<tKey,tVal>*parent) {       //copy sa preorder
    if(from) {
        to = new Cvor<tKey,tVal>(from->key, from -> value, parent, nullptr,nullptr);
        preorderCopy(to->left, from->left, to);
        preorderCopy(to->right, from->right, to);
    }
}

template<typename tKey, typename tVal>
inline Cvor<tKey,tVal>*BinStabloMapa<tKey,tVal>::Search(Cvor<tKey,tVal>* node, const tKey& findMe) const {     //serach
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
inline Cvor<tKey,tVal>* BinStabloMapa<tKey,tVal>::insert (Cvor<tKey,tVal>* &child, Cvor<tKey,tVal>*parent, const tKey& addMe) {       //insert
    if(child == nullptr) {
        child = new Cvor<tKey,tVal>(addMe, tVal(), parent, nullptr, nullptr);
        return child;
    }
    else if(child->key == addMe) return child;
    else if(addMe < child->key) return insert(child->left, child, addMe);
    else return insert(child->right, child, addMe);
}

template<typename tKey, typename tVal>
tVal& BinStabloMapa<tKey,tVal>:: operator[](const tKey& key) {     //serach i insert
    Cvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp == nullptr)
    {
        temp = insert(root,nullptr,key);
        size++;
    }
    return temp -> value;
}

template<typename tKey, typename tVal>
tVal BinStabloMapa<tKey,tVal>::operator[](const tKey& key) const {
    Cvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp) return temp->value;
    return tVal();
}

template<typename tKey, typename tVal>
inline void BinStabloMapa<tKey,tVal>::deleteTree(Cvor<tKey,tVal>* node) {
    if(node){

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        size = 0;
    }
    root = nullptr;
}

template<typename tKey, typename tVal>
BinStabloMapa<tKey,tVal>& BinStabloMapa<tKey,tVal>::operator=(const BinStabloMapa& tree) {
    if(this == &tree) return *this; // destruktivna samododjela

    deleteTree(this->root);
    preorderCopy(root,tree.root, nullptr);
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
BinStabloMapa<tKey,tVal>& BinStabloMapa<tKey,tVal>::operator=(BinStabloMapa&& tree) {
    if(this == &tree) return *this;

    deleteTree(root);
    root = tree.root;
    tree.root = nullptr;
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
Cvor<tKey,tVal>*BinStabloMapa<tKey,tVal>::findNasljednik(Cvor<tKey,tVal>*y,Cvor<tKey,tVal>*node)const {
    if(y->left == nullptr && y->parent == node) return y;
    if(y->left != nullptr) // idi lijevo
        return findNasljednik(y->left,node);
    if(y->left == nullptr && y->right != nullptr)
        return findNasljednik(y->right, node);
    return y;
}

template<typename tKey, typename tVal>
inline void BinStabloMapa<tKey,tVal>::obrisi(const tKey& key) {
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
        }
        else if(temp->right == nullptr) {                                       //case 1:jedno dijete ali lijevo
            Cvor<tKey,tVal>*cuvaj = temp->left;
            temp->key = temp->left->key;
            temp->value = temp->left->value;
            temp->right = temp->left->right;
            //promjena parenta
            if(cuvaj->left) {
                temp->left = temp->left->left;
                cuvaj->left->parent = temp;
            }else temp->left = nullptr;
            if(cuvaj->right) {
               // temp->right = temp->left->right;
                cuvaj->right->parent = temp;
            }//else temp->right = nullptr;
            delete cuvaj;
            size--;
        }
        else if(temp->left == nullptr) {                                        // case 2: ima samo jedno dijete left or right
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
        }
        else{                                                                   // case 3: 2 children
            // moram nac nasljednika, napravi funkciju da ide kroz stablo

            Cvor<tKey,tVal>*nasljednik = findNasljednik(temp->right,temp);
            if(nasljednik->parent == temp) {
                //prvi desni nasljednik, a nema left child po funkciji
                nasljednik -> left = temp->left;
                nasljednik -> parent= temp->parent;
                if(temp == root) root = nasljednik;
                delete temp;
                size--;
            }
            else {
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

void popuni(Mapa<int,int> &object) {
    srand(time(NULL));
    for(int i = 0; i < 5000; i++)
        object[i] = rand();
}

int main()
{
    BinStabloMapa<int,int>bin;
    NizMapa<int,int>niz;

    std::cout << "DODAVANJE:\n";
    // Binary
    clock_t time_1 = clock();
    popuni(bin);
    clock_t time_2 = clock();
    double approxTime = (time_2 - time_1) / (CLOCKS_PER_SEC/1000);
    std::cout << "VRIJEME ZA BINARY TREE " << approxTime / 5000 << std::endl;

    //NizMap
    clock_t time_3 = clock();
    popuni(bin);
    clock_t time_4 = clock();
    approxTime = (time_4 - time_3) / (CLOCKS_PER_SEC/1000);
    std::cout << "VRIJEME ZA NIZ " << approxTime / 5000<< std::endl;

    std::cout << "PRISTUP ELEMENTIMA:\n";

    //Binary
    clock_t time_5 = clock();
    for(int i = 0; i < 5000; i++)
        bin[i];
    clock_t time_6 = clock();
    approxTime = (time_6 - time_5) / (CLOCKS_PER_SEC/1000);
    std::cout << "VRIJEME ZA BINARY TREE "<<approxTime/5000<<std::endl;

    //NizMap
    clock_t time_7 = clock();
    for(int i = 0; i < 5000;i++)
        niz[i];
    clock_t time_8 = clock();
    approxTime = (time_8 - time_7) / (CLOCKS_PER_SEC/1000);
    std::cout << "VRIJEME ZA NIZ " << approxTime / 5000<< std::endl;













    return 0;
}
