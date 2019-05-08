#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>

#define HAND_SIZE 4

using namespace std;

bool checkHand(int hand[HAND_SIZE], int goalNum, int permutations[][HAND_SIZE]);
float doOperation(float a, float b, int opCode);
long long factorial(unsigned int n);
float solveHand(int hand[HAND_SIZE], int i, int operatorList[], int permutations[][HAND_SIZE]);

int main() {
    ofstream fout("24Probability.csv");
    int hand[HAND_SIZE];

    int permutations[factorial(HAND_SIZE)][HAND_SIZE];

    //initialize hand order
    int handOrder[HAND_SIZE];
    for (int i = 0; i < HAND_SIZE; i++) {
        handOrder[i] = i;
    }

    //generate permutations of hand
    for (int i = 0; i < HAND_SIZE; i++) {
        permutations[0][i] = handOrder[i];
    }
    for (int i = 1; next_permutation(handOrder, handOrder + HAND_SIZE); i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            permutations[i][j] = handOrder[j];
        }
    }

    //fill card array with card numbers
    int cards[40];
    for (int i = 0; i < 10; i++) {
        cards[4*i] = i + 1;
        cards[4*i+1] = i + 1;
        cards[4*i+2] = i + 1;
        cards[4*i+3] = i + 1;
    }

    int goalNum;
    cout << "Enter target sum: ";
    cin >> goalNum;
    int workingHands = 0; //counts the number of working permutations for each target

    //generate every possible hand permutation
    for (int i = 0; i < 40; i++) {
        hand[0] = cards[i];
        for (int j = 0; j < 39; j++) {
            if (i <= j) {
                hand[1] = cards[j + 1];
            }
            else {
                hand[1] = cards[j];
            }
            for (int k = 0; k < 38; k++) {
                if (i <= k) {
                    if (j <= k + 1) {
                        hand[2] = cards[k + 2];
                    }
                    else {
                        hand[2] = cards[k + 1];
                    }
                }
                else {
                    hand[2] = cards[k];
                }
                for (int l = 0; l < 37; l++) {
                    if (i <= l) {
                        if (j <= l + 1) {
                            if (k <= l + 2) {
                                hand[3] = cards[l + 3];
                            }
                            else {
                                hand[3] = cards[l + 2];
                            }
                        }
                        else {
                            hand[3] = cards[l + 1];
                        }
                    }
                    else {
                        hand[3] = cards[l];
                    }
                    if (checkHand(hand, goalNum, permutations)) {
                        workingHands++;
                    }
                }
            }
        }
    }
    //print out result to screen and file
    cout << goalNum << ": " << workingHands << endl;
    fout << goalNum << ", " << workingHands << "," << endl;
    fout.close();
}

//checks if a hand can create 24
bool checkHand(int hand[HAND_SIZE], int goalNum, int permutations[][HAND_SIZE]) {
    for (int i = 0; i < factorial(HAND_SIZE); i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                for (int l = 0; l < 4; l++) {
                    int operatorList[HAND_SIZE - 1];
                    operatorList[0] = j;
                    operatorList[1] = k;
                    operatorList[2] = l;
                    float tempTotal = solveHand(hand, i, operatorList, permutations);
                    if (tempTotal == goalNum && tempTotal - int(tempTotal) == 0.0) {
                        //can sum to goal num & doesn't take advantage of float truncation
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//converts an int into an operation
float doOperation(float a, float b, int opCode) {
    switch(opCode) {
        case 0: return a + b;
        case 1: return a - b;
        case 2: return a * b;
        case 3: return a / b;

    }
}

//takes in a hand and a list of operators and outputs the result when summed
float solveHand(int hand[HAND_SIZE], int i, int operatorList[], int permutations[][HAND_SIZE]) {
    float sum = hand[permutations[i][0]];
    for (int index = 1; index < HAND_SIZE; index++) {
        sum = doOperation(sum, hand[permutations[i][index]], operatorList[index - 1]);
    }
    return sum;
}

//computes n!
long long factorial(unsigned int n) {
    if (n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
