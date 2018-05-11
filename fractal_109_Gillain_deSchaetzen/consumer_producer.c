#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <getopt.h>
#include <limits.h>
#include "libfractal/fractal.h"
#include "consumer_producer.h"


void *compare(void* a) {

  int* d = (int*)a;
  char buffer1[70];
  char buffer2[5]=".bmp";
  frac_t* leplus=NULL; /* Fractal qui a la plus grande valeur moyenne */
  frac_t* tmp=NULL; /* Stockage temporaire pour pouvoir liberer l'espace memoire */
  double bestMean=(double) INT_MIN;

  while (NB_F_COMP!=NB_F_OUT || NB_FILE_DOWN != NB_FILE) /* Condition pour évaluer si il reste des choses à calculer */
  {
    frac_t*  f = sbuf_catch1(&sp1); /*On prend dans le second buffer*/
    if(*d){ /*Option -d présente?*/
      strcpy(buffer1, (f->name));
      int error = write_bitmap_sdl(f, strcat(buffer1, buffer2)); /*buffer1 = nom de la fractale, buffer2 = .bnp*/
    }
    if(bestMean<f->mean){
      bestMean=f->mean;
      tmp=leplus;
      leplus=f;
      if(tmp!=NULL)
      fractal_free(tmp); /* On free la plus grande fractale précédente */
    }
    else{
      fractal_free(f); /* Si f n'est pas la plus grande on peut la free */
    }
  }
  strcpy(buffer1, fichier_out);
  printf("fichierout=%s",fichier_out);
  if(leplus==NULL){
    printf("hey\n");
  }
  else{
    print_fract(leplus);
    strcat(buffer1,buffer2);
    printf("%s",buffer1);
    int error = write_bitmap_sdl(leplus, buffer1);
    fractal_free(leplus);
  }
  sbuf_clean(&sp); /* Libere l'espace alloué pour le premier buffer */
  sbuf_clean(&sp1); /* Libere l'espace alloué pour le second buffer */
  pthread_exit((void*)1);
  return NULL;
}

void *producer(void *fichier) {
  char* filename;
  FILE *file;
  filename = (char*) fichier;
  char line[85]; /* Taille max d'une ligne */

  /* LECTURE SUR L'ENTREE STANDARD */
  if (strcmp(filename,"-") == 0){
    file=stdin;
  }

  /*LECTURE SUR UN FICHIER */
  else {
    file = fopen(filename, "r");
    if(file>=0){
    } /* En lecture seulement */
    if(strstr(filename,".txt")!=NULL){
      if (file != NULL) {
        while (fgets(line, 85, file) != NULL) /* Lire une ligne */
        {
          frac_t *  frac;
          frac= readLine(line);

          if (frac != NULL) {
            sbuf_insert(&sp, frac);
          }

        }
        printf("le fichier %s est fermé \n",filename);
        fclose(file);
      }
      else
      {
        perror(file);
        return NULL;
      }
    }
    else{
      fclose(file);
    }
  }
  pthread_mutex_lock(&mutex2); /* Debut de section critique */
  NB_FILE_DOWN++;
  pthread_mutex_unlock(&mutex2); /* Fin de section critique */
  return NULL;
}

void *consumer(void* a) {

  while (NB_F_INSERT!=NB_F_OUT || NB_FILE_DOWN!=NB_FILE) { /* Condition pour évaluer si il reste des choses à calculer */
    frac_t* f = sbuf_catch(&sp); /* On prend dans le premier buffer */
    int w, h, x, y;
    w = fractal_get_width(f);
    h = fractal_get_height(f);
    for (x = 0; x < w; x++) {
      for (y = 0; y < h; y++) {
        fractal_compute_value(f, x, y);
      }
    }
    fractal_set_mean(f, w*h);
    sbuf_insert1(&sp1, f); } /* On insère dans le second buffer */
    return NULL;
  }

  void sbuf_init(sbuf_t* sbufer,int n)
  {
    sbufer->buf = (frac_t**) malloc(sizeof(frac_t*)*n);
    if (!sbufer->buf) perror("malloc: ");
    sbufer->n = n;                       /* Taille du buffer */
    sbufer->rear = sbufer->front = 0;    /* Buffer vide si front == rear */
    sem_init(&sbufer->slots, 0, n);      /* Au début, n slots vides */
    sem_init(&sbufer->items, 0, 0);     /* Au début, rien à consommer */
  }

  void sbuf_clean(sbuf_t *sper){
    free(sper->buf);
  }
  void print_fract(frac_t *f){
    printf("Nom : %s \n",f->name);
    printf("Height : %d\n", f->height);
    printf("Width : %d\n", f->width);
    printf("Reel : %f\n",f->reel);
    printf("Imaginaire : %f\n",f->imaginaire);
    printf("mean:%f\n",f->mean);
  }


  void sbuf_insert(sbuf_t *sper, frac_t *fract)
  {
    sem_wait(&sper->slots); /* Attends qu'une place soit libre dans le buffer */
    pthread_mutex_lock(&mutex); /* Debut de section critique */
    sper->rear += 1;
    sper->buf[(sper->rear)%(sper->n)] = fract;
    NB_F_INSERT += 1;
    pthread_mutex_unlock(&mutex); /* Fin de section critique */
    sem_post(&sper->items); /* Une place de libre en moins */
  }

  frac_t *sbuf_catch(sbuf_t *sper)
  {
    sem_wait(&sper->items); /* Attend qu'une place soit prise dans le buffer */
    pthread_mutex_lock(&mutex); /* Debut de section critique */
    frac_t *retour = sper->buf[(sper->front + 1) % (sper->n)];
    sper->front += 1;
    NB_F_OUT += 1;
    pthread_mutex_unlock(&mutex); /* Fin de section critique */
    sem_post(&sper->slots); /* Un slot de libre en plus */
    return retour;
  }
  void sbuf_insert1(sbuf_t *sper, frac_t *fract)
  {
    sem_wait(&sper->slots); /* Attends qu'une place soit libre dans le buffer */
    pthread_mutex_lock(&mutex1); /* Debut de section critique */
    sper->rear += 1;
    sper->buf[(sper->rear) % (sper->n)] = fract;
    pthread_mutex_unlock(&mutex1); /* Fin de section critique */
    sem_post(&sper->items); /* Une place de libre en moins */
  }
  frac_t *sbuf_catch1(sbuf_t *sper)
  {
    sem_wait(&sper->items); /* Attend qu'une place soit prise dans le buffer */
    pthread_mutex_lock(&mutex1); /* Debut de section critique */
    frac_t *retour = sper->buf[(sper->front + 1) % sper->n];
    sper->front += 1;
    NB_F_COMP += 1; //Premiere valeure de leplusgrand
    pthread_mutex_unlock(&mutex1); /* Fin de section critique */
    sem_post(&sper->slots); /* Un slot de libre en plus */
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
    entries = sscanf(line, "%s %d %d %f %f", name, &width, &height, &a, &b); /* Pattern Matching */

    if ((entries == 5) && (*name != '#'))
    return fractal_new(name, width, height, (double)a, (double)b);
    else return NULL;
  }
