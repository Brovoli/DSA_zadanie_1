#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX 200000

int main(){
    int i,r;
    int insert, odstran, hladaj;
    int temp;
    FILE *fp;
    fp=fopen("test.txt","w");
    int num[MAX];

    printf("Parametre na generovanie\npocet prvkov:\npocet odstraneni:\npocet hladani:\n");
    scanf("%d %d %d", &insert, &odstran, &hladaj);

    srand(time(NULL));

    for(i=0;i<MAX;i++)
        num[i]=i + 1;

    fprintf(fp,"t\n");
    for(i=0;i<insert;i++)
        {
        r = rand() % MAX;
        if(num[r]!=0)
            {
            temp = num[r];
            num[r] = 0;
            }
        else
            {
            i--;
            continue;
            }
        fprintf(fp,"n %d %d\n", temp, temp % 33);
        }
    fprintf(fp,"c\n");

    if(odstran>0)
        fprintf(fp,"t\n");
    for(i=0;i<odstran;i++)
    {
        r = rand() % MAX;         ///odstráni iba prvky čo tam su↓
        if(num[r]==0)
            {
            temp = r + 1;
            num[r] = r + 1;
            }
        else
            {
            i--;
            continue;
            }                    ///

        /*r = rand() % MAX + 1;       ///odstráni aj prvky čo tam nie sú↓
        temp = r;                   ///
        num[r-1] = r;*/

        fprintf(fp,"d %d\n", temp);
        }
    if(odstran>0)
        fprintf(fp,"c\n");

    if(hladaj>0)
        fprintf(fp,"t\n");
    for(i=0; i<hladaj; i++)
    {
        r = rand() % MAX;         ///vyhlada iba prvky čo tam su↓
        if(num[r]==0)
            {
            temp = r + 1;
            num[r] = r + 1;
            }
        else
            {
            i--;
            continue;
            }                    ///

        /*r = rand() % MAX + 1;       ///vyhlada aj prvky čo tam nie sú↓
        temp = r;     */              ///

        fprintf(fp,"s %d\n", temp);
    }
    if(hladaj>0)
        fprintf(fp,"c\n");
    fprintf(fp,"e\n");  ///ukončovaci znak pre program
    fclose(fp);
    fp=NULL;

    fp=fopen("vysledky.txt","w");
    for(i=0;i<200000;i++)
        if(num[i]==0)
            fprintf(fp,"%d %d\n",i+1,(i+1)%33);
}
