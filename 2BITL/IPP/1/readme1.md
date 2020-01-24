# Dokumentace projektu IPP - 1. část
**Petr Křehlík (xkrehl04)** 
## Úvodní řádek
Skript vyžaduje jako úvodní řádek ``.IPPcode19``, nicméně zadání nespecifikuje, jestli je nutné, aby byl na prvním řádku vstupu. Zvolil jsem řešení, kdy nemusí být. Skript postupně prohledává řádky a hledá úvodní řádek. Pokud je na řádku komentář tak ho ignoruje, jinak zkontroluje jestli je řádek ve správném formátu.
## Kontrola instrukcí
Kontrola instrukcí probíhá ve smyčce po řádcích. Jako první se odstraní bílé znaky na začátku a konci a poté pomocí regulárního výrazu odstraní přebytečné bílé znaky a ověří jestli není řádek komentářem. Řetězec se rozdělí podle mezer na samostatné části (operační kód, operandy).
Instrukce jsou uložené v poli, které je založené na principu klíč => hodnota, kdy klíč je řetězec s operačním kódem instrukce a hodnota je pole obsahující seznam operandů, které instrukce vyžaduje.
### Operační kód
Při operačním kódu se ověřuje existence klíče v již zmíněném poli, přičemž se všechny řetězce převedou na velká písmena.
### Operandy
Nejdříve se ověří jestli je správný počet operandů, a poté cyklus postupně prochází operandy a pomocí regulárních výrazů kontroluje jestli je operand ve správném formátu, dle jeho typu získaného z pole.
## Generování XML výstupu
Pro generování XML je použit ``xmlwriter``. XML se postupně generuje na zásobník a pokud lexikální a syntaktická kontrola proběhne bez chyby, tak je XML vygenerováno na standardní výstup.

## Zpracování argumentů příkazové řádky
Pro zpracování argumentů příkazové řádky jsem použil funkci ``getopt``. Skript zpracovává jediný volitelný argument ``--help`` pro zobrazení nápovědy. Skript reaguje jen na tento argument. Jakékoliv jiné jsou ignorovány, bez ovlivnění funkčnosti.