# ⚡ CHEAT SHEET - Rychlá Referenční Příručka

## 🎯 Šablona Pro Všechny 4 Příklady

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// KROK 1: Definuj Struktury
typedef struct {
    int hour, min, sec;
} TIME_DATA;

typedef struct {
    int day, month, year;
} DATE_DATA;

typedef struct {
    int position;
    char surname[20];
    char firstname[20];
    // Přidej další pole...
} DATA;

// KROK 2: Parsování
DATA parseData(DATA data, char *storage) {
    char *token;
    for (int i = 0; (token = strtok(i == 0 ? storage : NULL, "DELIMITERS")) != NULL; i++) {
        switch(i) {
            case 0: data.position = atoi(token); break;
            case 1: strcpy(data.surname, token); break;
            // ... další případy
        }
    }
    return data;
}

// KROK 3: Načtení Dat
DATA *fillData(int *total) {
    FILE *fr = fopen("INPUT.txt", "r");
    DATA *resized = NULL;
    char storage[100];
    
    fgets(storage, 100, fr);  // Skip header
    
    while (fgets(storage, 100, fr) != NULL) {
        resized = realloc(resized, (*total + 1) * sizeof(DATA));
        resized[*total] = parseData(resized[*total], storage);
        *total += 1;
    }
    
    fclose(fr);
    return resized;
}

// KROK 4: Třídění (Bubble Sort)
void bubbleSort(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (COMPARISON_CONDITION) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}

// KROK 5: Výstup
void printResults(DATA *data, int total) {
    FILE *fw = fopen("OUTPUT.txt", "w");
    for (int i = 0; i < total; i++) {
        fprintf(fw, "%d | %s | %s\n", data[i].position, 
                data[i].surname, data[i].firstname);
    }
    fclose(fw);
}

int main() {
    int total = 0;
    DATA *data = fillData(&total);
    bubbleSort(data, total);
    printResults(data, total);
    free(data);
    return 0;
}
```

---

## 📝 Vzory Delimiters

| Příklad | Delimiters | Příklad Řádku |
|---------|-----------|--------------|
| SLALOM | ` :` | `1 Vonn Lindsey 0:55.65 0:59.45` |
| BĚH | `;:.` | `1;ZUREK;Jiri;28.02.1983;CZE` |
| CYKLISTIKA | `;:.` | `5001;FOJTIK;Jan;CZE;1988;FORCE;9:23:45.4` |
| JIZERSKÁ | `;,():` | `16;Eliassen, Petter (1985);NOR;2:02:29` |

---

## 🔤 Konverze Typů

```c
atoi("123")          // String → int:      123
atof("123.45")       // String → float:    123.45
strcpy(dest, src)    // Zkopíruj string
strncpy(dest, src, n) // Zkopíruj max n znaků (bezpečnější!)
strcmp(s1, s2)       // Porovnání stringů (0 = stejné)
```

---

## ⏱️ Práce s Časem

### Konverze na Sekundy
```c
// Minuty a sekundy (SLALOM)
float time_sec = minutes * 60 + seconds;

// Hodiny, minuty, sekundy (BĚH, JIZERSKÁ)
int time_sec = hour * 3600 + min * 60 + sec;
```

### Systémový Čas
```c
#include <time.h>

int getYear() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    return 1900 + t->tm_year;  // 2025
}

// Věk = rok - rok_narození
int age = getYear() - birth_year;
```

---

## 🧮 Porovnávací Funkce pro Třídění

### Čas - Vzestupně (nejrychlejší první)
```c
if (convertTime(data[j]) > convertTime(data[j+1])) {
    SWAP;
}
```

### Rok - Nejstaršího (nejmenší rok)
```c
if (data[j].year > data[j+1].year) {
    SWAP;  // menší rok = starší
}
```

### Rok - Nejmladšího (největší rok)
```c
if (data[j].year < data[j+1].year) {
    SWAP;  // větší rok = mladší
}
```

---

## 📤 Formátování Výstupu

```c
// Obecný formát
fprintf(fw, "%d | %-15s | %02d:%05.2f\n", num, name, min, sec);

