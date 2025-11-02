#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VSTUP  "beh.txt"
#define VYSTUP "vysledkova_listina.txt"
#define MAX    200
#define ODDELOVACE ";\n"

typedef struct {
    int den;
    int mesic;
    int rok;
} DATUM;

typedef struct {
    int hodiny;
    int minuty;
    int sekundy;
} CAS;

typedef struct {
    int startovniCislo;
    char prijmeni[30];
    char jmeno[30];
    DATUM narozeni;
    char stat[4];
    char oddil[80];
    CAS cas;
} ZAVODNIK;

ZAVODNIK *otevriTo(int *pocet);
void vypis(ZAVODNIK *zavodnik, int pocet);
int rokNarozeni(ZAVODNIK z);
int vekZeSystemu(DATUM d);
void vypisNejstarsiho(ZAVODNIK *zavodnik, int pocet);
void vypisPocetCechu(ZAVODNIK *zavodnik, int pocet);
int casNaSekundy(CAS c);
CAS sekundyNaCas(int s);
void swapZavodnik(ZAVODNIK *a, ZAVODNIK *b);
void bubbleSort(ZAVODNIK *arr, int n);
void zapis(ZAVODNIK *zavodnik, int pocet);

ZAVODNIK *otevriTo(int *pocet) {
    FILE *pFile;
    char mystring[MAX];
    int radky = 0;
    ZAVODNIK *zavodnik = NULL;
    ZAVODNIK *tmp = NULL;

    pFile = fopen(VSTUP, "r");
    if (pFile == NULL) {
        printf("Soubor %s nebyl otevren\n", VSTUP);
        return NULL;
    }

    if (fgets(mystring, MAX, pFile) == NULL) {
        printf("Soubor %s je prazdny.\n", VSTUP);
        fclose(pFile);
        return NULL;
    }

    while (fgets(mystring, MAX, pFile) != NULL) {
        tmp = (ZAVODNIK *) realloc(zavodnik, (radky + 1) * sizeof(ZAVODNIK));
        if (tmp == NULL) {
            free(zavodnik);
            puts("Error (re)allocating memory");
            break;
        }
        zavodnik = tmp;

        {
            char *pch = strtok(mystring, ODDELOVACE);
            int sloupce = 0;

            while (pch != NULL) {
                sloupce++;
                switch (sloupce) {
                    case 1:
                        zavodnik[radky].startovniCislo = atoi(pch);
                        break;
                    case 2:
                        strncpy(zavodnik[radky].prijmeni, pch, sizeof(zavodnik[radky].prijmeni));
                        zavodnik[radky].prijmeni[sizeof(zavodnik[radky].prijmeni) - 1] = '\0';
                        break;
                    case 3:
                        strncpy(zavodnik[radky].jmeno, pch, sizeof(zavodnik[radky].jmeno));
                        zavodnik[radky].jmeno[sizeof(zavodnik[radky].jmeno) - 1] = '\0';
                        break;
                    case 4:
                        sscanf(pch, "%d.%d.%d",
                               &zavodnik[radky].narozeni.den,
                               &zavodnik[radky].narozeni.mesic,
                               &zavodnik[radky].narozeni.rok);
                        break;
                    case 5:
                        strncpy(zavodnik[radky].stat, pch, sizeof(zavodnik[radky].stat));
                        zavodnik[radky].stat[sizeof(zavodnik[radky].stat) - 1] = '\0';
                        break;
                    case 6:
                        while (*pch == ' ' || *pch == '\t') pch++;
                        strncpy(zavodnik[radky].oddil, pch, sizeof(zavodnik[radky].oddil));
                        zavodnik[radky].oddil[sizeof(zavodnik[radky].oddil) - 1] = '\0';
                        break;
                    case 7:
                        sscanf(pch, "%d:%d:%d",
                               &zavodnik[radky].cas.hodiny,
                               &zavodnik[radky].cas.minuty,
                               &zavodnik[radky].cas.sekundy);
                        break;
                }
                pch = strtok(NULL, ODDELOVACE);
            }
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
    int i;
    printf("B E H   –   S T A R T O V N I   L I S T I N A\n");
    printf("---------------------------------------------------------------\n");
    printf("st.c.  prijmeni        jmeno           nar.        stat  oddil                 cas\n");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < pocet; i++) {
        printf("%5d  %-14s %-14s %02d.%02d.%4d  %-3s  %-22s %d:%02d:%02d\n",
               zavodnik[i].startovniCislo,
               zavodnik[i].prijmeni,
               zavodnik[i].jmeno,
               zavodnik[i].narozeni.den,
               zavodnik[i].narozeni.mesic,
               zavodnik[i].narozeni.rok,
               zavodnik[i].stat,
               zavodnik[i].oddil,
               zavodnik[i].cas.hodiny,
               zavodnik[i].cas.minuty,
               zavodnik[i].cas.sekundy);
    }
    printf("\nCelkovy pocet zavodniku: %d\n", pocet);
}

int rokNarozeni(ZAVODNIK z) {
    return z.narozeni.rok;
}

int vekZeSystemu(DATUM d) {
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    int vek = (now->tm_year + 1900) - d.rok;
    if (d.mesic > (now->tm_mon + 1) ||
        (d.mesic == (now->tm_mon + 1) && d.den > now->tm_mday)) {
        vek--;
    }
    return vek;
}

void vypisNejstarsiho(ZAVODNIK *zavodnik, int pocet) {
    int i;
    int idx = 0;
    int maxVek = vekZeSystemu(zavodnik[0].narozeni);
    for (i = 1; i < pocet; i++) {
        int vek = vekZeSystemu(zavodnik[i].narozeni);
        if (vek > maxVek) {
            maxVek = vek;
            idx = i;
        }
    }
    printf("Nejstarsi zavodnik: %s %s %02d.%02d.%4d %s %s vek %d let\n",
           zavodnik[idx].prijmeni,
           zavodnik[idx].jmeno,
           zavodnik[idx].narozeni.den,
           zavodnik[idx].narozeni.mesic,
           zavodnik[idx].narozeni.rok,
           zavodnik[idx].stat,
           zavodnik[idx].oddil,
           maxVek);
}

void vypisPocetCechu(ZAVODNIK *zavodnik, int pocet) {
    int i, c = 0;
    for (i = 0; i < pocet; i++) {
        if (strcmp(zavodnik[i].stat, "CZE") == 0) {
            c++;
        }
    }
    printf("Pocet ceskych zavodniku (CZE): %d\n", c);
}

int casNaSekundy(CAS c) {
    return c.hodiny * 3600 + c.minuty * 60 + c.sekundy;
}

CAS sekundyNaCas(int s) {
    CAS c;
    if (s < 0) s = 0;
    c.hodiny = s / 3600;
    s %= 3600;
    c.minuty = s / 60;
    c.sekundy = s % 60;
    return c;
}

void swapZavodnik(ZAVODNIK *a, ZAVODNIK *b) {
    ZAVODNIK t = *a;
    *a = *b;
    *b = t;
}

void bubbleSort(ZAVODNIK *arr, int n) {
    int i, j, swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (casNaSekundy(arr[j].cas) > casNaSekundy(arr[j + 1].cas)) {
                swapZavodnik(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void zapis(ZAVODNIK *zavodnik, int pocet) {
    FILE *pFile = fopen(VYSTUP, "w");
    if (pFile == NULL) {
        printf("Soubor %s nebyl otevren\n", VYSTUP);
        return;
    }

    fprintf(pFile, "B E H   -   V Y S L E D K O V A   L I S T I N A\n");
    fprintf(pFile, "---------------------------------------------------------------\n");
    fprintf(pFile, "por.  st.c.  prijmeni        jmeno           stat  rocnik  oddil                    cas       ztrata\n");

    if (pocet > 0) {
        int casPrvniho = casNaSekundy(zavodnik[0].cas);
        for (int i = 0; i < pocet; i++) {
            int casAkt = casNaSekundy(zavodnik[i].cas);
            CAS ztrata = sekundyNaCas(casAkt - casPrvniho);
            fprintf(pFile, "%3d. %5d  %-14s %-14s %-3s   %4d   %-23s %d:%02d:%02d   +%d:%02d:%02d\n",
                    i + 1,
                    zavodnik[i].startovniCislo,
                    zavodnik[i].prijmeni,
                    zavodnik[i].jmeno,
                    zavodnik[i].stat,
                    rokNarozeni(zavodnik[i]),
                    zavodnik[i].oddil,
                    zavodnik[i].cas.hodiny,
                    zavodnik[i].cas.minuty,
                    zavodnik[i].cas.sekundy,
                    ztrata.hodiny,
                    ztrata.minuty,
                    ztrata.sekundy);
        }
    }

    if (fclose(pFile) == EOF) {
        printf("soubor %s se nepodarilo zavrit.\n", VYSTUP);
    }
}

int main(void) {
    ZAVODNIK *zavodnik;
    int pocet;

    zavodnik = otevriTo(&pocet);
    if (zavodnik == NULL) {
        return 1;
    }

    vypis(zavodnik, pocet);    
    vypisNejstarsiho(zavodnik, pocet);
    vypisPocetCechu(zavodnik, pocet);

    bubbleSort(zavodnik, pocet);
    zapis(zavodnik, pocet);

    free(zavodnik);
    return 0;
}
