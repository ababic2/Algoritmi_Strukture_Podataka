#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include<queue>
using namespace std;

int numOfDigits(int number) {
    int length = (int) floor( log10 (number) ) + 1;
    return length;
}

int selectedDigit(int number, int place) {
    return number/ place % 10;
}

int maxDigit(const std::vector<int>&vec) {
    int max = vec.at(0);
    for(int i = 1; i < vec.size(); i++)
        if(vec.at(i) > max) max = vec.at(i);
    return numOfDigits(max);
}

void radixSort(std::vector<int>& a) {
    const int MAX_DIGIT = maxDigit(a);
    const int radix = 10;
    std::vector<std::queue<int>>bins;
    bins.resize(10);

    for(int d = 1, place = 1; d <=MAX_DIGIT; place *= radix, d++) {
        for(int i = 0; i < a.size(); i++)
            bins[selectedDigit(a[i],place)].push(a.at(i));

        a.resize(0);
        for(int i = 0; i < 10; i++) {
            while(!bins[i].empty()) {
                a.push_back(bins[i].front());
                bins[i].pop();
            }
        }
    }
}

bool isList(int i, int velicina) {
    return ((i >= velicina / 2) && (i < velicina));
}

void popraviDolje(int i, int velicina, std::vector<int>&a) {
    while(!isList(i,velicina)) {
        int veci = 2 * i + 1; // lijevo dijete
        int dd = 2 * i + 2; // desno dijete
        if((dd < velicina) && a[dd] > a[veci]) veci = dd;
        if(a[i] > a[veci]) return;
        std::swap(a[i],a[veci]);
        i = veci;
    }
}

void stvoriGomilu(std::vector<int>&a) {
    int velicina = a.size();
    for(int i = velicina / 2; i >= 0; i--)
        popraviDolje(i, velicina,a);
}

int roditelj(int i) {
    return (i - 1) / 2;
}

void popraviGore(std::vector<int>&a, int i) {

    while(i != 0 && a[i] > a[roditelj(i)]) {
        int j = roditelj(i);
        std::swap(a[i],a[j]);
        i = roditelj(i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    velicina--;
    std::swap(a[0],a[velicina]);
    if(velicina != 0)
        popraviDolje(0,velicina,a);
    return a[velicina];
}

void organise(std::vector<int>&a, int vel, int i) {
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < vel && a[left] > a[max])
        max = left;
    if(right < vel && a[right] > a[max])
        max = right;
    if(max != i){
        std::swap(a[i],a[max]);
        organise(a,vel,max);
    }
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    for(int i = a.size() - 1; i >= 0; i--) {
        std::swap(a[0],a[i]);
        organise(a,i,0);
    }
}

int main() {

    { // radix sort test
        std::vector<int>a{170, 45, 75, 90, 802, 24, 2, 66};
        std::vector<int>expected{2, 24, 45, 66, 75, 90, 170, 802};
        radixSort(a);
        if(a == expected) std::cout << "OK";
        else std::cout << "NOT OK";
    }
       vector<int> a;
    a.push_back(546);
    a.push_back(1);
    a.push_back(31);
    a.push_back(1001);
    a.push_back(104351);
    a.push_back(7);
    a.push_back(6);
    a.push_back(60045);
    gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }
    return 0;
}
