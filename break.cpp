#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

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
    cout << "   ";
    for(size_t i = 0; i < vec[0].size(); ++i) { cout << vec[0][i] << "   "; }
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
    auto temp = vec[target1];
    vec[target1] = vec[target2];
    vec[target2] = temp;
    
    // swap columns
    for(size_t i = 0; i < vec.size(); ++i) {
        for(size_t j = 0; j < vec[i].size(); ++j) {
            int temp2 = vec[i][target1];
            vec[i][target1] = vec[i][target2];
            vec[i][target2] = temp2;
        }
    }
}


int main() {
    ifstream ifs("test1_1.txt");
    vector<vector<int>> dp(28, vector<int>(28,0)); //  diagram of freq. for alphabet
    
    
    /*
        Set the Alphabet Diagram
        <space> ... z is in range from [1,27] for rows and cols
    */
    dp[0][1] = 35; //  add <space> ASCII at 0th row
    dp[1][0] = 35; //  add <space> ASCII at 0th col
    for(size_t i = 2; i < dp.size(); i++) {
        dp[0][i] = 95 + i;
        dp[i][0] = 95 + i;
    }
    
    string line;
    while(getline(ifs,line)) {  //  takes care if text in file is not one line
        for(unsigned int i = 0; i < line.size(); ++i) {  //  run through the string of text and compare nth character with n + 1st character
            int row = line[i] - 95;     //  match ASCII to row
            int col = line[i+1] - 95;   // match ASCII to col
            if(row < 1) { row = 1; }    
            if(col < 1) { col = 1; }
            dp[row][col] += 1;
//            cout << "row: " << row << " col: " << col << " :" << dp[row][col] << endl;  //  use this as check 
        }
    }
    displayAlph(dp);
    ifs.close();
    
    /*
        Cipher Diagram setup
    */
    
    //  the key goes from 1 to 105
    vector<vector<int>> dc(106, vector<int>(106,0));
    for(int i = 0; i < dc.size(); ++i) { dc[0][i] = i; }
    for(int i = 0; i < dc.size(); ++i) { 
        dc[i][0] = i;
    }
    
    ifstream afs("ciphertext.txt");
    int cipher;
    char comma;
    vector<int> cipherset;
    
    //  push cipher onto ciphertext
    while(afs >> cipher >> comma) { cipherset.push_back(cipher); } 
    
    //  pull ciphers out of ciphertext into diagram
    for(int i = 0; i < cipherset.size() - 1; ++i) {
        int row = cipherset[i];
        int col = cipherset[i + 1];
        dc[row][col] += 1;
    }
    
    swap(dp, 97, 102);
    displayAlph(dp);
    //displayCip(dc);
    afs.close();
    
    
    

    
    
}




