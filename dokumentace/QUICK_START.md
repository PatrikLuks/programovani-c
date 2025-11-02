# ⚡ QUICK START - Pro Netrpělivé (15 minut)

> Nemáš čas na 50 stran? OK! Tady je to nejdůležitější! ⚡

---

## 🎯 Nejdůležitější 5 Minut

### Struktura Všech 4 Příkladů
```c
typedef struct {
    int position;
    char name[20];
    float time;
} DATA;
```

### Vždy Stejný Postup
1. **Parsuj** - `strtok()` s správnými delimitery
2. **Sortuj** - Bubble sort dle času
3. **Vypiš** - Do souboru `fprintf()`

### Delimitery
- SLALOM: ` :`
- BĚH: `;:.`
- CYKLISTIKA: `;:.`
- JIZERSKÁ: `;,():`

**To je všechno!** 🚀

---

## 🔧 Kód - Copy-Paste Šablona

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int pos;
    char name[20];
} DATA;

DATA parseData(DATA d, char *line) {
    char *token;
    for (int i = 0; (token = strtok(i == 0 ? line : NULL, "DELIM")) != NULL; i++) {
        switch(i) {
            case 0: d.pos = atoi(token); break;
            case 1: strcpy(d.name, token); break;
        }
    }
    return d;
}

DATA *fillData(int *total) {
    FILE *fr = fopen("input.txt", "r"); // Otevře vstupní soubor pro čtení
    DATA *arr = NULL;                   // Inicializuje ukazatel na pole struktur
    char line[100];
    fgets(line, 100, fr);  // Skip header - přeskočí hlavičku souboru
    while (fgets(line, 100, fr) != NULL) { // Čte řádky dokud nejsou všechny načteny
        arr = realloc(arr, (*total + 1) * sizeof(DATA)); // Realokuje pole pro další prvek
        arr[*total] = parseData(arr[*total], line);      // Naparsuje řádek do struktury
        *total += 1;                                     // Zvýší počet načtených záznamů
    }
    fclose(fr); // Zavře soubor
    return arr; // Vrátí ukazatel na pole struktur
}

