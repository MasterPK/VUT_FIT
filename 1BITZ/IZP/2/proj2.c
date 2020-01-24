/*
 * File:  	proj2.c
 * Date:  	2017/11/10
 * Author:  Petr Krehlik, xkrehl04
 * Project: Second project to IZP
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

// vytisknutí nápovědy
void show_help(){
	printf("\
Name\n\
	proj2 - Iteration calculations\n\n\
Synopsis\n\
	./proj2 --help\n\
	./proj2 --tan --tan A N M\n\
	./proj2 [-c X] -m A [B]\n\n\
Description\n\
	Calculate the distance and height of the measured object.\n\
	Optionally compare results of tanges by various methods.\n\n\
Commands\n\
	--help print this help\n\
	--tan calculate tangens with math library, Taylor polynomial and chained fractions. Print comparsion.\n\
	-m calculate the distance and height\n\n\
Parameters\n\
	A - input angle in radians to calculate distance\n\
		if agrument -m is used then must apply 0<A<=1.4\n\
	B - input angle in radians to calculate height 0<B<=1.4\n\
	N M - interval of iterations that will be printed 0<N<=M<14\n\
	X - height of measuring device <X<=100\n\
\nPetr Křehlík\n");
	return;
}

typedef enum argument{ERR=1,HELP,TAN,M,CM} argument;


//funkce zjituje jaká operace se bude provádět a jestli odpovídá počet argumentů
argument check_argv(int argc, char *argv[]){
	if(strcmp(argv[1],"--help")==0){
		return HELP;
	}else if(strcmp(argv[1],"--tan")==0 && argc==5)
	return TAN;
	else if(strcmp(argv[1],"-m")==0 && argc>=3 && argc<=4){
		return M;
	}else if(strcmp(argv[1],"-c")==0 && argc>=5 && argc <=6){
		if(strcmp(argv[3],"-m")==0)
			return CM;
		else return ERR;
	}else
		return ERR;
}

/*výpočet pomocí zřetězeného zlomku
čitatel je konstantní 1
jmenovatel se větví vždy jako (dvojnásobek aktuálního průchodu-1/původní x)-další zřetězený zlomek
*/

double cfrac_tan(double x, unsigned int n){
	double cf=0.;
	double b=1.;
	for(unsigned int i=n;i>0;i--){
		double a=(2*i-1)/x;
		cf=b/(a-cf);
	}
	return cf;
}

/*výpočet pomocí taylorova polynomu
jedná se o součtovou řadu
koeficienty jsou zadané
mocnina v čitateli je vždy vynásobena původním x^2
*/

double taylor_tan(double x, unsigned int n){
	long int citatel[13]={1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
	long int jmenovatel[13]={1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
	double result=0.;
	double t=x*x;
	for(unsigned int i=0;i<n;i++){
		result=result+x*citatel[i]/jmenovatel[i];
		x=x*t;
	}
	return result;
}

typedef enum {OK,ERR_IN,ERR_ARG} result;

/*
funkce pro výpočet tangens
ověřuje správnost vstupních dat (nuly, interval)
poté provede n iterací pomocí tří metod a vypisuje průběžné výsledky
návratová hodnota určuje OK nebo chybu
*/

result tangens(char *argv[]){
	char *xe,*ne,*me;
	double x=strtod(argv[2],&xe);
	double n=strtod(argv[3],&ne);
	double m=strtod(argv[4],&me);
	if(!(*xe==0 && *ne==0 && *me==0))
		return ERR_ARG;
	if(!(n<=m && m<14 && n>0))
		return ERR_ARG;

	double tan_math=tan(x);
	double tan_taylor;
	double tan_cfrac;
	
	for(unsigned int i=n;i<=m;i++){
		tan_taylor=taylor_tan(x,i);
		tan_cfrac=cfrac_tan(x,i);
		printf("%d %e %e %e %e %e\n",i,tan_math,tan_taylor,fabs(tan_taylor-tan_math),tan_cfrac,fabs(tan_cfrac-tan_math));
	}
	return OK;
}


/*funkce pro výpočet vzdálenosti a výšky
na základě vstupních parametrů počítá buď vzdálenost a nebo obojí
rozlišuje vstup výšky měřícího přístroje, pokud není zádán tak =1.5
ověřuje správnost vstupů, jestli všechna čísla odpovídají ureným inetrvalům
poté vypočítá pomocí vztahu pravoúhlého trojúhelníku tan(x)=protilehlá/přilehlá vzdálenost a výšku
pro výpočet tan používá funkci zřetězeného zlomku
*/
double metric(double c,int argc, char *argv[]){
	double a;
	double b=-1.;
	bool check=false;

	if(c==1){
		c=strtod(argv[2],NULL);
		a=strtod(argv[4],NULL);
		if(argc==6){
			b = strtod(argv[5],NULL);
			check=true;
		}
	}else{
		c=1.5;
		a=strtod(argv[2],NULL);
		if(argc==4){
			b = strtod(argv[3],NULL);
			check=true;
		}
	}

	if(!(0<a && a<=1.4 && 0<c && c<=100)){
		return EXIT_FAILURE;
	}

	if(check)
		if(!(b>0 && b<=1.4))
			return EXIT_FAILURE;

	a=c/cfrac_tan(a,10);
	printf("%.10e\n",a);

	if(check){
		b=cfrac_tan(b,10)*a;
		printf("%.10e\n",b+c);
	}


	return EXIT_SUCCESS;
}


/*funkce zavolá funkci pro kontrolu vstupních parametrů(kontroluje první argument určují funkci, a poté počet argumentů)
dle návratové hodnoty zavolá příslušné funkce a vrátí příslušnou hodnotu
*/

result input_check(int argc, char *argv[]){
	if(argc<2)
		return ERR_IN;
	switch(check_argv(argc,argv)){
		case HELP: show_help();return OK;
		case TAN: {
			switch(tangens(argv)){
				case OK: return OK;
				case ERR_ARG: return ERR_ARG;
				case ERR_IN: return ERR_IN;
			}
		}
		case M:	if(metric(0,argc,argv)==EXIT_FAILURE){return ERR_ARG;}else{return OK;}
		case CM: if(metric(1,argc,argv)==EXIT_FAILURE){return ERR_ARG;}else{return OK;}
		case ERR:return ERR_IN;
	}
	return EXIT_FAILURE;
}
/*
main pouze volá ověřovací funkci

Invalid input značí, že určení operace (--tan,-m) není správné nebo není pro danou operaci zadán správný počet argumentů
Invalid arguments značí, že vstupní čísla neodpovídají vstupním intervalům (0<A<=1.4)
*/

int main(int argc, char *argv[]) {
	switch(input_check(argc,argv)){
		case ERR_IN: fprintf( stderr, "Error: Invalid input! Try: --help\n"); return EXIT_FAILURE;
		case ERR_ARG: fprintf(stderr, "Error: Invalid arguments! Try --help\n"); return EXIT_FAILURE;
		case OK: return EXIT_SUCCESS;
	};
}
