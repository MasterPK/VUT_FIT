# Dokumentace k 1. projektu IPK - varianta 1

## Úvod
HTTP server je napsán v jazyce Python. Spustitelný je přes makefile.

## Zpracování požadavků
Server zpracovává požadavky pomocí funkce `accept`, tedy čeká se na připojení klienta a poté se přijmou data. Server dokáže zpracovávat více požadavků zároveň. Podporovány jsou pouze požadavky typu `GET`.
### Hostname
Požadavek `hostname` vypíše celé doménové jméno počítače. Hodnota je získána pomocí funkce `gethostname`.
### Informace o CPU
Požadavek `cpu-name` vypíše jméno procesoru. To je získáno ze souboru `/proc/cpuinfo`, který se prochází a hledá se hodnota `model name`.
### Vytížení CPU
Požadavek `load` vypočítá a vypíše aktuální vytížení procesoru. Výpočet probíhá z hodnot ze souboru `/proc/stat`. Hodnoty se získávají dvakrát s jedno sekundovým zpožděním. Z hodnot se poté dle zadání vypočítá celkové využití CPU v procentech.
#### Automatické obnovení
Pokud požadavek obsahuje parametr `refresh` s celočíselnou hodnotou tak je v hlavičce odeslán také meta tag `Refresh`.
## Odpověď serveru
Server dokáže odpovědět dvěma způsoby: `text/plain` a `application/json`. Při zpracovávání požadavku se kontroluje hlavička `Accept`. 
Priorita výběru typu odpovědi:
1. `application/json`
2. `text/plain`
3. `*/*` => `text/plain`
## Chybové kódy
### 400
V případě špatného parametru `refresh`. Např.: `refresh=ah`, je chyba kvůli tomu že, je očekáváno celé číslo.
### 404
Chybný požadavek. Např.: `/hosname`
### 405
Chybná metoda požadavku. Např.: `POST`, server podporuje pouze `GET`.
### 406
Hlavička `Accept` neobsahuje žádný podporovaný formát.
### 505
Požadavek není v podporované verzi HTTP.