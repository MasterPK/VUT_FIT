/* Projekt IMS 2019
* Petr Krehlik(xkrehl04)
* Martin Klobusicky(xklobu03)
*/

#include "stdlib.h"
#include "string.h"
#include "simlib.h"

#define PLAST 0
#define SKLO 1
#define PAPIR 2
#define OSTATNI 3

#define TRIDENY 0
#define KOMUNALNI 4

Store Tridirna("Tridirna",2);
Stat efektivitaNalozeni("Efektivita nalozeni");
Histogram casOkruhu("Doba za jak dlouho auto objede kolecko", 0, 50, 10);
Histogram dobaNakladani("Doba nakladani auta", 0, 10, 10);

int tridenyOdpad=0;
int tridenyOdpadPole[4];
int ostatniOdpad = 0;

//argumenty
int odpadNaTyden=6052; // kg/1000
int pocetAut=105;
double dopravniModifikator=1;
int pocetSmen=3;
class Auto : public Process
{
    int typAuta;
    public:Auto(int typ)
    {
        typAuta=typ;
    }

    void Behavior() {
        int kapacita=5;
        int typOdpadu=0;
        int typTridenehoOdpadu;
        Wait(Normal(30*dopravniModifikator, 7));//odjezd z depa k dane ctvrti
        int hodiny=Time;
        while(1)
        {
            int zacatekOkruhu=Time;
            if((Time-hodiny)>=(8*60)) // vymena posadky
            {
                int konecSmeny=Time;
                hodiny=Time;
                Wait(Normal(30*dopravniModifikator, 7*dopravniModifikator)); // odjezd do depa
                switch(pocetSmen)
                {
                    case 1:Wait(16*60);break;
                    case 2:Wait(8*60);break;
                    case 3:break;
                }
                Wait(Normal(30*dopravniModifikator, 7*dopravniModifikator)); // odjezd z depa k dane ctvrti
                hodiny=Time;
            }
            else
            {
                Wait(Normal(5*dopravniModifikator, 0.3*dopravniModifikator));//presun
            }
   
            //zpracovani trideneho odpadu
            int zacatekNakladani=Time;
            if(tridenyOdpad>0 && typAuta>=PLAST && typAuta<=OSTATNI)
            {
                if(kapacita==5)
                {
                    typOdpadu=TRIDENY;
                    if(tridenyOdpadPole[typAuta]>0)
                    {
                        typTridenehoOdpadu=typAuta;
                    }
 
                    if(typOdpadu==TRIDENY && typTridenehoOdpadu==typAuta)
                    {
                        
                        while(tridenyOdpadPole[typTridenehoOdpadu]>0 && kapacita>0)
                        {
                            Wait(Normal(18*dopravniModifikator, 0.2*dopravniModifikator));//presun
                            if(tridenyOdpadPole[typTridenehoOdpadu]>0)
                            {
                                tridenyOdpadPole[typTridenehoOdpadu]=tridenyOdpadPole[typTridenehoOdpadu]-1;
                                tridenyOdpad--;
                                kapacita--;
                                Wait(Normal(9*dopravniModifikator, 0.3*dopravniModifikator));//nalozeni
                            }
                        }
                    }
                }
            }
            //zpracovani komunalniho odpadu
            else if(ostatniOdpad>0 && typAuta==KOMUNALNI)
            {
                if(kapacita==5)
                {
                    typOdpadu=KOMUNALNI;
                }
                if(typOdpadu==KOMUNALNI)
                {
                    while(ostatniOdpad>0 && kapacita>0)
                    {
                        Wait(Normal(6*dopravniModifikator, 0.2*dopravniModifikator));//presun
                        if(ostatniOdpad>0)
                        {
                            ostatniOdpad--;
                            kapacita--;
                            Wait(Normal(9*dopravniModifikator, 0.2*dopravniModifikator));//nalozeni
                        }
                    }
                }
                
            }
  
            if(kapacita==0)
            {
                dobaNakladani(Time-zacatekNakladani);
                efektivitaNalozeni(5-kapacita);
                if(typOdpadu==KOMUNALNI)
                {
                    Wait(Normal(40*dopravniModifikator,9*dopravniModifikator)); //po naplneni cesta na skladku
                    Wait(Normal(10,1)); // vylozeni
                    kapacita=5;
                    Wait(Normal(40*dopravniModifikator,9*dopravniModifikator)); // cesta zpet
                }
                else
                {
                    Wait(Normal(30*dopravniModifikator,7*dopravniModifikator)); //po naplneni cesta do tridirny
                    Enter(Tridirna, 1); //tridirna ma omezeny prostor pro vylozeni
                    Wait(Normal(10,1)); // vylozeni
                    kapacita=5;
                    Leave(Tridirna, 1); // uvolneni dojicky
                    Wait(Normal(30*dopravniModifikator,7*dopravniModifikator)); // cesta zpet
                }
                casOkruhu(Time-zacatekOkruhu);
            }
            else if(kapacita <5 && ( (ostatniOdpad == 0 && typAuta==KOMUNALNI) || (tridenyOdpad==0 && typAuta>=PLAST && typAuta<=OSTATNI) ))
            {
                dobaNakladani(Time-zacatekNakladani);
                efektivitaNalozeni(5-kapacita);
                if(typOdpadu==KOMUNALNI)
                {
                    Wait(Normal(40*dopravniModifikator,9*dopravniModifikator)); //po naplneni cesta na skladku
                    Wait(Normal(10,1)); // vylozeni
                    kapacita=5;
                    Wait(Normal(40*dopravniModifikator,9*dopravniModifikator)); // cesta zpet
                }
                else
                {
                    Wait(Normal(30*dopravniModifikator,7*dopravniModifikator)); //po naplneni cesta do tridirny
                    Enter(Tridirna, 1); //tridirna ma omezeny prostor pro vylozeni
                    Wait(Normal(10,1)); // vylozeni
                    kapacita=5;
                    Leave(Tridirna, 1); // uvolneni dojicky
                    Wait(Normal(30*dopravniModifikator,7*dopravniModifikator)); // cesta zpet
                }
                casOkruhu(Time-zacatekOkruhu);
                Wait(Normal(30*dopravniModifikator, 7*dopravniModifikator));//navrat do depa
            }
        }
    }
};

