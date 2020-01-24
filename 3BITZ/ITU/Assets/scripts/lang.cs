//Petr Křehlík (xkrehl04)

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using UnityEngine;

public class Lang
{
    private Dictionary<string,Hashtable> strings = new Dictionary<string, Hashtable>(){
        { "Czech",new Hashtable(){
            {"new_game","Nová hra" },
            {"color_select_txt","Barva figurek" },
            {"difficulty_select_txt","Obtížnost" },
            {"max_time_select_txt","Max. doba hry" },
            {"easy","Jednoduchá" },
            {"medium","Normální" },
            {"hard","Nejtěžší" },
            {"play","Hrát proti počítači" },
            {"none","Žádný" },
            {"3_min","3 minuty" },
            {"5_min","5 minut" },
            {"10_min","10 minut" },
            {"tutorial","Manuál" },
            {"close_title","Opravdu?" },
            {"close_message","Opravdu chcete ukončit tuto hru?" },
            {"yes","Ano" },
            {"no","Ne" },
            {"resetBtnTxt","Restartovat hru" },
            {"newGameBtnTxt","Návrat do menu" },
            {"test","Testovací řetězec" },
            {"congrats","Vyhrál jsi!" },
            {"looser","Prohrál jsi!" },
            {"timeLeftTxt","Zbývající čas" },
            {"multiplayerBtnTxt","Hrát proti člověku" },
            {"draw","Remíza!" },
            {"welcomeMsg","Vítejte v manuálových stránkách hry Dáma.\nVyberte si jednu z kategorii manuálu pro pokračování." },
            {"manualTxt1","• Partii vyhrává hráč, jehož soupeř nemá tah podle pravidel, tzn. že nemá již žádné figury, nebo jsou jeho figury zablokovány, nebo se vzdal.\n\n• Partie skončí nerozhodně dohodou hráčů, nebo když se vyskytne 3x stejná pozice." },
            {"manualTxt2","• Kameny se skáče jen dopředu, dáma může skákat přes libovolný kámen na diagonále dopředu i dozadu a dokončit skok na kterémkoli poli za přeskočeným kamenem.\n\n• Hráč při skoku přenese svou figuru na konečné pole skoku a poté odstraní přeskočené figury z desky.\n\n• Přes jeden kámen lze skákat jen jednou.\n\n• Skákání je povinné a figura po dokončení tahu nesmí mít další možnost skoku.\n\n• Je-li více možností skákání, může si hráč vybrat bez ohledu na množství přeskočených kamenů soupeře.\n\n• Opomenuté skákání se promlčuje dalším tahem soupeře.\n\n• Dáma má při skákání vždy přednost před kamenem." },
            {"manualTxt3","• Hraje se na desce o velikosti 8×8, na každé straně mají hráči 12 kamenů ve 3 řadách na černých polích.\n\n• Tyto kameny se táhnou vždy diagonálně o 1 políčko vpřed.\n\n• Pokud hráč narazí na protivníkovu figurku, která taky leží diagonálně vpravo nebo vlevo o 1 políčko a políčko za touto figurkou je volné, může tuto figurku vzít její přeskočením.\n\n• Kameny, které se dostanou na poslední řadu se stávají dámami, které se můžou pohybovat diagonálně i vzad." },
            {"manualTxt4","• Před zahájením hry jsou k dispozici různá nastavení, kterými si můžete přispůsobit farbu figurky, obtížnost hry a taky i trvání délky hry.\n\n• Farbu figurky si jde zvolit ako černou nebo bílou kliknutím na černou, respektive bílou figurku v hlavním menu hry.\n\n• Pro začátečníky je k dispozici jednoduchá obtížnost, přičemž zkušení hráči si přijdou na své v obtížném módě, který je v kombinaci s nastaveným časovačem skutečnou výzvou.\n\n• Tyto nastavení platí jenom pro hru proti počítači, avšak je k dispozici i hra pro 2 hráče na jednom počítači." },
            {"manualTxt5","• Tato aplikace byla vytvořena s úmyslem jednoduchého ovládání, a do této kategorie spadá i samotní hra.\n\n• Pohyb figurek je ovládán stisknutím myši nad zvolenou figurkou a následným potáhnutím na jedno z předem označených míst a uvolněním tohoto tlačítka.\n\n• Ovládání aplikace mimo hru se vykonává jednoduchým kliknutím myši na tlačítka, nebo stisknutím, potáhnutím a uvolněním při nastavování obtížnosti hry." },
            {"manualTxt6","• Naše aplikace Vám přináší kvalitní zážitek z hry i díky hudbě.\n\n• Tato funkce je ovšem volitelná a je možné ji vypnout stisknutím tlačidla reproduktoru v hlavním menu nebo i ve hře.\n\n• Toto tlačidlo se nachází v pravém horním rohu okna a křížek při něm signalizuje vypnutý zvuk, jinak je zvuk zapnutý." },
            {"manualTxt7","• Aby byla tato hra dostupná pro všechny, je potřebné, aby ji rozuměli.\n\n• Z tohoto důvodu jsou k dispozici na výběr 2 jazyky v hlavním menu hry.\n\n• V levém horním rohu okna je možno vidět dvě vlajky, kterými je možné přepnout na zvolený jazyk.\n\n• Po kliknutí na anglickou vlajku se vaše hra přepne do anglického jazyka, avšak návrat do češtiny je zrovna tak jednoduchý, takže nemusíte mít strach, že nebudete něčemu rozumět." },
            {"manualBtn1","Manuál" },
            {"manualBtn2","Konec hry" },
            {"manualBtn3","Typy tahů" },
            {"manualBtn4","Typy figurek" },
            {"manualBtn5","Pravidla hry" },
            {"manualBtn6","Nastavení hry" },
            {"manualBtn7","Ovládání hry" },
            {"manualBtn8","Zvuk" },
            {"manualBtn9","Jazyk" },
            {"manualBtn10","Nastavení aplikace" },

        }},
        { "English",new Hashtable(){
            {"new_game","New game" },
            {"color_select_txt","Color of figures" },
            {"difficulty_select_txt","Difficulty" },
            {"max_time_select_txt","Max. time of game" },
            {"easy","Easy" },
            {"medium","Medium" },
            {"hard","Hard" },
            {"play","Play against computer" },
            {"none","None" },
            {"3_min","3 minutes" },
            {"5_min","5 minutes" },
            {"10_min","10 minutes" },
            {"tutorial","Manual" },
            {"close_title","Really?" },
            {"close_message","Do you really want to close this game?" },
            {"yes","Yes" },
            {"no","No" },
            {"resetBtnTxt","Restart game" },
            {"newGameBtnTxt","Back to menu" },
            {"test","Test string" },
            {"congrats","You won!" },
            {"looser","You lost!" },
            {"timeLeftTxt","Time left" },
            {"multiplayerBtnTxt","Play against human" },
            {"draw","Draw!" },
            {"welcomeMsg","Welcome to Checkers manual pages.\n Choose one of the manual categories to continue." },
            {"manualTxt1","• The game is won by a player whose opponent does not move according to the rules, ie. that he no longer has any figures, or his figures are blocked, or has given up.\n\n• The game ends in a tie by players decision or when the same position occurs 3 times." },
            {"manualTxt2","• The stones jump only forward, the queen can jump over any stone on the diagonal forwards and backwards and complete the jump on any field behind the skipped stone.\n\n• The player moves his / her figure to the final jump field and then removes the skipped pieces from the board.\n\n• You can jump over one stone only once.\n\n• Jumping is mandatory and the figure must not have another jump option after the turn is completed.\n\n• If there are multiple jumping options, the player can choose regardless of the amount of opponent's skipped stones.\n\n• The missed jumping is barred by another opponent's move." },
            {"manualTxt3","• It is played on an 8 × 8 board, with each player having 12 stones in 3 rows on black boxes.\n\n• These stones always move diagonally one space forward.\n\n• If a player encounters an opponent's piece, which also lies diagonally to the right or left by 1 square and the square behind that piece is free, he can take that piece by getting over it.\n\n• The stones that reach the last row become queen that can move diagonally even backwards." },
            {"manualTxt4","• Before starting the game, there are various settings available to customize the color of the piece, the difficulty of the game and the duration of the game.\n\n• The color of the figure can be selected as black or white by clicking on the black or white figure in the main game menu.\n\n• A simple difficulty is available for beginners, and experienced players will find their way in a hard difficult that, combined with a game timer, is a real challenge.\n\n• These settings apply only to a game against a computer, but you can play even a local multiplayer." },
            {"manualTxt5","• This application was created with the intention of simplicity, and this category includes the game itself.\n\n• The movement of the figures is controlled by pressing the mouse over the selected figure and then dragging to one of the pre-marked places and releasing this button.\n\n• Out-of-game application control is performed by simply clicking on the buttons or by pressing, dragging and releasing to adjust the difficulty of the game." },
            {"manualTxt6","• Our application brings you a quality gaming experience also thanks to music.\n\n• However, this function is optional and can be turned off by pressing the speaker button in the main menu or even in the game.\n\n• This button is located in the upper right corner of the window and a cross indicates the sound is muted, otherwise the sound is on." },
            {"manualTxt7","• To make this game accessible to everyone, they need to understand it.\n\n• For this reason, 2 languages ​​are available to choose from in the main game menu.\n\n• In the upper left corner of the window you can see two flags that can be used to switch to the selected language.\n\n• When you click on the English flag, your game will switch to English, but returning to Czech is just as easy, so you don't have to worry about not understanding anything." },
            {"manualBtn1","Manual" },
            {"manualBtn2","End of game" },
            {"manualBtn3","Types of moves" },
            {"manualBtn4","Types of figures" },
            {"manualBtn5","Game rules" },
            {"manualBtn6","Game options" },
            {"manualBtn7","Game controls" },
            {"manualBtn8","Audio" },
            {"manualBtn9","Language" },
            {"manualBtn10","Application settings" },
        }},
    };
    private string languageSet;

    public Lang(string _path, string language, bool _web)
    {
        setLanguage(null, language);
        
    }

    public void setLanguage(string _old,string language)
    {
        if (language == "Czech" || language == "English")
        {
            languageSet = language;
        }
        else
        {
            throw new Exception("Unsupported language");
        }
    }

    public string getString(string name)
    {
        if(languageSet!=null)
        {
            if(strings[languageSet].ContainsKey(name))
            {
                return (string)strings[languageSet][name];
            }
            else
            {
                throw new Exception("Key \""+name+"\" does not exist");
            }
        }
        else
        {
            throw new Exception("No language selected");
        }
    }
    

}