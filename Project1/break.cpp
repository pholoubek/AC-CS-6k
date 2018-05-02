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

vector<pair<int,char>> bestInitKey;

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

vector<vector<int>>& initiDA(vector<vector<int>>& vec);
vector<vector<int>>& initiDC(vector<vector<int>>& vec, vector<int>& vec2, const vector<int>& mp);
void displayCip(const vector<vector<int>>& vec);
void displayAlph(const vector<vector<int>>& vec);
void swap(vector<vector<int>>& vec, int target1, int target2);
map<char, int>& makeFreqMap(map<char, int>& mp, const vector<vector<int>>& vec, const int sizeT, const int sizeC);
//map<int, float>& makeFreqMap(map<int, float>& mp, const vector<vector<int>>& vec, const int size);
vector<pair<int,char>>& initialKey(vector<pair<int,char>>& keySet, const map<char,int>& fa, const vector<int>& cipherDomain,int sizeC, const vector<int>& cipherT);
vector<vector<int>>& makeDP(vector<vector<int>>& dp, const vector<pair<int,char>>& keySet, vector<int>& cipherT); 
int computeDistance(vector<vector<int>>& dp, vector<vector<int>>& da);
int innerHillClimb(vector<vector<int>>& dp, vector<vector<int>>& da, vector<pair<int,char>> keySet, vector<int>& cipherT);
int randomInitialKey(const map<char,int>& fa, vector<vector<int>>& da, const vector<int>& cipherDomain,int sizeC, vector<int>& cipherT);
void outerSwap(map<char,int>& fa, char trg1, char trg2);
void outputTheSolution(vector<pair<int,char>>& keySet, vector<int>& cipherT);

void outerHillClimb(const vector<vector<int>>& dc, vector<vector<int>>& da, 
    map<char,int>& fa, const vector<int>& cipherDomain,int sizeC, vector<int>& cipherset) 
{
    vector<pair<int,char>> bestKey;
    map<char,int> faPrime = fa;
    int bestScore = randomInitialKey(fa, da, cipherDomain, sizeC, cipherset);
    bestKey = bestInitKey;
    for(int i = 0; i < 25; ++i){
        map<char,int>::iterator indexInMap = faPrime.begin();
        for(int j = 0; j < 26 - i; ++j){
            advance(indexInMap, j);
            map<char,int>::iterator tempInMap = indexInMap;
            advance(tempInMap, i);
            outerSwap(faPrime, indexInMap->first,tempInMap->first);
            int score = randomInitialKey(fa, da, cipherDomain, sizeC, cipherset);
            if (score < bestScore) {
                fa = faPrime;
                bestScore = score;
                bestKey = bestInitKey;
            } else {
                faPrime = fa;
                advance(indexInMap, i);
                outerSwap(faPrime,indexInMap->first, tempInMap->first);
                score = randomInitialKey(fa, da, cipherDomain, sizeC, cipherset);
                if(score < bestScore) {
                    fa = faPrime;
                    bestScore = score;
                    bestKey = bestInitKey;
                    cout << " reached the final if state" << endl;
                }
                cout << " reached the final else state" << endl;
            }
        }
    }
    outputTheSolution(bestKey, cipherset);
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
 
void swapKey(vector<pair<int,char>>& keySet, int target1, int target2) {
    if(target1 >= keySet.size()) { target1 = target1 % keySet.size(); }
    if(target2 >= keySet.size()) { target2 = target2 % keySet.size(); }
    char temp = keySet[target1].second;
//    cout << "swapping : " << temp << " with " << keySet[target2].second << endl; 
    keySet[target1].second = keySet[target2].second;
    keySet[target2].second = temp;
} 

void outerSwap(map<char,int>& fa, char trg1, char trg2){
    fa[trg1] += 1;
    fa[trg2] -= 1; 
}
    


int randomInitialKey(const map<char,int>& fa, vector<vector<int>>& da,
    const vector<int>& cipherDomain,int sizeC, vector<int>& cipherT) 
{
    vector<pair<int,char>> keySet;
    vector<vector<int>> dp(28, vector<int>(28,0));
    int bestInitScore = numeric_limits<int>::max();
    
    //  this depends on how much we want to make it work
    for(int i = 0; i < 1; ++i){
        keySet = initialKey(keySet, fa, cipherDomain, sizeC, cipherT); 
//        cout << "after the initialKey" << endl;
        dp = makeDP(dp, keySet, cipherT);
//        cout << "after the makeDp" << endl;
        int initScore = innerHillClimb(dp, da, keySet, cipherT);
        if(initScore < bestInitScore) {
            bestInitScore = initScore;
            bestInitKey = keySet;
        }
    }
    return bestInitScore;
}

int innerHillClimb(vector<vector<int>>& dp, vector<vector<int>>& da, vector<pair<int,char>> keySet, vector<int>& cipherT) {
    int innerScore = 0;
    vector<vector<int>> dpPrime = dp;
    innerScore = computeDistance(dp, da);
//    cout << innerScore << endl;
    
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10-i; ++j) {
            vector<pair<int,char>> tempKeySet = keySet;
            swapKey(tempKeySet,j,j+1);
            dpPrime = makeDP(dpPrime, tempKeySet, cipherT);  //  This doesn't work as it should
            int checkScore = computeDistance(dpPrime, da);
            if (checkScore < innerScore) {
                innerScore = checkScore;
                keySet = tempKeySet;  
                dp = dpPrime;
            }
        }
    }
    return innerScore;    
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