class GeneratorOdpad : public Event
{
    void Behavior() {
        //generovani odpadu dle procent
        for(int a=0; a<odpadNaTyden; a++)
        {
            if(Random()<=0.172)
            {
                
                tridenyOdpad++;
                double rozdeleni = Random();
                if(rozdeleni<=0.415)
                {
                    tridenyOdpadPole[PAPIR]++;
                }
                else if(0.415<rozdeleni && rozdeleni<=0.672)
                {
                    tridenyOdpadPole[PLAST]++;
                }
                else if(0.672<rozdeleni && rozdeleni<=0.979)
                {
                    tridenyOdpadPole[SKLO]++;
                }
                else
                {
                    tridenyOdpadPole[OSTATNI]++;
                }
            }
            else
            {
                ostatniOdpad++;
            };
            
        }
    }
};

void printHelp()
{
    Print("Simulace svozu odpadu ve meste Praha\n");
    Print("Autori:\n");
    Print("Petr Krehlik\n");
    Print("Martin Klobusicky\n\n");
    Print("Argumenty\n");
    Print("[-h] ... Zobrazi tuto napovedu\n");
    Print("[-o odpadNaTyden] ... Pocet odpadu na tyden v tunach - implicitne 6052\n");
    Print("[-a pocetAut] ... Pocet svozovych aut - implicitne 105\n");
    Print("[-d dopravniModifikator] ... Modifikator dopravni situace - implicitne 1 - ovlivnuje dopravni situaci, cim vyssi cislo tim vetsi zhorseni situace\n");
    Print("[-s pocetSmen] ... Nastaveni vicesmenneho provozu - implicitne 3 - urcuje pocet smen na den\n");
    Print("Priklad pouziti: ./xkrehl04_xklobu03 -o 1000 -a 20 -d 1.1 -s 2\n");
}

