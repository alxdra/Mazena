///Using SDL and standard IO
#include <SDL.h>
#include <string>
#include <stdio.h>
#include <stdlib.h> //enable rand for random numbers
#include <SDL_image.h> //enable different image type to be loaded
#include <time.h>

// Définition des variables
#define SCREEN_WIDTH  675
#define SCREEN_HEIGHT  675

#define IA_WIDTH 10
#define IA_HEIGHT 5

#define SPEED 3 // vitesse de déplacement de l'IA

int r[2]={3,SCREEN_HEIGHT/2}; //tableau r direction : r0 = x et r1 = y
int dir; //haut, droite, bas, gauche



//Crée la fenetre SDL
bool init();

//Charge les différentes images
bool loadImage();

//affichage des images
void affichage(int x, int y);

//déplace le joueur en fonction de la direction
void deplacePerso(int pas);

//définie la couleur que le joeur doit suivre
int bColor (int x, int y);
//condition de déplacement du joueur, reste dans la fenetre, ne rentre pas dans les murs
int condition(int x, int y);

//lance les fenetres
void Initarena();//
void Initgame();
void menu();

//jeu
int game();
//si le joueur est au centre de la map
bool center(int x, int y);


//Frees media and shuts down SDL
void close();




//The window we'll be rendering to
SDL_Window* Window = NULL;  //declaration d'un pointeur

//The surface contained by the window
SDL_Surface* ScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* Perso2 = NULL;
SDL_Surface* Fond = NULL;

//Image position
SDL_Rect position;