void bubbleSort(DATA *arr, int n) {
    for (int i = 0; i < n - 1; i++) { // Prochází pole n-1 krát
        for (int j = 0; j < n - 1 - i; j++) { // Porovnává sousední prvky
            if (arr[j].pos > arr[j+1].pos) {  // Porovnání podle pole pos
                DATA tmp = arr[j];            // Provede výměnu pomocí dočasné proměnné
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

int main() {
    int total = 0;
    DATA *data = fillData(&total);
    bubbleSort(data, total);
    
    FILE *fw = fopen("output.txt", "w");
    for (int i = 0; i < total; i++) {
        fprintf(fw, "%d %s\n", data[i].pos, data[i].name);
    }
    fclose(fw);
    free(data);
    return 0;
}
```

**Jen nahraď:**
- `"DELIM"` → správné delimitery
- `int pos` → všechna tvá pole
- `case 0, 1` → tolik casos kolik polí máš

---

## 🧠 10 Věcí Kterou MUSÍŠ Znát

1. ✅ **Skip header** - První řádek neparse!
2. ✅ **Všechny delimitery** - Nic nezapomeň!
3. ✅ **NULL initialization** - `DATA *arr = NULL;`
4. ✅ **Newline odebrání** - `line[strcspn(line,"\n")]=0;`
5. ✅ **Konverze typu** - `atoi()` pro int, `atof()` pro float
6. ✅ **Swap s temp** - Nebudeš mít data dva krát!
7. ✅ **strcmp()** - Ne `==` pro stringy!
8. ✅ **fprintf()** - Do souboru, ne printf()!
9. ✅ **free()** - Osvoboď paměť na konci!
10. ✅ **Formátování** - Udělej výstup čitelný!

---

## 🎯 Pro Každý Příklad - Úkoly

### SLALOM
- [ ] Parsuj 7 polí (pozice, příjmení, jméno, 2x časy)
- [ ] Sortuj dle součtu obou kol
- [ ] Vypiš s ztrátou vůči vítězi

### BĚH
- [ ] Parsuj 9 polí (data se zvlášť!)
- [ ] Vypočítej věk
- [ ] Sortuj dle času (HH:MM:SS)

### CYKLISTIKA
- [ ] Parsuj s desetinným časem
- [ ] Najdi nejstaršího (nejmenší rok)
- [ ] Sortuj dle času

### JIZERSKÁ
- [ ] Parsuj jméno v závorkách
- [ ] Odstraň nežádoucí mezery
- [ ] Sortuj dle času

---

## ⚡ 3 Příkazy Které Všechno Záchraní

### #1 - Odstraň Newline
```c
line[strcspn(line, "\n")] = 0;
```

### #2 - Konverz Čas na Sekundy
```c
int sec = h * 3600 + m * 60 + s;
```

### #3 - Najdi Nejstaršího
```c
if (data[i].year < min_year) min_year = data[i].year;
```

---

## 🚨 3 Věci Kterých se Vyvaruj

### ❌ Chyba 1: Buffer Overflow
```c
char name[5];
strcpy(name, "VeryLongName");  // PADÁ!
// Řešení: strncpy(name, token, 4);
```

### ❌ Chyba 2: Neinicializovaný Pointer
```c
DATA *arr;
arr = realloc(arr, ...);  // Undefined!
// Řešení: DATA *arr = NULL;
```

### ❌ Chyba 3: Špatný Delimiter
```c
strtok(line, " ");  // Chybí ':'!
// Řešení: strtok(line, " :");
```

---

## 🎓 Finální Checklist

Než půjdeš na zkoušku:

- [ ] Umím parsovat řetězec
- [ ] Umím sortovat pole
- [ ] Umím psát do souboru
- [ ] Znám všechny delimitery
- [ ] Testoval/a jsem svůj kód
- [ ] Mám hotové všechny 4 příklady

✅ Jestli máš všechno → **JOOOO! Jsi připraven/a!** 🚀

---

## 📖 Když Potřebuješ Víc Detailů

| Potřebuji | Čti |
|-----------|-----|
| Vysvětlení | STUDIJNÍ_MATERIÁL.md |
| Kód s komentáři | DETAILNÍ_PRŮVODCE.md |
| Vzor či formát | CHEAT_SHEET.md |
| Otázky na test | PŘÍPRAVNÝ_TEST.md |
| Orientaci | INDEX.md |

---

## ⏱️ Jak Na To Za 2 Dny?

### Den 1 (5 hodin)
- [ ] Ráno: README.md (30 min)
- [ ] Dopoledne: SLALOM - všechno (2 hod)
- [ ] Odpoledne: BĚH - všechno (2.5 hod)

### Den 2 (5 hodin)
- [ ] Ráno: CYKLISTIKA (2 hod)
- [ ] Dopoledne: JIZERSKÁ (2 hod)
- [ ] Odpoledne: TEST + opravy (1 hod)

**Výsledek: Připraven/a na zkoušku!** ✅

---

## 💡 Jediný Tip Co Opravdu Stojí Za To

> Procvičuj VŠECHNY 4 příklady. Každý je jiný, ale postup je vždy stejný.

Jakmile pochopíš 1. příklad → ostatní jdou samo! 🎯

---

## 🎯 One-Liner Pro Každý Příklad

| Příklad |核心 |
|---------|-----|
| SLALOM | Parsuj časy v minutách a sekundách |
| BĚH | Parsuj datum a počítej věk |
| CYKLISTIKA | Pracuj se systémovým časem |
| JIZERSKÁ | Parsuj jméno v závorkách |

---

## 🚀 Jdi Na To!

**Máš všechno co potřebuješ.** ✅
**Věníš tomu čas?** ⏰
**Testy procvičuješ?** 🧪
**Pak to určitě zvládneš!** 💪

---

```
Čas: 5-10 minut na čtení
Užitek: 80% toho co potřebuješ
Cena: Zdarma
Výsledek: Maturita v kapse 📚
```

**Hodně štěstí!** 🍀✨
