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
void display(const vector<vector<int>>& vec) {
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
//            cout << "row: " << row << " col: " << col << " :" << dp[row][col] << endl;
        }
    }
    display(dp);

    
    
}




