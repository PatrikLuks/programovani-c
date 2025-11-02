# 📚 Studijní Materiál - Příklady na Maturitu z Programování v C

---

## 🎯 Přehled Všech Příkladů

Máš 4 hlavní příklady zaměřené na **čtení dat ze souboru, jejich zpracování a zápis výsledků**:

1. **SLALOM** - Sportovní závod se dvěma kolami
2. **BĚH** - Závodníci s údaji o narozenich a národnosti
3. **CYKLISTIKA** - Horská kola s údaji o závodě
4. **JIZERSKÁ 50** - Lyžařský běh s více starty

---

## 1️⃣ SLALOM (NEJJEDNODUŠÍ)

### 📋 Zadání:
- Načíst data o závodníkovi (startovní pozice, jméno, příjmení, časy v kolech)
- Vypsat startovní listinu na obrazovku
- Vypsat listinu dle vzoru na obrazovku
- Napsat funkci pro jistý účet, který zjišťuje součet oběhu času
- Vytvořit textový soubor `vysledkova_listina.txt` dle vzoru
- Dodržet zasady strukturovaného programování

### 📁 Vstupní Data (`zavodnici.txt`):
```
1 Filimon Petteri 0:55.71 1:01.11
2 Gjorstai Johanna 0:56.08 1:02.51
...
```

### 🔧 Klíčové Struktury:
```c
typedef struct {
    int min;
    float sec;
} ROUNDS_DATA;

typedef struct {
    int startPosition;
    char surname[15];
    char firstname[15];
    ROUNDS_DATA firstRound;
    ROUNDS_DATA secRound;
} DATA;
```

### 💡 Postup:
1. **Otevřít a Načíst** - Čti soubor řádek po řádku
2. **Parsovat Data** - Rozděluj řádky podle delimiteru (mezera, dvojtečka)
3. **Vypočítat Časy** - Sečti oba kola a vypočítej rozdíl
4. **Seřadit** - Seřaď podle celkového času
5. **Vypsat** - Zapiš výsledky do souboru a na obrazovku

### ⚠️ Výzvy:
- Správné parsování datových typů (int vs float)
- Práce s dynamickými poli (`malloc`, `realloc`)
- Formatování výstupu (zarovnání sloupců)

---

## 2️⃣ BĚH (STŘEDNÍ)

### 📋 Zadání:
- Načíst data běžců (pozice, jméno, datum narození, národ, tým, čas)
- Kontrola vstupů a výstupů
- Vypsat startovní listinu
- Zjistitít a vypsat celkový počet závodníků
- Napsat funkci pro ziskání ročníku narození
- Vypsat řádku dle vysledkova_listina.txt
- Funkci pracující s časem
- Seřadítí závodníky podle dosažené času vzestupně

### 📁 Vstupní Data (`beh.txt`):
```
1;Goggia;Sofia;18.01.1990;CZE;Marathón klub;0:55.71;1:01.11
...
```

### 🔧 Klíčové Struktury:
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
    char tempBirthDate[DATA_LENGTH];
    char nation[4];
    char team[2 * DATA_LENGTH];
    TIME_FORMATED timeFormated;
    BIRTH_DATE birthdate;
} DATA;
```

### 💡 Postup:
1. **Dvoustupňové Parsování** - Nejdřív hlavní data, pak datum narození
2. **Konverze Datumu** - Změní "18.01.1990" na strukturu DATE
3. **Práce s Časem** - Převádí "0:55:71" na strukturu TIME
4. **Filtrování** - Hledá určité závodníky dle kritérií
5. **Třídění** - Seřadí podle času vzestupně
6. **Výstup** - Formátovaný zápis do souboru

### ⚠️ Výzvy:
- Dva různé delimitery (`;`, `:`, `.`)
- Konverze datových typů (string → int)
- Počítání věku z data narození
- Třídění pole struktur

---

## 3️⃣ CYKLISTIKA (STŘEDNÍ)

### 📋 Zadání:
- Načíst data cyklistů (pozice, jméno, hora narozeni, národ)
- Kontrola vstupů a výstupů
- Vypsat startovní listinu na obrazovku
- Zjistitít a vypsat celkový počet závodníků
- Nejstaršího závodníka (vypiště dle vzoru), věk závodníka zjistitít ze systémového času
- Napsat funkci, popř. více funkcí, které pracují s časem
- Seřaďit závodníky dle dosažené času vzestupně

### 📁 Vstupní Data (`cyklistika.txt`):
```
1;ROCZKOWSKI;Arthur;08.08.1965;POL;SSD Energetyk Rybnik;3:11:50
...
```

### 🔧 Klíčové Struktury:
```c
typedef struct {
    int hour, min, sec;
} FINISH_TIME;