int main(int argc, char *argv[])
{

    //zpracovat argumenty
    char *end;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
        
        if (argc != i + 1)
        {
            odpadNaTyden=strtol( argv[i + 1] ,&end,10);
            if(odpadNaTyden<=0)
            {
                Print("Spatny format argumentu -o!\n");
                exit(1);
            }
        }
            
        else
        {
            Print("Spatny format argumentu -o!\n");
            exit(1);
        }
        i++;
        if ((i + 1) >= argc)
            break;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            printHelp();
            exit(0);
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
        if (argc != i + 1)
        {
            pocetAut=strtol( argv[i + 1] ,&end,10);
            if(!(pocetAut>0))
            {
                Print("Spatny format argumentu -a!\n");
                exit(1);
            }
        }
        else
        {
            Print("Spatny format argumentu -a!\n");
            exit(1);
        }
        i++;
        if ((i + 1) >= argc)
            break;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
        if (argc != i + 1)
        {
            dopravniModifikator=strtod( argv[i + 1] ,&end);
            if(!(dopravniModifikator>0))
            {
                Print("Spatny format argumentu -d!\n");
                exit(1);
            }
        }
        else
        {
            Print("Spatny format argumentu -d!\n");
            exit(1);
        }
        i++;
        if ((i + 1) >= argc)
            break;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
        if (argc != i + 1)
        {
            pocetSmen=strtol( argv[i + 1] ,&end,10);
            if(!(pocetSmen==1 || pocetSmen==2 || pocetSmen==3))
            {
                Print("Spatny format argumentu -s!\n");
                exit(1);
            }
        }
        else
        {
            Print("Spatny format argumentu -s!\n");
            exit(1);
        }
        i++;
        if ((i + 1) >= argc)
            break;
        }
    }

    Init(0, 10080);
    //DebugON();
    (new GeneratorOdpad)->Activate();

    //Generovani aut
    int autaPapir=0;
    int autaPlast=0;
    int autaSklo=0;
    int autaOstatni=0;
    int autaKomunalni=0;
    for (int a=0; a<pocetAut; a++)
    {
       
        if(Random()<=0.172)
        {
            double rozdeleni = Random();
            if(rozdeleni<=0.415)
            {
                autaPapir++;
                (new Auto(PAPIR))->Activate();
            }
            else if(0.415<rozdeleni && rozdeleni<=0.672)
            {
                autaPlast++;
                (new Auto(PLAST))->Activate();
            }
            else if(0.672<rozdeleni && rozdeleni<=0.979)
            {
                autaSklo++;
                (new Auto(SKLO))->Activate();
            }
            else
            {
                autaOstatni++;
                (new Auto(OSTATNI))->Activate();
            }
        }
        else
        {
            autaKomunalni++;
            
        };
        
    }
    // Pokud neni zadne auto dle typu tak odecist od komunalnich
        if(autaPapir==0)
        {
            if(autaKomunalni>1)
            {
                autaKomunalni--;
                (new Auto(PAPIR))->Activate();
            }
            
        }
        if(autaPlast==0)
        {
            if(autaKomunalni>1)
            {
                autaKomunalni--;
                (new Auto(PLAST))->Activate();
            }
        }
        if(autaSklo==0)
        {
            if(autaKomunalni>1)
            {
                autaKomunalni--;
                (new Auto(SKLO))->Activate();
            }
        }
        if(autaOstatni==0)
        {
            if(autaKomunalni>1)
            {
                autaKomunalni--;
                (new Auto(OSTATNI))->Activate();
            }
        }
        Print("KomAut:%d\n",autaKomunalni);
        for(int i =0;i<autaKomunalni;i++)
        {
            (new Auto(KOMUNALNI))->Activate();
        }
    Run();
    

    dobaNakladani.Output();
    casOkruhu.Output();
    efektivitaNalozeni.Output();
    
    Print("Pocet zbyleho trideneho odpadu:%d\n",tridenyOdpad);
    Print("Pocet zbyleho ostatniho odpadu:%d\n",ostatniOdpad);

    if(tridenyOdpad==0 && ostatniOdpad==0)
    {
        Print("Vsechen odpad byl odvezen uspesne!\n");
    }
    else
    {
        Print("Nepodarilo se odvezt vsechen odpad!\n");
    }

    return 0;
}