// Specifikátory:
// %d       = číslo
// %s       = string
// %-15s    = string zarovnaný vlevo, šířka 15
// %02d     = číslo s nulou na začátek (2 místa)
// %05.2f   = float, celkem 5 míst (včetně tečky), 2 místa za tečkou
// %%       = výstup znaku '%'
```

---

## ✅ Checklist Pro Každý Příklad

- [ ] Chápem strukturu vstupních dat
- [ ] Vím jaké jsou delimitery
- [ ] Naprogramoval jsem `parseData()`
- [ ] Testoval jsem parsování na prvním řádku
- [ ] Naprogramoval jsem `fillData()`
- [ ] Vím podle čeho se má třídit
- [ ] Naprogramoval jsem `bubbleSort()`
- [ ] Naprogramoval jsem výstup
- [ ] Testoval jsem celý program
- [ ] Ošetřil jsem chyby (malloc, fopen, buffer overflow)

---

## 🚨 10 Nejčastějších Chyb

1. **Zapomenutý Header** - První řádek je popis, ne data!
   ```c
   // ŠPATNĚ:
   while (fgets(line, 100, fr) != NULL) { ... }
   
   // SPRÁVNĚ:
   fgets(line, 100, fr);  // Skip header
   while (fgets(line, 100, fr) != NULL) { ... }
   ```

2. **Špatný Delim** - Zapomeneš nějaký separator
   ```c
   // ŠPATNĚ: chybí ':'
   strtok(line, " ");
   
   // SPRÁVNĚ:
   strtok(line, " :");
   ```

3. **Memory Leak** - Neosvobodit paměť
   ```c
   free(data);  // Nedělej si starost, je to důležité!
   ```

4. **Buffer Overflow** - Příliš dlouhý string
   ```c
   // ŠPATNĚ:
   char name[5];
   strcpy(name, "VeryLongName");  // PÁD!
   
   // SPRÁVNĚ:
   strncpy(name, token, 4);
   ```

5. **Inicializace NULL** - `resized` musí začínat NULL
   ```c
   // ŠPATNĚ:
   DATA *resized;
   resized = realloc(...);  // Undefined behavior!
   
   // SPRÁVNĚ:
   DATA *resized = NULL;
   resized = realloc(...);
   ```

6. **Newline v Stringech** - `fgets()` čte i `\n`
   ```c
   // Řešení:
   line[strcspn(line, "\n")] = 0;  // Odstraň newline
   ```

7. **Špatné Porovnání Stringů**
   ```c
   // ŠPATNĚ:
   if (name == "CZE") { ... }
   
   // SPRÁVNĚ:
   if (strcmp(name, "CZE") == 0) { ... }
   ```

8. **Chybný Casting**
   ```c
   // SLALOM: sekund.sec je float!
   float sec = atof(token);  // Ne atoi!
   ```

9. **Swap Bez Temp Proměnné**
   ```c
   // ŠPATNĚ:
   data[j] = data[j+1];
   data[j+1] = data[j];  // data[j] je již přepsáno!
   
   // SPRÁVNĚ:
   DATA temp = data[j];
   data[j] = data[j+1];
   data[j+1] = temp;
   ```

10. **Nevhodné Třídění**
    ```c
    // SLALOM: musíš součet obou kol!
    // ŠPATNĚ:
    if (data[j].firstRound.sec > data[j+1].firstRound.sec)
    
    // SPRÁVNĚ:
    if (convertTime(data[j].firstRound, data[j].secRound) > 
        convertTime(data[j+1].firstRound, data[j+1].secRound))
    ```

---

## 🔧 Základní Frázíčky

### Dynamické Pole
```c
// Alokace
int *arr = malloc(10 * sizeof(int));

// Zvětšení o 1
arr = realloc(arr, (current_size + 1) * sizeof(int));

// Uvolnění
free(arr);
```

### Práce se Soubory
```c
// Otevření pro čtení
FILE *fr = fopen("input.txt", "r");

// Otevření pro zápis
FILE *fw = fopen("output.txt", "w");

// Čtení řádku
fgets(line, MAX_LEN, fr);

// Zápis řádku
fprintf(fw, "%s\n", line);

// Zavření
fclose(fr);
fclose(fw);
```

### Bezpečné Parsování
```c
char line[] = "1:Novak:Ivan:0:55.71:1:01.11";
char *token;

token = strtok(line, ":");  // První token
while (token != NULL) {
    printf("%s\n", token);
    token = strtok(NULL, ":");  // Další token
}
```

---

## 🎯 Příklad - Od Nuly Po Hotovo (SLALOM)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Krok 1: Parsování
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

// Krok 2: Konverze času
float convertTime(ROUNDS_DATA r1, ROUNDS_DATA r2) {
    return (float)(r1.min + r2.min) * 60 + (r1.sec + r2.sec);
}

// Krok 3: Třídění
void bubbleSort(DATA *data, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (convertTime(data[j].firstRound, data[j].secRound) > 
                convertTime(data[j+1].firstRound, data[j+1].secRound)) {
                DATA temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}

// Krok 4: Načtení
DATA *fillData(int *total) {
    FILE *fr = fopen("zavodnici.txt", "r");
    DATA *resized = NULL;
    char storage[100];
    
    fgets(storage, 100, fr);  // Skip header
    
    while (fgets(storage, 100, fr) != NULL) {
        resized = realloc(resized, (*total + 1) * sizeof(DATA));
        resized[*total] = parseData(resized[*total], storage);
        *total += 1;
    }
    fclose(fr);
    return resized;
}

// Krok 5: Výstup
void printToFile(DATA *data, int total) {
    FILE *fw = fopen("vysledky.txt", "w");
    fprintf(fw, "poradi | prijmeni | jmeno | cas\n");
    for (int i = 0; i < total; i++) {
        fprintf(fw, "%2d | %s | %s | %.2f\n", i+1, data[i].surname, data[i].firstname,
                convertTime(data[i].firstRound, data[i].secRound));
    }
    fclose(fw);
}

// Main
int main() {
    int total = 0;
    DATA *data = fillData(&total);
    bubbleSort(data, total);
    printToFile(data, total);
    free(data);
    printf("Hotovo!\n");
    return 0;
}
```

---

## 🎓 Jak Používat Tento Cheat Sheet

1. **Začni SLALOMEM** - Zkopíruj šablonu
2. **Zaměň DELIMITERS** - Dej správné `" :"`
3. **Doplň switch-case** - Pro každé pole jeden case
4. **Testuj parseData()** - Tiskni prvních 5 tokenů
5. **Implementuj třídění** - Podle zadání
6. **Výstup** - Formátuj pěkně do souboru

---

**Teď máš vše co potřebuješ! Jdi na to!** 💪
