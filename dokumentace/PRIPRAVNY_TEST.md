# 🧪 PŘÍPRAVNÝ TEST - Kontrola Vědomostí

## Test 1: SLALOM - Základy

### Otázka 1.1
Máš řádek: `"5 Novak Ivan 0:55.71 1:01.11"`

Jaké delimitery potřebuješ v `strtok()`?
```
A) " "
B) ":"
C) " :"  ✓ SPRÁVNĚ
D) "-"
```

### Otázka 1.2
Jaké jsou jednotlivé tokeny v pořadí?
```
Token 0: "5"          → startPosition
Token 1: "Novak"      → surname
Token 2: "Ivan"       → firstname
Token 3: "0"          → firstRound.min
Token 4: "55"         → firstRound.sec
Token 5: "71"         → Chyba! Mělo by být "1" (secRound.min)!
```

**Správné rozdělení:**
```
Delim " :": [ "5", "Novak", "Ivan", "0", "55.71", "1", "01.11" ]
```

### Otázka 1.3
Jak vypočítáš celkový čas v sekundách?

**Vstup:** firstRound: 0:55.71, secRound: 1:01.11
```
float total = (0 + 1) * 60 + (55.71 + 1.11) = 60 + 56.82 = 116.82 sekund
```

---

## Test 2: BĚH - Datum a Čas

### Otázka 2.1
Máš řádek: `"1;ZUREK;Jiri;28.02.1983;CZE;Team;3:33:09"`

Jaké delimitery potřebuješ?
```
A) ";"
B) ";:"
C) ";;:"
D) ";:."  ✓ SPRÁVNĚ (pro případ číslovaných dat)
```

### Otázka 2.2
Jak správně naparsovati datum "28.02.1983"?

**ŠPATNĚ:**
```c
char *token = strtok("28.02.1983", ".");
data.day = atoi(token);           // 28 ✓
token = strtok(NULL, ".");
data.month = atoi(token);         // 02 ✓
token = strtok(NULL, ".");
data.year = atoi(token);          // 1983 ✓
```

**SPRÁVNĚ:** Stejně, ale musíš uložit "28.02.1983" do `tempBirthDate` a pak zparsovat!

### Otázka 2.3
Jak správně porovnat dva dny narození?

```c
// Chci najít nejstaršího (nejmenší rok)
if (data[0].year > data[1].year) {
    // data[1] je starší
}
// Pokud stejný rok, porovnám měsíc
else if (data[0].year == data[1].year) {
    if (data[0].month > data[1].month) {
        // data[1] je starší
    }
}
```

### Otázka 2.4
Jak vypočítáš věk v C?

```c
#include <time.h>

int getYear() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    return 1900 + t->tm_year;
}

int age = getYear() - birth_year;  // 2025 - 1983 = 42
```

---

## Test 3: CYKLISTIKA - Pokročilejší

### Otázka 3.1
Máš řádek: `"5001;FOJTIK;Jan;CZE;1988;FORCE KCK;9:23:45.4"`

Co znamená `.4` na konci?

```
Odpověď: Desetinná část sekundy (0.4 sekund)
Parsování:
- "9"  → hour
- "23" → min
- "45" → sec
- "4"  → desetinná část (kterou můžeš ignorovat)
```

### Otázka 3.2
Jak správně parsovat čas s desetinou?

```c
DATA parseData(DATA data, char *storage) {
    char *token;
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, ";:.(")) != NULL; i++) {
        switch(i) {
            // ... dalších 5 polí
            case 6: data.ft.hour = atoi(token); break;  // 9
            case 7: data.ft.min = atoi(token); break;   // 23
            case 8: data.ft.sec = atoi(token); break;   // 45
            // case 9: ignoruj desetinku
        }
    }
    return data;
}
```

### Otázka 3.3
Jak vypočítáš věk cyklisty narozeného v roce 1988?

```
age = 2025 - 1988 = 37 let
```

---

## Test 4: JIZERSKÁ 50 - Nejtěžší

### Otázka 4.1
Máš řádek: `"16;Eliassen, Petter (1985);NOR;2:02:29"`

Tokenizuj to na jednotlivé části:
```
Delim ";,()": 
Token 0: "16"                → position
Token 1: "Eliassen"          → surname (ale s úvodní mezerou!)
Token 2: " Petter "          → firstname (s mezerami!)
Token 3: "1985"              → year
Token 4: "NOR"               → nation (s úvodní mezerou!)
Token 5: "2"                 → ft.hour
Token 6: "02"                → ft.min
Token 7: "29"                → ft.sec
```

### Otázka 4.2
Jaký je problém s tokenem " Petter " (s mezerami)?

```c
// ŠPATNĚ:
strcpy(data.firstname, " Petter ");  // Uloží mezery!

// SPRÁVNĚ: Odstraň mezery
char *token = " Petter ";
while (*token == ' ') token++;  // Přeskoč počáteční mezery
strcpy(data.firstname, token);
```

### Otázka 4.3
Jak konvertuješ čas `2:02:29` na sekundy?

```
2 * 3600 + 02 * 60 + 29 = 7200 + 120 + 29 = 7349 sekund
```

### Otázka 4.4
Jak najdeš nejstaršího lyžaře?

```c
int findTheOldest(DATA *data, int total) {
    int index = 0;
    
    for (int i = 1; i < total; i++) {
        if (data[i].year < data[index].year) {  // Menší rok = starší!
            index = i;
        }
    }
    
    return index;  // Index nejstaršího
}
```

---

## Test 5: Praktické Úkoly

### Úkol 5.1: Parsování SLALOMU
**Zadání:** Naprogramuj `parseData()` pro SLALOM

