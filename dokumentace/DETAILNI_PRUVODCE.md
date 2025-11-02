# 📖 DETAILNÍ PRŮVODCE - Konkrétní Implementace Příkladů

## 1. SLALOM - Nejjednoduší příklad

### 📊 Struktura Dat

```
INPUT: zavodnici.txt
1 Vonn Lindsey 0:55.65 0:59.45
2 Goggia Sofia 0:55.71 1:01.11
...

OUTPUT: vysledky_olympiady.txt
- seřazené dle celkového času
- se ztrátou vůči vítězi
```

### 🔍 Parsování - NEJDŮLEŽITĚJŠÍ ČÁST!

Máš řádek: `"1 Vonn Lindsey 0:55.65 0:59.45"`

Potřebuješ ho rozdělit podle **mezer a dvojteček**.

```c
// ŠPATNĚ - Neošetřeš双tečky
char delims[] = " ";  // Chybí ":"!
token = strtok(line, delims);

// SPRÁVNĚ - Všechny separátory
char delims[] = " :";
token = strtok(line, delims);
```

### ✅ Kompletní Parsování Funkce

```c
DATA parseData(DATA data, char *storage) {
    char *token;
    
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, " :")) != NULL; i++) {
        switch(i) {
            case 0:
                // 1. token = "1"
                data.startPosition = atoi(token);
                printf("Pozice: %d\n", data.startPosition);
                break;
            case 1:
                // 2. token = "Vonn"
                strcpy(data.surname, token);
                printf("Příjmení: %s\n", data.surname);
                break;
            case 2:
                // 3. token = "Lindsey"
                strcpy(data.firstname, token);
                printf("Jméno: %s\n", data.firstname);
                break;
            case 3:
                // 4. token = "0"
                data.firstRound.min = atoi(token);
                printf("1. kolo - minuty: %d\n", data.firstRound.min);
                break;
            case 4:
                // 5. token = "55"
                data.firstRound.sec = atof(token);
                printf("1. kolo - sekundy: %.2f\n", data.firstRound.sec);
                break;
            case 5:
                // 6. token = "0"
                data.secRound.min = atoi(token);
                break;
            case 6:
                // 7. token = "59"
                data.secRound.sec = atof(token);
                break;
            default:
                exit(-999);  // Chyba - více tokenů než očekáváno
        }
    }
    return data;
}
```

### 🧮 Konverze Času na Sekundy

Pro správné třídění potřebuješ čas v jednotné jednotce.

```c
// SLALOM: Součet obou kol
float convertTime(ROUNDS_DATA firstR, ROUNDS_DATA secondR) {
    // Sečti obě kola: (0:55.71) + (1:01.11)
    // = (0 min 55.71 sec) + (1 min 1.11 sec)
    // = 55.71 + 60 + 1.11 = 116.82 sec
    
    return (float)(firstR.min + secondR.min) * 60 + (firstR.sec + secondR.sec);
}
```

### 🔄 Bubble Sort - Třídění

```c
void bubbleSort(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        int swapped = 0;
        
        for (int j = 0; j < total - 1 - i; j++) {
            // Porovnání časů
            float time1 = convertTime(data[j].firstRound, data[j].secRound);
            float time2 = convertTime(data[j+1].firstRound, data[j+1].secRound);
            
            // Výměna, pokud je čas j větší
            if (time1 > time2) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
                swapped++;
            }
        }
        
        // Optimalizace - jestli nedošlo k výměně, pole je seřazeno
        if (swapped == 0) break;
    }
}
```

### 📤 Výstup do Souboru

```c
void fprintOut(DATA *data, int total) {
    FILE *fw = fopen("vysledky_olympiady.txt", "w");
    
    fprintf(fw, "V Y S L E D K O V A  L I S T I N A\n");
    fprintf(fw, "poradi | cislo | prijmeni | jmeno | 1. kolo | 2. kolo | celkem | ztrata\n");
    
    for (int i = 0; i < total; i++) {
        float totalTime = convertTime(data[i].firstRound, data[i].secRound);
        float winnerTime = convertTime(data[0].firstRound, data[0].secRound);
        float loss = totalTime - winnerTime;
        
        fprintf(fw, "%2d | %2d | %s | %s | %d:%.2f | %d:%.2f | %.2f | ",
            i+1, data[i].startPosition, data[i].surname, data[i].firstname,
            data[i].firstRound.min, data[i].firstRound.sec,
            data[i].secRound.min, data[i].secRound.sec,
            totalTime);
        
        // Vítěz nemá ztrátu
        if (i == 0) {
            fprintf(fw, "-\n");
        } else {
            fprintf(fw, "+%.2f\n", loss);
        }
    }
    
    fclose(fw);
}
```