typedef struct {
    int position;
    char surname[20];
    char firstname[20];
    int year;
    char nation[4];
    FINISH_TIME ft;
} DATA;
```

### 💡 Postup:
1. **Parsování** - Rozděluj podle `;` a `()`
2. **Čas Dokončení** - Převod "3:11:50" na strukturu
3. **Výpočet Věku** - Použi `time()` pro aktuální rok
4. **Hledání Nejstaršího** - Projdi všechny a najdi minimální rok
5. **Třídění** - Seřaď podle času

### ⚠️ Výzvy:
- Práce se systémovým časem (`time_t`, `struct tm`)
- Složitější parsování s více typy delimiters
- Správné výpočty věku přes roky

---

## 4️⃣ JIZERSKÁ 50 (NEJSLOŽITĚJŠÍ)

### 📋 Zadání:
- Načíst data lyžařů (pozice, jméno, rok, národ, start, čas)
- Kontrola vstupů a výstupů
- Vypsat startovní listinu
- Zjistitít a vypsat celkový počet závodníků
- Nejmlađšího závodníka (vypiště dle vzoru), věk závodníka zjistitít ze systémového času
- Napsat funkci, popř. více funkcí, které pracují s časem
- Seřaďit závodníky dle dosažené času vzestupně

### 📁 Vstupní Data (`jizerska50.txt`):
```
1|Elliasen,Petter|(1985);NOR;2:02:29
2|Oerbodaelen,Tor Arne|(1983);NOR;2:02:28
...
```

### 🔧 Klíčové Struktury:
```c
typedef struct {
    int hour, min, sec;
} FINISH_TIME;

typedef struct {
    int position;
    char surname[20];
    char firstname[20];
    int year;
    char nation[4];
    FINISH_TIME ft;
} DATA;
```

### 💡 Postup:
1. **Komplexní Parsování** - Více delimiters: `;`, `,`, `()`, `:`
2. **Extrahování Roku** - Z formátu "(1985)" → 1985
3. **Rozdělení Jména** - Z "Elliasen,Petter" → jméno a příjmení
4. **Čas** - Převod "2:02:29" na strukturu
5. **Nalezení Nejmladšího** - Najdi maximální rok
6. **Třídění** - Vzestupně podle času

### ⚠️ Výzvy:
- **Nejtěžší parsování** - Několik typů delimiters v různých polích
- Práce se znaky v závorkách
- Správné oddělení příjmení a jména

---

## 🎓 Obecný Postup pro Všechny Příklady

### Krok 1: PŘÍPRAVA
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "soubor.txt"
#define OUTPUT "vysledky.txt"
#define MAX_LINE 100
#define DELIMS " :"
```

### Krok 2: DEFINICE STRUKTUR
```c
typedef struct {
    // všechna pole ze souboru
} DATA;
```

### Krok 3: PARSOVÁNÍ (nejdůležitější!)
```c
DATA parseData(DATA data, char *storage) {
    char *token;
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, DELIMS)) != NULL; i++) {
        switch(i) {
            case 0: data.field1 = atoi(token); break;
            case 1: strcpy(data.field2, token); break;
            // ...
        }
    }
    return data;
}
```

### Krok 4: NAČÍTÁNÍ DAT
```c
DATA *fillData(int *total) {
    FILE *fr = fopen(INPUT, "r");
    DATA *resized = NULL;
    char storage[MAX_LINE];
    
    fgets(storage, MAX_LINE, fr); // Skip header
    
    while (fgets(storage, MAX_LINE, fr) != NULL) {
        resized = realloc(resized, (*total + 1) * sizeof(DATA));
        resized[*total] = parseData(resized[*total], storage);
        *total += 1;
    }
    
    fclose(fr);
    return resized;
}
```

### Krok 5: TŘÍDĚNÍ (Bubble Sort)
```c
void sortData(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            // Porovnání - záleží na kritériu
            if (compareData(data[j], data[j+1]) > 0) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}
```

