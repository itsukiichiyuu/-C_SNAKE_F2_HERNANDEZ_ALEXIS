#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MUN -1
#define ZERO 0
#define UN 1
#define DEUX 2
#define TROIS 3
#define QUATRE 4
#define CINQ 5
#define SIX 6
#define SEPT 7
#define HUIT 8
#define NEUF 9
#define DIX 10
#define LONGUEUR 20
#define LARGEUR 30
#define VITESSETXT 50
#define VITESSE 100

typedef struct xy{
    int x,y;
}xy;

enum Bool {
    True,False,//true = 0 et false = 1 !!!
};

typedef struct snake{
    xy place;
    enum Bool isTete,isQueue,isSnake;
}snake;

typedef struct terrain{
    xy place;
    enum Bool isMiam,isRien,isBonus;
}terrain;

void GoToXY(int, int);
void initMap(int matrice[LARGEUR][LONGUEUR]);
void initSnake(int matrice[LARGEUR][LONGUEUR]);
void deplacement(int* ,int* ,int* ,int*);
void afficheMap(int*,int matrice[LARGEUR][LONGUEUR]);
void placementCurseur(int* , int* );
int testColision(int matrice[LARGEUR][LONGUEUR],int* ,int* ,int* );
void PlacementTete(int* ,int* ,int* ,int matrice[LARGEUR][LONGUEUR],int* ,int* );
void apparitionMiam(int matrice[LARGEUR][LONGUEUR],int* );
void affichageText(char phrase[VITESSETXT] );
void animationDepart();
void initVariables(int* ,int* ,int* , int* ,int* ,int* ,int* ,int* );
int playAgain();
void hidecursor();

int main()
{
    srand(time(NULL));
    hidecursor();

    int i,j,last,invert,perdu,tailleSnake,miam,score;
    int matrice[LARGEUR][LONGUEUR];

    animationDepart();

    //--------------------------------------DEBUT DE BOUCLE DE JEU------------------------------------
    do
    {
        system("cls");

        initVariables(&i,&j,&last,&invert,&perdu,&tailleSnake,&miam,&score);

        initMap(matrice);

        initSnake(matrice);

        placementCurseur(&i,&j);




        //-------------------------------------BOUCLE DE PARTIE----------------------------------------
        do
        {

            deplacement(&i,&j,&last,&invert);// gère l'incrémentation du i et du j

            perdu=testColision(matrice,&i,&j,&tailleSnake);

            PlacementTete(&i,&j,&tailleSnake,matrice,&miam,&score);

            apparitionMiam(matrice,&miam);

            afficheMap(&tailleSnake,matrice);

            Sleep(VITESSE);

        }
        while(perdu==ZERO);

        system("cls");
        GoToXY(DIX,HUIT);
        affichageText("GAME OVER");
        GoToXY(DIX,NEUF);
        affichageText("vous avez marque : ");
        printf("%i points !",score);

        //-----------------------------------FIN BOUCLE DE PARTIE--------------------------------------
    }
    while(playAgain());
    //---------------------------------------FIN DE BOUCLE DE JEU--------------------------------------
    return ZERO;
}