---

## 2. BĚH - Střední Složitost

### 📊 Vstupní Data

```
Input: beh.txt
st.c;prijmeni;jmeno;narozeni;stat;oddil;cas
1;ZUREK;Jiri;28.02.1983;CZE; JOF-X Team;3:33:09
```

### ⚠️ Rozdíl od SLALOMU

**3 komplexnosti navíc:**
1. Dvě **různé delimitery** (`;` a `.`)
2. Datum ve formátu `DD.MM.YYYY`
3. Čas ve formátu `HH:MM:SS` (ne jen MM:SS)

### 🔍 Dvoustupňové Parsování

```c
typedef struct {
    int hour, min, sec;
} TIME_FORMATED;

typedef struct {
    int year, month, day;
} BIRTH_DATE;

typedef struct {
    int position;
    char surname[DATA_LENGTH];
    char firstname[DATA_LENGTH];
    char tempBirthDate[DATA_LENGTH];  // Dočasné uložení "28.02.1983"
    char nation[4];
    char team[2 * DATA_LENGTH];
    TIME_FORMATED timeFormated;
    BIRTH_DATE birthdate;
} DATA;
```

### 📋 Parsování - Dvě Fáze

```c
DATA parseData(DATA data, char *storage) {
    char *token;
    
    // FÁZE 1: Parsování hlavních polí (delim ";")
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, ";:")) != NULL; i++) {
        switch(i) {
            case 0: data.position = atoi(token); break;              // 1
            case 1: strcpy(data.surname, token); break;              // ZUREK
            case 2: strcpy(data.firstname, token); break;            // Jiri
            case 3: strcpy(data.tempBirthDate, token); break;        // 28.02.1983
            case 4: strcpy(data.nation, token); break;               // CZE
            case 5: strcpy(data.team, token); break;                 // JOF-X Team
            case 6: data.timeFormated.hour = atoi(token); break;     // 3
            case 7: data.timeFormated.min = atoi(token); break;      // 33
            case 8: data.timeFormated.sec = atoi(token); break;      // 09
        }
    }
    
    // FÁZE 2: Parsování data narození (delim ".")
    char *bDayToken;
    for (int j = 0; (bDayToken = strtok(j == 0 ? data.tempBirthDate : NULL, ".")) != NULL; j++) {
        switch(j) {
            case 0: data.birthdate.day = atoi(bDayToken); break;     // 28
            case 1: data.birthdate.month = atoi(bDayToken); break;   // 02
            case 2: data.birthdate.year = atoi(bDayToken); break;    // 1983
        }
    }
    
    return data;
}
```

### ⏰ Práce se Systémovým Časem

```c
#include <time.h>

int getTime() {
    time_t seconds = time(NULL);           // Počet sekund od 1.1.1970
    struct tm *localTime = localtime(&seconds);  // Převod na kalendářní čas
    return localTime->tm_year + 1900;      // Aktuální rok
}

// Příklad:
// Dnes je 1. listopadu 2025
// int current_year = getTime();  // Vrátí 2025
// Data: 28.02.1983
// Věk: 2025 - 1983 = 42 let
```

### 🔍 Hledání Nejstaršího Závodníka

```c
int findTheOldest(DATA *data, int total) {
    int index = 0;
    
    for (int i = 1; i < total; i++) {
        // Nejdřív porovnáš rok
        if (data[index].birthdate.year > data[i].birthdate.year) {
            // i je starší (menší rok = dřívější narozeniny)
            index = i;
        }
        // Pokud stejný rok, porovnáš měsíc
        else if (data[index].birthdate.year == data[i].birthdate.year) {
            if (data[index].birthdate.month > data[i].birthdate.month) {
                index = i;
            }
            // Pokud stejný měsíc, porovnáš den
            else if (data[index].birthdate.month == data[i].birthdate.month) {
                if (data[index].birthdate.day > data[i].birthdate.day) {
                    index = i;
                }
            }
        }
    }
    
    return index;
}
```

