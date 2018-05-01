#include <pthread.h>
#include <semaphore.h>
#ifndef _FRACTAL_H
#define _FRACTAL_H


typedef struct fractal {
    char name[64];
    int height;
    int width;
    double reel;
    double imaginaire;
    int *value;
	double mean;
} frac_t;

typedef struct {
    int n;             /* Nombre de slots dans le buffer */
    frac_t *buf[100];   /* Buffer partagé */
    int front;         /* buf[(front+1)%n] est le premier élément */
    int rear;          /* buf[rear%n] est le dernier */
    sem_t slots;       /* Nombre de places libres */
    sem_t items;       /* Nombre d'items dans le buffer */
} sbuf_t;

extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern pthread_mutex_t mutex3;
extern int *NB_F_INSERT;          //Nbre de fractales introduises dans le premier buffer
extern int *NB_F_OUT;             //Nbre de fractales sorties du premier buffer
extern int *NB_F_COMP;            //Nbre de fractales comparées
extern int *NB_FILE_DOWN;         //Nombre de fichiers lus par les consomateurs
extern int *NB_FILE;              // Nbre de fichiers totales
extern frac_t **leplusgrand;      //Pointeur de pointeur pouvant ainsi modifier la fractale qui a la plus grande               valeur moyenne
extern sbuf_t *sp;                //le premier buffer
extern sbuf_t *sp1;               //le deuxième buffer

/*
 * fractal_new: alloue une nouvelle structure fractal
 *
 * @name: nom de la fractale
 * @width: largeur de l'image finale
 * @height: hauteur de l'image finale
 * @a: partie réelle des coordonnées de la fractale
 * @b: partie imaginaire des coordonnées de la fractale
 * @return: un pointeur vers une struct fractal, ou NULL si erreur
 */
struct fractal *fractal_new(const char *name, int width, int height, double a, double b);

/*
 * fractal_free: libère la mémoire utilisée par une struct fractal
 *
 * @f: fractale à libérer
 */
void fractal_free(struct fractal *f);

/*
 * fractal_get_name: retourne le nom de la fractale
 *
 * @f: fractale
 * @return: nom de la fractale
 */
const char *fractal_get_name(const struct fractal *f);

/*
 * fractal_get_value: retourne la valeur correspondant à un pixel de l'image
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @return: valeur du pixel (x,y) de l'image de la fractale
 */
int fractal_get_value(const struct fractal *f, int x, int y);

/*
 * fractal_set_value: défini la valeur correspondant à un pixel de l'image
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @val: valeur
 */
void fractal_set_value(struct fractal *f, int x, int y, int val);

/*
 * fractal_get_width: retourne la largeur de l'image de la fractale
 *
 * @f: fractale
 * @return: largeur
 */
int fractal_get_width(const struct fractal *f);

/*
 * fractal_get_height: retourne la hauteur de l'image de la fractale
 * 
 * @f: fractale
 * @return: hauteur
 */
int fractal_get_height(const struct fractal *f);

/*
 * fractal_get_a: retourne la partie réelle des coordonnées de la fractale
 * 
 * @f: fractale
 * @return: partie réelle
 */
double fractal_get_a(const struct fractal *f);

/*
 * fractal_get_b: retourne la partie imaginaire des coordonnées de la fractale
 *
 * @f: fractale
 * @return: partie imaginaire
 */
double fractal_get_b(const struct fractal *f);

/*
 * fractal_compute_value
 *
 * Applique la valeur de récurrence sur la fractale, aux coordonnées correspondantes
 * au pixel (x,y) de l'image finale, et appelle fractal_set_value() pour
 * enregistrer le nombre d'itérations effectuées.
 *
 * @f: fractale
 * @x: abscisse
 * @y: ordonnée
 * @return: nombre d'itérations
 */
int fractal_compute_value(struct fractal *f, int x, int y);

/*
 * write_bitmap_sdl
 *
 * Transforme une fractale en un fichier bitmap (BMP)
 *
 * @f: fractale à transformer
 * @fname: nom du fichier de sortie
 * @return: 0 si pas d'erreurs, -1 sinon
 */
int write_bitmap_sdl(const struct fractal *f, const char *fname);

void *compare(void* a);

void *producer(void *fichier);

void *consumer(void*);

void sbuf_init(sbuf_t *sp, int n);

void sbuf_clean(sbuf_t *sp);

void sbuf_insert(sbuf_t *sp, frac_t *fract);

frac_t *sbuf_catch(sbuf_t *sp);

void sbuf_insert1(sbuf_t *sp, frac_t *fract);

frac_t *sbuf_catch1(sbuf_t *sp);

frac_t *readLine(char* line);

void calculate(frac_t* f);




#endif
