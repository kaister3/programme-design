//
//  DPLL.h
//  Sudoku
//
//  Created by Kaister Wang on 2019/3/13.
//  Copyright © 2019 com.KaisterWang. All rights reserved.
//
//  CNF 文件用标记行“p cnf A B”来区分注释行和子句行，
//  即这行之后的每一行都表示一个布尔子句，所有的子句用 AND 连接，
//  组成一个完整的布尔表达式。这里的 A 表示变量的个数，B 表示子句的个数。
//  clsNum clause number
//  varNum varible number

#ifndef DPLL_h
#define DPLL_h
#include <stdio.h>
#include <stdlib.h>
#define PATH "./data/n.cnf"
#define RES_PATH "./data/n.res"


//some data structers
typedef struct Node {
    int data;
    struct Node *nextNode;
}Node;

typedef struct List {
    struct List *nextList;
    Node *head;
}List;

//functions
List *loadFile(int fileNum, int *varNum, int *clsNum){
    FILE *fp;
    int start_read = 0, tmp = 0, over = 0;
    char path[] = PATH;
    List *L = (List*)malloc(sizeof(List));//
    List *lp = L;
    List *lp_pre = NULL;
    Node *np = NULL;
    path[7] = fileNum + 48;
    fp = fopen(path, "r");
    if (!fp) {
        printf("cannot open file!\n");
        exit(-1);
    }
    int i = 1;
    while (true) {
        i++;
        if (over) {
            break;
        }
        if (!start_read) {
            char ch;//the char read next
            fscanf(fp, "%c", &ch);
            if (ch == 'p') {//ready to read cnf
                start_read = 1;
                for (int i = 0; i < 4; i++) {
                    //read & skip "p cnf"
                    fgetc(fp);
                }
                fscanf(fp, "%d %d", varNum, clsNum);
                //get the amount of var &clause
            }else if(ch == 'c'){//which means comment lines
                while (true) {
                    char c = fgetc(fp);
                    if (c == '\n') {
                        break;//next line
                    }
                }
            }else printf("This is not a standard cnf file.\n");
        }else {//begin reading cnf sentence
            int num = 0, status = 1;
            lp->head = (Node*)malloc(sizeof(Node));
            np = lp->head;
            while (true) {
                status = fscanf(fp, "%d", &tmp);
                //read a number & store it in tmp
                if (status != 1) {
                    //which means no number left
                    over = 1;
                    break;
                }
                if (num == 0) {
                    np->data = tmp;
                    //store the number in node
                    np->nextNode = NULL;
                    num++;
                    continue;
                }
                if (tmp != 0) {
                    np->nextNode = (Node*)malloc(sizeof(Node));
                    np->nextNode->data = tmp;
                    np->nextNode->nextNode = NULL;
                    np = np->nextNode;
                    num++;
                } else {
                    //tmp == 0, another line
                    //lp's length = num
                    lp->nextList = (List*)malloc(sizeof(List));
                    lp_pre= lp;//shang yi hang
                    lp = lp->nextList;
                    lp->nextList = NULL;
                    break;
                }
            }
        }
    }
    lp_pre->nextList = NULL;//free the last line
    fclose(fp);
    printf("file loaded complete : var : %d clause : %d\n", *varNum, *clsNum);
    return L;
}
/*
void printList(List *L){
    printf("==========\n");
    List *lp = L;
    int i = 0;
    Node *np = NULL;
    while(lp){
        np = lp->head;
        while (np) {
            printf("%3d", np->data);
            np = np->nextNode;//go on
        }
        printf("\n");
        lp = lp->nextList;
        i++;
    }
}
*/
List* copySubL(List *L){//1 dimension
    //return a copy of L
    List *L_copy = (List*)malloc(sizeof(List));
    L_copy->nextList = NULL;
    if (L->head == NULL) {
        //L is empty
        L_copy->head = NULL;
        return L_copy;
    }
    L_copy->head = (Node*)malloc(sizeof(Node));
    Node *np = L->head;
    Node *np_copy = L_copy->head;
    while (np) {
        np_copy->data = np->data;
        if (np->nextNode == NULL) {
            np_copy->nextNode = NULL;
            break;
        }
        np_copy->nextNode = (Node*)malloc(sizeof(Node));
        np_copy = np_copy->nextNode;
        np = np->nextNode;
    }
    return L_copy;
}

List* copyL(List *L){//copy list head
    List* L_copy = copySubL(L);
    List *lp = L->nextList;
    List *lp_copy = L_copy;
    while (lp) {
        //on every sublist's head
        lp_copy->nextList = copySubL(lp);
        //copy every sublist
        if (lp->nextList == NULL) {
            break;
        }
        lp_copy = lp_copy->nextList;
        lp = lp->nextList;
    }
    return L_copy;
}