### 📊 Konverze Času na Sekundy

```c
int convertTime(TIME_FORMATED timeForm) {
    // 3:33:09 → 3*3600 + 33*60 + 9
    //        = 10800 + 1980 + 9
    //        = 12789 sekund
    
    return timeForm.hour * 3600 + timeForm.min * 60 + timeForm.sec;
}

// Třídění dle času vzestupně
void bubbleSort(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (convertTime(data[j].timeFormated) > convertTime(data[j+1].timeFormated)) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}
```

---

## 3. CYKLISTIKA - Střední Složitost

### 📊 Vstupní Data

```
Input: cyklistika.txt
cislo;Prijmeni;Jmeno;Narodnost;Rocnik;Klub;Cas
5001;FOJTIK;Jan;CZE;1988;FORCE KCK;9:23:45.4
```

### 🔧 Klíčový Rozdíl

- Čas s **desetinným oddělovačem**: `9:23:45.4`
- Rok je přímé číslo (ne datum)
- Delimitery: `;`, `:`, `.`

### 📋 Parsování

```c
DATA parseData(DATA data, char *storage) {
    char *token;
    
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, ";:.(")) != NULL; i++) {
        switch(i) {
            case 0: data.position = atoi(token); break;    // 5001
            case 1: strcpy(data.surname, token); break;    // FOJTIK
            case 2: strcpy(data.firstname, token); break;  // Jan
            case 3: strcpy(data.nation, token); break;     // CZE
            case 4: data.year = atoi(token); break;        // 1988
            case 5: strcpy(data.club, token); break;       // FORCE KCK
            case 6: data.ft.hour = atoi(token); break;     // 9
            case 7: data.ft.min = atoi(token); break;      // 23
            case 8: data.ft.sec = atoi(token); break;      // 45
            // 9. token by byl "4" (desetinná část), kterou ignorujeme
        }
    }
    
    return data;
}
```

### 🔄 Hledání Nejstaršího (Největšího Roku)

```c
int findTheYoungest(DATA *data, int total) {
    int index = 0;
    
    for (int i = 1; i < total; i++) {
        // Nejvyšší rok = nejmladší
        if (data[i].year > data[index].year) {
            index = i;
        }
    }
    
    return index;
}

// Výstup:
// Nejmladším závodníkem je VACLAVEK Lukas (1991, věk: 2025-1991 = 34 let)
```

---

## 4. JIZERSKÁ 50 - Nejsložitější!

### 📊 Vstupní Data

```
Input: jizerska50.txt
st. cislo.|prijmeni, jmeno (rocnik);narodnost;cas
16;Eliassen, Petter (1985);NOR;2:02:29
17;Gjerdalen, Tord Asle (1983);NOR;2:02:28
```

### ⚠️ Nejtěžší Parsování!

Máš tyto výzvy:
1. **Jméno a příjmení** jsou oddělena **čárkou** a **mezerou**: `"Eliassen, Petter"`
2. **Rok** je v **závorkách**: `(1985)`
3. **Čas** je ve formátu: `2:02:29`
4. **Více delimiters**: `;`, `,`, `()`, `:`

### 🔍 Krok-Po-Kroku Parsování

```c
DATA parseData(DATA data, char *storage) {
    char *token;
    
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, ";,()")) != NULL; i++) {
        switch(i) {
            case 0:
                // "16"
                data.position = atoi(token);
                break;
            case 1:
                // "Eliassen" (bez mezer!)
                // Musíš odstranit úvodní mezer
                while (*token == ' ') token++;  // Přeskoč mezery
                strcpy(data.surname, token);
                break;
            case 2:
                // " Petter" (s mezerou na začátku!)
                while (*token == ' ') token++;  // Přeskoč mezery
                strcpy(data.firstname, token);
                break;
            case 3:
                // "1985"
                data.year = atoi(token);
                break;
            case 4:
                // "NOR"
                while (*token == ' ') token++;  // Přeskoč mezery
                strcpy(data.nation, token);
                break;
            case 5:
                // "2"
                data.ft.hour = atoi(token);
                break;
            case 6:
                // "02"
                data.ft.min = atoi(token);
                break;
            case 7:
                // "29"
                data.ft.sec = atoi(token);
                break;
        }
    }
    
    return data;
}
```

