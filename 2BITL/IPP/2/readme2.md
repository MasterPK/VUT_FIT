# Implementační dokumentace k 2. úloze do IPP 2018/2019
* Jméno a příjmení: Petr Křehlík
* Login: xkrehl04

## interpret.py
Interpret je rozdělen z důvodu přehlednosti do několika souborů v podsložce.
* `interpret.py` - postupně volá hlavní funkce z podsouborů
* `arguments.py` - zpracovává argumenty
* `xml.py` - zpracovává XML vstup
* `error.py` - funkce pro výpis chyb
* `instructions.py` - interpretace instrukcí

Jako první se zpracovávají argumenty, načte XML vstup a další vstupní soubory. Následuje načtení instrukcí a jejich seřazení ve správném pořadí. Během načítání probíhá první část syntaktické kontroly instrukcí a argumentů. Po seřazení se načtou návěští a poté se spustí samotná interpretace. Ta probíhá dynamickým voláním funkcí podle jména(`opcode`) instrukce. Funkce již vykonají potřebné operace, včetně  další kontroly argumentů.

## test.php

Skript testuje dva předešlé skripty. Jako první kontroluje argumenty, tedy nastavuje buď implicitní hodnoty nebo hodnoty přijaté argumenty.
Pro průchod složkou běžným způsobem se využívá `DirectoryIterator` a pro rekurzivní průchod všemi podsložkami se využívá kombinace `RecursiveIteratorIterator` a `RecursiveDirectoryIterator`. Následuje průchod soubory, klíčové jsou soubory s příponou `.src`, ostatní soubory se ignorují. Když se najde soubor s již zméněnou příponou tak se zkontrolují další soubory stejného jména a pokud nejsou k dispozici tak se vytvoří. Poté probíhá samotné testování a to dle argumentů buď pouze parseru nebo interpretu, nebo obou dohromady. Vždy se kontroluje existence skriptu a poté se funkcí `shell_exec` provede spuštění skriptu s příslušnými soubory. Pro zjištění návratového kódu se ihned po vykonání volá `echo $?`. V závisloti na variantě se buď porovná výstup XMl pomocí nástroje `JExamXML` nebo výstupy čistě pomocí `diff`. Pokud se rovnají návratové kódy a výstupy tak se test prohlásí za úspěšný a do tabulky se přidá řádek s informacemi (Číslo, Jméno, Návratové kódy, Výsledek).
Na konci je souhrn s počtem úspěšných testů.