vector<vector<int>>& makeDP(vector<vector<int>>& dp, const vector<pair<int,char>>& keySet,
     vector<int>& cipherT) 
{   
    vector<char> newCipher;
    for(int i = 0; i < cipherT.size(); ++i) {
        auto index = find(keySet.begin(), keySet.end(), cipherT[i]);        //  overloaded find function
        if(index->first == 0) { continue; }
        else { newCipher.push_back(index->second); } 
    }
//    for(auto x : newCipher) { cout << x;}
//    cout << endl;
      
    dp[0][1] = 35; //  add <space> ASCII at 0th row
    dp[1][0] = 35; //  add <space> ASCII at 0th col
    for(size_t i = 2; i < dp.size(); i++) {
        dp[0][i] = 95 + i;
        dp[i][0] = 95 + i;
    }
     
    //  Resolved.. Those were values with no cipher attached to it. bug here: within newCipher are NUL values which don't correspond to anything in the guessed text 
    for(unsigned int i = 0; i < newCipher.size(); ++i) {  //  run through the string of text and compare nth character with n + 1st character
        int row = newCipher[i] - 95;     //  match ASCII to row
        int col = newCipher[i+1] - 95;   // match ASCII to col
        if(row < 1) { row = 1; }
        if(col < 1) { col = 1; }    
        dp[row][col] += 1;
    }    
    //displayAlph(dp);
    return dp;
}


int main() {
    ifstream ifs("test1_1.txt");
    vector<vector<int>> da(28, vector<int>(28,0));      //  diagram of count. for alphabet
    map<char, int> fa;                               //  map for alphabetic frequency
    vector<int> cipherDomain;                                 //  map for cipher frequency
    
    //  set initial values to alph diagram
    da = initiDA(da);   
    
    string line;
    int sizeA = 0;
    while(getline(ifs,line)) {  //  takes care if text in file is not one line
        for(unsigned int i = 0; i < line.size(); ++i) {  //  run through the string of text and compare nth character with n + 1st character
            sizeA += 1;
            int row = line[i] - 95;     //  match ASCII to row
            int col = line[i+1] - 95;   // match ASCII to col
            if(row < 1) { row = 1; }    
            if(col < 1) { col = 1; }
            da[row][col] += 1;
        }
    }
    ifs.close();
    
//    fa = makeFreqMap(fa,da, sizeA);

    ifstream afs("ciphertext.txt");
    int sizeC = 0;
    int cipher;
    char comma; //  to skip comma in the ciphertext file
    vector<int> cipherset;
    
    // push cipher onto ciphertext
    // skips the last cipher since there's no comma after the nth cipher
    while(afs >> cipher >> comma) { 
        cipherset.push_back(cipher);
        auto it = find(cipherDomain.begin(), cipherDomain.end(), cipher);
        if(it == cipherDomain.end()) { cipherDomain.push_back(cipher); }
    }   
    //  plug, it needs better solution through tokenization
    afs >> cipher;  
    cipherset.push_back(cipher);
    auto it = find(cipherDomain.begin(), cipherDomain.end(), cipher);
    if(it == cipherDomain.end()) { cipherDomain.push_back(cipher); }
       
    vector<vector<int>> dc(cipherDomain.size() + 1, 
        vector<int>(cipherDomain.size() + 1,0));    //  diagram for ciphers, initialized forcipher which exists
    
    dc = initiDC(dc, cipherset, cipherDomain);
    sizeC = cipherDomain.size();
    fa = makeFreqMap(fa,da, sizeA,sizeC);
    vector<pair<int,char>>* finalKey;
//    for(auto x : cipherDomain) { cout << x << " ";}
//    cout << endl;
//    void outerHillClimb(const vector<vector<int>>& dc, vector<vector<int>>& da, 
//    const map<char,float>& fa, const vector<int>& cipherDomain,int sizeC, vector<int>& cipherT)
      outerHillClimb(dc, da, fa, cipherDomain, sizeC, cipherset);
//    vector<pair<int,char>> finalKey2;
//    finalKey2 = *finalKey;
//    for(auto x : finalKey2) { cout << x.second << " has " << x.first; }
//    cout << endl;

    afs.close();    
}

