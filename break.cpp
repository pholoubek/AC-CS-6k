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

vector<vector<int>>& initiDA(vector<vector<int>>& vec);
vector<vector<int>>& initiDC(vector<vector<int>>& vec, vector<int>& vec2, const map<int,float>& mp);
void displayCip(const vector<vector<int>>& vec);
void displayAlph(const vector<vector<int>>& vec);
void swap(vector<vector<int>>& vec, int target1, int target2);
map<char, float>& makeFreqMap(map<char, float>& mp, const vector<vector<int>>& vec, const int size);
map<int, float>& makeFreqMap(map<int, float>& mp, const vector<vector<int>>& vec, const int size);
map<char, vector<int>>& initialKey(map<char, vector<int>>& keySet, const map<char,float>& fa, const map<int,float>& fc,int sizeC, const vector<int>& cipherT);


int main() {
    ifstream ifs("test1_1.txt");
    vector<vector<int>> da(28, vector<int>(28,0));      //  diagram of count. for alphabet
    map<char, float> fa;                               //  map for alphabetic frequency
    map<int, float> fc;                                 //  map for cipher frequency
    
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
//    displayAlph(da);
    ifs.close();
    
    fa = makeFreqMap(fa,da, sizeA);


//    for(auto x : fa){
//        cout << x.first << " " << x.second << endl;
//    }
    

    
    ifstream afs("ciphertext.txt");
    int sizeC = 0;
    int cipher;
    char comma; //  to skip comma in the ciphertext file
    vector<int> cipherset;
    
    // push cipher onto ciphertext
    // skips the last cipher since there's no comma after the nth cipher
    while(afs >> cipher >> comma) { 
        cipherset.push_back(cipher);
        fc.insert(pair<int,float>(cipher,0));
    }   
    //  plug, it needs better solution through tokenization
    afs >> cipher;  
    cipherset.push_back(cipher);
    
    fc.insert(pair<int,float>(cipher,0));   
    vector<vector<int>> dc(fc.size() + 1, vector<int>(fc.size() + 1,0));    //  diagram for ciphers, initialized for cipher which exists
    
    dc = initiDC(dc, cipherset, fc);
    //fc = makeFreqMap(fc, dc, sizeC);
    sizeC = fc.size();
    
    map<char, vector<int>> keySet;
    keySet= initialKey(keySet, fa, fc, sizeC, cipherset);
    //swap(da, 97, 102);
    //displayAlph(da);
    //displayCip(dc);

    afs.close();

}

map<char, vector<int>>& initialKey(map<char, vector<int>>& keySet, const map<char,float>& fa, const map<int,float>& fc,int sizeC, const vector<int>& cipherT) {
    map<char,float>::const_iterator itA;
    vector<int> temp;
    
    for(auto it : fc){ temp.push_back(it.first); }
    random_shuffle(temp.begin(), temp.end());
    
    for(itA = fa.begin(); itA != fa.end(); ++itA){
        int set = itA->second * sizeC + 0.5;
        vector<int> numSet;
        for(int i = 0; i < set; ++i){
            numSet.push_back(temp[temp.size()-1]);
            temp.pop_back();         
        }
        keySet.insert(pair<char,vector<int>>(itA->first, numSet));
        random_shuffle(temp.begin(), temp.end());
    }
    return keySet;
}

vector<vector<int>>& initiDC(vector<vector<int>>& vec, vector<int>& vec2, const map<int,float>& mp) {
    int index = 1;
    for(auto x : mp) {
        vec[index][0] = x.first;
        vec[0][index] = x.first;
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
map<char, float>& makeFreqMap(map<char, float>& mp, const vector<vector<int>>& vec, const int size) {
    float sum = 0;
    for(size_t i = 1; i < vec.size(); ++i){
        for(size_t j = 1; j < vec[i].size(); ++j){ sum += vec[i][j]; }
        mp.insert(pair<char, float>(vec[i][0], sum/size));
        sum = 0;
    }
    return mp;
}

map<int, float>& makeFreqMap(map<int, float>& mp, const vector<vector<int>>& vec, const int size) {
    float sum = 0;
    for(size_t i = 1; i < vec.size(); ++i){
        for(size_t j = 1; j < vec[i].size(); ++j){ sum += vec[i][j]; }
        mp[vec[i][0]] = sum / size;
        sum = 0;
    }
    return mp;
}




