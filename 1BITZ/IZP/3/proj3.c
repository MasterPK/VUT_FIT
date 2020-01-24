/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> // strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/

void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);
	if(cap==0){
		c->size=0;
		c->capacity=cap;
		c->obj=NULL;
	}else{
		c->size=0;
		c->capacity=cap;
		c->obj=malloc(sizeof(struct obj_t)*cap);
		if(c->obj==NULL){
			fprintf(stderr, "Error: Alloc failed!\n");
			exit(1);
		}
	}
    // TODO

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	free(c->obj); 
	c->size=0;
	c->capacity=0;
	
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
		return NULL;

	c->obj = (struct obj_t*)arr;
	c->capacity = new_cap;
	return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if(c->capacity>c->size){
		c->obj[c->size]=obj;
		c->size=c->size+1;
	}else{
		if(resize_cluster(c,c->capacity+CLUSTER_CHUNK)!=NULL){
			c->obj[c->size]=obj;
			c->size=c->size+1;
		}else{
			fprintf(stderr, "Error: Resize cluster failed!!\n"); 
		}

	}

}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);
	
	for(int i=0;i<c2->size;i++){
		append_cluster(c1,c2->obj[i]);
	}
	sort_cluster(c1);
}

void free_all(struct cluster_t *clusters,int count){
	for(int i=0;i<count;i++){
		clear_cluster(&clusters[i]);
	}
	free(clusters);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);

	clear_cluster(&carr[idx]);
	for(int i=idx;i<narr-1;i++){
		carr[i]=carr[i+1];
	}

	return narr-1;
    // TODO
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
	assert(o1 != NULL);
	assert(o2 != NULL);
	
	float x=o1->x-o2->x;
	float y=o1->y-o2->y;
	float res = sqrtf((x*x)+(y*y));

	return res;
    // TODO
}