vector<pair<int,char>>& initialKey(vector<pair<int,char>>& keySet, const map<char,int>& fa, 
    const vector<int>& cipherDomain,int sizeC, const vector<int>& cipherT) 
{
    int count = 0;
    vector<int> temp;
    
    for(auto x : cipherDomain){ temp.push_back(x); }
    int sentinel = temp.size();
    
    map<char,int>::const_iterator it;
    for(it = fa.begin(); it != fa.end(); ++it){
        if(count < sentinel) {
            count += it->second;
            for(size_t i = 0; i < it->second; ++i) {
                keySet.push_back(pair<int,char>(temp.back(), it->first));
                temp.pop_back();
                random_shuffle(temp.begin(),temp.end());
//                cout << count << endl;
            }
        }
    } 
    return keySet;   
}

vector<vector<int>>& initiDC(vector<vector<int>>& vec, vector<int>& vec2, const vector<int>& mp) {
    int index = 1;
    for(auto x : mp) {
        vec[index][0] = x;
        vec[0][index] = x;
        ++index;
    }
    
    int r = 0;
    int c = 0;
    for(size_t i = 0; i < vec2.size(); ++i){
        for(size_t j = 1; j < vec[0].size(); ++j) {
            if(vec2[i] == vec[0][j]) {
                r = j;
                break;
            }
        }
        for(size_t k = 1; k < vec[0].size(); ++k) {
            if(vec2[i + 1] == vec[0][k]) {
                c = k;
                break;
            }
        }
        vec[r][c] += 1;
    }
    
    return vec;  
}

vector<vector<int>>& initiDA(vector<vector<int>>& vec) {    
    /*
        Set the Alphabet Diagram
        <space> ... z is in range from [1,27] for rows and cols
    */
    vec[0][1] = 35; //  add <space> ASCII at 0th row
    vec[1][0] = 35; //  add <space> ASCII at 0th col
    for(size_t i = 2; i < vec.size(); i++) {
        vec[0][i] = 95 + i;
        vec[i][0] = 95 + i;
    }
    return vec;
}

//  Display Matrix where 0 col && 0 row is Alphabet title
//  3 spaces in cout due to max 3 digit occurence
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

void swap(vector<vector<int>>& vec, int target1, int target2) { 
    //  swap rows first
    target1 = target1 - 95;
    target2 = target2 - 95;
    auto temp = vec[target1];
    vec[target1] = vec[target2];
    vec[target2] = temp;
    int temp3 = vec[0][target1];
    vec[0][target1] = vec[0][target2];
    vec[0][target2] = temp3;
    
    // swap columns
    for(size_t i = 0; i < vec.size(); ++i) {
        for(size_t j = 0; j < vec[i].size(); ++j) {
            int temp2 = vec[i][target1];
            vec[i][target1] = vec[i][target2];
            vec[i][target2] = temp2;
        }
    }
}

//  for each letter in Alphabet, sum a row of the matrix and divide the sum by the entire size of the message.
map<char, int>& makeFreqMap(map<char, int>& mp, const vector<vector<int>>& vec, const int sizeT, const int sizeC) {
    float sum = 0;
    int count = 0;
    for(size_t i = 1; i < vec.size(); ++i){
        for(size_t j = 1; j < vec[i].size(); ++j){ sum += vec[i][j]; }
            count = (sum/sizeT) * sizeC;
            if(count == 0) count = 1;
            mp.insert(pair<char, float>(vec[i][0],count));
            sum = 0;
    }
//    for(auto x : mp) { cout << x.first << " has " << x.second << endl; }
    return mp;
}

//map<int, float>& makeFreqMap(map<int, float>& mp, const vector<vector<int>>& vec, const int size) {
//    float sum = 0;
//    for(size_t i = 1; i < vec.size(); ++i){
//        for(size_t j = 1; j < vec[i].size(); ++j){ sum += vec[i][j]; }
//        mp[vec[i][0]] = sum / size;
//        sum = 0;
//    }
//    return mp;
//}




