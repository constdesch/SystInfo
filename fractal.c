#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <getopt.h>

frac_t *fractal_new(const char *name, int width, int height, double a, double b)
{
    frac_t *frac = (frac_t *) malloc(sizeof(frac_t));
    if(!frac) return NULL;
    strcpy(frac->name,name);
    frac->height = height;
    frac->width = width;
    frac->reel = a;
    frac->imaginaire = b;
    frac->value = (int *) malloc(sizeof(int)*height*width);
    if(!frac->value){return NULL;}
    return frac;
    
}



void fractal_free(struct fractal *f)
{
   free(f->value);  free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
    return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    return *(f->value + y*f->width + x);
    
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    *(f->value + y*(f->width) + x) = val;
    f->mean += val;
}

void fractal_set_mean(frac_t *f, int n){
    f->mean = (double) f->mean/n;
}
int fractal_get_width(const struct fractal *f)
{
    return f->width;
}

int fractal_get_height(const struct fractal *f)
{
    return f->height;
}

double fractal_get_a(const struct fractal *f)
{
    return f->reel;
}

double fractal_get_b(const struct fractal *f)
{
    return f->imaginaire;
}

void *compare(void* a) {
    int* d = (int*)a;
    frac_t* f=malloc(sizeof(frac_t));
    char buffer1[70] ;
    char buffer2[4] = ".bmp" ;
    while (*(NB_F_COMP)!=*(NB_F_OUT) && *(NB_FILE_DOWN) != *(NB_FILE))
    {
        f = sbuf_catch1(sp1);
        if (*leplusgrand == NULL) {
            *leplusgrand = f;
        }
        else if((*(leplusgrand))->mean < f->mean) {
            if (*d) {
                    strcpy(buffer1, (*(leplusgrand))->name);
                int error = write_bitmap_sdl(*leplusgrand, strcat(buffer1, buffer2));
            }
            *(leplusgrand) = f;
            
        }
        if (*d){
            strcpy(buffer1, (f->name));
            int error = write_bitmap_sdl(f, strcat(buffer1, buffer2));
                       }
    }
    pthread_exit((void**)leplusgrand);
    sbuf_clean(sp);
    sbuf_clean(sp1);
}



void *producer(void *fichier) {
    char* filename;
    FILE *file;
     filename = (char*) fichier;
    if (strcmp(filename,"-") ){
        file = fopen(stdin, "r");/* file est un pointeur de pointeur */
    }
    else {
        file = fopen(filename, "r"); /* En lecture seulement */
    }
    if (file != NULL) {
        char line[85]; /* Taille max d'une ligne */
        while (fgets(line, sizeof(line), file) != NULL) /* Lire une ligne */
        {
            
            frac_t *frac = malloc(sizeof(frac_t));
            
            if (!frac) {
                fclose(file);
                return NULL;
            }
            else
                frac = readLine(line);
            
            if (frac != NULL) {
                sbuf_insert(sp, frac);
                
            }
        }
        fclose(file);
        mutex_lock(&mutex2);
        *(NB_FILE_DOWN)++;
        mutex_unlock(&mutex2);
        
    }
    else
    {
        perror(file);
        return NULL;
    }
    return NULL;
}

void *consumer(void* a) {
    frac_t* f;
    while (*(NB_F_INSERT)!=*(NB_F_OUT) && *(NB_FILE_DOWN)!=*(NB_FILE)) {
        f = sbuf_catch(sp);
        // Calcul des fractales
        int w, h, x, y;
        w = fractal_get_width(f);
        h = fractal_get_height(f);
        for (x = 0; x < w; x++) {
            for (y = 0; y < h; y++) {
                fractal_compute_value(f, x, y);
            }
        }
        sbuf_insert1(sp1, f);
    }
    return NULL;
}

void sbuf_init(sbuf_t *sp, int n)
{

    sp->n = n;
    sp->rear = sp->front = 0;    /* Buffer vide si front == rear */
    sem_open(&sp->slots, 0, n);      /* Au début, n slots vides */
    sem_open(&sp->items, 0, 0);      /* Au début, rien à consommer */
}

void sbuf_clean(sbuf_t *sp){
    free(sp->buf);
    free(sp);
}


void sbuf_insert(sbuf_t *sp, frac_t *fract)
{
    sem_wait(&sp->slots);
    mutex_lock(&mutex);
    sp->rear += 1;
    (sp->buf)[sp->rear%sp->n] = fract;
    *(NB_F_INSERT) += 1;
    mutex_unlock(&mutex);
    sem_post(&sp->items);
}

frac_t *sbuf_catch(sbuf_t *sp)
{
    sem_wait(&sp->items);
    mutex_lock(&mutex);
    frac_t *retour = (sp->buf)[(sp->front + 1) % sp->n];
    sp->front += 1;
    *(NB_F_OUT) += 1;
    mutex_unlock(&mutex);
    sem_post(&sp->slots);
    return retour;
}
void sbuf_insert1(sbuf_t *sp, frac_t *fract)
{
    sem_wait(&sp->slots);
    mutex_lock(&mutex);
    sp->rear += 1;
    (sp->buf)[sp->rear%sp->n] = fract;
    mutex_unlock(&mutex);
    sem_post(&sp->items);
}
frac_t *sbuf_catch1(sbuf_t *sp)
{
    sem_wait(&sp->items);
    mutex_lock(&mutex1);
    frac_t *retour = (sp->buf)[(sp->front + 1) % sp->n];
    sp->front += 1;
    *(NB_F_COMP) += 1;
    mutex_unlock(&mutex1);
    sem_post(&sp->slots);
    return retour;
}
/* Lis une ligne d'un fichier, crée la fractale et la renvoie
 *
 *
 */
frac_t *readLine(char* line) {
    char name[65];
    int width, height, entries;
    float a, b;
    entries = sscanf(line, "%s %d %d %f %f", name, &width, &height, &a, &b);
    
    if ((entries == 5) && (*name != '#'))
        return fractal_new(name, width, height, (double)a, (double)b);
    else return NULL;
}

void calculate(frac_t* f) {
    int w, h, x, y;
    w = fractal_get_width(f);
    h = fractal_get_height(f);
    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            fractal_compute_value(f, x, y);
        }
    }
    fractal_set_mean(f, w*h);
}


