//
//  main.c
//  Sudoku
//
//  Created by Kaister Wang on 2019/3/12.
//  Copyright © 2019 com.KaisterWang. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include "soduku.h"
#include "DPLL.h"

void mainMenu(){
    system("clear");
    printf("1. Solve SAT\n");
    printf("2. Play soduku\n");
    printf("3. Exit\n\n");
    printf("Enter your option: ");
}

void satMenu(){
    printf("1. read from cnf file\n");
    printf("2. Exit to main menu\n\n");
    printf("Enter your option: ");
}

void sudokuMenu() {
    printf("1. Game Init\n");
    printf("2. Solve Soduku(no)\n");
    printf("4. Exit to main menu\n\n");
    printf("Enter your option: ");
}


int main(int argc, const char * argv[]) {
    //test();
    clock_t start;
    clock_t stop;
    int option;
    
    //List *L = (List*)malloc(sizeof(List));
    int quit = 0;
    while (true) {
        mainMenu();
        if (scanf("%d", &option) != 1) {
            printf("Input format error!\n");
            return -1;
        }
        switch(option){
            //enter sat menu
            case 1:{
                quit = 0;
                satMenu();
                while (true) {
                    if (quit) {
                        break;
                    }
                    if (scanf("%d", &option)!=1) {
                        printf("Input format error!\n");
                        return -1;
                    }
                    switch (option) {
                        case 1:{
                            printf("Input the filename : ");
                            int filename;
                            if (scanf("%d", &filename) != 1) {
                                printf("file name error");
                                break;
                            }
                            int varNum = 0;
                            int clauseNum = 0;
                            List *L = loadFile(filename, &varNum, &clauseNum);
                            int *ans = (int*)malloc(sizeof(int) * varNum);
                            //to store answer & initial
                            for (int i = 0; i < varNum; i++) {
                                ans[i] = 0;
                            }
                            //printf("Test\n");
                            start = clock();
                            //printList(L);
                            int ret = dpll(L, varNum, ans);
                            //zhe li yao ba zhi zhen chuan guo qu
                            if (ret == 1) {
                                printf("\nFind solution\n\n");
                                outputResult(ans, varNum);
                            }else printf("\nNo solution\n\n");
                            stop = clock();
                            double cost = (double)(stop - start) / CLOCKS_PER_SEC;
                            printf("Time cost: %lf ms (%lf s)", cost*1000, cost);
                            save2file(ans, varNum, ret, cost, filename);
                            getchar();
                            getchar();
                            system("clear");
                            satMenu();
                            //return 0;
                            break;
                        }
                        case 2:{
                            quit = 1;
                            break;
                        }
                        default:{
                            break;
                        }
                    }
                }
                break;
            }//end of case 1
            case 2:{
                //enter sudoku menu
                sudokuMenu();
                quit = 0;
                while (1) {
                    if (quit)
                    break;
                    if (scanf("%d", &option) != 1) {
                        printf("Input format error!\n");
                        return -1;
                    }
                    switch (option) {
                        case 1:{
                            printf("Game start\n");
                            GameBoard s = *new GameBoard();
                            s.PrintBoard();
                            break;
                        }
                        case 2:{/*
                            //solve sudoku
                            printf("you can slove it by yourself(笑)\n");
                            int position, value;
                            printf("Input position and value");
                            while (scanf("%d %d", &position, &value) == 2) {
                                s.addNum(value, position);
                            }*/
                        }
                        default:{
                            quit = 1;
                            break;
                        }
                    }
                }
                break;

            }//end of case 2
            default:{
                return 0;
            }//end of case default
        }
    }
    return 0;
}

void test(){
    GameBoard s = *new GameBoard();
    s.PrintBoard();
}
