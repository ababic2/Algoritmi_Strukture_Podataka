#include <iostream>
// BIN STABLO KOPIRANO SA P8, kao i za AV stablo uz prepravke
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

template<typename tKey, typename tVal>
struct AVLCvor {
    tKey key;
    tVal value;
    AVLCvor<tKey,tVal>* parent, *left, *right;
    int balans = 0;
    AVLCvor(tKey key = tKey(), tVal value = tVal(), AVLCvor<tKey,tVal>* parent = nullptr, AVLCvor<tKey,tVal>*left = nullptr, AVLCvor<tKey,tVal>* right = nullptr) : key(key), value(value),parent(parent),left(left),right(right) {}
};

template<typename tKey, typename tVal>
class AVLStabloMapa : public Mapa<tKey,tVal>
{
    AVLCvor<tKey,tVal>* root;
    int size;
    //serach delete insert copy
    AVLCvor<tKey,tVal>* preorderCopy(AVLCvor<tKey,tVal>*  &to, AVLCvor<tKey,tVal>* from, AVLCvor<tKey,tVal>*parent);    // reef ??
    AVLCvor<tKey,tVal>* Search(AVLCvor<tKey,tVal>* node, const tKey& findMe)const;
    AVLCvor<tKey,tVal>* insert(AVLCvor<tKey,tVal>* &child, AVLCvor<tKey,tVal>*parent, const tKey& addMe);
    void deleteTree(AVLCvor<tKey,tVal>* node);
    AVLCvor<tKey,tVal>*findNasljednik(AVLCvor<tKey,tVal>*y,AVLCvor<tKey,tVal>*node)const;
    void rightRotate(AVLCvor<tKey,tVal>*z);
    void leftRotate(AVLCvor<tKey,tVal>*z);
    void azurirajBalans(AVLCvor<tKey,tVal>*cvor);
    void find_unbalanced(AVLCvor<tKey,tVal>*&z,AVLCvor<tKey,tVal>*&y,AVLCvor<tKey,tVal>*&x)const;
    int get_balans(AVLCvor<tKey,tVal>*element) const{
        if(element != nullptr) return element->balans;
        return 0;
    }

public:
    AVLStabloMapa() : root(nullptr), size(0){}
    ~AVLStabloMapa() {
        deleteTree(root);
    }

    AVLStabloMapa(const AVLStabloMapa& tree){
        preorderCopy(root, tree.root, nullptr);
        size = tree.size;
    }
    AVLStabloMapa(AVLStabloMapa&& tree) {
        root = tree.root;
        tree.root = nullptr;
        size = tree.size;
    }


    AVLStabloMapa& operator=(const AVLStabloMapa& tree);    //treba copy i deleteSve
    AVLStabloMapa& operator=(AVLStabloMapa&& tree);

    tVal& operator[](const tKey& key);          // treba serach i insert
    tVal operator[](const tKey& key) const;

    int brojElemenata()const override { return size; }

    void obrisi() override {
        deleteTree(root);
    }
    void obrisi(const tKey& key) override; // jos ovo
};

template<typename tKey, typename tVal>
void AVLStabloMapa<tKey,tVal>::find_unbalanced(AVLCvor<tKey,tVal>*& z,AVLCvor<tKey,tVal>*& y,AVLCvor<tKey,tVal>*& x)const {
    if(z==nullptr || z->balans < -1 || z->balans > 1) return;
    x=y;
    y=z;
    z = z->parent;
    find_unbalanced(z,y,x);
}

template<typename tKey, typename tVal>
void AVLStabloMapa<tKey,tVal>::leftRotate(AVLCvor<tKey,tVal>* z) {
     AVLCvor<tKey,tVal>*y,*t2;
     y = z->right;
     t2 = y->left;
     z->right = t2;
     if(t2) t2->parent = z;
     y->parent = z->parent;
     z->parent = y;
     y->left = z;
     auto c = get_balans(z->left);
     auto d = get_balans(z->right);
     z->balans = (c+d);
     auto a = get_balans(y->left);
     auto b = get_balans(y->right);
     y->balans = (a+b);
}

template<typename tKey, typename tVal>
void AVLStabloMapa<tKey,tVal>::rightRotate(AVLCvor<tKey,tVal>* z) {
    AVLCvor<tKey,tVal>*y,*t3;
    y = z->left;
    t3 = y->right;
    z->left = t3;
    if(t3) t3->parent = z;
    y->parent = z->parent;
    z->parent = y;
    y->right = z;
    auto c = get_balans(z->left);
    auto d = get_balans(z->right);
    z->balans = (c+d);
    auto a = get_balans(y->left);
    auto b = get_balans(y->right);
    y->balans = (a+b);
}
template<typename tKey, typename tVal>
void AVLStabloMapa<tKey,tVal>::azurirajBalans(AVLCvor<tKey,tVal>* cvor) {
    if(cvor == root) return;
    AVLCvor<tKey,tVal>* dad = cvor->parent;
    if(dad->left == cvor) dad->balans++;
    else dad->balans--;
    if(dad->balans > 1) rightRotate(dad);
    else if(dad->balans < -1) leftRotate(dad);
    azurirajBalans(dad);
}

template<typename tKey, typename tVal>
inline AVLCvor<tKey,tVal>* AVLStabloMapa<tKey,tVal>::preorderCopy(AVLCvor<tKey,tVal>* &to, AVLCvor<tKey,tVal>* from, AVLCvor<tKey,tVal>*parent) {       //copy sa preorder
    if(from) {
        to = new AVLCvor<tKey,tVal>(from->key, from -> value, parent, nullptr,nullptr);
        preorderCopy(to->left, from->left, to);
        preorderCopy(to->right, from->right, to);
    }
}

