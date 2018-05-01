#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fractal.h"
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <getopt.h>




int main(int argc, char *argv[])
{
    int d_flag;
    int maxthreads_flag;
    int maxthreads_value;
	sbuf_init(sp, 100);
	sbuf_init(sp1, 100);
	*leplusgrand = NULL;
	*NB_F_INSERT = *NB_FILE_DOWN = *NB_F_COMP = *NB_F_OUT = 0;
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
	if (pthread_mutex_init(&mutex3, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
	}

    int c;

    while (1)
    {
        static struct option long_options[] =
        {
            {"maxthreads", required_argument,            0,'m'},
            {"doption"         , no_argument      ,          & d_flag, 1},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, ":m:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'm':
                maxthreads_flag = 1;
                maxthreads_value = atoi(optarg);
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort ();
        }
    }

    /* Instead of reporting ‘--verbose’
     and ‘--brief’ as they are encountered,
     we report the final status resulting from them. */
    if (d_flag)
        puts ("d flag is set");
    if (maxthreads_flag){
        puts ("maxthreads_flag is set");
        printf("value : %d \n",maxthreads_value);
    }
    *NB_FILE = argc - d_flag - maxthreads_flag - 2 ; //le 2 vient du fichierout et du premier argument qui est le nom du prog. /!\ si pas de fichier, Nb_file sera nega.
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
           pthread_t* tab[argc - optind];
           int a=0;
           printf ("non-option ARGV-elements: ");
        while (optind < argc-1){ /* s'rrête au dernier fichier in! */
             int error = pthread_create(tab[a++],NULL,producer, argv[optind]);
            printf ("%s ", argv[optind++]);
        }
        putchar ('\n');
    }

    if (maxthreads_value == 0) {
        pthread_t* tabular[4];
        int i;
        for (i = 0; i < 4; i++) {
            int error = pthread_create(tabular[i], NULL, consumer, NULL);
        }
    }
    else {
        pthread_t* tabular[(maxthreads_value)];
        int i;
        for (i = 0; i < (maxthreads_value); i++) {
            int error = pthread_create(tabular[i], NULL, consumer, NULL);
        }
    }
    pthread_t* compa;
    int error=pthread_create(compa,NULL,compare,(void*) &d_flag);
    void** retourf;
    pthread_join(*compa, retourf);
    frac_t** retour = malloc(8);
    retour = (frac_t**) retourf;
    return 0;
}
