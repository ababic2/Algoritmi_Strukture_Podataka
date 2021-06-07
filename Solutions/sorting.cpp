#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>

template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    for(int i = 0; i < vel; i++){
        for(int j = 0; j<vel-1;j++){
            if(niz[j]>niz[j+1]){
                auto temp = niz[j];
                niz[j]=niz[j+1];
                niz[j+1]=temp;
            }
        }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel){
    for (int i = 0; i < vel; i++) {
        int min = i;
        for (int j = i+1; j < vel; j++) {
            if(niz[min]>niz[j]){
                min = j;
            }
        }
        auto temp = niz[min];
        niz[min]=niz[i];
        niz[i]=temp;
    }
}


template<typename Tip>
int partition(Tip* niz, int vel){
    int pivot = vel-1;
    int i=0,j=0;
    for(j=0;j<vel-1;j++){
        if(niz[j] <= niz[pivot] ){
            auto temp = niz[j];
            niz[j] = niz[i];
            niz[i] = temp;
            i++;
        }
    }
    auto temp = niz[i];
    niz[i] = niz[pivot];
    niz[pivot] = temp;
    return i;
}


template<typename Tip>
void quick_sort(Tip* niz, int vel){
    if(vel > 0){
        int pivot = partition(niz,vel);
        quick_sort(niz,pivot);
        quick_sort(niz+pivot+1,vel-pivot-1);
    }
}

template<typename Tip>
void merge(Tip* lniz, int lvel, Tip* dniz, int dvel){
    int i=0,j=0;
    Tip* sorted = new Tip[lvel+dvel]{};
    int k=0;
    while(i<lvel && j<dvel){
        if(lniz[i] < dniz[j]){
            sorted[k] = lniz[i];
            i++;
        }else{
            sorted[k] = dniz[j];
            j++;
        }
        k++;
    }
    while(i<lvel){
        sorted[k++] = lniz[i++];
    }
    while(j<dvel){
        sorted[k++] = dniz[j++];
    }
    k = 0;
    while(k<dvel+lvel){
        lniz[k] = sorted[k];
        k++;
    }
    delete[] sorted;
}



template<class Tip>
void merge_sort(Tip* niz, int vel){
    if(vel > 1){
        int prvaPolovica = vel/2;
        int drugaPolovica = vel - vel/2;
        merge_sort(niz,prvaPolovica);
        merge_sort(niz+prvaPolovica, drugaPolovica);
        merge(niz,prvaPolovica,niz+prvaPolovica,drugaPolovica);
    }

}

void generisi(std::string filename, int number){
    std::ofstream file(filename, std::ofstream::out);
    srand(time(NULL));
    for(int i = 0; i < number; i++){
        int broj = rand();
        file << broj << "\n";
    }
    file.close();
}



void ucitaj(std::string filename, int*& niz, int& vel){
    std::ifstream file(filename,std::ifstream::in);
    int broj;
    vel = 0;
    while(!file.eof()){
        file >> broj;
        vel++;
    }
    vel--;
    file.close();
    file.open(filename);
    niz = new int[vel];
    for(int i = 0; i< vel; i++){
        file >> niz[i];
    }
    file.close();
}

bool EqualArray(int *niz1, int* niz2, int vel) {
     for(int i = 0; i < 10; i++)
        if(niz1[i] != niz2[i]) return false;
    return true;
}

void ispisi_izbor() {
    std::cout << "\nIzbor sortiranja\n";
    std::cout << "Unesite 1 za bubble_sort, 2 za selection_sort, 3 za quick_sort, 4 za merge_sort\n";
}

int main() {
    int*niz;
    int n = 6;
    generisi("nizovi.txt",6);
    ucitaj("nizovi.txt",niz,n);

    for(int i = 0; i < n; i++)
        std::cout << niz[i]<<" ";

    ispisi_izbor();

    clock_t t1 = 0, t2 = 0;

    for(;;) {
        int unos;
        std::cin >> unos;
        if(unos == 1) {
            t1 = clock();
            bubble_sort(niz,n);
            t2 = clock();
            break;
        } else if(unos == 2) {
            t1 = clock();
            selection_sort(niz,n);
            t2 = clock();
            break;
        } else if(unos == 3) {
            t1 = clock();
            quick_sort(niz,n);
            t2 = clock();
            break;
        }else if(unos == 4) {
            t1 = clock();
            merge_sort(niz,n);
            t2 = clock();
            break;
        }else {
            std::cout << "Pogresan unos !\n";
            break;
        }
    }

    std::cout << "Vrijeme izvrsenja: "<< (t2 - t1) / (CLOCKS_PER_SEC/1000);

    for(int i = 1; i < n; i++)
        if(niz[i] < niz[i - 1]) {
            std::cout << "Not sorted well";
            break;
        }

    std::cout << "Sortiran je\n";

     for(int i = 0; i < n; i++)
     std::cout << niz[i] << " ";

    std::ofstream izlazni_tok("NovaDat.txt");
    for(int i = 0; i < n; i++)
        izlazni_tok << niz[i] << " ";
    izlazni_tok.close();
    delete[] niz;

    return 0;
}
