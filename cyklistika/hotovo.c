/* main.c – MTB maraton – zpracovani souboru CYKLISTIKA.TXT
   podle zadani "Opakovani 2. rocniku – zpracovani textoveho souboru – prakticka proverka"
   Souhlas s VZOR.c stylem: nacteni -> vypis -> tridení -> zapis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VSTUP  "cyklistika.txt"
#define VYSTUP "vysledkova_listina.txt"
#define MAX 300

/* ----------------------------------------------------------
   STRUKTURY
   ---------------------------------------------------------- */
typedef struct {
    int hodiny;
    int minuty;
    int vteriny;
    int desetiny;   /* ve vstupu je jedna desetina, napr. 9:23:45.4 */
} CAS;

typedef struct {
    int cislo;
    char prijmeni[30];
    char jmeno[30];
    char narodnost[6];
    int rocnik;
    char klub[80];
    CAS cas;
} ZAVODNIK;

/* ----------------------------------------------------------
   FUNKCE PRO CAS
   ---------------------------------------------------------- */

/* prevod casu na desetiny sekund – kvuli trideni a rozdilum */
int casNaDesetiny(CAS c) {
    /* 1 hod = 3600 s = 36000 desetiny */
    return c.hodiny * 36000 + c.minuty * 600 + c.vteriny * 10 + c.desetiny;
}

/* z desetiny zpet na CAS – kvuli vypisu ztraty na viteze */
CAS desetinyNaCas(int d) {
    CAS c;
    if (d < 0) d = 0;
    c.hodiny = d / 36000;
    d = d % 36000;
    c.minuty = d / 600;
    d = d % 600;
    c.vteriny = d / 10;
    c.desetiny = d % 10;
    return c;
}

/* ----------------------------------------------------------
   NACITANI SOUBORU
   ---------------------------------------------------------- */
/* nacte CYKLISTIKA.TXT, preskoci hlavicku, vrati dynamicke pole */
ZAVODNIK *nactiSoubor(int *pocet) {
    FILE *f;
    char radek[MAX];
    int r = 0;
    ZAVODNIK *z = NULL;
    ZAVODNIK *tmp = NULL;

    f = fopen(VSTUP, "r");
    if (f == NULL) {
        printf("Soubor %s se nepodarilo otevrit.\n", VSTUP);
        return NULL;
    }

    /* 1. radek = nazvy sloupcu -> preskocit */
    if (fgets(radek, MAX, f) == NULL) {
        printf("Soubor je prazdny.\n");
        fclose(f);
        return NULL;
    }

    /* cteni radek po radku */
    while (fgets(radek, MAX, f) != NULL) {
        /* odstraneni \n */

        /* realokace pro dalsiho zavodnika */
        tmp = (ZAVODNIK *) realloc(z, (r + 1) * sizeof(ZAVODNIK));
        if (tmp == NULL) {
            printf("Chyba (re)alokace pameti.\n");
            free(z);
            fclose(f);
            return NULL;
        }
        z = tmp;

        /* parsovani – polozky jsou ODDELENE STREDNIKEM
           1 cislo
           2 prijmeni
           3 jmeno
           4 narodnost
           5 rocnik
           6 klub
           7 cas (h:mm:ss.d)
        */
        char *pch;
        int sloupec = 0;
        pch = strtok(radek, ";");
        while (pch != NULL) {
            sloupec++;
            switch (sloupec) {
                case 1:
                    z[r].cislo = atoi(pch);
                    break;
                case 2:
                    strcpy(z[r].prijmeni, pch);
                    break;
                case 3:
                    strcpy(z[r].jmeno, pch);
                    break;
                case 4:
                    strcpy(z[r].narodnost, pch);
                    break;
                case 5:
                    z[r].rocnik = atoi(pch);
                    break;
                case 6:
                    strcpy(z[r].klub, pch);
                    break;
                case 7: {
                    /* cas – tvar h:mm:ss.d */
                    char casStr[40];
                    char *pchCas;
                    int cast = 0;
                    strcpy(casStr, pch);
                    /* rozdelime podle ":" a "." */
                    pchCas = strtok(casStr, ":.");
                    while (pchCas != NULL) {
                        cast++;
                        switch (cast) {
                            case 1:
                                z[r].cas.hodiny = atoi(pchCas);
                                break;
                            case 2:
                                z[r].cas.minuty = atoi(pchCas);
                                break;
                            case 3:
                                z[r].cas.vteriny = atoi(pchCas);
                                break;
                            case 4:
                                z[r].cas.desetiny = atoi(pchCas);
                                break;
                        }
                        pchCas = strtok(NULL, ":.");
                    }
                    /* pojistka – kdyby nebyla desetina v souboru */
                    if (cast == 3) {
                        z[r].cas.desetiny = 0;
                    }
                    break;
                }
            }
            pch = strtok(NULL, ";");
        }

        r++;
    }

    if (fclose(f) == EOF) {
        printf("Soubor %s se nepodarilo zavrit.\n", VSTUP);
    }

    *pocet = r;
    return z;
}

