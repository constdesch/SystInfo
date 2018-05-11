#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfractal/fractal.h"
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <getopt.h>
#include "consumer_producer.h"


pthread_mutex_t mutex; /*  */
pthread_mutex_t mutex1; /*  */
pthread_mutex_t mutex2; /*  */
int NB_F_INSERT = 0;          //Nbre de fractales introduises dans le premier buffer
int NB_F_OUT = 0;             //Nbre de fractales sorties du premier buffer
int NB_F_COMP = 0;            //Nbre de fractales comparées
int NB_FILE_DOWN = 0;         //Nombre de fichiers lus par les consomateurs
int NB_FILE = 0;              // Nbre de fichiers totales      //Pointeur de pointeur pouvant ainsi modifier la fractale qui a la plus grande               valeur moyenne
sbuf_t sp;                //le premier buffer
sbuf_t sp1;               //le deuxième buffer
char fichier_out[65];

int main(int argc, char *argv[])
{

  sbuf_init(&sp,10); /* Premier buffer */
  sbuf_init(&sp1,10); /* Second buffer */
  /* Mutex */
  if (pthread_mutex_init(&mutex, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }

  if (pthread_mutex_init(&mutex1, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }
  if (pthread_mutex_init(&mutex2, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }
  int c;
  int digit_optind = 0;
  int maxthreads_value = 4; /* Valeur choisie arbitrairement à 4 si le parametre n'est pas spécifié */
  int maxthreads_flag = 0;
  int d_flag = 0;

  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"d"     ,  no_argument,       0,  1 },
      {"maxthreads",  required_argument, 0, 'm'},
      {0,         0,                 0,  0 }
    };

    c = getopt_long(argc, argv, "m:d",
    long_options, &option_index);
    if (c == -1)
    break;

    switch (c) {
      case 0:
      printf("option %s", long_options[option_index].name);
      if (optarg)
      printf(" with arg %s", optarg);
      printf("\n");
      break;

      case '0':
      case '1':
      case '2':
      if (digit_optind != 0 && digit_optind != this_option_optind)
      printf("digits occur in two different argv-elements.\n");
      digit_optind = this_option_optind;
      printf("option %c\n", c);
      break;

      case 'm':
      maxthreads_value = atoi(optarg);
      maxthreads_flag = 1;
      printf("option maxthreads with value '%d'\n", maxthreads_value);
      break;

      case 'd':
      d_flag = 1;
      printf("option d with flag_value : %d \n", d_flag);
      break;

      case '?':
      break;

      default:
      printf("?? getopt returned character code 0%o ??\n", c);
    }
  }
  if(maxthreads_flag)
  NB_FILE = argc - d_flag - maxthreads_flag - 2-1 ;
 //le 2 vient du fichierout et du premier argument qui est le nom du prog. /!\ si pas de fichier, Nb_file sera nega.
else
NB_FILE = argc - d_flag - maxthreads_flag - 2;
if(argc-optind<2){
  printf("vous n'avez pas rentrer assez d'argument.\n");
  return 0;
}
else{
pthread_t* tab;
  if (optind < argc)
  {
     tab=(pthread_t*)malloc(sizeof(pthread_t)*(argc - optind));
    int a=0;

    while (optind < argc-1){ /* s'rrête au dernier fichier in! */
    int error = pthread_create(&tab[a],NULL,producer, argv[optind]); /* On lance les producteurs */
    if (error != 0){ printf("pthread_create failed"); return -1;}
    int z = strcmp(argv[optind], "file.txt");
    a++;
    optind++;
  }
  strcpy(fichier_out,argv[optind]);
  printf("nom du fichier out : %s \n",fichier_out);
}
pthread_t *tabular=NULL;
if (!maxthreads_flag) {
   tabular=(pthread_t*)malloc(4*sizeof(pthread_t));
  int i;
  for (i = 0; i < 4; i++) {
    int error = pthread_create(&tabular[i], NULL, consumer, NULL); /* On lance les consommateurs */
    if (error != 0){ printf("pthread_create failed"); return -1;}
  }
}
else {
 tabular=(pthread_t*)malloc(sizeof(pthread_t)*(maxthreads_value));
  int i;
  for (i = 0; i < (maxthreads_value); i++) {
    int error = pthread_create(&tabular[i], NULL, consumer, NULL); /* On lance les consommateurs - producteurs */
    if (error != 0){ printf("pthread_create failed"); return -1;}
  }
}

pthread_t *compa=(pthread_t*)malloc(sizeof(pthread_t));
int error = pthread_create(&compa[0],NULL,compare,(void*) &d_flag); /* On lance les deuxiemes consommateurs */
if (error != 0){ printf("pthread_create failed"); return -1;}
int id;
pthread_join(compa[0],(void**)&id);
printf("id=%d\n",id);
if(compa!=NULL)
free(compa);
if(tab!=NULL)
free(tab);
if(tabular!=NULL)
free(tabular);
return 0;
}
}