bool init()
{
    SDL_Init(SDL_INIT_VIDEO);
    //Create window
    Window = SDL_CreateWindow( "SDL MAZENA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    //Get window surface
    ScreenSurface = SDL_GetWindowSurface( Window );

    if (Window == 0)
    {
        printf("Window Problem: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

bool loadImage(SDL_Surface** pt, char* name)// charge les différentes images selon le pointeur SDL_Surface donné
{
    //Load splash image
    *pt = IMG_Load(name);

    if (*pt == 0)
    {
        printf( "Unable to load image %s! SDL Error: %s\n", name, SDL_GetError() );
        return true;
    }
    return false;
}


void affichage(int x, int y)
{
    //clean screen

    SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0, 0, 0));

    //Affiche fond
    SDL_BlitSurface(Fond, NULL, ScreenSurface, NULL );

    //affiche Perso
    position.x = x;
    position.y = y;
    SDL_BlitSurface(Perso2, NULL, ScreenSurface, &position);
   // printf("c'est affiche\n");
    //Update the surface
    SDL_UpdateWindowSurface( Window );
}

void deplacePerso(int pas)
{
      int s;
      switch (dir)
        {
        case 0 :
            r[0]+=pas;
            s = 0;
         //   printf("droite\n");
            break;
        case 1 :
            r[0]-=pas;
            s = 1;
         //   printf("gauche\n");
             break;
         case 2 :
            r[1]+=pas;
            s = 2;
          //  printf("bas\n");
             break;
         case 3 :
            r[1]-=pas;
            s = 3;
          //  printf("haut\n");
             break;
        }
        int c = condition(r[0],r[1]);
           if (c == 1)
                affichage(r[0],r[1]);
            else if (s == 0)
                r[0]-=pas;
            else if (s == 1)
                r[0]+=pas;
            else if (s == 2)
                r[1]-=pas;
            else if (s == 3)
                r[1]+=pas;
}
void close()
{
     //Deallocate surface
    SDL_FreeSurface(Perso2);
    SDL_FreeSurface(Fond);
    Perso2 = NULL;

    // Close and destroy the window
    SDL_DestroyWindow(Window);

    // Clean up
    SDL_Quit();
}

int bColor (int x, int y) //Bit shifted Color
{
    unsigned int *ptr = (unsigned int*)ScreenSurface->pixels;
    int lineoffset = y * (ScreenSurface->pitch / 4);
    return ptr[lineoffset + x]; //return Bit shifted color code
}

int condition(int x, int y)
{
    //Pour ne pas sortir de la fenetre
    if (x>SCREEN_WIDTH-IA_WIDTH || x<0 || y<0 || y>SCREEN_HEIGHT-IA_HEIGHT )
    {
        deplacePerso(-SPEED);
        printf("Ce n'est pas la sortie!\n\n");
        return 1;
    }
    int fColor = bColor(5,5); //floor color, référence = position fixe
    printf("floor color is = %d \n",fColor);
    int ColorLeft = bColor(x,y); //couleur à gauche de IA
    printf("colorLeft is = %d \n",ColorLeft);
    int ColorRight = bColor(x+IA_WIDTH-3,y+IA_HEIGHT-3);
    printf("colorRight is = %d \n",ColorRight);
    if (ColorLeft == fColor || ColorRight==fColor)
    {
        printf("J'avance\n");
        return 1;
    }
    else
    {
        printf("On ne traverse pas un mur ! \n",dir);
        return 0;
    }
}

//si le personnage atteint cet endroit, on renvoie vrai
bool center(int x, int y)
{
    if((x<SCREEN_WIDTH/2+20&&x>SCREEN_WIDTH/2-20)&&(y<SCREEN_HEIGHT/2+36&&y>SCREEN_HEIGHT/2-40))
    {
        printf("Passe 2e niveau \n");
        return true;
    }
    else return false;
}


int arena()
{

}

void Initarena() //arene final
{
    if (init())
    {
        printf("failed to initialize! \n");
    }
    else
    {
        //load media
        if (loadImage(&Fond,"salle.png"))
        {
            printf("failed to load arena\n");
        }
        else
        {
            affichage(r[0],r[1]);

             int keep = 1;
           // int* r;
            while (keep) // tant qu'on ne ferme pas la fenetre, ou arrive au centre, elle reste ouverte
            {
                SDL_Event go;
                SDL_WaitEvent(&go);
                switch (go.type)
                {
                case SDL_QUIT :
                    close();
                    break;
                }

            }
        }
    }
}


int game()
{
    SDL_Event direction;
                SDL_WaitEvent(&direction);
                switch (direction.type)
                {
                case SDL_QUIT :
                    close();
                    break;
                case SDL_KEYDOWN :
                    switch(direction.key.keysym.sym) // change la direction du joueur selon la fleche
                        {
                        case SDLK_UP :
                            dir = 3;
                            deplacePerso(SPEED);
                            break;
                        case SDLK_DOWN :
                            dir = 2;
                            deplacePerso(SPEED);
                            break;
                        case SDLK_RIGHT :
                            dir = 0;
                            deplacePerso(SPEED);
                            break;
                        case SDLK_LEFT :
                            dir = 1;
                            deplacePerso(SPEED);
                            break;
                        }
                        if (center(r[0], r[1])) // si le perso arrive au centre, on ouvre la 2e arène
                        {
                            close();
                            Initarena();
                            return 0;
                        }
                        else   return 1;
                }
}


void Initgame()
{
     //Start up SDL and create window
    if (init())
    {
        printf("failed to initialize! \n");
    }
    else
    {
        //load media
        if (loadImage(&Fond,"laby.png"))
        {
            printf("failed to load maze\n");
        }
        else
        {
            affichage(r[0],r[1]); // affiche le fond et l'image


            int keep = 1;
           // int* r;
            while (keep) // tant qu'on ne ferme pas la fenetre, ou arrive au centre, elle reste ouverte
            {
                keep=game();
            }
        }
    }
}



void menu()
{
     if (init())
    {
        printf("failed to initialize! \n");
    }
    else
    {
        SDL_Surface* Menu = NULL;

        if(loadImage(&Menu,"Menu.png")) printf("failed to load menu\n");
        else
            {
                 if (loadImage(&Perso2,"ttest.png"))
                {
                    printf("failed to load media! \n");
                }
                else {
             //apply the image
            SDL_BlitSurface(Menu, NULL, ScreenSurface, NULL );
            //Update the surface
            SDL_UpdateWindowSurface( Window );

            int continuer = 1; // garde la fenêtre ouverte tant que rien ne se passe
            SDL_Event choix;
            while (continuer)
            {
                SDL_WaitEvent(&choix);
                switch (choix.type)
                {

                    case SDL_QUIT: // ferme si clique sur la croix
                        close();
                        break;
                    case SDL_KEYDOWN:
                    switch (choix.key.keysym.sym)
                    {
                        case SDLK_1 : // si appuie sur 1 jeu commence
                            SDL_DestroyWindow(Window);// ferme la fenetre du menu
                            Initgame();
                            break;
                        case SDLK_2: // si appuie sur 2 jeu se ferme
                            continuer = 0;
                            break;
                        case SDLK_3:
                            arena();
                            break;
                    }
                }
            }
                }
            }
        }
    }

int main( int argc, char* args[] )
{
    srand(time(0));
    menu();
    //Free resources and close SDL
    close();
    return 0;
}


//test IA
/*
int *my_ai_play(int **map, int height, int width)
{
        switch (dir) // direction en IA en fonction de la direction choisie
        {
        case 0 :
            r[0]+=SPEED;
            printf("droite\n");
            break;
        case 1 :
            r[0]-=SPEED;
            printf("gauche\n");
             break;
         case 2 :
            r[1]+=SPEED;
            printf("bas\n");
             break;
         case 3 :
            r[1]-=SPEED;
            printf("haut\n");
             break;
        }
            //affichage(r[0],r[1]);
            return r;
    }

   game ()
              int i;
            int c;
            int *r;
            r = my_ai_play(0,SCREEN_WIDTH,SCREEN_HEIGHT);//r adresse du tableau des positions
            affichage(r[0],r[1]);
            for (i=0;i<217;i++)
            {

               /* if (loadImage(&Perso1,"Pixel1.png"))
                {
                    printf("failed to load media! \n");
                }
                else
                {

                    position1.x =300 + i ;
                    position1.y = 300 + i ;
                    //show new position
                    SDL_UpdateWindowSurface( Window );
                }
                if (loadImage(&Perso2,"ttest.png"))
                {
                    printf("failed to load media! \n");
                }
                else
                {

                    c = condition(r[0],r[1]); // condition de non collision entre le mur et l IA
                    r = my_ai_play(0,SCREEN_WIDTH,SCREEN_HEIGHT);//si collision, change direction et avance avec la nouvelle dir

                    if (c == 1)
                    {
                        affichage(r[0],r[1]);
                        printf("i moved \n\n");
                    }
                    else
                    {
                        printf("cant move : wall\n\n");
                        //affichage(r[0],r[1]);;
                    }
                    printf(" %d \n\n",i);
                }
            }
            */