/* ----------------------------------------------------------
   VYPIS STARTOVNI LISTINY – DLE VZORU
   ---------------------------------------------------------- */
void vypisStartovniListinu(ZAVODNIK *z, int pocet) {
    printf("M T B   M A R A T O N   -   S T A R T O V N I   L I S T I N A\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("cislo  prijmeni        jmeno           stat  rocnik  klub                                         cas\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < pocet; i++) {
        printf("%5d  %-14s %-14s %-4s  %6d  %-43s %2d:%02d:%02d.%1d\n",
               z[i].cislo,
               z[i].prijmeni,
               z[i].jmeno,
               z[i].narodnost,
               z[i].rocnik,
               z[i].klub,
               z[i].cas.hodiny,
               z[i].cas.minuty,
               z[i].cas.vteriny,
               z[i].cas.desetiny);
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("Pocet zavodniku: %d\n", pocet);
}

/* ----------------------------------------------------------
   NEJSTARSÍ ZAVODNIK – DLE VZORU
   ---------------------------------------------------------- */
void vypisNejstarsiho(ZAVODNIK *z, int pocet) {
    if (pocet == 0) return;

    /* zjisteni aktualniho roku ze systemu */
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int rok = tm_info->tm_year + 1900;

    int idx = 0;
    for (int i = 1; i < pocet; i++) {
        if (z[i].rocnik < z[idx].rocnik) {
            idx = i;
        }
    }

    int vek = rok - z[idx].rocnik;

    printf("N E J S T A R S I   Z A V O D N I K\n");
    printf("-------------------------------------------------\n");
    printf("cislo  prijmeni        jmeno           stat  rocnik  vek  klub                               cas\n");
    printf("-------------------------------------------------\n");
    printf("%5d  %-14s %-14s %-4s  %6d  %3d  %-32s %2d:%02d:%02d.%1d\n",
           z[idx].cislo,
           z[idx].prijmeni,
           z[idx].jmeno,
           z[idx].narodnost,
           z[idx].rocnik,
           vek,
           z[idx].klub,
           z[idx].cas.hodiny,
           z[idx].cas.minuty,
           z[idx].cas.vteriny,
           z[idx].cas.desetiny);
}

/* ----------------------------------------------------------
   POCTY ZAVODNIKU DLE NARODNOSTI – DLE VZORU
   ---------------------------------------------------------- */
void vypisPocetNarodnosti(ZAVODNIK *z, int pocet) {
    char narodnosti[50][6];  /* pole kódů národností */
    int pocty[50];           /* pole počtů */
    int pocetNar = 0;

    for (int i = 0; i < pocet; i++) {
        int found = 0;
        for (int j = 0; j < pocetNar; j++) {
            if (strcmp(narodnosti[j], z[i].narodnost) == 0) {
                pocty[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(narodnosti[pocetNar], z[i].narodnost);
            pocty[pocetNar] = 1;
            pocetNar++;
        }
    }

    printf("\nP O C E T   Z A V O D N I K U   D L E   N A R O D N O S T I\n");
    printf("-----------------------------------------------------------\n");
    printf("stat  pocet\n");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < pocetNar; i++) {
        printf("%-4s  %5d\n", narodnosti[i], pocty[i]);
    }
}

/* ----------------------------------------------------------
   TRIDENI – bubble sort jako ve vzoru
   ---------------------------------------------------------- */
void prohod(ZAVODNIK *a, ZAVODNIK *b) {
    ZAVODNIK tmp = *a;
    *a = *b;
    *b = tmp;
}

void seradPodleCasu(ZAVODNIK *z, int pocet) {
    int i, j, swapped;
    for (i = 0; i < pocet - 1; i++) {
        swapped = 0;
        for (j = 0; j < pocet - i - 1; j++) {
            if (casNaDesetiny(z[j].cas) > casNaDesetiny(z[j + 1].cas)) {
                prohod(&z[j], &z[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}

/* ----------------------------------------------------------
   ZAPIS VYSLEDKOVE LISTINY – DLE VZORU
   ---------------------------------------------------------- */
void zapisVysledkovaListina(ZAVODNIK *z, int pocet) {
    FILE *f = fopen(VYSTUP, "w");
    if (f == NULL) {
        printf("Soubor %s se nepodarilo otevrit pro zapis.\n", VYSTUP);
        return;
    }

    fprintf(f, "M T B   M A R A T O N   -   V Y S L E D K O V A   L I S T I N A\n");
    fprintf(f, "----------------------------------------------------------------------------------------------\n");
    fprintf(f, "por.  cislo  prijmeni        jmeno           stat  rocnik  klub                                   cas          ztrata\n");
    fprintf(f, "----------------------------------------------------------------------------------------------\n");

    if (pocet == 0) {
        fclose(f);
        return;
    }

    /* cas viteze – prvni po trideni */
    int casPrvniho = casNaDesetiny(z[0].cas);

    for (int i = 0; i < pocet; i++) {
        int casZav = casNaDesetiny(z[i].cas);
        int rozdil = casZav - casPrvniho;
        CAS ztrata = desetinyNaCas(rozdil);

        fprintf(f, "%3d. %5d  %-14s %-14s %-4s  %6d  %-37s %2d:%02d:%02d.%1d   %2d:%02d:%02d.%1d\n",
                i + 1,
                z[i].cislo,
                z[i].prijmeni,
                z[i].jmeno,
                z[i].narodnost,
                z[i].rocnik,
                z[i].klub,
                z[i].cas.hodiny,
                z[i].cas.minuty,
                z[i].cas.vteriny,
                z[i].cas.desetiny,
                ztrata.hodiny,
                ztrata.minuty,
                ztrata.vteriny,
                ztrata.desetiny);
    }

    if (fclose(f) == EOF) {
        printf("Soubor %s se nepodarilo zavrit.\n", VYSTUP);
    }
}

/* ----------------------------------------------------------
   MAIN – plni body 1–10 zadani
   ---------------------------------------------------------- */
int main(void) {
    ZAVODNIK *zavodnici;
    int pocet = 0;

    /* 1 + 2: nacteni + kontrola I/O */
    zavodnici = nactiSoubor(&pocet);
    if (zavodnici == NULL) {
        /* chybove hlaseni uz bylo vypsane */
        return 1;
    }

    /* 3: vypis startovni listiny dle vzoru */
    vypisStartovniListinu(zavodnici, pocet);

    /* 4: zjištění celkového počtu – už je vypsáno dole ve funkci */

    /* 5: nejstarsi zavodnik */
    vypisNejstarsiho(zavodnici, pocet);

    /* 6: pocty dle narodnosti */
    vypisPocetNarodnosti(zavodnici, pocet);

    /* 7: funkce pro cas – uz pouzito vyse */

    /* 8: setridit zavodniky dle dosaženeho casu vzestupne */
    seradPodleCasu(zavodnici, pocet);

    /* 9: vystupni soubor s vypoctem ztraty na prvniho */
    zapisVysledkovaListina(zavodnici, pocet);

    /* 10: strukturovane programovani – hotovo */

    free(zavodnici);
    return 0;
}