### 🎯 Odvozené Operace

```c
// Konverze času
int convertTime(FINISH_TIME ft) {
    return ft.hour * 3600 + ft.min * 60 + ft.sec;
}

// Najít nejstaršího (nejmenší rok)
int findTheOldest(DATA *data, int total) {
    int index = 0;
    for (int i = 1; i < total; i++) {
        if (data[i].year < data[index].year) {  // Menší rok = starší
            index = i;
        }
    }
    return index;
}

// Třídění dle času
void bubbleSort(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (convertTime(data[j].ft) > convertTime(data[j+1].ft)) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}
```

---

## 🧠 Srovnání Všech 4 Příkladů

| Vlastnost | SLALOM | BĚH | CYKLISTIKA | JIZERSKÁ 50 |
|-----------|--------|-----|-----------|------------|
| **Delimiters** | ` :` | `;:.` | `;:.` | `;,():` |
| **Parsování** | 1 fáze | 2 fáze | 1 fáze | 1 fáze (ale složitá) |
| **Čas** | MM:SS.SS | HH:MM:SS | HH:MM:SS.S | HH:MM:SS |
| **Datum** | Žádné | DD.MM.YYYY | Jen rok | Jen rok |
| **Složitost** | ⭐ | ⭐⭐ | ⭐⭐ | ⭐⭐⭐ |

---

## 💪 Praktická Cvičení

### Cvičení 1: Parsování SLALOMU
**Úkol:** Parsuj řádek bez chyb
```
Vstup:  "5 Novak Ivan 0:55.71 1:01.11"
Výstup: startPosition=5, surname="Novak", firstname="Ivan", 
        firstRound.min=0, firstRound.sec=55.71, ...
```

### Cvičení 2: Parsování BĚH s Datem
**Úkol:** Správně parsuj datum a čas
```
Vstup:  "1;ZUREK;Jiri;28.02.1983;CZE;Team;3:33:09"
Výstup: birthdate.day=28, birthdate.month=02, birthdate.year=1983
        timeFormated.hour=3, timeFormated.min=33, timeFormated.sec=09
```

### Cvičení 3: JIZERSKÁ 50 - Nejtěžší
**Úkol:** Správně parsuj jméno v závorkách
```
Vstup:  "16;Eliassen, Petter (1985);NOR;2:02:29"
Výstup: surname="Eliassen", firstname="Petter", year=1985
        ft.hour=2, ft.min=02, ft.sec=29
```

---

## ✨ Tipy na Implementaci

**1. Vytvor si testovací main()**
```c
int main() {
    DATA test;
    test = parseData(test, "1 Vonn Lindsey 0:55.65 0:59.45");
    
    printf("Pozice: %d\n", test.startPosition);      // 1
    printf("Příjmení: %s\n", test.surname);          // Vonn
    printf("Jméno: %s\n", test.firstname);           // Lindsey
    printf("1. kolo: %d:%.2f\n", test.firstRound.min, test.firstRound.sec);
    
    return 0;
}
```

**2. Debuguj strtok()**
```c
char line[] = "1 Vonn Lindsey 0:55.65 0:59.45";
char *token;
int idx = 0;

for (int i = 0; (token = strtok(i == 0 ? line : NULL, " :")) != NULL; i++) {
    printf("Token %d: '%s'\n", i, token);
}
```

**3. Kontroluj délky stringů**
```c
char surname[15];
strcpy(surname, "VeryLongNameThatDoesNotFit");  // Buffer overflow!
// Řešení: strncpy(surname, token, 14);
```

---

## 🎓 Závěrem

**Postup Učení:**
1. ✅ SLALOM - Zvládni základní parsování
2. ✅ BĚH - Přidej datum a dvou-fázové parsování
3. ✅ CYKLISTIKA - Pracuj se systémovým časem
4. ✅ JIZERSKÁ 50 - Zvládni nejsložitější parsování

Každý příklad je **stavební blok** - vždy jej můžeš „zkopírovat" a upravit pro další případ!

**Vždy testuj průběžně** - po parsování, po třídění, před výstupem.

Hodně štěstí! 🚀
