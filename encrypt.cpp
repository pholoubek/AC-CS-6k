#include <iostream>
#include <algorithm>
#include <time.h>
#include <string>
#include <fstream>
using namespace std;

int genKeyNums(int[]);
int genCipherTextforChar(char, int[]);
int genRand (int i) {return rand()%i;}

int main(int argc, char *argv[]){
  int keyNums[105];
  genKeyNums(keyNums);
  string ciphertext = "";
  //enter plaintext file to be encrypted as command line arg 1
  ifstream fin("test1_1.txt");
  ofstream fout("ciphertext.txt");
  
  char c;
  while(fin.get(c)){
    string s = to_string(genCipherTextforChar(c, keyNums));
    ciphertext = ciphertext + s + ",";
  }
  fin.close();
  fout << ciphertext.substr(0, ciphertext.size() - 1);
  fout.close();
}

int genKeyNums(int keyNums[]){
  //generate array of 105 nums in random order for the key
  for(int i=0; i<106; i++){
    keyNums[i] = i;
  }
  srand(time(0));
  random_shuffle(&keyNums[0], &keyNums[106], genRand);
  //debugging
  /*for(int i=0; i<106; i++){
    cout << keyNums[i] << "\n";
    }*/
  return 1;
}

int genCipherTextforChar(char a, int keyNums[]){
  srand(time(0));
  int randNum;
  switch(a){
  case ' ' :
    //19 nums
    //+0 offset
    randNum = rand() % 19;
    return keyNums[randNum];
  case 'a' :
    //7
    //+19
    randNum = rand() % 7 + 19;
    return keyNums[randNum];
  case 'b' :
    //1
    //+25
    randNum = 1 + 25;
    return keyNums[randNum];
  case 'c' :
    //2
    //+26
    randNum = rand() % 2 + 27;
    return keyNums[randNum];
  case 'd' :
    //4
    //+29
    randNum = rand() % 4 + 29;
    return keyNums[randNum];
  case 'e' :
    //10
    //+33
    randNum = rand() % 10 + 33;
    return keyNums[randNum];
  case 'f' :
    //2
    //+43
    randNum = rand() % 2 + 43;
    return keyNums[randNum];
  case 'g' :
    //2
    //+45
    randNum = rand() % 2 + 45;
    return keyNums[randNum];
  case 'h' :
    //5
    //+47
    randNum = rand() % 5 + 47;
    return keyNums[randNum];
  case 'i' :
    //6
    //+52
    randNum = rand() % 6 + 52;
    return keyNums[randNum];
  case 'j' :
    //1
    //+57
    randNum = 1 + 57;
    return keyNums[randNum];
  case 'k' :
    //1
    //+58
    randNum = 1 + 58;
    return keyNums[randNum];
  case 'l' :
    //3
    //+60
    randNum = rand() % 3 + 60;
    return keyNums[randNum];
  case 'm' :
    //2
    //+63
    randNum = rand() % 2 + 63;
    return keyNums[randNum];
  case 'n' :
    //6
    //+65
    randNum = rand() % 6 + 65;
    return keyNums[randNum];
  case 'o' :
    //6
    //+71
    randNum = rand() % 6 + 71;
    return keyNums[randNum];
  case 'p' :
    //2
    //+77
    randNum = rand() % 2 + 77;
    return keyNums[randNum];
  case 'q' :
    //1
    //+78
    randNum = 1 + 78;
    return keyNums[randNum];
  case 'r' :
    //5
    //+80
    randNum = rand() % 5 + 80;
    return keyNums[randNum];
  case 's' :
    //5
    //+85
    randNum = rand() % 5 + 85;
    return keyNums[randNum];
  case 't' :
    //7
    //+90
    randNum = rand() % 7 + 90;
    return keyNums[randNum];
  case 'u' :
    //2
    //+97
    randNum = rand() % 2 + 97;
    return keyNums[randNum];
  case 'v' :
    //1
    //+98
    randNum = 1 + 98;
    return keyNums[randNum];
  case 'w' :
    //2
    //+100
    randNum = rand() % 2 + 100;
    return keyNums[randNum];
  case 'x' :
    //1
    //+101
    randNum = 1 + 101;
    return keyNums[randNum];
  case 'y' :
    //2
    //+103
    randNum = rand() % 2 + 103;
    return keyNums[randNum];
  case 'z' :
    //1
    //+104
    randNum = 1 + 104;
    return keyNums[randNum];
  default:
    return 1001;
  }
}

