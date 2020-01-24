/**
 * @file proj3.h
 * @author Petr Křehlík
 * @date 2017/12/9
 * @brief Hlavní soubor programu
 */

/*! \mainpage
* Jednoduchý program pro vytváření shlukové analýzy.
*/

/** @defgroup Operace_s_clusterem
 * @brief Tento modul pracuje s clusterem
*/

/** @brief Objekt
* @pre X a Y musí být v rozmezí 0<=X<=1000
 */
struct obj_t {
    int id;	/**< Jedinečný identifikátor */  
    float x; /**< X souřadnice */ 
    float y; /**< Y souřadnice */ 
};

/** @brief Cluster
 *
 *	Obsahuje velikost, kapacitu a pole objektů
 */
struct cluster_t {
    int size; /**< Skutečný počet objektů */ 
    int capacity; /**< Maximální počet objektů (alokovaná pamět)*/ 
    struct obj_t *obj; /**< Ukazatel na pole objektů */ 
};

/** @addtogroup Operace_s_clusterem
 * @brief Incializuje cluster a alokuje potřebnou paměť
 * @param *c Ukazatel na cluster
 * @param cap Kapacita
 */
void init_cluster(struct cluster_t *c, int cap);

/** @brief Odstraní všechny objekty clusteru a uvolní pamět
 * @param *c Ukazatel na cluster
 * @addtogroup Operace_s_clusterem
 */
void clear_cluster(struct cluster_t *c);

/** @brief Určuje o kolik se rozšíří kapacita clusteru
 */
extern const int CLUSTER_CHUNK;

/** @brief Změní kapacitu clusteru
 * @param *c Ukazatel na cluster
 * @param new_cap Požadovaná kapacita
 * @return Ukazatel na cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/** @brief Přidá na konec clusteru objekt
 * @param *c Ukazatel na cluster
 * @param obj Objekt
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/** @brief Spojí dva clustery
 * @param *c1 Ukazatel na cluster
 * @param *c2 Ukazatel na cluster
 * @post V clusteru C1 bude výsledný cluster seřazený vzestupně podle ID
 * @post C2 zůstane nezměněný
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/** @brief Odstraní cluster z pole clusterů
 * @param *carr Ukazatel na pole clusterů
 * @param narr Počet objektů v clusteru
 * @param idx ID shluku, který bude odstraněn
 * @return Nový počet clusterů v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/** @brief Počítá vzdálenost dvou objektů Pythagorovou větou
 * @param *o1 Ukazatel na objekt 1
 * @param *o2 Ukazatel na objekt 2
 * @return Vzdálenost objektů
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/** @brief Počítá vzdálenost dvou clusterů
 * 
 * Jako velikost clusteru se bere průměr vzdálenosti všech jeho objektů.
 * Z výsledných hodnoty se vytvoří rozdíl
 * @param *c1 Ukazatel na cluster 1
 * @param *c2 Ukazatel na cluster 2
 * @return Vzdálenost clusterů
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/** @brief Hledá dva nejbližší clustery
 * 
 * Projde všechny clustery v poli clusterů a najde dva nejbližší clustery
 * @param *c1 Index prvního nalezeného clusterů
 * @param *c2 Index druhého nalezeného clusterů
 * @param narr Počet clusterů v poli
 * @param *carr Ukazatel na pole clusterů
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/** @brief Seřadí objekty v clusteru vzestupně podle ID
 * @param *c Ukazatel na cluster
 */
void sort_cluster(struct cluster_t *c);

/** @brief Vypíše cluster na výstup
 * @param *c Ukazatel na cluster
 */
void print_cluster(struct cluster_t *c);

/** @brief Načítání clusterů
 *
 * Načte objekty z souboru a každý objekt vloží do samostatného clusteru v poli clusterů
 * @param **arr Ukazatel na ukazatel na pole clusterů
 * @param *filename Ukazatel na jméno souboru
 * @return Počet načtených clusterů
 */
int load_clusters(char *filename, struct cluster_t **arr);

/** @brief Vypíše všechny clustery z pole na výstup
 * @param *c Ukazatel na pole clusterů
 */
void print_clusters(struct cluster_t *carr, int narr);
