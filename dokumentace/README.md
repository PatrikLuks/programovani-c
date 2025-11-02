# 📚 MATURITA Z PROGRAMOVÁNÍ V C - Kompletní Studijní Balíček

## 🎯 Co Máš K Dispozici

Vytvořil jsem pro tebe **4 studijní materiály** na přípravu na maturitu z programování:

```
📦 STUDIJNÍ BALÍČEK
├── 📖 STUDIJNI_MATERIAL.md           ← Začni tady! Přehled všech 4 příkladů
├── 📋 DETAILNI_PRUVODCE.md           ← Konkrétní implementace s kódem
├── ⚡ CHEAT_SHEET.md                 ← Rychlá referenční příručka
├── 🧪 PRIPRAVNY_TEST.md              ← Otázky a úkoly na procvičení
└── 📁 Složky s Projekty             ← Existující kódů
    ├── zadani_proverky_slalom/
    ├── zadaniProverkyBeh/
    ├── zadaniProverkyCyklistika/
    └── zadaniProverkyJizerska50/
```

---

## 🚀 PLÁN STUDIA (Doporučený Postup)

### Týden 1: Základy (SLALOM)
1. **Čti:** `STUDIJNI_MATERIAL.md` → kapitola SLALOM
2. **Nauč se:** Parsování s `strtok()` v `DETAILNI_PRUVODCE.md`
3. **Vyzkoušej:** Úkol 5.1 v `PRIPRAVNY_TEST.md`
4. **Projdi:** Otázky 1.1 - 1.3 v TESTU

**Cíl:** Pochopit parsování a základní strukturu

---

### Týden 2: Datum a Čas (BĚH)
1. **Čti:** `STUDIJNI_MATERIAL.md` → kapitola BĚH
2. **Nauč se:** Dvoustupňové parsování v `DETAILNI_PRUVODCE.md`
3. **Vyzkoušej:** Úkol 5.2 v `PRIPRAVNY_TEST.md`
4. **Projdi:** Otázky 2.1 - 2.4 v TESTU

**Cíl:** Zvládnout datum, věk a systémový čas

---

### Týden 3: Pokročilé Příklady (CYKLISTIKA + JIZERSKÁ)
1. **Čti:** `STUDIJNI_MATERIAL.md` → CYKLISTIKA a JIZERSKÁ
2. **Nauč se:** Komplexní parsování v `DETAILNI_PRUVODCE.md`
3. **Vyzkoušej:** Úkol 5.3 a 5.4 v `PRIPRAVNY_TEST.md`
4. **Projdi:** Otázky 3.1 - 6.4 v TESTU

**Cíl:** Zvládnout nejsložitější situace

---

### Týden 4: Opakování a Testování
1. **Znovu projdi** všechny 4 příklady
2. **Vyzkoušej** PŘÍPRRAVNÝ TEST kompletně
3. **Oprav chyby** - využij `CHEAT_SHEET.md`
4. **Projdi si kódy** v jednotlivých složkách

**Cíl:** Být 100% připraven/a

---

## 📖 Kterou Příručku Když Potřebuji?

### ❓ "Jak se to dělá?"
→ Jdi na **DETAILNI_PRUVODCE.md** - tam jsou konkrétní kódy s vysvětlením

### ⚡ "Potřebuji si rychle vzpomenout na syntax"
→ Jdi na **CHEAT_SHEET.md** - tam máš všechny frázíčky a šablony

### 📝 "Chci si vyzkoušet, jestli umím"
→ Jdi na **PRIPRAVNY_TEST.md** - tam jsou otázky a úkoly

### 🤔 "Co to znamená v Otázce 3.2?"
→ Jdi na **STUDIJNI_MATERIAL.md** - tam je všechno vysvětleno

---

## 🎯 4 Příklady - Přehled

| Příklad | Obtížnost | Klíčové Koncepty | Časový Úkol |
|---------|-----------|-----------------|-----------|
| **SLALOM** | ⭐ Lehké | Parsování, bubble sort, čas | 1 - 2 hodiny |
| **BĚH** | ⭐⭐ Střední | Datum, věk, systémový čas | 3 - 4 hodiny |
| **CYKLISTIKA** | ⭐⭐ Střední | Práce se systémovým časem | 2 - 3 hodiny |
| **JIZERSKÁ 50** | ⭐⭐⭐ Těžké | Komplexní parsování | 3 - 4 hodiny |

