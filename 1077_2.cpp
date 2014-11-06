/* Solve the Eight Digits Problem */
/* Algorithm: A* search */
/* Author: Jingyu Wang */
/* Date: 3/17/2014 */

#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>

#define NMAX 363000
#define LEN 9

using namespace std;

struct node{
    int deep, manh, total;
    int board[LEN];
};

/* Reload the comparison operator */
struct myCmp{
    bool operator()(node N1, node N2){
        return N1.total > N2.total;
    }
};


int Arr[LEN];/* Current board status */
bool Visited[NMAX];/* Use Ind to label visited or not */
int Father[NMAX];
int Move[NMAX];

int Dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
int Fac[LEN] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320};

/* Priority queue of A* search - min heap */
priority_queue<node, vector<node>, myCmp> pq;



/* Using Cantor expansion to map Arr to Ind */
int arrToInd(){
    int buf[LEN] = {0};
    int ans, i, j;
    for(ans = 0, i = 0; i < LEN; i++){
        for(j = i+1; j < LEN; j++){
            if(Arr[j] < Arr[i]) buf[i]++;
        }
        ans += buf[i]*Fac[8-i];
    }
    return ans;
}



/* Calculate the Manhattan Distance of current Arr status */
int getManh(){
    int sum, i;
    for(sum = 0, i = 0; i < LEN; i++){
        if(Arr[i] != 9) sum += abs((i+1)-Arr[i]);
    }
    return sum;
}


/*--------------------------------------*/
/* Utilities start from here */
void enQue(int ind, int deep){
    Visited[ind] = true;
    node N; N.deep = deep; N.manh = getManh();
    N.total = N.deep + N.manh;
    /* copy the current board status to 'board' attribute of node N */
    for(int i = 0; i < LEN; i++){
        N.board[i] = Arr[i];
    }
    pq.push(N); /* push into pq */
}


node deQue(){
    node N = pq.top();
    /* copy the 'board' attribute to board */
    for(int i = 0; i < LEN; i++){
        Arr[i] = N.board[i];
    }
    pq.pop();  /* pop from pq */
    return N;
}


/* judge if a move is legal */
bool isLegal(int row, int col){
    if(0 <= row && row < 3 && 0 <= col && col < 3) return true;
    else return false;
}


/* swap two numbers in the board */
void swap(int row1, int col1, int row2, int col2){
    int pos1 = row1*3+col1;
    int pos2 = row2*3+col2;
    int temp = Arr[pos1];
    Arr[pos1] = Arr[pos2];
    Arr[pos2] = temp;
}

/* Find X position in current board status */
int findX(){
    for(int i = 0; i < LEN; i++){
        if(Arr[i] == 9) return i;
    }
}



/*
 * A* search function: return true if solvable, false otherwise.
 * if we find the aim board (id == 0), return immediately, and during
 * the moving process, store the move steps in Move array, and store
 * a board's ancestor in Father array.
 */
bool Astar(){
    
    int ind, i, xPos;
    int row, col, newRow, newCol;
    int newInd, deep;
    node N;
    
    ind = arrToInd(); /* get the ind of this status */
    if(ind == 0) return true; /* first attemp is our goal */
    enQue(ind, 1); /* if not, enque the first elem */
    
    /* while queue is not empty */
    while(pq.size() != 0){
        N = deQue(); deep = N.deep;
        ind = arrToInd();
        xPos = findX();
        row = xPos/3; col = xPos%3; /* get the row and col of X */
        for(i = 0; i < 4; i++){     /* search in four directions */
            newRow = row + Dir[i][0];
            newCol = col + Dir[i][1];
            if(isLegal(newRow, newCol)){
                swap(row, col, newRow, newCol);      /* make move */
                newInd = arrToInd();     /* get the new board id */
                if(!Visited[newInd]){
                    enQue(newInd, deep+1);
                    Father[newInd] = ind; /* store: from which board we get the new board */
                    Move[newInd] = i;     /* store: what move can lead to the new board */
                    if(newInd == 0) return true; /* Yeah! */
                }
                swap(row, col, newRow, newCol);/* Cancel the move */
            }
        }
    }
    return false;
}


/* If solvable, print the moves arrording to the
 * Move and Father arrays. Start from the very end status:
 * (id == 0).
 */
void printResult(){
    int ptr = 0;
    int ind = 0;/* Start from final status */
    int Step[NMAX];/* Record a reversal sequence */
    while(Father[ind] != ind){
        Step[ptr] = Move[ind];
        ind = Father[ind];
        ptr++;
    }
    for(int i = ptr-1; i >= 0; i--){
        switch (Step[i]){
            case 0: cout << 'r'; break;
            case 1: cout << 'l'; break;
            case 2: cout << 'd'; break;
            case 3: cout << 'u'; break;
        }
    }
    cout << endl;
}


int main(){
    char temp;
    int i;
    bool result;
    for(i = 0; i < LEN; i++){
        cin >> temp;
        if(temp == 'x') Arr[i] = 9;
        else Arr[i] = temp - '0';
    }
    for(i = 0; i < NMAX; i++){
        Father[i] = i;
    }
    result = Astar();
    if(result) printResult();
    else cout << "unsolvable" << endl;
    return 0;
}