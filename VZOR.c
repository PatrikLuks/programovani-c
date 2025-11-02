#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VSTUP "startovni_listina_kombinace_zeny.txt"
#define VYSTUP "vysledkova_listina_kombinace_zeny.txt"
#define MAX 100
#define ODDELOVACE " :,"

typedef struct {
    int minuty;
    int vteriny;
    int setiny;
} CAS;

typedef struct {
    int startovniCislo;
    char jmeno[30];
    char prijmeni[30];
    char stat[4];
    CAS sjezd;
    CAS slalom;
} ZAVODNIK;

ZAVODNIK *otevriTo(int *pocet) {
    FILE *pFile;
    char mystring[MAX];
    int radky = 0;
    ZAVODNIK *zavodnik = NULL;
    ZAVODNIK *tmpZavodnik = NULL;
    pFile = fopen(VSTUP, "r");
    if (pFile == NULL) {
        printf("Soubor %s nebyl otevren\n", VSTUP);
        return NULL;
    }
    while (fgets(mystring, MAX, pFile) != NULL) {
        puts(mystring);
        tmpZavodnik = (ZAVODNIK *) realloc(zavodnik, (radky + 1) * sizeof(ZAVODNIK));
        if (tmpZavodnik != NULL) {
            zavodnik = tmpZavodnik;
            //parsovani
            char *pch;
            int sloupce = 0;
            //nastaveni minut u slalomu na 0
            zavodnik[radky].slalom.minuty = 0;
            pch = strtok(mystring, ODDELOVACE);
            while (pch != NULL) {
                sloupce++;
                switch (sloupce) {
                    case 1:
                        zavodnik[radky].startovniCislo = atoi(pch);
                        break;
                    case 2:
                        strcpy(zavodnik[radky].jmeno, pch);
                        break;
                    case 3:
                        strcpy(zavodnik[radky].prijmeni, pch);
                        break;
                    case 4:
                        strcpy(zavodnik[radky].stat, pch);
                        break;
                    case 5:
                        zavodnik[radky].sjezd.minuty = atoi(pch);
                        break;
                    case 6:
                        zavodnik[radky].sjezd.vteriny = atoi(pch);
                        break;
                    case 7:
                        zavodnik[radky].sjezd.setiny = atoi(pch);
                        break;
                    case 8:
                        zavodnik[radky].slalom.vteriny = atoi(pch);
                        break;
                    case 9:
                        zavodnik[radky].slalom.setiny = atoi(pch);
                        break;
                }
                pch = strtok(NULL, ODDELOVACE);
            }
        } else {
            free(zavodnik);
            puts("Error (re)allocating memory");
            break;
        }
        radky++;
    }
    if (fclose(pFile) == EOF) {
        printf("soubor %s se nepodarilo zavrit.\n", VSTUP);
    }

    *pocet = radky;
    return zavodnik;
}

void vypis(ZAVODNIK *zavodnik, int pocet) {
    printf("Alpske lyzovani startovni listina K O M B I N A C E     Z E N Y\n");
    printf("--------------------------------------------------------------------\n");
    printf("startovni cislo      jmeno        prijmeni   stat      sjezd  slalom\n");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < pocet; i++) {
        printf("%15d %10s %15s %6s    %d:%02d,%02d   %02d,%02d\n",
               zavodnik[i].startovniCislo, zavodnik[i].jmeno, zavodnik[i].prijmeni, zavodnik[i].stat,
               zavodnik[i].sjezd.minuty, zavodnik[i].sjezd.vteriny, zavodnik[i].sjezd.setiny,
               zavodnik[i].slalom.vteriny, zavodnik[i].slalom.setiny);
    }
}

int naSetiny(CAS cas) {
    return cas.minuty * 6000 + cas.vteriny * 100 + cas.setiny;
}

int soucetCasu(CAS sjezd, CAS slalom) {
    return naSetiny(sjezd) + naSetiny(slalom);

}

void swap(ZAVODNIK *zavodnik1, ZAVODNIK *zavodnik2) {
    ZAVODNIK tmp = *zavodnik1;
    *zavodnik1 = *zavodnik2;
    *zavodnik2 = tmp;
}

void bubbleSort(ZAVODNIK *arr, int n) {
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0; // Počáteční nastavení pro detekci prohození
        for (j = 0; j < n - i - 1; j++) {
            if (soucetCasu(arr[j].sjezd, arr[j].slalom) > soucetCasu(arr[j + 1].sjezd, arr[j + 1].slalom)) {
                // Prohození prvků
                swap(&arr[j], &arr[j + 1]);
                swapped = 1; // Označení, že došlo k prohození
            }
        }
        // Pokud v tomto průchodu nedošlo k žádnému prohození, pole je seřazené
        if (swapped == 0) {
            break;
        }
    }
}

CAS setinyNaCas(int celkemSetiny) {
    CAS cas;
    cas.minuty = celkemSetiny / 6000;
    cas.vteriny = celkemSetiny % 6000 / 100;
    cas.setiny = celkemSetiny % 100;
    return cas;
}

void zapis(ZAVODNIK *zavodnik, int pocet) {
    FILE *pFile = fopen(VYSTUP, "w");
    CAS celkem;
    CAS ztrata;
    int setinyPrvniho = soucetCasu(zavodnik[0].sjezd, zavodnik[0].slalom);
    if (pFile == NULL) {
        printf("Soubor %s nebyl otevren\n", VYSTUP);
    }
    fprintf(pFile, "A L P S K E    L Y Z O V A N I - K O M B I N A C E     Z E N Y\n");
    fprintf(pFile, "--------------------------------------------------------------\n");
    fprintf(pFile, "poradi      jmeno        prijmeni   stat      sjezd  slalom     celkem     ztrata\n");
    for (int i = 0; i < pocet; i++) {
        int celkemSetiny = soucetCasu(zavodnik[i].sjezd, zavodnik[i].slalom);
        celkem = setinyNaCas(celkemSetiny);
        ztrata = setinyNaCas(celkemSetiny - setinyPrvniho);
        fprintf(pFile, "%5d. %10s %15s %6s    %d:%02d,%02d   %02d,%02d    %d:%02d,%02d    %d:%02d,%02d\n",
                i + 1, zavodnik[i].jmeno, zavodnik[i].prijmeni, zavodnik[i].stat,
                zavodnik[i].sjezd.minuty, zavodnik[i].sjezd.vteriny, zavodnik[i].sjezd.setiny,
                zavodnik[i].slalom.vteriny, zavodnik[i].slalom.setiny, celkem.minuty, celkem.vteriny, celkem.setiny,
                ztrata.minuty, ztrata.vteriny, ztrata.setiny);
    }
    if (fclose(pFile) == EOF) {
        printf("soubor %s se nepodarilo zavrit.\n", VYSTUP);
    }
}


int main(void) {
    ZAVODNIK *zavodnik;
    int pocet;
    zavodnik = otevriTo(&pocet);
    vypis(zavodnik, pocet);
    bubbleSort(zavodnik, pocet);
    zapis(zavodnik, pocet);
    free(zavodnik);
    return 0;
}