**Vstup:** `"1 Vonn Lindsey 0:55.65 0:59.45"`

**Výstup na obrazovku:**
```
startPosition: 1
surname: Vonn
firstname: Lindsey
firstRound: 0:55.65
secRound: 0:59.45
totalTime: 115.10 sekund
```

**Řešení:**
```c
DATA parseData(DATA data, char *storage) {
    char *token;
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, " :")) != NULL; i++) {
        switch(i) {
            case 0: data.startPosition = atoi(token); break;
            case 1: strcpy(data.surname, token); break;
            case 2: strcpy(data.firstname, token); break;
            case 3: data.firstRound.min = atoi(token); break;
            case 4: data.firstRound.sec = atof(token); break;
            case 5: data.secRound.min = atoi(token); break;
            case 6: data.secRound.sec = atof(token); break;
        }
    }
    return data;
}
```

### Úkol 5.2: Parsování BĚH s Datem
**Zadání:** Naprogramuj parsování data

**Vstup:** `"28.02.1983"`

**Výstup:**
```
day: 28
month: 02
year: 1983
age: 42
```

**Řešení:**
```c
char date[] = "28.02.1983";
char *token;
BIRTH_DATE birth;

for (int j = 0; (token = strtok(j == 0 ? date : NULL, ".")) != NULL; j++) {
    switch(j) {
        case 0: birth.day = atoi(token); break;
        case 1: birth.month = atoi(token); break;
        case 2: birth.year = atoi(token); break;
    }
}

int age = 2025 - birth.year;
```

### Úkol 5.3: Bubble Sort pro SLALOM
**Zadání:** Seřaď 3 závodníky dle času

**Data:**
```
1. Vonn:      115.10 sekund
2. Goggia:    116.82 sekund
3. Schnarf:   118.59 sekund
```

**Očekávaný Výstup (již seřazený):**
```
1. Vonn:      115.10 sekund (1. místo)
2. Goggia:    116.82 sekund (2. místo, +1.72)
3. Schnarf:   118.59 sekund (3. místo, +3.49)
```

**Řešení:** Viz Cheat Sheet - Bubble Sort

### Úkol 5.4: Hledání Nejstaršího v BĚH
**Zadání:** Z 5 běžců najdi nejstaršího

**Data:**
```
1. ZUREK:     28.02.1983
2. JURICA:    26.05.1979 ← Nejstarší!
3. TOMAI:     06.08.1976
4. ROCZKOWSKI: 08.08.1965 ← Nejstarší!
5. VYTISK:    01.11.1949 ← Nejstarší!
```

**Odpověď:** VYTISK (1949)

---

## Test 6: Složité Situace

### Otázka 6.1: Co se stane?

```c
char name[5];
strcpy(name, "PatrikAKontinuace");  // Více než 5 znaků!
```

**Odpověď:** **Buffer Overflow!** Paměť se přepíše.

**Řešení:**
```c
strncpy(name, token, 4);  // Max 4 znaky
name[4] = '\0';           // Konec stringu
```

### Otázka 6.2: Co je špatně?

```c
DATA *data;  // Bez inicializace!
data = realloc(data, sizeof(DATA));  // Undefined behavior!
```

**Odpověď:** Musíš inicializovat na `NULL`

**Správně:**
```c
DATA *data = NULL;
data = realloc(data, sizeof(DATA));
```

### Otázka 6.3: Jak odebrát newline?

```c
char line[100];
fgets(line, 100, fr);  // Obsahuje "\n" na konci!
```

**Řešení:**
```c
line[strcspn(line, "\n")] = 0;  // Odstraň newline
```

### Otázka 6.4: Porovnání Stringů

```c
if (nation == "CZE") {  // ŠPATNĚ!
    // ...
}

if (strcmp(nation, "CZE") == 0) {  // SPRÁVNĚ!
    // ...
}
```

---

## 📊 Vyhodnocení Testu

**Počet správných odpovědí:**

- **0-5:** 😰 Znovu si projdi SLALOM a DETAILNÍ PRŮVODCE
- **6-10:** 😐 Máš základy, pracuj na DETAILÍCH
- **11-15:** 😊 Solidní! Jsi na správné cestě
- **16-20:** 🎉 Excellent! Jsi připraven/a na maturitu!

---

## 💡 Tipy pro Test

1. ✅ **Vzdy testuj parsování** - Tiskni první 5 tokenů
2. ✅ **Kontroluj delimitery** - Sont všichni v `strtok()`?
3. ✅ **Počítej s newlinem** - Odstraň `\n` z `fgets()`
4. ✅ **Inicializuj na NULL** - `DATA *data = NULL;`
5. ✅ **Používej `strcpy` bezpečně** - Raději `strncpy`
6. ✅ **Debug printfem** - Vytiskni všechny kroky

---

## 🎓 Závěrečný Checklist

Předtím, než půjdeš k maturitě:

- [ ] Umím parsovat jakýkoliv řetězec se `strtok()`
- [ ] Umím konvertovat časy do sekund
- [ ] Umím naprogramovat bubble sort
- [ ] Umím pracovat s datumem a věkem
- [ ] Umím bezpečně pracovat s pamětí (`malloc`, `realloc`, `free`)
- [ ] Umím psát do souboru `fprintf()`
- [ ] Umím najít minimum a maximum v poli
- [ ] Znám všechny 4 příklady (SLALOM, BĚH, CYKLISTIKA, JIZERSKÁ)

Pokud máš ✅ u všeho, jsi **PŘIPRAVEN/A NA MATURITU!** 🚀

---

Hodně štěstí! 💪
