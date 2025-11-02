#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "zavodnici.txt"
#define OUTPUT "vysledky_olympiady.txt"
#define ODDELOVACE " :."
#define MAX 100


typedef struct {
    int minuty;
    int vteriny;
    int setiny;
} CAS;

typedef struct {
    int startovniCislo;
    char jmeno[30];
    char prijmeni[30];
    CAS prvniKolo;
    CAS druheKolo;
} DATA;


int casNaSetiny(CAS cas){   
    return cas.minuty * 6000 + cas.vteriny * 100 + cas.setiny;
}


CAS setinyNaCas(int setiny){
  CAS cas;
  cas.minuty = setiny / 6000;
  cas.vteriny = (setiny % 6000) / 100;
  cas.setiny = setiny%100;
  return cas;
}

int soucetKol(DATA data){
  return casNaSetiny(data.prvniKolo) + casNaSetiny(data.druheKolo);
}

void swap(DATA *a, DATA *b){
  DATA tmp = *a;
  *a = *b;
  *b = tmp;
}


void seradPodleCasu(DATA *pole, int pocet){
  int i, j, swapped;
  for(int i = 0; i < pocet - 1; i++){
    swapped = 0;
    for(int j = 0; j < pocet - i - 1; j++){
      if(soucetKol(pole[j])>soucetKol(pole[j+1])){
        swap(&pole[j], &pole[j+1]);
        swapped = 1;
      }
    }
    if(!swapped) break;
  }
}

DATA *otevriTo(int*pocet){
  FILE * pFile;
  char retezec[MAX];
  int radky=0;
  DATA *data = NULL;
  DATA * tmp = NULL;
  pFile = fopen (INPUT,"r");
  
  if (pFile==NULL)
  {
    printf("Soubor %s se nepodarilo otevrit.\n", INPUT);
    return NULL;
  }

  if (fgets(retezec, MAX, pFile) == NULL) {
      printf("Soubor %s je prazdny.\n", INPUT);
      fclose(pFile);
      return NULL;
  }

  while(fgets(retezec, MAX, pFile) != NULL){
    tmp = (DATA *) realloc (data, (radky+1)*sizeof(DATA));
    if (tmp == NULL){
      printf("chyba pri alokaci pameti.\n");
      free(data);
      fclose(pFile);
      return NULL;
    }
    data = tmp;


    char *pch;
    int sloupec=0;

    pch = strtok(retezec, ODDELOVACE);
    while (pch != NULL){
      sloupec++;
      switch(sloupec){
        case 1:
          data[radky].startovniCislo = atoi(pch);
          break;
        case 2:
          strcpy(data[radky].prijmeni, pch);
          break;
        case 3:
          strcpy(data[radky].jmeno, pch);
          break;
        case 4:
          data[radky].prvniKolo.minuty = atoi(pch);
          break;
        case 5:
          data[radky].prvniKolo.vteriny = atoi(pch);
          break;
        case 6:
          data[radky].prvniKolo.setiny = atoi(pch);
          break;
        case 7:
          data[radky].druheKolo.minuty = atoi(pch);
          break;
        case 8:
          data[radky].druheKolo.vteriny = atoi(pch);
          break;
        case 9:
          data[radky].druheKolo.setiny = atoi(pch);
          break;
        default:
          break;
      }
      pch = strtok(NULL, ODDELOVACE);
    }
    radky++;
  }
  if (fclose(pFile) == EOF) {
      printf("Soubor %s se nepodarilo zavrit.\n", INPUT);
  }

  *pocet = radky;
  return data;
}

void vypisStartovniListinu(DATA *z, int pocet){
  int i;
    printf("S T A R T O V N I   L I S T I N A  -  S L A L O M   Z E N Y\n");
    printf("-----------------------------------------------------------\n");
    printf("startovni cislo|prijmeni       |jmeno          |1.kolo |2.kolo\n");
    printf("-----------------------------------------------------------\n");
    for(i=0; i < pocet; i++){
      printf("%15d|%15s|%15s|%2d:%.2f|%2d:%.2f|\n",
        z[i].startovniCislo,
        z[i].prijmeni,
        z[i].jmeno,
        z[i].prvniKolo.minuty,
        z[i].prvniKolo.vteriny + z[i].prvniKolo.setiny / 100.0,
        z[i].druheKolo.minuty,
        z[i].druheKolo.vteriny + z[i].druheKolo.setiny / 100.0
      );
    }
}

void zapisVysledky(DATA *z, int pocet){
  FILE * pFile;
  pFile = fopen (OUTPUT,"w");
  
  if (pFile==NULL)
  {
    printf("Soubor %s se nepodarilo otevrit.\n", OUTPUT);
    return;
  }

  int casPrvni = soucetKol(z[0]);
  
  fprintf(pFile, "V Y S L E D K O V A   L I S T I N A\n");
  fprintf(pFile, "-----------------------------------------------------------\n");
  fprintf(pFile, "poradi|cislo|prijmeni|jmeno|1.kolo|2.kolo|celkem|ztrata\n");
  fprintf(pFile, "-----------------------------------------------------------\n");
  
  for(int i=0; i < pocet; i++){
    int celkemSetiny = soucetKol(z[i]);
    CAS celkem = setinyNaCas(celkemSetiny);

    fprintf(pFile, "%d|%d|%s|%s|%d:%02d.%02d|%d:%02d.%02d|%02d:%02d.%02d|",
      i + 1,
      z[i].startovniCislo,
      z[i].prijmeni,
      z[i].jmeno,
      z[i].prvniKolo.minuty,
      z[i].prvniKolo.vteriny,
      z[i].prvniKolo.setiny,
      z[i].druheKolo.minuty,
      z[i].druheKolo.vteriny,
      z[i].druheKolo.setiny,
      celkem.minuty,
      celkem.vteriny,
      celkem.setiny
    ); 
    if(i==0){
      fprintf(pFile, "\n");
    } else {
      int ztrataSetiny = celkemSetiny - casPrvni;
      CAS ztrata = setinyNaCas(ztrataSetiny);
      fprintf(pFile, "%02d:%02d.%02d\n",
        ztrata.minuty,
        ztrata.vteriny,
        ztrata.setiny
      );
    }

  }
  if (fclose(pFile) == EOF) {
      printf("Soubor %s se nepodarilo zavrit.\n", OUTPUT);
  }
}



int main(){
  DATA * data = NULL;
  int pocet =0;

  data = otevriTo(&pocet);
  if(data == NULL){
    return 1;
  }

  vypisStartovniListinu(data, pocet);

  seradPodleCasu(data, pocet);
  zapisVysledky(data, pocet);

  free(data);
  return 0;

}