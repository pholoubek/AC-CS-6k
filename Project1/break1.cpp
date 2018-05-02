#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
using namespace std;


vector<int>::iterator find (vector<int>::iterator firstIt, vector<int>::iterator last, const int val)
{
  while (firstIt!=last) {
    if (*firstIt==val) return firstIt;
    ++firstIt;
  }
  return last;
}


template<class InputIterator>
InputIterator find (InputIterator firstIt, InputIterator last, const int val)
{
  while (firstIt!=last) {
    if (firstIt->first==val) return firstIt;
    ++firstIt;
  }
  return last;
}


void outputTheSolution(vector<pair<int,char>>& keySet, vector<int>& cipherT) {
    vector<char> newCipher;
    for(int i = 0; i < cipherT.size(); ++i) {
        auto index = find(keySet.begin(), keySet.end(), cipherT[i]);        //  overloaded find function
        if(index->first == 0) { continue; }
        else { newCipher.push_back(index->second); } 
    }
    for(auto x : newCipher) { cout << x; };
    cout << endl;
}



int main() {
  
}