template<typename tKey, typename tVal>
inline AVLCvor<tKey,tVal>*AVLStabloMapa<tKey,tVal>::Search(AVLCvor<tKey,tVal>* node, const tKey& findMe) const {     //serach
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
inline AVLCvor<tKey,tVal>* AVLStabloMapa<tKey,tVal>::insert (AVLCvor<tKey,tVal>* &child, AVLCvor<tKey,tVal>*parent, const tKey& addMe) {       //insert
    if(child == nullptr) {
        child = new AVLCvor<tKey,tVal>(addMe, tVal(), parent, nullptr, nullptr);
        return child;
    }
    else if(child->key == addMe) return child;
    else if(addMe < child->key) {
        child->balans--;
        return insert(child->left, child, addMe);
    }
    else {
        child->balans++;
        return insert(child->right, child, addMe);
    }
}

template<typename tKey, typename tVal>
tVal& AVLStabloMapa<tKey,tVal>:: operator[](const tKey& key) {     //serach i insert
    AVLCvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp == nullptr)
    {
        temp = insert(root,nullptr,key);
        if(temp->parent)
        if(temp->parent && temp == temp->parent->left && temp->parent->right != nullptr)
            temp->parent->balans = 0;
        else if(temp == temp->parent->right && temp->parent->left != nullptr)
            temp->parent->balans = 0;
        else azurirajBalans(temp);
        size++;
    }
    return temp -> value;
}

template<typename tKey, typename tVal>
tVal AVLStabloMapa<tKey,tVal>::operator[](const tKey& key) const {
    AVLCvor<tKey,tVal>* temp = nullptr;
    temp = Search(root, key);
    if(temp) return temp->value;
    return tVal();
}

template<typename tKey, typename tVal>
inline void AVLStabloMapa<tKey,tVal>::deleteTree(AVLCvor<tKey,tVal>* node) {
    if(node){

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        size = 0;
    }
    root = nullptr;
}

template<typename tKey, typename tVal>
AVLStabloMapa<tKey,tVal>& AVLStabloMapa<tKey,tVal>::operator=(const AVLStabloMapa& tree) {
    if(this == &tree) return *this; // destruktivna samododjela

    deleteTree(this->root);
    preorderCopy(root,tree.root, nullptr);
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
AVLStabloMapa<tKey,tVal>& AVLStabloMapa<tKey,tVal>::operator=(AVLStabloMapa&& tree) {
    if(this == &tree) return *this;

    deleteTree(root);
    root = tree.root;
    tree.root = nullptr;
    size = tree.size;
    return *this;
}

template<typename tKey, typename tVal>
AVLCvor<tKey,tVal>*AVLStabloMapa<tKey,tVal>::findNasljednik(AVLCvor<tKey,tVal>*y,AVLCvor<tKey,tVal>*node)const {
    if(y->left == nullptr && y->parent == node) return y;
    if(y->left != nullptr) // idi lijevo
        return findNasljednik(y->left,node);
    if(y->left == nullptr && y->right != nullptr)
        return findNasljednik(y->right, node);
    return y;
}

template<typename tKey, typename tVal>
inline void AVLStabloMapa<tKey,tVal>::obrisi(const tKey& key) {
    // prvo nadjem AVLCvor koji ima trazeni key i ispitujem sljedece slucajeve
    AVLCvor<tKey,tVal>*temp = Search(root, key);
    if(temp) {
        if(temp->right == nullptr && temp->left == nullptr) {                   //case 1: nema djece nikako
            if(temp == root) root = nullptr;
            //omg niisiiii postavilllaaaa parent left i right da je nullptr
            AVLCvor<tKey,tVal>*roditelj = temp->parent;
            if(roditelj && roditelj->right == temp) roditelj->right = nullptr;
            if(roditelj && roditelj->left == temp) roditelj->left = nullptr;
            delete temp;
            size--;
        }
        else if(temp->right == nullptr) {                                       //case 1:jedno dijete ali lijevo
            AVLCvor<tKey,tVal>*cuvaj = temp->left;
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
            AVLCvor<tKey,tVal>*cuvaj = temp->right;
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

            AVLCvor<tKey,tVal>*nasljednik = findNasljednik(temp->right,temp);
            if(nasljednik->parent == temp) {
                //prvi desni nasljednik, a nema left child po funkciji
                nasljednik -> left = temp->left;
                nasljednik -> parent= temp->parent;
                if(temp == root) root = nasljednik;
                if(nasljednik->parent) {
                    if(nasljednik->parent->right == temp)
                        nasljednik->parent->right = nasljednik;
                    else nasljednik->parent->left = nasljednik;
                }
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



int main() {
    // Osnovni test funkcionalnosti AVLStabloMapa
AVLStabloMapa<std::string,std::string> m;
m["Sarajevo"] = "BiH";
m["Zagreb"] = "Hrvatska";
cout << m.brojElemenata() << " " << m["Zagreb"] << " '"; // 2 Hrvatska

// Test praznih elemenata
cout << m["Beograd"] <<"' '"; // ''
cout << m["zagreb"] << "' "; // ''
m.obrisi("Zagreb");

// Potpuno korektna AVLStabloMapa nece kreirati objekte "Beograd" i "zagreb"
// jer oni nemaju vrijednosti pa ce se ispod ispisati 1
// Ali to zahtijeva implementaciju "Pristupnika" pa cemo priznavati
// rjesenje koje ispise broj 3
cout << m.brojElemenata(); // 3 ili 1

// Test brisanja
cout << " '" << m["Zagreb"] << "' " ; // ''
m.obrisi();
cout << m.brojElemenata(); // 0
cout << " '" << m["Sarajevo"] << "' "; // ''

    return 0;
}
