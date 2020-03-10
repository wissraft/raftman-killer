#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// Pour compiler le fichier : gcc threads.c -o tp1 -lpthread
#define N  8
#define M  10
#define P  5
#define N_THREADS N*P
#define MAX_RANDOM 15

typedef struct {
    int  iThread;
    int Longueur;
    double Vecteur1[M];
    double Vecteur2[M];
    double *pResultat;
} ParametresThread;

/* Voici la fonction correspondant a chaque thread - à compléter */
void *ProduitScalaire(void *data) {
    ParametresThread *pParam = (ParametresThread*)data;
    double res = 0;
    for(int i = 0; i< M; i++)res += pParam->Vecteur1[i]*pParam->Vecteur2[i];
    pParam->pResultat = malloc(sizeof(pParam->pResultat));
    *pParam->pResultat = res;
    pthread_exit(NULL);
}

/* Une fonction pour afficher une matrice a l'ecran */
void AfficherMatrice(double *pMatrice, int nrangees, int ncolonnes) {
    int i,j;
    printf("Matrice : \n");
    for (i=0; i<nrangees; i++) {
        for (j=0; j<ncolonnes; j++ ) {
            printf("%10.3f ",pMatrice[i*ncolonnes+j]);
        }
        printf("\n");
    }
    printf("\n");
}

void setVecteur1(double matriceG[N][M], ParametresThread* threadParamPointer, int rowNumber){
    int column = 0;
    while (column < M) {
        threadParamPointer->Vecteur1[column] = matriceG[rowNumber][column];
        column++;
    }
}

void setVecteur2(double matriceH[M][P], ParametresThread* threadParamPointer, int columnNumber){
    int row = 0;
    while (row < M){
        threadParamPointer->Vecteur2[row] = matriceH[row][columnNumber];
        row++;
    }
}

int main(int argc, char *argv[]) {

    struct timespec end, start;
    clock_gettime(CLOCK_REALTIME, &start);

    srand(time(NULL)); //  allows rand() to generate a different int each time.

    // Creation des deux matrices
    printf("Nous allons creer les matrice de %dx%d et %dx%d\n",N,M,M,P);

    int i,j,index;
    double G[N][M];
    double H[M][P];
    double R[N][P];

    // Initialisation de la premiere matrice
    for (i=0;i<N;i++) {
        for (j=0; j<M; j++) {
            G[i][j] = (double) (rand() % 20) + (double)j*0.01; // *0.01; // vous devez remplacer i par un nombre généré aléatoire
        }
    }
    AfficherMatrice(&G[0][0],N,M);

    // Initialisation de la deuxieme matrice
    for (i=0;i<M;i++) {
        for (j=0; j<P; j++) {
            H[i][j] = (double) (rand() % 20) + (double)j*0.01; //*0.01;  // vous devez remplacer i par un nombre généré aléatoire
        }
    }
    AfficherMatrice(&H[0][0],M,P);


    // Votre code de creation de thread commence ici...
    ParametresThread threadsParams[N][P];

    for(int i = 0;i< N;i++){
        for(int j = 0; j< P; j++){
            //threadsParams[i][j].iThread = i+j;
            threadsParams[i][j].Longueur = M;
            setVecteur1(G, &threadsParams[i][j],i);
            setVecteur2(H, &threadsParams[i][j],j);
        }
    }

    pthread_t theThreads[N][P];
    for(int i = 0; i < N; i++){
        for(int j = 0;j< P; j++){

            int status = pthread_create((pthread_t *) &theThreads[i][j], NULL, ProduitScalaire, (void *) &threadsParams[i][j]);
            if(status != 0)printf("uh oh code d'erreur pthread: %d\n", status);
         }
    }

    for(int i = 0; i < N; ++i){
        for(int j = 0;j< P; ++j){
            pthread_join(theThreads[i][j],NULL);
        }
    }


    // Et se termine essentiellement ici...
    printf("Main(): Tous les threads ont termines! On affiche la reponse...\n");
    for (int row = 0;row < N; row++){
        for(int column = 0; column < P; column++){
            R[row][column] = *(threadsParams[row][column].pResultat);
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0;j< P; j++){
            free(threadsParams[i][j].pResultat);
        }
    }

    AfficherMatrice(&R[0][0],N,P);

    clock_gettime(CLOCK_REALTIME, &end);
    double benchmark = (double)((end.tv_sec + end.tv_nsec * 1e-9) - (double)(start.tv_sec + start.tv_nsec * 1e-9)); // tv_nsec sont des nano sec dont on ramene sa en secondes
    printf("real time taken by threads.c to finish %fs\n", benchmark);
    exit(0);
}