### Krok 6: VÝSTUP
```c
void printResults(DATA *data, int total) {
    FILE *fw = fopen(OUTPUT, "w");
    
    for (int i = 0; i < total; i++) {
        fprintf(fw, "%d | %-15s | %-15s | ...\n", 
                data[i].position, data[i].surname, data[i].firstname);
    }
    
    fclose(fw);
}
```

---

## 🔑 Klíčové Funkce a Techniky

### 1. `strtok()` - Parsování Řetězců
```c
char *token;
char delimiters[] = " :";
for (int i = 0; (token = strtok(i == 0 ? line : NULL, delimiters)) != NULL; i++) {
    // i je index tokenu, token je aktuální část
}
```

### 2. `atoi()` vs `atof()` - Konverze Typů
```c
int number = atoi("123");      // String → int
float decimal = atof("123.45"); // String → float
```

### 3. `malloc()` a `realloc()` - Dynamická Paměť
```c
int *arr = malloc(sizeof(int) * 10);     // Alokace
arr = realloc(arr, sizeof(int) * 20);    // Zvětšení
free(arr);                                 // Uvolnění
```

### 4. `fprintf()` - Formátovaný Výstup
```c
fprintf(fw, "%d | %-15s | %02d:%02d\n", num, name, hour, min);
// %d = int, %-15s = left-aligned string, %02d = int s nulami
```

### 5. Práce s Časem
```c
#include <time.h>

time_t now = time(NULL);
struct tm *t = localtime(&now);
int year = 1900 + t->tm_year;  // Aktuální rok
```

---

## 📝 Příklady na Cvičení

### Cvičení 1: Jednoduché Parsování
Máš řetězec: `"5:Novak:Ivan:0:55.71:1:01.11"`

Cíl: Rozdělit ho do struktury DATA

```c
// Řešení:
DATA data;
char *token;
char delims[] = ":";
int idx = 0;

token = strtok("5:Novak:Ivan:0:55.71:1:01.11", delims);
while (token != NULL) {
    switch(idx) {
        case 0: data.startPosition = atoi(token); break;  // 5
        case 1: strcpy(data.surname, token); break;        // Novak
        case 2: strcpy(data.firstname, token); break;      // Ivan
        case 3: data.firstRound.min = atoi(token); break;  // 0
        case 4: data.firstRound.sec = atof(token); break;  // 55.71
        // ...
    }
    token = strtok(NULL, delims);
    idx++;
}
```

### Cvičení 2: Třídění Pole
Máš pole 5 závodníků, chceš je seřadit podle času.

```c
// Bubble Sort
void sortByTime(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            // Porovnání celkových časů
            float time1 = data[j].totalTime;
            float time2 = data[j+1].totalTime;
            
            if (time1 > time2) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}
```

---

## ⚠️ Časté Chyby

1. **Zapomenout Skip Header** - První řádek je popis sloupců!
2. **Nevalidní Parsování** - Špatné delimitery
3. **Memory Leak** - Neosvobodit paměť (`free()`)
4. **Buffer Overflow** - Příliš dlouhé jméno do krátkého pole
5. **Špatná Inicializace** - `resized = NULL` na začátku
6. **Zapomenutí `\n`** - V `fgets()` zůstává newline!

---

## 📚 Jak se Učit

1. **Nejdřív prostuduj SLALOM** - Nejjednoduší, naučíš se základy
2. **Pak BĚH** - Přidá se parsování datumu
3. **Pak CYKLISTIKA** - Práce se systémovým časem
4. **Nakonec JIZERSKÁ 50** - Nejsložitější kombinace všeho

**Pro Každý Příklad:**
- ✅ Přečti zadání v PDF
- ✅ Pochop vstupní data
- ✅ Naprogramuj parseData()
- ✅ Naprogramuj fillData()
- ✅ Vymysli třídění
- ✅ Naprogramuj výstup
- ✅ Otestuj na reálných datech

---

## 🚀 Tipy na Úspěch

✨ **Začni malým** - Nejdřív jen načti a vypiš data
✨ **Testuj průběžně** - Po každé funkci
✨ **Kombinuj kód** - Opakuj se, nepiš vše od nuly
✨ **Pečuj o paměť** - `malloc`, `realloc`, `free`
✨ **Formátuj výstup** - Aby byl čitelný
✨ **Dokumentuj kod** - Poznámky ti pomohou

---

**Hodně štěstí u maturity!** 💪
