#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct Osoba
{
       char imie[100];
       int wiek;
       struct Osoba* next;
              } Osoba ;

// zlicza dlugosc listy
int dlugosc_listy(Osoba* lista)
{
    int d = 0;
    Osoba* wsk = lista;
    while(wsk != NULL)
    {
              d++;
              wsk = wsk->next;
    }
    return d;
}

// dodaje nowy wezel do listy
void dodaj(Osoba** lista, Osoba* nowa)
{
     nowa->next=NULL;

     if((*lista)==NULL)
     {
      *lista = nowa;
                     }
     else
     {
         Osoba* wsk = *lista;
        while(wsk->next != NULL)	wsk = wsk->next;
         
         wsk->next = nowa;

         }
     }              

// dodaje osobe do listy              
void dodajOsobe(Osoba** lista)
{    
	Osoba* nowa = (Osoba*)malloc(sizeof(Osoba));
	int w = rand() % 100 + 1;//losuje liczbe od 1 do 100
	nowa->wiek=w;

   int n = rand() % 100 + 1;//losuje liczbe od 1 do 100
if(n<=10) strcpy(nowa->imie, "Jovan");
if(n>10 && n<=20) strcpy(nowa->imie, "Filip");
if(n>20 && n<=40) strcpy(nowa->imie, "Luka");
if(n>40 && n<=50) strcpy(nowa->imie, "Nikola");
if(n>50 && n<=60) strcpy(nowa->imie, "Stefan");
if(n>60 && n<=70) strcpy(nowa->imie, "Petar");
if(n>70 && n<=80) strcpy(nowa->imie, "Lazar");
if(n>80 && n<=90) strcpy(nowa->imie, "Milan");
if(n>90) strcpy(nowa->imie, "Marko");

     dodaj(lista, nowa);     
}

//Wypisuje cala liste
void wypisz_liste(Osoba* lista)
{

     Osoba* wsk = lista;

     if(lista == NULL)
     printf("LISTA JEST PUSTA");
     else
     printf("Lista zawiera %d elementow: \n", dlugosc_listy(lista) );
     int i = 1;
     while( wsk != NULL)
     {
            printf("%d %s  %d \n", i, wsk->imie,  wsk->wiek);
            wsk=wsk->next;
            i++;
     }
}



         void wypisz_osobe_lista(Osoba* lista, int ID)
         {
              int i;
              Osoba* wsk = lista;
          for(i = 1; i < ID; i++)
             {
                     wsk=wsk->next;

             }
             printf("%d %s %d \n", i, wsk->imie,  wsk->wiek);     
	}

          //wypisz osobe o podanym indexie
          void wypisz_osobe(Osoba* lista)
          {
               int ID;

               printf("Podaj numer osoby: " );
               scanf("%d", &ID);

               if((ID > dlugosc_listy(lista)) || (ID < 1))
                 {
           printf("Nie ma takiego indeksu");

           }
           else
           wypisz_osobe_lista(lista, ID);    

               }

//marge'uje listy
void merge(Osoba** lista1, Osoba** lista2)
{
	Osoba* nowa = lista2;
	//nowa=nowa->next;

     if((*lista1)==NULL)
     {
      *lista1 = nowa;
                     }
     else
     {
         Osoba* wsk = lista1;
        while(wsk->next != NULL)	wsk = wsk->next;
         
         wsk->next = nowa;

         }

}


int main()
{
	Osoba* lista1 = NULL;
	Osoba* lista2 = NULL;
		int option=1;
	float time=0;

 while(option != 0)
           {
printf("\n\n MENU");
printf("\n nieparzyste to 1 lista parzyste to 2\n");
printf("\n\n\n 0 Wyjscie z programu");
printf("\n 1 Dodaj 1000 osob do 1 listy");
printf("\n 2 Dodaj 1000 osob do 2 listy");
printf("\n 3 Wyswietl osobe o podanym indeksie w 1 liscie");
printf("\n 4 Wyswietl osobe o podanym indeksie w 2 liscie");
printf("\n 5 Wyswietl cala 1 liste z CZASEM");
printf("\n 6 Wyswietl cala 2 liste");
printf("\n 7 Polacz");
printf("\n 8 WYSWIETL 1000 OSOB O LOSOWYM INDEKSIE z CZASEM");
printf("\n 9 WYSWIETL wyswietl czaasy dostepu do 1 i ostatniego elementu\n");

           scanf("%d", &option);

           switch(option)
     			      {             

                         case 1: 
                              printf("DODAWANIE OSOBY\n");
				for(int i=0;i<1000;i++)
                              dodajOsobe(&lista1);
                              break;      

                         case 2: 
                              printf("DODAWANIE OSOBY\n");
				for(int i=0;i<1000;i++)
                              dodajOsobe(&lista2);
                              break;       

                         case 3: 
                              printf("WYSWIETL OSOBE O PODANYM INDEKSIE\n");
                              wypisz_osobe(lista1);
                              break;

                         case 4: 
                              printf("WYSWIETL OSOBE O PODANYM INDEKSIE\n");
                              wypisz_osobe(lista2);
                              break;

                         case 5: 
                              printf("WYSWIETL CALA BAZE z czasem\n");
				clock_t start1 = clock();
                              wypisz_liste(lista1);
				clock_t end1 = clock();
				time=(float)(end1 - start1) / CLOCKS_PER_SEC * 100.0;
				printf("\nCZAS: %f\n",time);
                              break;

                         case 6: 
                              printf("WYSWIETL CALA BAZE\n");
                              wypisz_liste(lista2);
                              break;

                         case 7: 
                              printf("Lacze\n");
                              merge(lista1, lista2);
                              break;
			
                         case 8: 
                              printf("WYSWIETL OSOBE O LOSOWYM INDEKSIE z czasem\n");
				clock_t start2 = clock();
                              wypisz_liste(lista1);
				for(int i=1;i<=1000;i++)
				{
                              		int ID = rand() % dlugosc_listy(lista1) + 1;//losuje liczbe od 1 do 100

               				if( 0 == dlugosc_listy(lista1) )
                 			{
           						printf("Lista pusta");
					}
		        	   	else
			        	 wypisz_osobe_lista(lista1, ID);   
                              
				}	
				clock_t end2 = clock();
				time=(float)(end2 - start2) / CLOCKS_PER_SEC * 100.0;
				printf("\nCZAS: %f\n",time);		
				break;
                         case 9: 
				printf("CZASY\n");
				clock_t start4 = clock();
				wypisz_osobe_lista(lista1,1);
				clock_t end4 = clock();
				time=(float)(end4 - start4) / CLOCKS_PER_SEC * 100.0;
				printf("CZAS do 1 elementu: %f\n",time);

				clock_t start3 = clock();
				wypisz_osobe_lista(lista1,1000);
				clock_t end3 = clock();
				time=(float)(end3 - start3) / CLOCKS_PER_SEC * 100.0;
				printf("Czas dostepu do ostatniego elementu: %f\n",time);
                              break;
                         }
          }
	return 0;
    }
