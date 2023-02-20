#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ZACIATOCNA_VELKOST 17

typedef struct prvok{
    char kluc[100];
    char data[100];
    struct prvok *dalsi;
}PRVOK, *P_PRVOK, **PP_PRVOK;

void uvolni(PP_PRVOK tabulka, int velkost); ///uvolni tabulku a všetky prvky v nej
void uvolni_zoznam(P_PRVOK node);
int hash_funkcia (char key[], int velkost); ///hash kľúču podla veľkosti tabulky
P_PRVOK odstran_hodnotu (PP_PRVOK tabulka, char key[], int velkost, int *pocet);    ///odstráni prvok podla klúču z tabulky
void vypis_spajany (P_PRVOK node);  ///vypíše spájaný zoznam od posledného vloženého prvku
void uloz_prvok (PP_PRVOK tabulka, P_PRVOK node, int velkost);  ///uloží prvok do tabulky
P_PRVOK getPrvok(P_PRVOK node); ///vráti posledný prvok spájaného zoznamu
PP_PRVOK realokuj(PP_PRVOK tabulka, int *velkost, int *prvocisla);  ///realokuje celu tabulku na viac ako dvojnásobnú velkosť a prehashuje všetky hodnoty
P_PRVOK vytvorPrvok (char kluc[], char data[]); ///vytvorí prvok s klúčom a dátami
P_PRVOK search_zoznam(P_PRVOK node, char key[]);
P_PRVOK search_tabulka (PP_PRVOK tabulka, char key[], int velkost); ///vyhladá prvok v tabulke
P_PRVOK najdi_prechodcu (P_PRVOK prvy, P_PRVOK hladany);

int main()
{
    int i, velkost_tabulka = ZACIATOCNA_VELKOST, pocet_ulozenych=0, kontrola = 1;
    char kluc[100], data[100], action[10];
    P_PRVOK temp;

    PP_PRVOK tabulka;
    tabulka = (PP_PRVOK)malloc(sizeof(P_PRVOK) * ZACIATOCNA_VELKOST);
    for(i=0; i<velkost_tabulka; i++)
        tabulka[i]=NULL;

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
    fp = fopen("test.txt","r");
    clock_t begin;
    clock_t end;
    k=1;
    while(k==1)    ///testovanie zo súbpru test.txt
    {
        fscanf(fp,"%s",action);
        switch(action[0])
        {
        case('n'):
        pocet_ulozenych++;
        if(pocet_ulozenych >= velkost_tabulka)
            tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        fscanf(fp,"%s %s",kluc,data);
        uloz_prvok(tabulka, vytvorPrvok(kluc, data), velkost_tabulka);
        break;

        case('d'):
        fscanf(fp,"%s", kluc);
        temp = odstran_hodnotu(tabulka, kluc, velkost_tabulka, &pocet_ulozenych);
        if(temp != NULL)
            tabulka[hash_funkcia(kluc, velkost_tabulka)] = temp;
        break;

        case('s'):
        fscanf(fp,"%s", kluc);
        temp = search_tabulka(tabulka,kluc,velkost_tabulka);
        if(temp==NULL)
            printf("Nenašiel\n");
        else printf("%s\n", temp->data);
        break;

        /*case('r'):
        tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        printf("\nova velkost: %d", velkost_tabulka);
        break;

        case('p'):
        for(i=0; i<velkost_tabulka; i++)
        {
            if(tabulka[i]!=NULL) printf("%d",i);
            vypis_spajany(tabulka[i]);
        }
        break;*/
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
            k=0;
            break;
        }
        fclose(fp);
        fp=NULL;
        uvolni(tabulka, velkost_tabulka);
        return 0;
        }
    }

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
    if(search_tabulka(tabulka,kluc,velkost_tabulka)==NULL)
        printf("nastala chyba");
    temp = odstran_hodnotu(tabulka,kluc,velkost_tabulka,&pocet_ulozenych);
    if(temp != NULL)
        tabulka[hash_funkcia(kluc, velkost_tabulka)] = temp;
    }
    for(i=0;i<velkost_tabulka;i++)
        if(tabulka[i]!=NULL)
            printf("nastala chyba");

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
    while(1)    ///manuálne testovanie
    {
        scanf("%s",action);
        switch(action[0])
        {
        case('n'):
        pocet_ulozenych++;
        if(pocet_ulozenych >= velkost_tabulka)
            tabulka = realokuj(tabulka, &velkost_tabulka, prv);
        printf("key , data: ");
        scanf("%s %s",kluc,data);
        uloz_prvok(tabulka, vytvorPrvok(kluc, data), velkost_tabulka);
        break;

        case('d'):
        printf("key: ");
        scanf("%s", kluc);
        temp = odstran_hodnotu(tabulka, kluc, velkost_tabulka, &pocet_ulozenych);
        if(temp != NULL)
            tabulka[hash_funkcia(kluc, velkost_tabulka)] = temp;
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
            if(tabulka[i]!=NULL) printf("%d",i);
            vypis_spajany(tabulka[i]);
        }
        break;

        case('e'):
        uvolni(tabulka, velkost_tabulka);
        return 0;
        }
        printf("\n------------------\n");
    }*/
}

void spajany_do_array(P_PRVOK source, PP_PRVOK dest, int iteracia){
    if(source == NULL)
        return;
    dest[iteracia]=source;
    iteracia++;
    spajany_do_array(source->dalsi, dest, iteracia);
    dest[iteracia - 1]->dalsi = NULL;
    return;
}

