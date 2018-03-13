/*
  @author: Petr Holoubek
  @version: 1.0
  @description:
    This program uses HillClimb approach to break Homophonic cipher by rotating matrices of 
    letters/symbols generated from the message space and given cipher space and finding the least
    deviation of frequency. 
*/
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

class BreakAlgorithm {
public:
    BreakAlgorithm(istream& ifs, vector<int>& cipherDomain, vector<int>& cipherText) : da(28, vector<int>(28,0)), 
        dc(cipherDomain.size() + 1, vector<int>(cipherDomain.size() + 1,0)), dp(28, vector<int>(28,0)),
        cipherDomain(cipherDomain), cipherText(cipherText), sizeC(cipherDomain.size()), sizeA(0)
    {
        initDA(ifs);
        initDC();
        outerHillClimb();
    }
      
      
    void outerHillClimb() {
        vector<pair<int,char>> tempKey;
        map<char,int> faPrime(fa);
        int bestScore = randomInitialKey(fa);
        bestKey = bestInitKey;
        char trg1;
        char trg2;
        
        for(int i = 1; i < 3; ++i) {
            for(int j = 1; j < 4 - i; ++j) {
                trg1 = da[0][j];
                trg2 = da[j+i][0];
                outerSwap(faPrime, trg1, trg2);
                int score = randomInitialKey(faPrime);
                if (score < bestScore) {
                    key = tempKey;
                    bestScore = score;
                    bestKey = bestInitKey;
                } else {
                    tempKey = key;
                    trg1 = da[0][j+i];
                    trg2 = da[0][j];
                    outerSwap(faPrime, trg1,trg2);
                    score = randomInitialKey(faPrime);
                    if(score < bestScore) {
                        key = tempKey;
                        bestScore = score;
                        bestKey = bestInitKey;
                    }
                }
            }
        }
        outputTheSolution();
    }
     

    /*
        Creates a possible Key Space for the Crypto Message
    */
    int randomInitialKey(map<char,int>& primeFA) {
        int bestInitScore = numeric_limits<int>::max();

        for(int i = 0; i < 2; ++i){
            key = initialKey(primeFA); 
            initDP(dp, key);
            int initScore = innerHillClimb();
            
            if(initScore < bestInitScore) {
                bestInitScore = initScore;
                bestInitKey = key;
            }
        }
        return bestInitScore;
    }
    
    /*
        Find the matrix which has the least deviation from the Message Space frequency
    */
    int innerHillClimb(){
        vector<vector<int>> dpPrime(dp.size(),vector<int>(dp.size()));      
        int innerScore = computeDistance(dp, da);
        for(int i = 1; i < 2; ++i) {
            for(int j = 1; j < 3-i; ++j) {
                vector<pair<int,char>> tempKeySet(key);
                swapKey(tempKeySet,j,j+i);
                initDP(dpPrime, tempKeySet);
                int checkScore = computeDistance(dpPrime, da);
                if (checkScore < innerScore) {
                    innerScore = checkScore;
                    key = tempKeySet;  
                    dp = dpPrime;
                }
            }
        }
        return innerScore;  
    }
    
private:
    vector<pair<int,char>> bestInitKey;
    vector<pair<int,char>> bestKey;
    vector<pair<int,char>> key;
    vector<vector<int>> da;
    vector<vector<int>> dc;
    vector<vector<int>> dp;
    vector<int> cipherDomain;
    vector<int> cipherText; 
    //  This was given frequency of the Message Space
    map<char,int> fa {
        {'#',19},{'a',7},{'b',1},{'c',2},
        {'d',4},{'e',10},{'f',2},{'g',2},
        {'h',5},{'i',6},{'j',1},{'k',1},
        {'l',3},{'m',2},{'n',6},{'o',6},
        {'p',2},{'q',1},{'r',5},{'s',5},
        {'t',7},{'u',2},{'v',1},{'w',2},
        {'x',1},{'y',2},{'z',1}};
    int sizeC;
    int sizeA;
    
    void outputTheSolution() {
        vector<char> newCipher;
        for(unsigned int i = 0; i < cipherText.size(); ++i) {
            auto index = find(bestKey.begin(), bestKey.end(), cipherText[i]);
            if(index->second == '#') { index->second = ' '; }
            newCipher.push_back(index->second); 
        }
        for(auto x : newCipher) { cout << x; };
        cout << endl;
    }
    
    void outerSwap(map<char,int>& fa, char trg1, char trg2){
        fa[trg1] += 1;       
        fa[trg2] -= 1;
    }
    
    void swapKey(vector<pair<int,char>>& keySet, unsigned int target1, unsigned int target2) {
        if(target1 >= keySet.size()) { target1 = target1 % keySet.size(); }
        if(target2 >= keySet.size()) { target2 = target2 % keySet.size(); }
        char temp = keySet[target1].second;
        keySet[target1].second = keySet[target2].second;
        keySet[target2].second = temp;
    } 
    
    int computeDistance(vector<vector<int>>& dp, vector<vector<int>>& da) {
        int innerDistance = 0;
        for(size_t i = 1; i < da.size(); ++i){
            for(size_t j = 1; j < da.size(); ++j) {
                innerDistance += dp[i][j] - da[i][j];
            }
        }
        return innerDistance;
    }
    
