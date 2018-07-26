#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <malloc.h>
#include <conio.h>
#define maxsize 5

using namespace std;

//对树节点的定义
typedef struct Bnode {
    char data[2];
    struct Bnode *Lchild, *Rchild, *father;
} Btnode, *Btptr;

string epr[maxsize];//储存输入的表达式
bool abc[26];//储存对表达式中字母的统计
int num, numf;

//简单的自定义报错函数
void Error(string s) {
    cout << s << " is wrong!" << endl;
}

//输入表达式
void Input(int numf) {
    if (numf > 0 && numf <= maxsize) {
        for (int i = 0; i < numf; i++) {
            cin >> epr[i];
        }
    }
    else Error("numf");
}

//将输入的表达式用树的形式储存起来
Btptr CreateBtree(string str) {
    Btptr root = (Btptr)malloc(sizeof(Btnode));
    Btptr p = root, q = root;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '(') {
            if (p -> Lchild) {
                p -> Rchild = (Btptr)malloc(sizeof(Btnode));
                q = p;
                p = p -> Rchild;
                p -> father = q;
            }
            else {
                p -> Lchild = (Btptr)malloc(sizeof(Btnode));
                q = p;
                p = p -> Lchild;
                p -> father = q;
            }
        }
        else if (str[i] == ')') {
            p = p -> father;
        }
        else if (str[i] >= 'a' && str[i] <= 'z') {
            if (p -> Lchild) {
                p -> Rchild = (Btptr)malloc(sizeof(Btnode));
                q = p;
                p =p -> Rchild;
                p -> father = q;
                p -> data[0] = str[i];
                p = p -> father;
            }
            else {
                p -> data[0] = str[i];
                p = p -> father;
            }
        }
        else if(str[i] == '~') {
            p -> data[1] = str[i];
        }
        else {
            p -> data[0] = str[i];
            if (p == root) {
                p -> Rchild = (Btptr)malloc(sizeof(Btnode));
                q = p;
                p =p -> Rchild;
                p -> father = q;
            }
        }
    }
    return root;
}

//对表达式中的字母进行统计
void preprea() {
    num = 0;
    int len = 0, temp;
    for (int i = 0; i < numf; i ++) {
        for (int j = 0; j < epr[i].length(); j++) {
            if (epr[i][j] >= 'a' && epr[i][j] <= 'z') {
                temp = int(epr[i][j]) - 97;
                if (abc[temp] == false) {
                    abc[temp] = true;
                    num ++;
                }
            }
        }
    }
}

int main() {
    cout << "请输入条件表达式的个数:" << endl;
    cin >> numf;
    Input(numf);
}