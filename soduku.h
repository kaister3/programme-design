//
//  soduku.h
//  Sudoku
//
//  Created by Kaister Wang on 2019/3/12.
//  Copyright © 2019 com.KaisterWang. All rights reserved.
//
//  l = 10
//  L = 3 * 10 + 4 = 34
//  h = 3
//  H = 3 * 3 + 4 = 13
//  gap : |[]9[][]9[][]9[]|
//
//
//  OPERATION
//

/*
 +-----------+-----------+-----------+
 |  9  9  9  |           |           |
 |  ~  ~  ~  |           |           |
 |           |           |           |
 +-----------+-----------+-----------+
 |           |           |           |
 |           |           |           |
 |           |           |           |
 +-----------+-----------+-----------+
 |           |           |           |
 |           |           |           |
 |           |           |           |
 +-----------+-----------+-----------+
 */

#ifndef soduku_h
#define soduku_h
#include <stdio.h>
#include <stdlib.h>

class GameBoard{
    private:
    int board[9][9];
    
    public:
    ///initialize the object
    GameBoard(){
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j] = 0;
            }
        }//init whth 0
        
        for (int i = 0; i < 9; i ++) {
            int pos = RandomNum(0, 81);
            int line = pos / 9 + 1;
            int row = pos % 9 + 1;
            printf("[the random coordinate is (%d, %d)]\n", line, row);
            board[line - 1][row - 1] = i + 1;
        }//init with random numbers
    }
    ///prinf the game board
    void PrintBoard(){
        for (int i = 0; i < 9; i++) {
            if ((i % 3) == 0) {
                printf("+-----------+-----------+-----------+\n");
                //print the grid additionly
            }
            for (int j = 0; j < 9; j++) {
                if ((j % 3) == 0) {
                    printf("|  ");
                }
                if (!board[i][j]){
                    printf("~  ");
                }
                else printf("%d  ", board[i][j]);
            }
            printf("|\n");
        }
        printf("+-----------+-----------+-----------+\n");
    }
    
    
    ///add number to game board
    bool addNum(int addnum, int position){
        if (position > 81 || position < 1) {
            printf("input error");
            return false;
        }
        int line = position / 9;
        int row = position % 9;
        if (this->board[line - 1][row - 1] != 0) {
            return false;
        }
        else board[line][row] = addnum;
        return true;
    }
    
    ///return a random number in the range of min and max
    int RandomNum(int roof, int ceiling){
        return roof + rand() % (ceiling - roof);
    }
};
#endif /* soduku_h */

