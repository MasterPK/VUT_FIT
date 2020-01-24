/*
 * File:  proj1.c
 * Date:   2017/10/15
 * Author:   Petr Krehlik, xkrehl04
 * Project: First project to IZP
 */

//load libraries

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*test input is correct
-1: err
 0: no arg
>0: input length
*/

int test_input(int argc, char *argv[]){
	int input_len=0;
	if(argc==2 && argv[1]!=NULL){
		input_len=strlen((argv[1]));
	}else if(argc>2){
		return -1;
	}
	return input_len;
}

//load input to array

int load_input(char *argv[], char *input){
	unsigned int a=0;
	while(a<strlen(argv[1])){
		if(!(argv[1][a]>31 && argv[1][a]<127)){
			return -1;
		}
		input[a]=toupper(argv[1][a]);
		a++;
	}
	input[a]='\0';
	return 0;
}

/*load cities from input ot array
return count of cities
*/

int load_cities(char cities[42][101]){
	int character = 0;
	int char_count=0;
	int cities_count=0;
	int long_name=0;

	while((character=toupper(getchar()))!=EOF){
		while(character!='\n'){
			if(!(character>31 && character<127)){
				return -1;
			}
			if(char_count<100)
				cities[cities_count][char_count]=character;
			char_count++;
			character=toupper(getchar());
		}
		if(char_count>100){
			cities[cities_count][100]='\0';
			if(long_name==0){
				fprintf( stderr, "Warning: Some of city name is longer than 100 characters! Rest of characters in that name will be dropped!\n");
				long_name=1;
			}
		}else{
			cities[cities_count][char_count]='\0';
		}
		char_count=0;
		cities_count++;
	}
	return cities_count;
}

//sort array in alphabetical order

void sort(char *result_chars, int result_count){
	char v;
	for(int i=0;i<result_count;i++){
		for(int z=0;z<result_count;z++){
			if(result_chars[i]<result_chars[z]){
				v=result_chars[i];
				result_chars[i]=result_chars[z];
				result_chars[z]=v;
			}
		}
	}
}

//print enabled chars

void print_enable(char *result_chars, int result_count){
	printf("Enable: ");
	for(int i=0;i<result_count;i++){
		printf("%c",result_chars[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	//variable init
	char cities[42][101];
	int cities_count=0;
	
	int input_len=test_input(argc,argv);

	char input[input_len];

	// test of input length
	if(input_len>0){
		if(load_input(argv,input)==-1){
			fprintf( stderr, "Error: Invalid input!\n");
			return EXIT_FAILURE;
		};
	}else if(input_len==-1){
		fprintf( stderr, "Error: Invalid input!\n");
		return EXIT_FAILURE;
	}


	cities_count=load_cities(cities);
	char result_chars[cities_count+1];


	//print all chars if no argument
	int result_count=0;
	if(input_len==0){
		fprintf( stderr, "Warning: No input parameter detected!\n");
		for(int i=0;i<cities_count;i++){
			int exist=0;
			for(int y=0;y<result_count;y++){
				if(result_chars[y]==cities[i][0]){
					exist=1;
					break;
				}
			}
			if(exist==0)
				result_chars[result_count++]=cities[i][0];
		}
		sort(result_chars,result_count);
		print_enable(result_chars,result_count);
		return EXIT_SUCCESS;
	}

	// test of cities count
	if(cities_count>42){
		fprintf( stderr, "Error: Cities count is bigger than 42! Program won't accept it!\n");
		return EXIT_FAILURE;
	}else if(cities_count==-1){
		fprintf( stderr, "Error: Cities input is not valid ASCII characters!\n");
		return EXIT_FAILURE;
	}

	int i=0;
	int found=0;
	int prefix_found=0;
	int prefix_id=0;
	// main processing cycle
	while(i<cities_count){	//
		if(strlen(cities[i])==strlen(input)){
			int equal=1;
			int z=0;
			while(cities[i][z]!='\0'){
				if(input[z]==cities[i][z]){
				}else{
					equal=0;
					break;
				}
				z++;
			}
			if(equal==1 && input_len!=0 && found==0){
				found=1;
				printf("Found: %s\n",input);
			}
		}else if(strlen(cities[i])>strlen(input)){
			int equal=1;
			int z=0;

			while(input[z]!='\0'){
				if(input[z]==cities[i][z]){
				}else{
					equal=0;
					break;
				}
				z++;
			}
			if(equal==1){
				prefix_found++;
				prefix_id=i;
				int exist=0;
				for(int y=0;y<result_count;y++){
					if(result_chars[y]==cities[i][z]){
						exist=1;
						break;
					}
				}
				if(exist==0)
					result_chars[result_count++]=cities[i][z];
			}
		}
		i++;
	}
	if(prefix_found==1 && found==0){
		printf("Found: %s\n",cities[prefix_id]);
	}else if(result_count>0){
		sort(result_chars,result_count);
		print_enable(result_chars,result_count);
	}else if(found==0){
		printf("Not found\n");
	}

	return EXIT_SUCCESS;
}
