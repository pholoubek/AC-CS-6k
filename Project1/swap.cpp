#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void display(vector<vector<int>>& vec) {
    for(size_t i = 0; i < vec.size(); ++i) {
        for(size_t j = 0; j < vec[i].size(); ++j) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int count = 0;
    vector<vector<int>> vec(5, vector<int>(5,0));
    for(size_t i = 0; i < vec.size(); ++i) {
        for(size_t j = 0; j < vec[i].size(); ++j) {
            vec[i][j] = count;
            count += 1; 
        }
        
    }
    display(vec);
    
    cout << endl;
    // swap 2d matrix columns
    // need two atrributes 
    
    vector<int> temp;
    temp = vec[0];
    vec[0] = vec[3];
    vec[3] = temp;
    for(int i = 0; i < vec.size(); ++i) {
        for(int j = 0; j < vec[i].size(); ++j) {
            int temp = vec[i][0];
            vec[i][0] = vec[i][3];
            vec[i][3] = temp;
        }
    }
    
    display(vec);
        
}