PP_PRVOK realokuj(PP_PRVOK tabulka, int *velkost, int *prvocisla){  ///realokuje na x2 + velkosť a prehashuje všetky hodnoty
    PP_PRVOK nova_tabulka;
    P_PRVOK temp, temp2;
    int nova_velkost, i, j;

    nova_velkost = 2 * (*velkost);
    while(prvocisla[nova_velkost]==1)
        nova_velkost++;
    nova_tabulka = (PP_PRVOK)malloc(sizeof(P_PRVOK) * nova_velkost);
    for(i=0; i<nova_velkost; i++)
        nova_tabulka[i]=NULL;

    PP_PRVOK temp_tabulka;
    temp_tabulka = (PP_PRVOK)malloc(sizeof(P_PRVOK) * *velkost);

    for(i=0; i<(*velkost); i++)
    {
        j=0;
        spajany_do_array(tabulka[i], temp_tabulka, 0);
        while(temp_tabulka[j]!=NULL)
        {
            uloz_prvok(nova_tabulka,temp_tabulka[j],nova_velkost);
            temp_tabulka[j] = NULL;
            j++;
        }

    }

    free(temp_tabulka);
    free(tabulka);

    *velkost = nova_velkost;
    return nova_tabulka;
}

P_PRVOK vytvorPrvok (char kluc[], char data[]){   ///vrati prvok do tabulky
    P_PRVOK temp;
    temp = (P_PRVOK)malloc(sizeof(PRVOK));
    strcpy(temp->kluc,kluc);
    strcpy(temp->data,data);
    temp->dalsi = NULL;
    return temp;
}

P_PRVOK search_zoznam(P_PRVOK node, char key[]){    ///prehladavam spajany zoznam
    if(node == NULL)
        return NULL;
    else if (strcmp(node->kluc, key)==0)
        return node;
    else
        return search_zoznam(node->dalsi, key);
}
P_PRVOK search_tabulka (PP_PRVOK tabulka, char key[], int velkost){ ///prehladavam tabulku
    int hash = hash_funkcia(key, velkost);
    if(tabulka[hash]==NULL)
        return NULL;
    if (strcmp(tabulka[hash]->kluc, key)==0)
        return tabulka[hash];
    else
        return search_zoznam(tabulka[hash]->dalsi, key);
}

P_PRVOK getPrvok(P_PRVOK node){ ///vráti posledný prvok v spájanom zozname (NULL ak je prázdny)
    if(node == NULL)
        return NULL;
    if(node->dalsi == NULL)
        return node;
    if(node->dalsi != NULL)
        return getPrvok(node->dalsi);
    return NULL;
}

void uloz_prvok (PP_PRVOK tabulka, P_PRVOK node, int velkost){   ///ulozi prvok do tabulky
    int hash = hash_funkcia(node->kluc, velkost);
    P_PRVOK temp;
    if (tabulka[hash] == NULL)
        tabulka[hash] = node;
    else
        {
        temp = tabulka[hash];
        tabulka[hash] = node;
        node->dalsi = temp;
        }
}

void uvolni_zoznam(P_PRVOK node){
    if(node == NULL)
        return;
    uvolni_zoznam(node->dalsi);
    free(node);
    return;
}
void uvolni(PP_PRVOK tabulka, int velkost){ ///uvolní všetky hodnoty v danej tabulke
    int i;
    for(i=0;i<velkost; i++)
        uvolni_zoznam(tabulka[i]);
    free(tabulka);
    tabulka = NULL;
    return;
}

int hash_funkcia (char key[], int velkost){ ///vypluje hash kluca
    int sum = 0;
    int i;
    for(i=0;i<strlen(key);i++){
        sum = (sum * 33 + key[i]) % velkost;
    }
    //printf("hash: %d\n",sum);
    return sum;   //sucet charakterov key-u % velkosť tabulky
}

P_PRVOK najdi_prechodcu (P_PRVOK prvy, P_PRVOK hladany){    ///nájde predchádzajúci prvok v spájanom zozname v tabulke
    if(prvy==hladany)
        return NULL;
    if(prvy->dalsi == hladany)
        return prvy;
    else
        return najdi_prechodcu(prvy->dalsi, hladany);
}

P_PRVOK odstran_hodnotu (PP_PRVOK tabulka, char key[], int velkost, int *pocet){    ///odstrani prvok podla kluča
    P_PRVOK tempDestroy;
    tempDestroy = search_tabulka(tabulka, key, velkost);
    int i = hash_funkcia(key, velkost);
    if(tempDestroy == NULL)
        return NULL;

    (*pocet)--;
    if (tabulka[i] == tempDestroy)
    {
        tabulka[i]=tempDestroy->dalsi;
        free(tempDestroy);
        tempDestroy=NULL;
        return NULL;
    }
    else
    {
        najdi_prechodcu(tabulka[i],tempDestroy)->dalsi = tempDestroy->dalsi;
        free(tempDestroy);
        tempDestroy = NULL;
        return NULL;
    }
}

void vypis_spajany (P_PRVOK node){  ///vypíše spájaný zoznam po od posledného vloženého
    if(node == NULL)
        return;
    else
    vypis_spajany(node->dalsi);
    printf("\t%s\t%s\n",node->kluc, node->data);
    return;
}