---

## 🧠 Co Musíš Znát Pro Maturitu

### 1. **Parsování Stringů** (ZÁKLADNÍ)
```c
char *token;
for (int i = 0; (token = strtok(i == 0 ? line : NULL, "DELIM")) != NULL; i++) {
    // Každý token se zpracovává podle indexu
}
```

### 2. **Struktury** (ZÁKLADNÍ)
```c
typedef struct {
    int position;
    char name[20];
    float time;
} DATA;
```

### 3. **Dynamické Pole** (ZÁKLADNÍ)
```c
DATA *arr = NULL;
arr = realloc(arr, (n + 1) * sizeof(DATA));
```

### 4. **Bubble Sort** (STŘEDNĚ POKROČILÉ)
```c
for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++) {
        if (shouldSwap(arr[j], arr[j+1])) {
            SWAP;
        }
    }
}
```

### 5. **Práce se Soubory** (ZÁKLADNÍ)
```c
FILE *fr = fopen("input.txt", "r");
FILE *fw = fopen("output.txt", "w");
fgets(line, MAX, fr);
fprintf(fw, "format", args);
fclose(fr); fclose(fw);
```

### 6. **Systémový Čas** (POKROČILÉ)
```c
#include <time.h>
time_t now = time(NULL);
struct tm *t = localtime(&now);
int year = 1900 + t->tm_year;
```

---

## 💡 10 Nejdůležitějších Tipů

1. ✅ **Vždycky skip header** - První řádek je popis!
2. ✅ **Všechny delimitery do `strtok()`** - Nic nezapomeň!
3. ✅ **Testuj po každé funkci** - Printfem si ověř, že funguje
4. ✅ **Inicializuj `data = NULL`** - Před `realloc()`
5. ✅ **Odstraň newline** - `line[strcspn(line, "\n")] = 0;`
6. ✅ **Používej `strcpy()` opatrně** - Raději `strncpy()`
7. ✅ **Ulož si temp proměnnou** - Při swapu v sortu
8. ✅ **Formátuj výstup pěkně** - Zarovnávej sloupce
9. ✅ **Ulož si výstup do souboru** - Nejen na obrazovku
10. ✅ **Osvoboď paměť** - `free(data)` na konci

---

## 🎓 Jak Používat Příručky

### STUDIJNÍ MATERIÁL (ZÁKLADNÍ)
**Kdy:** Když se chceš naučit nový koncept
**Co tam najdeš:** Vysvětlení + příklady použití
**Jak ho používat:** Čti postupně, zapiš si poznámky

### DETAILNÍ PRŮVODCE (KONKRÉTNÍ KÓD)
**Kdy:** Když potřebuješ vědět PŘESNĚ jak to implementovat
**Co tam najdeš:** Kompletní kód s komentáři
**Jak ho používat:** Kopíruj si šablony, adaptuj pro svůj případ

### CHEAT SHEET (RYCHLÁ REFERENCE)
**Kdy:** Když si nejseš jistý/á syntaxí
**Co tam najdeš:** Frázíčky, formáty, běžné vzory
**Jak ho používat:** Hledej si co potřebuješ, okoukni si syntax

### PŘÍPRAVNÝ TEST (KONTROLA)
**Kdy:** Když si chceš ověřit, že to umíš
**Co tam najdeš:** Otázky, úkoly, řešení
**Jak ho používat:** Odpověď si a srovnrej se řešením

---

## 🔧 Praktický Workflow

### Příklad: Implementuješ SLALOM

**Krok 1: Pochop Zadání**
- Čti STUDIJNÍ MATERIÁL - SLALOM

**Krok 2: Nauč se Implementaci**
- Čti DETAILNÍ PRŮVODCE - SLALOM
- Všimni si rozdílů v parsování vs. BĚH

