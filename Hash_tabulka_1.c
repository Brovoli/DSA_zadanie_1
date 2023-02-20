#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ZACIATOCNA_VELKOST 37

typedef struct prvok{
    char key[100];
    char data[100];
    int deleted;
}PRVOK, *P_PRVOK, **PP_PRVOK;

int hash_funkcia(char key[], int velkost, int iteracia);

P_PRVOK vytvor_prvok(char key[], char data[]){
    P_PRVOK temp;
    temp = (P_PRVOK)malloc(sizeof(PRVOK));
    strcpy(temp->key, key);
    strcpy(temp->data, data);
    temp->deleted=0;
    return temp;
}

void uloz_prvok (PP_PRVOK tabulka, P_PRVOK node, int velkost){   ///ulozi prvok do tabulky
    int hash, iteracia = 0;

    while(1){
    hash = hash_funkcia(node->key, velkost, iteracia);
    if (tabulka[hash] == NULL)
        {
            tabulka[hash] = node;
            return;
        }
    else if (tabulka[hash]->deleted == 1)
        {
            free(tabulka[hash]);
            tabulka[hash] = node;
            return;
        }
    else
        iteracia++;
    }
}

P_PRVOK search_tabulka (PP_PRVOK tabulka, char key[], int velkost){ ///prehladavam tabulku
    int iteracia = 0;
    int hash;
    while(1){
    hash = hash_funkcia(key, velkost, iteracia);
    if(tabulka[hash]==NULL)
        return NULL;
    if (strcmp(tabulka[hash]->key, key) == 0 && tabulka[hash]->deleted == 0)
        return tabulka[hash];
    else
        iteracia++;
    }
}

int hash_funkcia(char key[], int velkost, int iteracia){ ///vypluje hash kluca podla kluce a iteracie
    int sum = 0;
    int i;
    for(i=0;i<strlen(key);i++){
        sum = (sum * 33 + key[i] + iteracia) % velkost;
    }
    //printf("hash: %d\n", sum);
    return sum;   //sucet charakterov key * 33^pozicia % velkosť tabulky
}

int odstran_hodnotu (PP_PRVOK tabulka, char key[], int velkost, int *pocet){    ///odstrani prvok podla kluča
    P_PRVOK tempDestroy;
    tempDestroy = search_tabulka(tabulka, key, velkost);
    if(tempDestroy == NULL)
        return 0;
    (*pocet)--;
    tempDestroy->deleted = 1;
    return 1;
}

PP_PRVOK realokuj(PP_PRVOK tabulka, int *velkost, int *prvocisla){  ///realokuje na x2 + velkosť a prehashuje všetky hodnoty
    PP_PRVOK nova_tabulka;
    int nova_velkost, i;

    nova_velkost = 2 * (*velkost);
    while(prvocisla[nova_velkost]==1)
        nova_velkost++;

    nova_tabulka = (PP_PRVOK)malloc(sizeof(P_PRVOK) * nova_velkost);
    for(i=0; i<nova_velkost; i++)
        nova_tabulka[i]=NULL;

    for(i=0; i<(*velkost); i++)
    {
        if(tabulka[i]!=NULL && tabulka[i]->deleted == 0)
        {
            uloz_prvok(nova_tabulka, tabulka[i], nova_velkost);
        }
    }

    free(tabulka);

    *velkost = nova_velkost;
    return nova_tabulka;
}

void uvolni(PP_PRVOK tabulka, int velkost_tabulka){
    int i;
    for(i=0;i<velkost_tabulka;i++){
        free(tabulka[i]);
        tabulka[i]=NULL;
    }
    free(tabulka);
    return;
}

