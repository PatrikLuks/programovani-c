
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VSTUP  "cyklistika.txt"
#define VYSTUP "vysledkova_listina.txt"
#define MAX 300
#define ODDELOVACE " :."

typedef struct {
    int hodiny;
    int minuty;
    int vteriny;
    int desetiny;
} CAS;

typedef struct {
    int cislo;
    char prijmeni[30];
    char jmeno[30];
    char narodnost[5];
    int rocnik;
    char klub[60];
    CAS cas;
} CYKLISTA;

typedef struct {
    char kod[5];
    int pocet;
} NAROD;


int casNaDesetiny(CAS cas) {
    return cas.hodiny * 36000
         + cas.minuty * 600
         + cas.vteriny * 10
         + cas.desetiny;
}

CAS desetinyNaCas(int d) {
    CAS cas;
    if (d < 0) d = 0;
    cas.hodiny = d / 36000; d %= 36000;
    cas.minuty = d / 600;   d %= 600;
    cas.vteriny = d / 10;
    cas.desetiny = d % 10;
    return cas;
}

CYKLISTA *otevriTo(int *pocet) {
    FILE *pFile;
    char radek[MAX];
    CYKLISTA *zavodnici = NULL;
    CYKLISTA *tmp = NULL;
    int radky = 0;

    pFile = fopen(VSTUP, "r");
    if (pFile == NULL) {
        printf("Soubor %s nebyl otevren\n", VSTUP);
        return NULL;
    }

    if (fgets(radek, MAX, pFile) == NULL) {
        printf("Soubor %s je prazdny\n", VSTUP);
        fclose(pFile);
        return NULL;
    }

    while (fgets(radek, MAX, pFile) != NULL) {
        printf("%s", radek); 

        radek[strcspn(radek, "\r\n")] = '\0';

        tmp = (CYKLISTA *) realloc(zavodnici, (radky + 1) * sizeof(CYKLISTA));
        if (tmp == NULL) {
            free(zavodnici);
            printf("Chyba alokace\n");
            fclose(pFile);
            return NULL;
        }
        zavodnici = tmp;

        {
            char *pch;
            int sloupec = 0;
            pch = strtok(radek, ";");
            while (pch != NULL) {
                sloupec++;
                switch (sloupec) {
                    case 1: zavodnici[radky].cislo = atoi(pch); break;
                    case 2: strcpy(zavodnici[radky].prijmeni, pch); break;
                    case 3: strcpy(zavodnici[radky].jmeno, pch); break;
                    case 4: strcpy(zavodnici[radky].narodnost, pch); break;
                    case 5: zavodnici[radky].rocnik = atoi(pch); break;
                    case 6: strcpy(zavodnici[radky].klub, pch); break;
                    case 7: {
                        char casStr[50];
                        char *pchCas;
                        int cast = 0;
                        strncpy(casStr, pch, 49);
                        casStr[49] = '\0';
                        pchCas = strtok(casStr, ODDELOVACE);
                        while (pchCas != NULL) {
                            cast++;
                            switch (cast) {
                                case 1: zavodnici[radky].cas.hodiny = atoi(pchCas); break;
                                case 2: zavodnici[radky].cas.minuty = atoi(pchCas); break;
                                case 3: zavodnici[radky].cas.vteriny = atoi(pchCas); break;
                                case 4: zavodnici[radky].cas.desetiny = atoi(pchCas); break;
                            }
                            pchCas = strtok(NULL, ODDELOVACE);
                        }
                        if (cast == 3) {
                            zavodnici[radky].cas.desetiny = 0;
                        }
                        break;
                    }
                }
                pch = strtok(NULL, ";");
            }
        }

        radky++;
    }

    fclose(pFile);
    *pocet = radky;
    return zavodnici;
}