void outputResult(int ans[], int varNum){
    for (int i = 0; i < varNum; i++) {
        printf("[%d:", i+1);
        if (ans[i] == 0) {
            printf("?]");
        }
        else printf("%d]", ans[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");//shi ci huan yi xia hang
        }
    }
}

void addVar(int var, List **L){
    //add a var on the top of the list
    List *lp = (List*)malloc(sizeof(List));
    lp->head = (Node*)malloc(sizeof(Node));
    lp->head->data = var;
    lp->head->nextNode = NULL;
    lp->nextList = *L;
    *L = lp;
}

int findMaxVar(List *L, int varNum){
    //find next most frquently variable
    int tmp = 0;
    int maxfrequency = 0;
    int posnum = 0;
    int negnum = 0;
    for (int i = 0; i < varNum; i++) {
        posnum = 0;
        negnum = 0;
        List *lp = L;
        Node *np = NULL;
        while (lp) {
            np = lp->head;
            while (np) {
                if (np->data == (i+1)) {
                    posnum++;
                }
                else if (np->data == -(i+1)){
                    negnum++;
                }
                np = np->nextNode;
            }
            lp = lp->nextList;
        }
        if (posnum + negnum > maxfrequency) {
            maxfrequency = posnum + negnum;
            if (posnum > negnum) {
                tmp = i + 1;
            }
            else tmp = - i - 1;
        }
    }
    return tmp;
}

//  return -1 : 0
//
//
int removeVar(List **L, int var){
    //remove clause with a var occured in a single cluse
    List *lp = *L;
    List *lp_pre = *L;
    while (lp) {//cycle of line
        if (lp->head == NULL) {
            return -1;
        }
        int remove_clause = 0;
        Node *np = lp->head;
        Node *np_pre = lp->head;
        while (np) {
            if (np->data == -var) {
                //remove -v
                if (np == lp->head) {
                    //which means np is the first node
                    lp->head = lp->head->nextNode;
                    free(np);
                    np = lp->head;
                    if (np == NULL) {
                        return -1;
                        //clause = 0, no zhenzhi
                    }
                } else {
                    np_pre->nextNode = np->nextNode;
                    free(np);
                    np = np_pre->nextNode;
                }
                continue;
            }else if (np->data == var){
                Node *head = lp->head;
                if (lp == *L) {
                    //shi di yi ge zi ju
                    *L = (*L)->nextList;
                    free(lp);
                    lp = (*L);
                    if(*L == NULL){
                        return 1;//dedaozhenzhi
                    }
                }else{
                    lp_pre->nextList = lp->nextList;
                    free(lp);
                    lp = lp_pre->nextList;
                }
                Node *bak = NULL;
                while (head) {
                    //free every node
                    bak = head;
                    head = head->nextNode;
                    free(bak);
                }
                remove_clause = 1;
                break;
            }
            np_pre = np;
            np = np->nextNode;
        }//end of while(np)
        if (remove_clause == 0) {
            //if didn't remove clause go on
            lp_pre = lp;
            lp = lp->nextList;
        }
    }//end of while(lp)
    return 0;
}

void freeList(List *L){
    List *lp = L;
    List *lp_pre = NULL;
    Node *np = NULL;
    while (lp) {
        Node *bak = NULL;
        np = lp->head;
        while (np) {
            bak = np;
            np = np->nextNode;
            free(bak);
        }
        lp_pre = lp;
        lp = lp->nextList;
    }
}

int dpll(List *L, int varNum, int ans[]){
    List * new_L = copyL(L);
    //printf("Test\n");
    //printList(new_L);
    //find single clause
    int i = 0;
    while (true) {
        List *lp = new_L;
        Node *np = NULL;
        int have_signle = 0;
        while (lp) {
            np = lp->head;
            while (np) {
                i++;
                if (lp->head && !lp->head->nextNode) {
                    //lp->head exists, ->next->head dont exists
                    //means is single clause
                    printf("赋值 : %d\n", np->data);
                    int value = (np->data) > 0 ? 1 : -1;
                    ans[abs(np->data) - 1] = value;
                    have_signle = 1;
                    //
                    //printf("%d\n", i);
                    int ret = removeVar(&new_L, np->data);
                    //error occurred here
                    //soloved
                    //printf("Test\n");
                    if (ret != 0) {
                        //dedaozhenzhi huo meiyouzhenzhi
                        return ret;
                    }
                    break;
                }else if(lp->head == NULL){
                    freeList(new_L);
                    return -1;
                }
                np = np->nextNode;
            }//end of cycle row
            if (have_signle) {
                //bu yong tiao
                break;
            }
            lp = lp->nextList;
        }//end of cycle line
        if (have_signle == 0) {
            break;
        }
    }
    if (new_L == NULL) {
        return 1;
    }
    int nextVar = findMaxVar(new_L, varNum);
    ans[abs(nextVar) - 1] = nextVar > 0 ? 1 : -1;
    addVar(nextVar, &new_L);
    if(dpll(new_L, varNum, ans) == 1) {
        //if empty means qiu jie wan bi
        freeList(new_L);
        return 1;
    } else {
        List *bak = new_L;
        new_L = new_L->nextList;
        free(bak->head);
        free(bak);//delete di yi hang
        addVar(-nextVar, &new_L);
        return dpll(new_L, varNum, ans);
    }
}

void save2file(int ans[], int varNum, int ret, double cost, int filename) {
    char path[] = RES_PATH;
    path[7] = filename+ 48;
    FILE *fp = fopen(path, "wt");
    fprintf(fp, "%d\n", ret);
    if (ret == 1) {
        for (int i = 0; i < varNum; i++) {
            if (ans[i] == 1)
            fprintf(fp, "%d ", i + 1);
            else
            fprintf(fp, "-%d ", i + 1);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "%lf", cost * 1000);
}
#endif /* DPLL_h */