void GoToXY(int X, int Y)
{
    COORD coord;
    coord.X = X*DEUX;//déplacements homogènes
    coord.Y = Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void initMap(int matrice[LARGEUR][LONGUEUR]){
int i=ZERO;
int j=ZERO;

    GoToXY(UN,UN);
    puts("###########################################################");
    GoToXY(UN,LONGUEUR-UN);
    puts("###########################################################");
    for(i=UN;i<=LONGUEUR-UN;i++){
        GoToXY(UN,i);
        puts("#");
    }
    for(i=UN;i<LONGUEUR-UN;i++){
        GoToXY(LARGEUR,i);
        puts("#");
    }
//INITIALISER LE XY DU SNAKE
    for(i=ZERO;i<LARGEUR;i++){
        for(j=ZERO;j<LONGUEUR;j++){
            matrice[i][j]=ZERO;
        }
    }
    GoToXY(LARGEUR+CINQ,DEUX);
    printf("score: 0");
}

void initSnake(int matrice[LARGEUR][LONGUEUR]){

    matrice[HUIT][TROIS]=DEUX;
    matrice[SEPT][TROIS]=TROIS;
    matrice[SIX][TROIS]=QUATRE;
    matrice[CINQ][TROIS]=CINQ;
    matrice[QUATRE][TROIS]=SIX;
    matrice[TROIS][TROIS]=SEPT;
}

void deplacement(int* i,int* j,int* last,int* invert){
    if (GetAsyncKeyState(VK_RIGHT) && (*invert!=VK_RIGHT)){
        *i=*i+UN;
        *last=VK_RIGHT;
        *invert=VK_LEFT;
    }
    else if (GetAsyncKeyState(VK_LEFT)&& (*invert!=VK_LEFT)){
        *i=*i-UN;
        *last=VK_LEFT;
        *invert=VK_RIGHT;
    }
    else if (GetAsyncKeyState(VK_UP) && (*invert!=VK_UP)){
        *j=*j-UN;
        *last=VK_UP;
        *invert=VK_DOWN;
    }
    else if (GetAsyncKeyState(VK_DOWN)&& (*invert!=VK_DOWN)){
        *j=*j+UN;
        *last=VK_DOWN;
        *invert=VK_UP;
    }
    else{
        if (*last==VK_RIGHT){
            *i=*i+UN;
        }
        if (*last==VK_LEFT){
            *i=*i-UN;
        }
        if (*last==VK_UP){
            *j=*j-UN;
        }
        if (*last==VK_DOWN){
            *j=*j+UN;
        }
    }
}

void afficheMap(int* tailleSnake, int matrice[LARGEUR][LONGUEUR]){
    int i=ZERO;
    int j=ZERO;

    for(i=DEUX;i<LARGEUR;i++){
        for(j=DEUX;j<LONGUEUR-UN;j++){
            if ((matrice[i][j]<=*tailleSnake)&&(matrice[i][j]!=ZERO)&&(matrice[i][j]!=MUN)){
                    if(matrice[i][j]!=UN){
                        GoToXY(i,j);
                        printf("O");
                        matrice[i][j]=matrice[i][j]+UN;
                    }
                    else{
                        GoToXY(i,j);
                        printf("S");
                        matrice[i][j]=matrice[i][j]+UN;
                    }

                //printf("%d",matrice[i][j]);//test

            }
            else if (matrice[i][j]==MUN){
                GoToXY(i,j);
                printf("X");
            }
            else {
                matrice[i][j]=ZERO;
                GoToXY(i,j);
                printf(" ");
            }
        }
    }
}

void placementCurseur(int* i, int* j){
        *i=HUIT;
        *j=TROIS;
    }

int testColision(int matrice[LARGEUR][LONGUEUR],int* i,int* j,int* tailleSnake){
    if ((matrice[*i][*j]>ZERO)&&(matrice[*i][*j]!=*tailleSnake+UN)&&(matrice[*i][*j]!=-UN)){
        return UN;
    }
    if ((*i>LARGEUR-UN)||(*i<DEUX)||(*j<DEUX)||(*j>LONGUEUR-DEUX)){
        return UN;
    }
    else{
        return ZERO;
    }
}

void PlacementTete(int* i,int* j,int* tailleSnake,int matrice[LARGEUR][LONGUEUR],int* miam,int* score){
    GoToXY(*i*DEUX,*j);

    if (matrice[*i][*j]==MUN){
        *miam=ZERO;
        *score=*score+UN;
        *tailleSnake=*tailleSnake+UN;
        GoToXY(LARGEUR+CINQ,DEUX);
        printf("score: %i",*score);
    }
    matrice[*i][*j]=UN;
}

void apparitionMiam(int matrice[LARGEUR][LONGUEUR],int* miam){
    int nbX=ZERO;
    int nbY=ZERO;

    if (*miam!=UN){
        do{
            nbX=rand()%(LARGEUR-QUATRE)+DEUX;
            nbY=rand()%(LONGUEUR-QUATRE)+DEUX;
        }
        while(matrice[nbX][nbY]>ZERO);

        matrice[nbX][nbY]=MUN;
        *miam=UN;
    }
}

void affichageText(char phrase[VITESSETXT]){
int i=ZERO;
int sortie=ZERO;
char test[UN]="\0";

    do{
        Sleep(VITESSETXT+VITESSETXT);
        printf("%c",phrase[i]);
        i++;
        if(phrase[i]==test[UN]){
            sortie=UN;
        }
    }
    while (sortie==ZERO);
}

void animationDepart(){
    GoToXY(DIX,DIX);
    affichageText("SNAKE");
    GoToXY(DIX,DIX+UN);
    affichageText("by Alexis Hernandez");
    GoToXY(DIX,DIX+TROIS);
    affichageText("appuyez sur entree pour commencer");
    getchar();
}

void initVariables(int* i,int* j,int* last, int* invert,int* perdu,int* tailleSnake,
                   int* miam,int* score){
    *i=UN;
    *j=UN;
    *last=VK_RIGHT;
    *invert=VK_LEFT;
    *perdu=ZERO;
    *tailleSnake=SIX;
    *miam=ZERO;
    *score=ZERO;
}

int playAgain(){
int restart=ZERO;
char reponse='n';

    GoToXY(DIX,DIX);
    affichageText("voulez-vous rejouer ? O/N");
    GoToXY(DIX,DIX+UN);
    fflush(stdin);
    scanf("%c",&reponse);
    if((reponse=='O')||(reponse=='o')){
        restart=UN;
    }
    else{
        restart=ZERO;
    }

    return restart;
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