void vypis(CYKLISTA *zavodnici, int pocet) {
    int i;
    printf("\nV Y S L E D K O V A   L I S T I N A - O B R  D R A S A L\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("cislo  prijmeni        jmeno           stat  rocnik  klub                                         cas\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < pocet; i++) {
        printf("%5d  %-14s %-14s %-4s  %6d  %-43s %2d:%02d:%02d.%1d\n",
               zavodnici[i].cislo,
               zavodnici[i].prijmeni,
               zavodnici[i].jmeno,
               zavodnici[i].narodnost,
               zavodnici[i].rocnik,
               zavodnici[i].klub,
               zavodnici[i].cas.hodiny,
               zavodnici[i].cas.minuty,
               zavodnici[i].cas.vteriny,
               zavodnici[i].cas.desetiny);
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("Pocet zavodniku: %d\n", pocet);
}

void vypisNejstarsiho(CYKLISTA *zavodnici, int pocet) {
    if (pocet == 0) return;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int rok = tm_info->tm_year + 1900;

    int idx = 0;
    for (int i = 1; i < pocet; i++) {
        if (zavodnici[i].rocnik < zavodnici[idx].rocnik)
            idx = i;
    }

    printf("N E J S T A R S I   Z A V O D N I K\n");
    printf("-------------------------------------------------\n");
    printf("cislo  prijmeni        jmeno           stat  rocnik  vek  klub                               cas\n");
    printf("-------------------------------------------------\n");
    printf("%5d  %-14s %-14s %-4s  %6d  %3d  %-32s %2d:%02d:%02d.%1d\n",
           zavodnici[idx].cislo,
           zavodnici[idx].prijmeni,
           zavodnici[idx].jmeno,
           zavodnici[idx].narodnost,
           zavodnici[idx].rocnik,
           rok - zavodnici[idx].rocnik,
           zavodnici[idx].klub,
           zavodnici[idx].cas.hodiny,
           zavodnici[idx].cas.minuty,
           zavodnici[idx].cas.vteriny,
           zavodnici[idx].cas.desetiny);
}

void vypisPoctyDleNarodnosti(CYKLISTA *zavodnici, int pocet) {
    NAROD nar[50];
    int n = 0;
    for (int i = 0; i < pocet; i++) {
        int found = -1;
        for (int j = 0; j < n; j++) {
            if (strcmp(nar[j].kod, zavodnici[i].narodnost) == 0) {
                found = j;
                break;
            }
        }
        if (found == -1) {
            strcpy(nar[n].kod, zavodnici[i].narodnost);
            nar[n].pocet = 1;
            n++;
        } else {
            nar[found].pocet++;
        }
    }

    printf("P O C T Y   Z A V O D N I K U   D L E   N A R O D N O S T I\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-4s : %d\n", nar[i].kod, nar[i].pocet);
    }
}

void swap(CYKLISTA *a, CYKLISTA *b) {
    CYKLISTA tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubbleSort(CYKLISTA *arr, int n) {
    int i, j, swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (casNaDesetiny(arr[j].cas) > casNaDesetiny(arr[j + 1].cas)) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void zapis(CYKLISTA *zavodnici, int pocet) {
    FILE *f = fopen(VYSTUP, "w");
    if (f == NULL) {
        printf("Soubor %s nebyl otevren\n", VYSTUP);
        return;
    }

    fprintf(f, "V Y S L E D K O V A   L I S T I N A - O B R  D R A S A L\n");
    fprintf(f, "----------------------------------------------------------------------------------------------\n");
    fprintf(f, "por.  cislo  prijmeni        jmeno           stat  rocnik  klub                                   cas          ztrata\n");
    fprintf(f, "----------------------------------------------------------------------------------------------\n");

    if (pocet > 0) {
        int casPrvniho = casNaDesetiny(zavodnici[0].cas);
        for (int i = 0; i < pocet; i++) {
            int casZav = casNaDesetiny(zavodnici[i].cas);
            CAS ztrata = desetinyNaCas(casZav - casPrvniho);

            fprintf(f,
                " %2d.  %4d  %-14s %-14s %-4s  %5d  %-35s %2d:%02d:%02d.%1d    %1d:%02d:%02d.%1d\n",
                i + 1,
                zavodnici[i].cislo,
                zavodnici[i].prijmeni,
                zavodnici[i].jmeno,
                zavodnici[i].narodnost,
                zavodnici[i].rocnik,
                zavodnici[i].klub,
                zavodnici[i].cas.hodiny,
                zavodnici[i].cas.minuty,
                zavodnici[i].cas.vteriny,
                zavodnici[i].cas.desetiny,
                ztrata.hodiny,
                ztrata.minuty,
                ztrata.vteriny,
                ztrata.desetiny
            );
        }
    }

    fclose(f);
}

int main(void) {
    CYKLISTA *zavodnici;
    int pocet;

    zavodnici = otevriTo(&pocet);
    if (zavodnici == NULL) {
        return 1;
    }

    vypis(zavodnici, pocet);
    vypisNejstarsiho(zavodnici, pocet);
    vypisPoctyDleNarodnosti(zavodnici, pocet);

    bubbleSort(zavodnici, pocet);

    zapis(zavodnici, pocet);

    free(zavodnici);
    return 0;
}