int main(){
    PP_PRVOK tabulka;
    P_PRVOK temp;
    int i, kontrola = 1, velkost_tabulka = ZACIATOCNA_VELKOST, pocet_ulozenych = 0;
    char action[10], kluc[100], data[100];
    tabulka = (PP_PRVOK)malloc(velkost_tabulka * sizeof(P_PRVOK));
    for(i=0;i<velkost_tabulka; i++)
        tabulka[i] = NULL;

    long int k,l;
    int prv[2000000] = { 0 };   ///vygenerujem zoznam prvočísel, pre velkosti tabulky

    for(k = 2; k < 2000000; k++) prv[k] = 0;

    for(k = 2; k < 2000000; k++)    //prechádzam čísla do 2 000 000
    {
    if (prv[k] == 0)    //0 znamená že to je prvočíslo
        for (l = k*k; l < 2000000; l+=k)    //začínajúc od druhej mocniny čísla, zapisujem násobky ako nie-prvočísla
            prv[l]=1;   //1 znamená kopozitné číslo
    }

    FILE *fp;
    if((fp = fopen("test.txt","r")) == NULL)
        {
        printf("nenašiel sa súbor");
        return 0;
        }
    clock_t begin;
    clock_t end;
    k = 1;
    while(k == 1)    ///testovanie zo súbpru test.txt
    {
        fscanf(fp,"%s",action);
        switch(action[0])
        {
        case('n'):
        pocet_ulozenych++;
        if(2 * pocet_ulozenych >= velkost_tabulka)
            tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        fscanf(fp,"%s %s",kluc,data);
        uloz_prvok(tabulka, vytvor_prvok(kluc, data), velkost_tabulka);
        break;

        case('d'):
        fscanf(fp,"%s", kluc);
        odstran_hodnotu(tabulka, kluc, velkost_tabulka, &pocet_ulozenych);
        break;

        case('s'):
        fscanf(fp,"%s", kluc);
        temp = search_tabulka(tabulka,kluc,velkost_tabulka);
        if(temp==NULL)
            printf("Nenašiel\n");
        else printf("%s\n", temp->data);
        break;

        case('t'):
        begin = clock();
        break;

        case('c'):
        end = clock();
        double time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
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
        uvolni(tabulka, velkost_tabulka);
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
    fscanf(fp,"%s %s", kluc, data);
    if(feof(fp)==1)
        break;
    if(odstran_hodnotu(tabulka, kluc, velkost_tabulka, &pocet_ulozenych)==0)
        printf("nastala chyba %s %d\n",kluc, kontrola);
    }
    for(i=0;i<velkost_tabulka;i++)
    {
        if(tabulka[i]!=NULL && tabulka[i]->deleted == 0)
            printf("nastala chyba %s %s",tabulka[i]->key, tabulka[i]->data);
    }
    printf("prebehla kontrola udajov\n");

    fclose(fp);
    fp=NULL;
    uvolni(tabulka, velkost_tabulka);

    return 1;

    ///časovač
    /*clock_t begin = clock();
    clock_t end = clock();
    double time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
    printf("cas: %f\n", time_spent);*/


    /*printf("n - novy prvok\nd - odstráň prvok\ns - search\nr - manually reallocate\np - print out\ne - exit\n");
    while(1)    ///manualne testovanie
    {
        scanf("%s",action);
        switch(action[0])
        {
        case('n'):
        pocet_ulozenych++;
        if(2 * pocet_ulozenych >= velkost_tabulka)
            tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        printf("key , data: ");
        scanf("%s %s",kluc,data);
        uloz_prvok(tabulka, vytvor_prvok(kluc, data), velkost_tabulka);
        break;

        case('d'):  ///needs done
        printf("key: ");
        scanf("%s", kluc);
        odstran_hodnotu(tabulka, kluc, velkost_tabulka, &pocet_ulozenych);
        break;

        case('s'):
        printf("key: ");
        scanf("%s", kluc);
        temp = search_tabulka(tabulka,kluc,velkost_tabulka);
        if(temp==NULL)
            printf("\nNenašiel\n");
        else printf("%s", temp->data);
        break;

        case('r'):
        tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        printf("\nova velkost: %d", velkost_tabulka);
        break;

        case('p'):
        for(i=0; i<velkost_tabulka; i++)
        {
            if(tabulka[i]!=NULL && tabulka[i]->deleted==0)
                printf("%d\t%s\t%s\n",i, tabulka[i]->key, tabulka[i]->data);
        }
        break;

        case('e'):
        uvolni(tabulka, velkost_tabulka);
        return 0;
        }
        printf("\n------------------\n");
    }*/
}
