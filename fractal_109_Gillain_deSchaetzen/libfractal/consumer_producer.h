
typedef struct {
    int n;             /* Nombre de slots dans le buffer */
    frac_t **buf;      /* Buffer partagé */
    int front;         /* buf[(front+1)%n] est le premier élément */
    int rear;          /* buf[rear%n] est le dernier */
    sem_t slots;       /* Nombre de places libres */
    sem_t items;       /* Nombre d'items dans le buffer */
} sbuf_t;


/* VARIABLES GLOBALES */
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern int NB_F_INSERT;          //Nbre de fractales introduises dans le premier buffer
extern int NB_F_OUT;             //Nbre de fractales sorties du premier buffer
extern int NB_F_COMP;            //Nbre de fractales comparées
extern int NB_FILE_DOWN;         //Nombre de fichiers lus par les consomateurs
extern int NB_FILE;              // Nbre de fichiers totales     //Pointeur de pointeur pouvant ainsi modifier la fractale qui a la plus grande               valeur moyenne
extern sbuf_t sp;                //le premier buffer
extern sbuf_t sp1;               //le deuxième buffer
extern char fichier_out[65];


/*
 * @compare: prend une structure fractale dans le buffer et compare la valeur moyenne de celle-ci avec
 *           celle qui a la plus grande. Garde en mémoire celle qui a la plus grande valeur moyenne.
 *           Si -d est présent, crée le fichier .bnp correspondant à chaque fractal.
 *           Lorsque les threads s'arrêtent, crée le fichier [fichier_out].bnp de la plus grande fractale
 *           avec fichier_out le nom mentionné dans les arguments de la fonction main.
 * @pre: a est le drapeau indiquant la présence de l'option -d
 * @return:
 */
void *compare(void* a);

/*
 * @producer: lit 'fichier', construit les structures fractales correspondantes et les insère dans le premier buffer.
 * @pre: fichier est une chaine de caractere contenant '-' ou le nom du fichier à lire.
 * @return: return NULL
 */
void *producer(void *fichier);

/*
 * @consumer: prend une structure fractale dans le buffer et fait le calcul par itération pour construire la fractale.
 *            Insère ensuite la fractale dans un second buffer.
 * @pre: /
 * @return: return NULL
 */
void *consumer(void*);

/*
 * @sbuf_init: initialise le buffer sper avec une taille n
 * @pre: sper est une structure sbuf_t, n est un entier non nul
 * @return: /
 */
void sbuf_init(sbuf_t* sper, int n);

/*
 * @sbuf_clean: libère la mémoire allouée pour le buffer
 * @pre: sp est un pointeur sbuf_t* non NULL
 * return: /
 */
void sbuf_clean(sbuf_t *sp);

/*
 * @sbuf_insert: insere la fractale fract dans le premier buffer sp
 * @pre: sp est un pointeur sbuf_t* non NULL et fract est un pointeur frac_t*
 * @return: /
 *
 */
void sbuf_insert(sbuf_t *sp, frac_t *fract);

/*
 * @sbuf_catch: prend une fractale dans le premier buffer sp
 * @pre: sp est un pointeur sbuf_t* non NULL
 * @return: /
 */
frac_t *sbuf_catch(sbuf_t *sp);

/*
 * @sbuf_insert1: insere la fractale dans le second buffer sp
 * @pre: sp est un pointeur sbuf_t* non NULL et fract est un pointeur frac_t*
 * @return: /
 */
void sbuf_insert1(sbuf_t *sp, frac_t *fract);

/*
 * @sbuf_catch1: prend une fractale dans le premier buffer sp
 * @pre: sp est un pointeur sbuf_t* non NULL
 * @return: /
 */
frac_t *sbuf_catch1(sbuf_t *sp);

/*
 * @readLine: Verifie si la chaine de caractere correspond a une fractale et la cree le cas echeant.
 * @pre: line est une chaine de caractere
 * @return: un pointeur *frac_t ou NULL si la chaine ne correspond pas a une fractal.
 */
frac_t *readLine(char* line);

/*
 * @print_fract: imprime sur la sortie standard les différentes caractéristiques de la fractal f
 * @pre: frac_t est un pointeur frac_t*
 * @return: /
 */
void print_fract(frac_t *f);
