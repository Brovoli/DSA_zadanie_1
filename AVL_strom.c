#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node{
    int num;
    char data[100];
    struct node *Lnode;
    struct node *Rnode;
    int h;
} NODE, *P_NODE, **PP_NODE;

int max(int i, int j){
    if(i>j)
        return i;
    else
        return j;
}

int height(P_NODE temp){    //vráti int výška pre daný node
    if(temp==NULL)
        return 0;
    return 1 + max(height(temp->Rnode), height(temp->Lnode));
}

P_NODE P_rotacia (P_NODE vrch){ //rotuje strom a vráti pointer na vrchný node
    P_NODE n_vrch = vrch->Lnode;
    P_NODE temp = n_vrch->Rnode;
    n_vrch->Rnode = vrch;
    vrch->Lnode = temp;

    vrch->h = height(vrch); //update výšok
    n_vrch->h = height(n_vrch);

    return n_vrch;
}

P_NODE L_rotacia (P_NODE vrch){
    P_NODE n_vrch = vrch->Rnode;
    P_NODE temp = n_vrch->Lnode;

    n_vrch->Lnode = vrch;
    vrch->Rnode = temp;

    vrch->h = height(vrch);
    n_vrch->h = height(n_vrch);
    return n_vrch;
}

P_NODE vytvor_node(int num, char data[]){
    P_NODE temp;
    temp = (P_NODE)malloc(sizeof(NODE));
    temp->num = num;
    strcpy(temp->data, data);
    temp->Lnode = NULL;
    temp->Rnode = NULL;
    temp->h = 0;
    return temp;
}

P_NODE insert_node(P_NODE vrch, int num, char data[]){
    if(vrch == NULL)
        {
        vrch = vytvor_node(num, data);
        return vrch;
        }

    if(num < vrch->num)
        vrch->Lnode = insert_node(vrch->Lnode, num, data);
    else //if(vrch->num < num)
        vrch->Rnode = insert_node(vrch->Rnode, num, data);
    /*else
        return vrch;*/

    vrch->h = height(vrch);
    int bal = height(vrch->Lnode) - height(vrch->Rnode);    //bal = vyváženie stromu, ak bal > 1 rotujem
    //printf("bal: %d\n", bal);

    if(bal > 1 && num < vrch->Lnode->num)   //lava lava
        return P_rotacia(vrch);
    if(bal < -1 && num > vrch->Rnode->num)  //pravá pravá
        return L_rotacia(vrch);
    if(bal > 1 && num > vrch->Lnode->num)   //zig zag - lavá pravá
    {
        vrch->Lnode = L_rotacia(vrch->Lnode);
        return P_rotacia(vrch);
    }
    if(bal < -1 && num < vrch->Rnode->num)  //zig zag - pravá lavá
    {
        vrch->Rnode = P_rotacia(vrch->Rnode);
        return L_rotacia(vrch);
    }

    return vrch;    //bez rotácie
}

void vypis(P_NODE vrch){    //pre-order výpis pre debug
    if(vrch!=NULL)
    {
        printf("%d %s\n", vrch->num, vrch->data);
        vypis(vrch->Lnode);
        vypis(vrch->Rnode);
    }
}

P_NODE search(P_NODE vrch, int num){
    if(vrch == NULL)
        return NULL;
    if(num == vrch->num)
        return vrch;
    if(num < vrch->num)
        return search(vrch->Lnode, num);
    if(num > vrch->num)
        return search(vrch->Rnode, num);
    return NULL;
}

P_NODE naj_lavy(P_NODE vrch){
    P_NODE temp;
    if(vrch == NULL)
        return NULL;
    if(vrch->Lnode == NULL)
        {
        return vrch;
        }
    if(vrch->Lnode->Lnode == NULL)
    {
        temp = vrch->Lnode;
        vrch->Lnode=temp->Rnode;
        return temp;
    }
    else
        return(naj_lavy(vrch->Lnode));
}

P_NODE destroy(P_NODE vrch, int num){
    P_NODE tempL, tempP;
    if(vrch==NULL)
        return NULL;

    if(vrch->num == num)
    {
        tempL = vrch->Lnode;
        tempP = vrch->Rnode;
        free(vrch);
        vrch = NULL;

        if(tempL == NULL)
            return tempP;
        if(tempP == NULL)
            return tempL;
        else
        {
        vrch = naj_lavy(tempP);
        vrch->Lnode = tempL;
        if(tempP != vrch)
            vrch->Rnode = tempP;
        return vrch;
        }
    }
    else if(num < vrch->num)
    {
        vrch->Lnode = destroy(vrch->Lnode, num);
    }
    else if(num > vrch->num)
    {
        vrch->Rnode = destroy(vrch->Rnode, num);
    }

    return vrch;
}

void uvolni(P_NODE vrch){
    if(vrch == NULL)
        return;
    uvolni(vrch->Lnode);
    uvolni(vrch->Rnode);
    free(vrch);
    return;
}

int main(){
    P_NODE vrch=NULL, temp;
    int num, k;
    char data[100], action[10];
    int kontrola = 1;

    FILE *fp;
    if((fp = fopen("test.txt","r")) == NULL)
        {
        printf("nenašiel sa súbor");
        return 0;
        }
    clock_t begin;
    clock_t end;
    double time_spent;
    k = 1;
    while(k == 1)    ///testovanie zo súboru test.txt
    {
        fscanf(fp,"%s",action);
        switch(action[0])
        {
        case('n'):
        fscanf(fp,"%d %s",&num,data);
        vrch = insert_node(vrch, num, data);
        break;

        case('d'):
        fscanf(fp,"%d", &num);
        vrch = destroy(vrch, num);
        break;

        case('s'):
        fscanf(fp,"%d", &num);
        temp = search(vrch, num);
        if(temp == NULL)
            printf("Nenašiel\n");
        else
            printf("%s\n", temp->data);
        break;

        case('t'):
        begin = clock();
        break;

        case('c'):
        end = clock();
        time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
        printf("cas: %f\n", time_spent);
        break;

        case('e'):
        if(kontrola == 1)
            {
            k = 0;
            break;
            }
        fclose(fp);
        fp=NULL;
        uvolni(vrch);
        return 0;
        }
    }

    fclose(fp);
    fp = NULL;

    fp = fopen("vysledky.txt","r");
    if(fp == NULL)
        return 0;

    kontrola = 0;
    while(feof(fp)==0)
    {
    kontrola++;
    fscanf(fp,"%d %s", &num, data);
    if(feof(fp)==1)
        break;
    vrch = destroy(vrch, num);
    }
    if(vrch==NULL)
        printf("nastala chyba %d", vrch->num);

    printf("prebehla kontrola udajov\n");

    fclose(fp);
    fp=NULL;
    uvolni(vrch);

    return 1;
}
