#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VSTUP  "jizerska50.txt"
#define VYSTUP "vysledkova_listina.txt"
#define MAX    200
#define ODDELOVACE " ;:(),"

typedef struct {
    int hodiny;
    int minuty;
    int vteriny;
} CAS;

typedef struct {
    int startovniCislo;
    char prijmeni[40];
    char jmeno[80];
    int rocnik;
    char stat[4];
    CAS cas;
} ZAVODNIK;

int casNaSekundy(CAS c) {
    return c.hodiny * 3600 + c.minuty * 60 + c.vteriny;
}

CAS sekundyNaCas(int s) {
    CAS c;
    if (s < 0) s = 0;
    c.hodiny = s / 3600;
    c.minuty = (s % 3600) / 60;
    c.vteriny = s % 60;
    return c;
}

void prohodZavodniky(ZAVODNIK *a, ZAVODNIK *b) {
    ZAVODNIK tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubbleSort(ZAVODNIK *pole, int pocet) {
    int i, j, vymena;
    for (i = 0; i < pocet - 1; i++) {
        vymena = 0;
        for (j = 0; j < pocet - i - 1; j++) {
            if (casNaSekundy(pole[j].cas) > casNaSekundy(pole[j + 1].cas)) {
                prohodZavodniky(&pole[j], &pole[j + 1]);
                vymena = 1;
            }
        }
        if (vymena == 0) break;
    }
}

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
        if (tmpZavodnik == NULL) {
            free(zavodnik);
            break;
        }
        zavodnik = tmpZavodnik;

        zavodnik[radky].startovniCislo = 0;
        zavodnik[radky].prijmeni[0] = '\0';
        zavodnik[radky].jmeno[0] = '\0';
        zavodnik[radky].rocnik = 0;
        zavodnik[radky].stat[0] = '\0';
        zavodnik[radky].cas.hodiny = 0;
        zavodnik[radky].cas.minuty = 0;
        zavodnik[radky].cas.vteriny = 0;

        {
            char *pch;
            int stav = 0;
            int casCast = 0;

            pch = strtok(mystring, ODDELOVACE);

            if (pch != NULL && !(pch[0] >= '0' && pch[0] <= '9')) {
                continue;
            }

            while (pch != NULL) {
                if (stav == 0) {
                    zavodnik[radky].startovniCislo = atoi(pch);
                    stav = 1;
                } else if (stav == 1) {
                    strcpy(zavodnik[radky].prijmeni, pch);
                    stav = 2;
                } else if (stav == 2) {
                    if (strlen(pch) == 4 &&
                        pch[0] >= '1' && pch[0] <= '2') {
                        zavodnik[radky].rocnik = atoi(pch);
                        stav = 3;
                    } else {
                        if (zavodnik[radky].jmeno[0] == '\0') {
                            strcpy(zavodnik[radky].jmeno, pch);
                        } else {
                            strcat(zavodnik[radky].jmeno, " ");
                            strcat(zavodnik[radky].jmeno, pch);
                        }
                    }
                } else if (stav == 3) {
                    strncpy(zavodnik[radky].stat, pch, 3);
                    zavodnik[radky].stat[3] = '\0';
                    stav = 4;
                } else if (stav == 4) {
                    if (casCast == 0) {
                        zavodnik[radky].cas.hodiny = atoi(pch);
                        casCast = 1;
                    } else if (casCast == 1) {
                        zavodnik[radky].cas.minuty = atoi(pch);
                        casCast = 2;
                    } else if (casCast == 2) {
                        zavodnik[radky].cas.vteriny = atoi(pch);
                        casCast = 3;
                    }
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

void vypis(ZAVODNIK *pole, int pocet) {
    int i;
    printf("J I Z E R S K A    5 0   -   S T A R T O V N I   L I S T I N A\n");
    printf("--------------------------------------------------------------------\n");
    printf("startovni cislo   prijmeni           jmeno             stat   rok     cas\n");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < pocet; i++) {
        printf("%15d   %-16s %-16s %-5s %5d   %d:%02d:%02d\n",
               pole[i].startovniCislo,
               pole[i].prijmeni,
               pole[i].jmeno,
               pole[i].stat,
               pole[i].rocnik,
               pole[i].cas.hodiny,
               pole[i].cas.minuty,
               pole[i].cas.vteriny);
    }
}

void vypisNejmladsiho(ZAVODNIK *pole, int pocet) {
    int i, idx = 0;
    time_t tt = time(NULL);
    struct tm *tm = localtime(&tt);
    int rok = tm->tm_year + 1900;
    for (i = 1; i < pocet; i++) {
        if (pole[i].rocnik > pole[idx].rocnik) idx = i;
    }
    printf("\nNejmladsi zavodnik: %s %s (%d) %s vek %d let\n",
           pole[idx].jmeno,
           pole[idx].prijmeni,
           pole[idx].rocnik,
           pole[idx].stat,
           (pole[idx].rocnik > 0) ? (rok - pole[idx].rocnik) : 0);
}

int pocetCechu(ZAVODNIK *pole, int pocet) {
    int i, c = 0;
    for (i = 0; i < pocet; i++) {
        if (strcmp(pole[i].stat, "CZE") == 0) c++;
    }
    return c;
}

void zapis(ZAVODNIK *pole, int pocet) {
    FILE *f = fopen(VYSTUP, "w");
    if (f == NULL) {
        printf("Soubor %s nebyl otevren\n", VYSTUP);
        return;
    }

    {
        int casPrvniho = casNaSekundy(pole[0].cas);
        int i;
        fprintf(f, "J I Z E R S K A    5 0   -   V Y S L E D K O V A   L I S T I N A\n");
        fprintf(f, "-----------------------------------------------------------------------\n");
        fprintf(f, "poradi  start. cislo   prijmeni           jmeno             stat   rok     cas       ztrata\n");
        for (i = 0; i < pocet; i++) {
            int muj = casNaSekundy(pole[i].cas);
            CAS ztr = sekundyNaCas(muj - casPrvniho);
            fprintf(f, "%5d. %10d   %-16s %-16s %-5s %5d   %d:%02d:%02d   %d:%02d:%02d\n",
                    i + 1,
                    pole[i].startovniCislo,
                    pole[i].prijmeni,
                    pole[i].jmeno,
                    pole[i].stat,
                    pole[i].rocnik,
                    pole[i].cas.hodiny,
                    pole[i].cas.minuty,
                    pole[i].cas.vteriny,
                    ztr.hodiny,
                    ztr.minuty,
                    ztr.vteriny);
        }
    }

    fclose(f);
}

int main(void) {
    ZAVODNIK *pole;
    int pocet;

    pole = otevriTo(&pocet);
    if (pole == NULL) return 1;

    vypis(pole, pocet);
    printf("\nCelkovy pocet zavodniku: %d\n", pocet);
    vypisNejmladsiho(pole, pocet);
    printf("Pocet ceskych zavodniku (CZE): %d\n", pocetCechu(pole, pocet));

    bubbleSort(pole, pocet);
    zapis(pole, pocet);

    free(pole);
    return 0;
}