**Krok 3: Zkopíruj si Šablonu**
- Jdi na CHEAT SHEET → Šablona
- Vyplň si delimitery ` :`
- Doplň switch-case pro 7 polí

**Krok 4: Testuj Parsování**
```c
// Přidej do main():
DATA test;
test = parseData(test, "1 Vonn Lindsey 0:55.65 0:59.45");
printf("%d %s %s %.2f\n", test.startPosition, test.surname, 
       test.firstname, convertTime(test.firstRound, test.secRound));
```

**Krok 5: Otestuj se v TESTU**
- Jdi na PŘÍPRAVNÝ TEST → Otázka 1.1 - 1.3
- Zkontroluj, jestli víš odpovědi

---

## ⚠️ Věci, Které Vás Mohou Zaskočit

### Věc 1: Newline v Stringech
```c
fgets(line, 100, fr);
// line = "1 Vonn Lindsey\n"  ← Je tam newline!

// Řešení:
line[strcspn(line, "\n")] = 0;
```

### Věc 2: Špatná Inicializace
```c
DATA *data;
data = realloc(data, sizeof(DATA));  // CHYBA!

// Správně:
DATA *data = NULL;
data = realloc(data, sizeof(DATA));  // OK
```

### Věc 3: Parsování se Ztrátou Dat
```c
// SLALOM: "1 Vonn Lindsey 0:55.71 1:01.11"
strtok(line, " ");  // Chybí ':'
// Ztratíš: "55" a "71" budou 1 token místo 2!

// Správně:
strtok(line, " :");
```

### Věc 4: Porovnávání Stringů
```c
if (nation == "CZE") { }  // ŠPATNĚ - porovnáváš adresy!
if (strcmp(nation, "CZE") == 0) { }  // SPRÁVNĚ
```

---

## 🎯 Finální Checklist Před Maturitou

- [ ] Umím parsovat jakýkoliv řetězec
- [ ] Umím vytvořit strukturu s libovolnými poli
- [ ] Umím pracovat s dynamickými poli
- [ ] Umím naprogramovat bubble sort
- [ ] Umím konvertovat časy do sekund
- [ ] Umím pracovat s datumy a věkem
- [ ] Umím psát do souboru
- [ ] Umím hledat minimum/maximum v poli
- [ ] Znám všechny 4 příklady
- [ ] Vím, co jsou nejčastější chyby a jak je vyhnout

**Pokud máš ✅ u všeho → JOOOO! Jsi připraven/a!** 🚀

---

## 📞 Jak Si Najít Konkrétní Info

| Co hledám | Kde | Kapitola/Část |
|-----------|-----|----------------|
| Jak funguje `strtok()` | DETAILNÍ PRŮVODCE | SLALOM → Parsování |
| Vzor pro konverzi času | CHEAT SHEET | Práce s Časem |
| Jak najít nejstaršího | DETAILNÍ PRŮVODCE | BĚH → Hledání |
| Otázka na datum | PŘÍPRAVNÝ TEST | Test 2 → Otázka 2.2 |
| Všechny delimitery | CHEAT SHEET | Vzory Delimiters |

---

## 🎁 Bonus: Společné Chyby Školitelů

1. **Záměna delimiters** - "BĚH má `;` a `.` ale zapomenu na `:`"
2. **Buffer Overflow** - "Názvy jsou moc dlouhé pro moje pole"
3. **Memory Leak** - "Nikdy neuvolním paměť"
4. **Špatné Třídění** - "Seřadim jen podle jednoho kritéria"
5. **Formátování** - "Výstup je nečitelný, sloupce nejdou pod sebou"

→ **Všechny jsou řešeny v CHEAT SHEET!**

---

## 💪 Poslední Slovo

Máš tady:
- ✅ Teorie + Vysvětlení
- ✅ Konkrétní Kódy
- ✅ Rychlé Reference
- ✅ Procvičovací Úkoly

**Teď jen:**
1. Věnuj 1-2 hodiny každému příkladu
2. Vyzkoušej si kód
3. Projdi si test
4. Buď sebevědomý/á

**Tvoja maturita z programování je V KAPSE!** 🎉

---

**Hodně štěstí a ať se ti vede! Zvládneš to!** 💪✨