    vector<pair<int,char>>& initialKey(map<char,int>& fa) {
        vector<pair<int,char>>* tempKey = new vector<pair<int,char>>();
        int count = 0;
        vector<int> temp;
        
        for(int x : cipherDomain){ temp.push_back(x); }
        int sentinel = temp.size();
        
        map<char,int>::const_iterator it;
        for(it = fa.begin(); it != fa.end(); ++it){
            if(count < sentinel) {
                count += it->second;
                for(unsigned int i = 0; i < it->second; ++i) {
                    tempKey->push_back(pair<int,char>(temp.back(), it->first));
                    temp.pop_back();
                }
            }
        }
        while(!temp.empty()) {
            int index = (temp.back() % 20) + 97;
            tempKey->push_back(pair<int,char>(temp.back(), index));
            temp.pop_back();
        }
    return *tempKey;  
    }
    
    /*
        Builds Diagram for the Frequency of the known Message Space
    */
    void initDA(istream& ifs) {
        da[0][1] = 35;  // ASCII for # instead of space
        da[1][0] = 35;
        dp[0][1] = 35;
        dp[1][0] = 35; 
        for(size_t i = 2; i < da.size(); i++) {
            da[0][i] = 95 + i;
            da[i][0] = 95 + i;
            dp[0][i] = 95 + i;
            dp[i][0] = 95 + i;
        }
        string line;
        while(getline(ifs,line)) {  
            for(unsigned int i = 0; i < line.size(); ++i) { 
                sizeA += 1;
                int row = line[i] - 95;     //  match ASCII to row
                int col = line[i+1] - 95;   // match ASCII to col
                if(row < 1) { row = 1; }    
                if(col < 1) { col = 1; }
                da[row][col] += 1;
            }
        }
    }
    
    void initDC() {
        int index = 1;
        for(auto symbol : cipherDomain) {
            dc[index][0] = symbol;
            dc[0][index] = symbol;
            ++index;
        }
        
        int r = 0;
        int c = 0;
        for(size_t i = 0; i < cipherText.size(); ++i){
            for(size_t j = 1; j < dc[0].size(); ++j) {
                if(cipherText[i] == dc[0][j]) { 
                    r = j; 
                    break;
                }
            }
            for(size_t k = 1; k < dc[0].size(); ++k) {
                if(cipherText[i + 1] == dc[0][k]) { 
                    c = k; 
                    break;
                }
            }                    
        dc[r][c] += 1;
        }
    }
       
    void initDP(vector<vector<int>>& dp, vector<pair<int,char>>& keySet) {
        vector<char> newCipher;
        
        for(unsigned int i = 0; i < cipherText.size(); ++i) {
            auto index = find(keySet.begin(), keySet.end(), cipherText[i]);        
            newCipher.push_back(index->second); 
        } 
                
        for(unsigned int i = 0; i < newCipher.size(); ++i) {  
            int row = newCipher[i] - 95;     //  match ASCII to row
            int col = newCipher[i+1] - 95;   // match ASCII to col
            if(row < 1) { row = 1; }
            if(col < 1) { col = 1; }
            dp[row][col] += 1;            
        }
    }
    
    void displayAlph(const vector<vector<int>>& vec) {
        cout << " ";
        for(size_t i = 0; i < vec[0].size(); ++i) { cout << char(vec[0][i]) << "   "; }
        cout << endl;
        for(size_t i = 1; i < vec.size(); ++i) {
            for(size_t j = 0; j < vec[i].size(); ++j) {
                if(j == 0) { cout << char(vec[i][0]) << "   "; }
                else { cout << vec[i][j] << "   "; }
                
            }
            cout << endl;
        }
    }
    
    void displayCip(const vector<vector<int>>& vec) {
        for(size_t i = 0; i < vec[0].size(); ++i) { cout << vec[0][i] << "  "; }
        cout << endl;
        for(size_t i = 1; i < vec.size(); ++i) {
            for(size_t j = 0; j < vec[i].size(); ++j) {
                if(j == 0) { cout << vec[i][0] << "   "; }
                else { cout << vec[i][j] << "   "; }   
            }
            cout << endl;
        }
    }   
    
    void makeFreqMap() {
        float sum = 0;
        int count = 0;
        for(size_t i = 1; i < da.size(); ++i){
            for(size_t j = 1; j < da[i].size(); ++j){ sum += da[i][j]; }
                count = (sum/sizeA) * sizeC;
                if(count == 0) count = 1;
                fa.insert(pair<char, int>(da[i][0],count));
                sum = 0;
        }
        for(auto x : fa) { cout << "{" << "'" << x.first << "'" <<  "," << x.second << "},"; }
    }
};

int main() {
    ifstream ifs("MessageSpace.txt");
    ifstream afs("ciphertext.txt");

    vector<int> cipherText;
    vector<int> cipherDomain;

    int cipher;
    char comma;
    while(afs >> cipher >> comma) { 
        cipherText.push_back(cipher);
        auto it = find(cipherDomain.begin(), cipherDomain.end(), cipher);
        if(it == cipherDomain.end()) { cipherDomain.push_back(cipher); }
    }      
    afs >> cipher;  
    cipherText.push_back(cipher);
    auto it = find(cipherDomain.begin(), cipherDomain.end(), cipher);
    if(it == cipherDomain.end()) { cipherDomain.push_back(cipher); }
     
    BreakAlgorithm test(ifs, cipherDomain, cipherText);
    ifs.close();
}