/*
 Pocita vzdalenost dvou shluku.
 prumer
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	int pocet=0;
	float soucet=0;

	for(int i=0;i<c1->size;i++){
		for(int k=0;k<c2->size;k++){
			soucet=soucet+obj_distance(&c1->obj[i],&c2->obj[k]);
			pocet++;
		}
	}
	if(pocet!=0)
		return soucet/pocet;
	else
		return -1;
    // TODO
}


//prémiové body - metoda nejvzdálěnějšího souseda
float cluster_distance_farthest(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	float max=INT_MIN;

	for(int i=0;i<c1->size;i++){
		for(int k=0;k<c2->size;k++){
			float max_c = obj_distance(&c1->obj[i],&c2->obj[k]);
			if(max_c>max)
				max=max_c;
		}
	}
	return max;
    // TODO
}

//prémiové body - metoda nejbližšího souseda
float cluster_distance_nearest(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	float min=INT_MAX;

	for(int i=0;i<c1->size;i++){
		for(int k=0;k<c2->size;k++){
			float min_c = obj_distance(&c1->obj[i],&c2->obj[k]);
			if(min_c<min)
				min=min_c;
		}
	}
	return min;
    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);

	float min_c=INT_MAX;
	int p=0;
	float min=0;
	for(int i=0;i<narr;i++){
		++p;
		for(int k=p;k<narr;k++){
			if(i==k)
				continue;
			min=cluster_distance(&carr[i],&carr[k]);
			
			if(min<min_c){
				min_c=min;
				*c1=i;
				*c2=k;
			}
		}
	}
}

void find_neighbours_farthest(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);

	float min_c=INT_MAX;
	int p=0;
	float min=0;
	for(int i=0;i<narr;i++){
		++p;
		for(int k=p;k<narr;k++){
			if(i==k)
				continue;
			min=cluster_distance_farthest(&carr[i],&carr[k]);
			
			if(min<min_c){
				min_c=min;
				*c1=i;
				*c2=k;
			}
		}
	}
}

void find_neighbours_nearest(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);

	float min_c=INT_MAX;
	int p=0;
	float min=0;
	for(int i=0;i<narr;i++){
		++p;
		for(int k=p;k<narr;k++){
			if(i==k)
				continue;
			min=cluster_distance_nearest(&carr[i],&carr[k]);
			
			if(min<min_c){
				min_c=min;
				*c1=i;
				*c2=k;
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
	const struct obj_t *o1 = (const struct obj_t *)a;
	const struct obj_t *o2 = (const struct obj_t *)b;
	if (o1->id < o2->id) return -1;
	if (o1->id > o2->id) return 1;
	return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);
	FILE *file;
	file =fopen(filename,"r");
	if(file==NULL){
		fprintf(stderr, "Error: Cannot open file!\n"); 
		return -1;
	}

	int count=0;
	fscanf(file,"count=%d",&count);
	if(count==0){
		fclose(file);
		fprintf(stderr, "Error: Bad file format!\n");
		return -1; 
	};
	*arr= malloc(sizeof(struct cluster_t)*count);

	if(*arr==NULL){
		fprintf(stderr, "Error: Alloc failed!\n");
		return -1;
	}

	int id=0;
	float x=0;
	float y=0;
	int i=0;
	int err=fscanf(file,"%d %f %f \n",&id,&x,&y);

	while(i<count)
	{ 
		init_cluster(&(*arr)[i],1);

		if(err!=3 || err==EOF){

			fprintf(stderr, "Error: Bad file format!\n");
			for(int k=0;k<count;k++){
				clear_cluster(&(*arr)[k]);
			}
			free(*arr);
			fclose(file);
			return -1; 
		}
		
		if(!(0<=x && x<=1000 && 0<=y && y<=1000)){
			fprintf(stderr, "Error: Bad input data!\n");
			for(int k=0;k<count;k++){
				clear_cluster(&(*arr)[k]);
			}
			free(*arr);
			fclose(file);
			return -1;
		}

		for(int j=0;j<i;j++){
			if(id==(*arr)[j].obj->id){
				fprintf(stderr, "Error: Found two same ID!\n");
				for(int k=0;k<count;k++){
					clear_cluster(&(*arr)[k]);
				}
				free(*arr);
				fclose(file);
				return -1;
			}
		}


		(*arr)[i].size=1;
		(*arr)[i].obj->id=id;
		(*arr)[i].obj->x=x;
		(*arr)[i].obj->y=y;	
		i++;
		err=fscanf(file,"%d %f %f \n",&id,&x,&y);
	}

	fclose(file);
	return count;    

}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++)
	{
		printf("cluster %d: ", i);
		print_cluster(&carr[i]);
	}
}

int main(int argc, char *argv[])
{
	struct cluster_t *clusters;
	int count=0;
	int n=1;
	char * error;
	int method=0;


	/*if(argc==2){
		count = load_clusters(argv[1],&clusters);
	}else if(argc==3){
		count = load_clusters(argv[1],&clusters);
		n=strtol(argv[2],NULL,0);
	}else{
		fprintf(stderr, "Error: Invalid arguments!\n"); 
		return EXIT_FAILURE;
	}
*/

	switch(argc){
		case 2:{
			count = load_clusters(argv[1],&clusters);
			break;
		}
		case 3:{
			n=strtol(argv[2],&error,0);
			if(error[0] != '\0'){
				fprintf(stderr, "Error: Invalid arguments! N must be bigger than zero and smaller than objects count!\n"); 
				return EXIT_FAILURE;
			}
			count = load_clusters(argv[1],&clusters);
			break;
		}
		case 4:{
			n=strtol(argv[2],&error,0);
			if(error[0] != '\0'){
				fprintf(stderr, "Error: Invalid arguments! N must be bigger than zero and smaller than objects count!\n"); 
				return EXIT_FAILURE;
			}
			if(strcmp(argv[3],"--avg")==0){
				method=0;
			}else if(strcmp(argv[3],"--max")==0)
				method=1;
			else if(strcmp(argv[3],"--min")==0){
				method=2;
			}else{
				fprintf(stderr, "Error: Invalid arguments!  \'%s\' is not valid argument!\n",argv[3]); 
				return EXIT_FAILURE;
			}
			count = load_clusters(argv[1],&clusters);
			break;
		}
		default:{
			fprintf(stderr, "Error: Invalid arguments!\n"); 
			return EXIT_FAILURE;
		}
	}



	if(count==-1)
		return EXIT_FAILURE;
	
	if(n>count || n<=0){
		fprintf(stderr, "Error: Invalid arguments! N must be bigger than zero and smaller than objects count!\n"); 
		return EXIT_FAILURE;
	}
	int i1;
	int i2;
	switch(method){
		case 0:{
			while(count!=n){
				find_neighbours(clusters,count,&i1,&i2);
				merge_clusters(&clusters[i1],&clusters[i2]);
				count=remove_cluster(clusters,count,i2);
			}
		}
		case 1:{
			while(count!=n){
				find_neighbours_farthest(clusters,count,&i1,&i2);
				merge_clusters(&clusters[i1],&clusters[i2]);
				count=remove_cluster(clusters,count,i2);
			}
		}
		case 2:{
			while(count!=n){
				find_neighbours_nearest(clusters,count,&i1,&i2);
				merge_clusters(&clusters[i1],&clusters[i2]);
				count=remove_cluster(clusters,count,i2);
			}
		}
	}


	if(count>0)
		print_clusters(clusters,count);

	free_all(clusters,count);


	return 0;

    // TODO
}
