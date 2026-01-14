/*
Space Invaders by leandrezpau. A recreation with a special interface of the original Arcade Space Invaders
My First graphic proyect, I hope the code is legible. 20/12/2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "esat/window.h"
#include "esat/draw.h"
#include "esat/input.h"
#include "esat/sprite.h"
#include "esat/time.h"

#ifdef _WIN32
   #include "esat_extra/soloud/soloud.h"
   #include "esat_extra/soloud/soloud_wav.h"
   #undef DrawText
#endif


struct posis{
   double x,y,w,h;
   int animation; //Number of the sprite animation It is playing
   int alive; //Lives It has
   esat::SpriteHandle s; //Sprite from enemy
};


//Targets positions
posis player = {100,650,64,64};
posis enemy[11][5];
posis barrier[4][4];
posis bullet = {-50,-50};
posis enemy_bullet = {-50,-50};
posis spaceship = {-50,60,48,54};
posis coin;
posis mouse;


//Credit insertion screen controll
int creditinsert=0;

//Number of line and column in the movement function
char score[5], level[4], credits[4], hiscore[5];
int numscore = 0, numlevel=1, numcredits=0, numhiscore = 0;


int line=0,column=0;
int line_down=0,column_down = 0;


int max_num_enemy = 55;
int num_enemy = 55;


int A = 1 , direccion = 1;
int attack,attack_column;
int bullet_speed=5;


int enemy_down = 0; //If enemey is ready to move down
int down_movement=0; //Number of down movements It has made
int horizontal=0;
int izq=0;


//0--> bullet is moving 1-->Bullet is ready to be shot
int bullet_init = 1;
int bulletspeed = 5;
int enemy_bullet_init = 1;
int spaceship_init = 1;
int ufoscore;


//Timers
double current_time,last_time,last_movement;
double enemy_movement_time = 20;
double enemy_shooting_time = 1200;
double current_bullet_time,last_bullet_time;
double last_down_movement;


//Sprites
esat::SpriteHandle player1,player2,life,bullet_sprite,enemy_bullet_sprite,background;
esat::SpriteHandle enemy1_1,enemy1_2,enemy2_1,enemy2_2,enemy3_1,enemy3_2,spaceship1,spaceship2;
esat::SpriteHandle barrier1_1,barrier1_2,barrier1_3,barrier1_4,barrier2_1,barrier2_2,barrier2_3,barrier2_4,barrier3_1,barrier3_2,barrier3_3,barrier3_4;
esat::SpriteHandle coin1,coin2,coin3,coin4,coin5,coin6,boton1,boton2,continue1,continue2;
//To controll Fps
#ifdef _WIN32
static bool backgroundsound = true;
SoLoud::Soloud soloud;
SoLoud::Wav audio[14];
SoLoud::handle background_soundHandle = 0;
#endif


unsigned char fps=60;
void Init();

void PlayerInput();

void CheckColsAndRows();
void ContinueGameplay();
void RestartGameplay();
void ScreenTexts();
void GameWinned();
void GameLost();
void GetCharScore();
void CreditGameplayHUD();
void CreditStartHUD();
void FullGameplay();
void WinOrDieScreen();

void EnemyMovement();
void EnemyDownMovement();
void EnemyAttacks();
void UfoMechanics();

void DrawSprites();
void Colides();

void InitSound();
void FreeSound();


/********************************************************/
/*          .___  ___.      ___       __  .__   __. 
            |   \/   |     /   \     |  | |  \ |  | 
            |  \  /  |    /  ^  \    |  | |   \|  | 
            |  |\/|  |   /  /_\  \   |  | |  . `  | 
            |  |  |  |  /  _____  \  |  | |  |\   | 
            |__|  |__| /__/     \__\ |__| |__| \__| 
                                                        */
/********************************************************/
int esat::main(int argc, char **argv) {
   //TO INIT EVERYTHING
   Init();
   InitSound();
   while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)){
        do{
            current_time = esat::Time();
        }while((current_time-last_time)<=1000.0/fps);
        last_time = esat::Time();
        //CREDIT ANIMATION TO INSERT COIN
        CreditStartHUD();
        //FULL GAMEPLAY WITH MECHANICS
        FullGameplay();
        //WHEN PLAYER WINS OR DIES
        WinOrDieScreen();
        //Control fps
        
        esat::WindowFrame();
   }
   FreeSound();
 esat::WindowDestroy();
 return 0;
}

//TO INIT EVERYTHING IN THE PROGRAM
void Init(){
   srand(time(NULL));
   esat::WindowInit(800,750);
   esat::WindowSetMouseVisibility(true);
   esat::DrawSetStrokeColor(255,255,0);
   esat::DrawSetTextFont("../assets/fonts/Korteque.ttf");
   esat::DrawSetTextSize(50);
   //INIT SPRITES<-
   player1 = esat::SpriteFromFile("../assets/sprites/arriba1.png");
   player2 = esat::SpriteFromFile("../assets/sprites/arriba2.png");
   life = esat::SpriteFromFile("../assets/sprites/life.png");
   bullet_sprite = esat::SpriteFromFile("../assets/sprites/fireball.png");
   enemy_bullet_sprite = esat::SpriteFromFile("../assets/sprites/blueball.png");
   background = esat::SpriteFromFile("../assets/sprites/background.png");
   enemy1_1  = esat::SpriteFromFile("../assets/sprites/enemy1_1.png");
   enemy1_2  = esat::SpriteFromFile("../assets/sprites/enemy1_2.png");
   enemy2_1  = esat::SpriteFromFile("../assets/sprites/enemy2_1.png");
   enemy2_2  = esat::SpriteFromFile("../assets/sprites/enemy2_2.png");
   enemy3_1  = esat::SpriteFromFile("../assets/sprites/enemy3_1.png");
   enemy3_2  = esat::SpriteFromFile("../assets/sprites/enemy3_2.png");
   spaceship1  = esat::SpriteFromFile("../assets/sprites/spaceship.png");
   spaceship2  = esat::SpriteFromFile("../assets/sprites/spaceship2.png");


   //AL DIFRENT BARRIERS2   
   barrier1_1 = esat::SpriteFromFile("../assets/sprites/barrier1_1.png");
   barrier1_2 = esat::SpriteFromFile("../assets/sprites/barrier1_2.png");
   barrier1_3 = esat::SpriteFromFile("../assets/sprites/barrier1_3.png");
   barrier1_4 = esat::SpriteFromFile("../assets/sprites/barrier1_4.png");


   barrier2_1 = esat::SpriteFromFile("../assets/sprites/barrier2_1.png");
   barrier2_2 = esat::SpriteFromFile("../assets/sprites/barrier2_2.png");
   barrier2_3 = esat::SpriteFromFile("../assets/sprites/barrier2_3.png");
   barrier2_4 = esat::SpriteFromFile("../assets/sprites/barrier2_4.png");


   barrier3_1 = esat::SpriteFromFile("../assets/sprites/barrier3_1.png");
   barrier3_2 = esat::SpriteFromFile("../assets/sprites/barrier3_2.png");
   barrier3_3 = esat::SpriteFromFile("../assets/sprites/barrier3_3.png");
   barrier3_4 = esat::SpriteFromFile("../assets/sprites/barrier3_4.png");


   coin1 = esat::SpriteFromFile("../assets/sprites/coin1.png");
   coin2 = esat::SpriteFromFile("../assets/sprites/coin2.png");
   coin3 = esat::SpriteFromFile("../assets/sprites/coin3.png");
   coin4 = esat::SpriteFromFile("../assets/sprites/coin4.png");
   coin5 = esat::SpriteFromFile("../assets/sprites/coin5.png");
   coin6 = esat::SpriteFromFile("../assets/sprites/coin6.png");


   boton1 = esat::SpriteFromFile("../assets/sprites/boton1.png");
   boton2 = esat::SpriteFromFile("../assets/sprites/boton2.png");
   continue1 = esat::SpriteFromFile("../assets/sprites/continue1.png");
   continue2 = esat::SpriteFromFile("../assets/sprites/continue2.png");




   //ENEMY SPRITE DECLARATIONS
   for(int i=0;i<11;i++){
       for(int e=0;e<5;e++){
           enemy[i][e].alive=1;
           //ENEMY COORDS
           enemy[i][e].x=90+i*43;
           enemy[i][e].y=300-e*40;
       }
   }
   //Barrier coords, stages and measures
   for(int i=0;i<4;i++){
       for(int e=0;e<4;e++){
           barrier[i][e].w=42;
           barrier[i][e].h=32;
           barrier[i][e].alive=3;
           switch(e){
               case 0:{
                   barrier[i][e].y= 550;
                   barrier[i][e].x= 100 + i * 175;
                   break;
                }
               case 1:{
                   barrier[i][e].y= 550;
                   barrier[i][e].x= 100 + 42 + i * 175;
                   break;
                }
               case 2:{
                   barrier[i][e].y= 550 + 32;
                   barrier[i][e].x= 100 + i * 175;
                   break;
                }
               case 3:{
                   barrier[i][e].y= 550 + 32;
                   barrier[i][e].x= 100 + 42 + i * 175;
                   break;
                }
           }
          
       }
   }
   player.alive = 3;
}
//TO CHECK LINES AND ROWS
void CheckColsAndRows(){
   if(line>=11){
       line=0;
       column++;
   }
   if(column>=5){
       column=0;
       line=0;
   }
   if(line_down>=11){
       line_down=0;
       column_down++;
   }
   if(column_down>=5){
       column_down=0;
       line_down=0;
   }
}


void ContinueGameplay(){
   //ENEMY SPRITE DECLARATIONS
   for(int i=0;i<11;i++){
       for(int e=0;e<5;e++){
           enemy[i][e].alive=1;
           enemy[i][e].animation=0;
           //ENEMY COORDS
           enemy[i][e].x=90+i*43;
           enemy[i][e].y=300-e*40;
       }
   }
   for(int i=0;i<4;i++){
       for(int e=0;e<4;e++){
           barrier[i][e].w=42;
           barrier[i][e].h=32;
           barrier[i][e].alive=3;
           switch(e){
               case 0:{
                   barrier[i][e].y=550;
                   barrier[i][e].x=100+i*175;
                   break;
               }
               case 1:{
                   barrier[i][e].y=550;
                   barrier[i][e].x=100+42+i*175;
                   break;
                }
               case 2:{
                   barrier[i][e].y=550+32;
                   barrier[i][e].x=100+i*175;
                   break;
                }
               case 3:{
                   barrier[i][e].y=550+32;
                   barrier[i][e].x=100+42+i*175;
                   break;
                }
           }
       }
   }
   num_enemy=55;
   horizontal=0;
   line=0;
   column=0;
   line_down=0;
   column_down=0;
   direccion=1;
   enemy_down=0;
   down_movement=0;
   enemy_bullet_init=1;
   bullet_init=1;
   izq=0;


   //More dificulty on the next level
   if(enemy_movement_time>5){
       enemy_movement_time-=5;
   }
   if(enemy_shooting_time>100){
       enemy_shooting_time-=100;
   }
   bulletspeed+=2;
  
}


void RestartGameplay(){
   //ENEMY SPRITE DECLARATIONS
   for(int i=0;i<11;i++){
       for(int e=0;e<5;e++){
           enemy[i][e].alive=1;
           enemy[i][e].animation=0;
           //ENEMY COORDS
           enemy[i][e].x=90+i*43;
           enemy[i][e].y=300-e*40;
       }
   }
   for(int i=0;i<4;i++){
       for(int e=0;e<4;e++){
           barrier[i][e].w=42;
           barrier[i][e].h=32;
           barrier[i][e].alive=3;
           switch(e){
               case 0:{
                   barrier[i][e].y=550;
                   barrier[i][e].x=100+i*175;
                   break;
                }
               case 1:{
                   barrier[i][e].y=550;
                   barrier[i][e].x=100+42+i*175;
                   break;
                }
               case 2:{
                   barrier[i][e].y=550+32;
                   barrier[i][e].x=100+i*175;
                   break;
                }
               case 3:{
                   barrier[i][e].y=550+32;
                   barrier[i][e].x=100+42+i*175;
                   break;
                }
           }
       }
   }
   num_enemy=55;
   player.alive=3;
   horizontal=0;
   line=0;
   column=0;
   line_down=0;
   column_down=0;
   direccion=1;
   enemy_down=0;
   down_movement=0;
   numscore=0;
   numlevel=1;
   izq=0;


   //Adjust normal dificulty in case It has been increased before
   enemy_movement_time=20;
   enemy_shooting_time=1200;
   bulletspeed=5;
}
//TO CALCULATE ENMY MOVEMENT SLOWLY AND SEPARATELY
void EnemyMovement(){
   current_time=esat::Time();//To calc current time
   //TIME CONTROLL
   //Substracting current time minus 'last current time' and if it is higer than time we want
   if(current_time-last_movement>enemy_movement_time){
       //to SKIP DEAD ENEMIES
       while(enemy[line][column].alive == 0) {
           line++;
           if(line>=11 && column>=4 && enemy_down==1){
               horizontal=1;
               direccion= -direccion;
           }
           CheckColsAndRows();
       }
      
       //TO SWAP ANIMATIONS
       if(enemy[line][column].animation==1){
           enemy[line][column].animation=0;
       }else{
           enemy[line][column].animation=1;
       }
       last_movement = current_time;//To save that current time for later use
       if(horizontal==0){
           //To MOVE ENEMIES 10px left or right
           enemy[line][column].x += 10* direccion;


           line++;
           if(line>=11 && column>=4 && enemy_down==1){
               horizontal=1;
               direccion=-direccion;
           }
           CheckColsAndRows();
       }
   }
}
//TO MOVE ENEMIES DOWN SLOWLY
void EnemyDownMovement(){
   current_time=esat::Time();//To calc current time
   if(current_time-last_down_movement>1){
       //To move back and foward 16 times per each enemy and then 1 down
       if(down_movement<=max_num_enemy){
           //to skip dead enemys
           while(enemy[line_down][column_down].alive==0){
               line_down++;
               CheckColsAndRows();
               down_movement++;
               if(down_movement>=max_num_enemy){
                   printf("\nSwap to Horizontal\n");
                   down_movement=0;
                   enemy_down=0;
                   horizontal=0;
               }
           }
           if(enemy_down!=0){
               enemy[line_down][column_down].y += 38;
               last_down_movement = current_time;//To save that current time for later use
               line_down++;
               CheckColsAndRows();
               down_movement++; 
           }        
       }
       if(down_movement>=max_num_enemy){
           printf("\nSwap to Horizontal\n");
           down_movement=0;
           enemy_down=0;
           horizontal=0;
       }
   }
}
void PlayerInput(){
   //KEYS TO MOVE PLAYER POSITION
   if(esat::IsKeyPressed('D')  || esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
       if(player.x<750-player.w){
           player.x+=3;
       }
       player.animation++;
   }
   if(esat::IsKeyPressed('A') || esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
       if(player.x>50){
           player.x-=3;
       }
       player.animation++;
   }
   //KEY TO INIT BULLET
   if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
       if(bullet_init==1){
           bullet.x=player.x+26;
           bullet.y=player.y+15;
           bullet_init=0; //TO INIT BULLET MOVEMENT
           #ifdef _WIN32
            soloud.play(audio[1], 0.5f /* volumen */, 0.0f /* pan */);
           #endif
       }
   }
   //WHEN THE BULLET IS MOVING
   if(bullet_init==0){
       bullet.y-=15;
       if(bullet.y<-20){
           bullet_init=1; //TO STOP BULLET MOVEMENT
       }
   }
}
//ENEMY ATTACKS
void EnemyAttacks(){
   current_bullet_time=esat::Time();
   if(current_bullet_time-last_bullet_time>enemy_shooting_time){
       if(enemy_bullet_init==1){
           attack_column=0;
           int attack = rand() % 11;
           //To select a alien alive on the lowest line
           while(enemy[attack][attack_column].alive==0){
               attack_column++;
               if(attack_column>=5){
                   attack = rand() % 11;
                   attack_column=0;
               }
           }


           //To locate the bullet were the enemy is
           enemy_bullet.x=enemy[attack][attack_column].x+15;
           enemy_bullet.y=enemy[attack][attack_column].y+15;
           //To init the bullet
           enemy_bullet_init=0;


           if(enemy[attack][attack_column].alive==0){
               enemy_bullet_init=1; //TO STOP BULLET MOVEMENT
           }
           last_bullet_time = current_bullet_time;
       }
      
   }


   //WHEN THE BULLET IS MOVING
   if(enemy_bullet_init==0){
       enemy_bullet.y+=bullet_speed;
       if(enemy_bullet.y>750){
           enemy_bullet_init=1; //TO STOP BULLET MOVEMENT
       }
   }
}


//FUNCTION TO DRAW EVERY SPRITE
void DrawSprites(){
   esat::DrawSprite(background,0,0);
   switch(player.alive){
       case 3:{
           esat::DrawSprite(life,5,720);
           esat::DrawSprite(life,41,720);
           esat::DrawSprite(life,77,720);
           break;
        }
       case 2:{
           esat::DrawSprite(life,5,720);
           esat::DrawSprite(life,41,720);
           break;
        }
       case 1:{
           esat::DrawSprite(life,5,720);
           break;
        }
   }


   if(bullet_init==0){
       esat::DrawSprite(bullet_sprite,bullet.x,bullet.y);
   }
   if(enemy_bullet_init==0){
       esat::DrawSprite(enemy_bullet_sprite,enemy_bullet.x,enemy_bullet.y);
   }
   for(int i=0;i<4;i++){
       switch (barrier[i][0].alive){
           case 3:{
               esat::DrawSprite(barrier1_1, barrier[i][0].x,barrier[i][0].y);
               break;
            }
           case 2:{
               esat::DrawSprite(barrier2_1, barrier[i][0].x,barrier[i][0].y);
               break;
            }
           case 1:{
               esat::DrawSprite(barrier3_1, barrier[i][0].x,barrier[i][0].y);
               break;
            }
           case 0:{
               break;
            }
       }
       switch (barrier[i][1].alive){
           case 3:{
               esat::DrawSprite(barrier1_2, barrier[i][1].x,barrier[i][1].y);
               break;
            }
           case 2:{
               esat::DrawSprite(barrier2_2, barrier[i][1].x,barrier[i][1].y);
               break;
            }
           case 1:{
               esat::DrawSprite(barrier3_2, barrier[i][1].x,barrier[i][1].y);
               break;
            }
           case 0:{
               break;
            }
       }
       switch (barrier[i][2].alive){
           case 3:{
               esat::DrawSprite(barrier1_3, barrier[i][2].x,barrier[i][2].y);
               break;
            }
           case 2:{
               esat::DrawSprite(barrier2_3, barrier[i][2].x,barrier[i][2].y);
               break;
            }
           case 1:{
               esat::DrawSprite(barrier3_3, barrier[i][2].x,barrier[i][2].y);
               break;
            }
           case 0:{
               break;
            }
       }
       switch (barrier[i][3].alive){
           case 3:{
               esat::DrawSprite(barrier1_4, barrier[i][3].x,barrier[i][3].y);
               break;
            }
           case 2:{
               esat::DrawSprite(barrier2_4, barrier[i][3].x,barrier[i][3].y);
               break;
            }
           case 1:{
               esat::DrawSprite(barrier3_4, barrier[i][3].x,barrier[i][3].y);
               break;
            }
           case 0:{
               break;
            }
       }
   }
  
   //Spaceship sprites
   if(spaceship_init==0){
       if(spaceship.animation<25){
       esat::DrawSprite(spaceship1,spaceship.x,spaceship.y);
       }else{
       esat::DrawSprite(spaceship2,spaceship.x,spaceship.y);
       }
       if(spaceship.animation==50) { spaceship.animation=0; }
   }
  


   //Player sprites
   if(player.animation<25){
       esat::DrawSprite(player1,player.x,player.y);
   }else{
       esat::DrawSprite(player2,player.x,player.y);
   }
   if(player.animation==50) { player.animation=0; }


   //Enemy sprites
   for(int i=0;i<11;i++){
       for(int e=0;e<5;e++){
           //To check if its dead
           if(enemy[i][e].alive==1){
               //To check which sprite to use
               if(enemy[i][e].animation==1){
                   //To animate each type of alien
                   switch(e){
                       case 0:
                       case 1:{
                           enemy[i][e].s = enemy1_1;
                           break;
                       }
                       case 2:
                       case 3:{
                           enemy[i][e].s = enemy2_1;
                           break;
                       }
                       case 4:{
                           enemy[i][e].s = enemy3_1;
                           break;
                       }
                   }                   
               }else{
                   //To animate each type of alien
                   switch(e){
                       case 0:
                       case 1:{
                           enemy[i][e].s = enemy1_2;
                           break;
                       }
                       case 2:
                       case 3:{
                           enemy[i][e].s = enemy2_2;
                           break;
                       }
                       case 4:{
                           enemy[i][e].s = enemy3_2;
                           break;
                       }
                   } 
               }
               esat::DrawSprite(enemy[i][e].s,enemy[i][e].x,enemy[i][e].y);
           }
       }
   }
}
//TO CHECK THE COLIDES OF BULLETS
void Colides(){
   //TO CHECK ENEMY COLIDES
   for (int i=0; i<11; i++){
       for (int e=0; e<5; e++){
           //If it is alive it check
           if(enemy[i][e].alive==1){
               if(bullet.x>enemy[i][e].x && bullet.y>enemy[i][e].y && bullet.x+9<enemy[i][e].x+34
                   && bullet.y+20<enemy[i][e].y+38){
                   //Kill the enemy
                   enemy[i][e].alive=0;
                   num_enemy--;
                   #ifdef _WIN32
                    soloud.play(audio[2], 0.3f /* volumen */, 0.0f /* pan */);
                   #endif
                   //To log when enemy is dead
                   printf("\nEnemy Killed Line-> %d Row-> %d",e+1,i+1);
                   //Kill the bullet
                   bullet_init=1;
                   //Moves the bullet away
                   bullet.x=-50;
                   bullet.y=-50;
                   switch(e){
                       case 0:
                       case 1:{
                           numscore+=10;
                           break;
                       }
                       case 2:
                       case 3:{
                           numscore+=20;
                           break;
                       }
                       case 4:{
                           numscore+=30;
                           break; 
                       }                          
                   }
               }
           }
       }
   }


   //TO SWAP ENEMY DIRECTIONS WHEN THEY REACH A BORDER
   if(enemy_down!=1){
     for (int i=0; i<11; i++){
           for (int e=0; e<5; e++){
               //If it is alive it check
               if(enemy[i][e].alive==1){
                   if(izq==1){
                       if(enemy[i][e].x<=85){
                           enemy_down=1;
                           izq=0;
                       }
                   }else{
                       if(enemy[i][e].x>=715-34/*Enemy Witdh*/){
                           enemy_down=1;
                           izq=1;
                       }
                   }
               }
               //COLIDES BETWEEN ENEMY AND PLAYER
               if ((enemy[i][e].x >= player.x && enemy[i][e].x <= player.x + player.w && enemy[i][e].y >= player.y && enemy[i][e].y <= player.y + player.h) ||
               (enemy[i][e].x + enemy[i][e].w >= player.x && enemy[i][e].x + enemy[i][e].w <= player.x + player.w && enemy[i][e].y >= player.y && enemy[i][e].y <= player.y + player.h) ||
               (enemy[i][e].x >= player.x && enemy[i][e].x <= player.x + player.w && enemy[i][e].y + enemy[i][e].h >= player.y && enemy[i][e].y + enemy[i][e].h <= player.y + player.h) ||
               (enemy[i][e].x + enemy[i][e].w >= player.x && enemy[i][e].x + enemy[i][e].w <= player.x + player.w && enemy[i][e].y + enemy[i][e].h >= player.y && enemy[i][e].y + enemy[i][e].h <= player.y + player.h)) {
                player.alive=0;
                if(player.alive==0){
                    numcredits--;
                }
               }
           }
       } 
   }


   //PLAYER BULLET && ENEMY BULLET
   if ((bullet.x >= enemy_bullet.x && bullet.x <= enemy_bullet.x + 9 && bullet.y >= enemy_bullet.y && bullet.y <= enemy_bullet.y + 20) ||
       (bullet.x + 9 >= enemy_bullet.x && bullet.x + 9 <= enemy_bullet.x + 9 && bullet.y >= enemy_bullet.y && bullet.y <= enemy_bullet.y + 20) ||
       (bullet.x >= enemy_bullet.x && bullet.x <= enemy_bullet.x + 9 && bullet.y + 20 >= enemy_bullet.y && bullet.y + 20 <= enemy_bullet.y + 20) ||
       (bullet.x + 9 >= enemy_bullet.x && bullet.x + 9 <= enemy_bullet.x + 9 && bullet.y + 20 >= enemy_bullet.y && bullet.y + 20 <= enemy_bullet.y + 20)) {
           enemy_bullet_init=1;
           bullet_init=1;
           bullet.x=-50;
           bullet.y=-50;
           enemy_bullet.x=-100;
           enemy_bullet.y=-100;
       }
  


    //TO CHECK PLAYER && ENEMY BULLET COLIDES
    if(enemy_bullet.x>player.x && enemy_bullet.y>player.y && enemy_bullet.x+9<player.x+64 && enemy_bullet.y+20<player.y+64){
       if(enemy_bullet_init==0){
           //Kill the bullet
           player.alive--;
           if(player.alive==0){
               numcredits--;
           }
           #ifdef _WIN32
            soloud.play(audio[5], 0.6f /* volumen */, 0.0f /* pan */);
           #endif
           player.x=100;
           enemy_bullet_init=1;
           //to log player lost 1hp
           printf("\nPlayer lost 1 Live");
           //Moves the bullet away
           enemy_bullet.x=-50;
           enemy_bullet.y=-50;
       }
   }
   // ENEMY BULLET & BARRIER COLLISION
   for(int i=0; i<4; i++){
       for(int e=0; e<4; e++){
           if(barrier[i][e].alive>=1  && enemy_bullet_init==0){
               // Check if any corner of the enemy bullet is inside the barrier square
               if((enemy_bullet.x >= barrier[i][e].x && enemy_bullet.x <= barrier[i][e].x + barrier[i][e].w && enemy_bullet.y >= barrier[i][e].y && enemy_bullet.y <= barrier[i][e].y + barrier[i][e].h) ||
                   //Top-left corner
                   (enemy_bullet.x + 9 >= barrier[i][e].x && enemy_bullet.x + 9 <= barrier[i][e].x + barrier[i][e].w && enemy_bullet.y >= barrier[i][e].y && enemy_bullet.y <= barrier[i][e].y + barrier[i][e].h) ||
                   //Top-right corner
                   (enemy_bullet.x >= barrier[i][e].x && enemy_bullet.x <= barrier[i][e].x + barrier[i][e].w && enemy_bullet.y + 20 >= barrier[i][e].y && enemy_bullet.y + 20 <= barrier[i][e].y + barrier[i][e].h) ||
                   //Top-right corner
                   (enemy_bullet.x + 9 >= barrier[i][e].x && enemy_bullet.x + 9 <= barrier[i][e].x + barrier[i][e].w && enemy_bullet.y + 20 >= barrier[i][e].y && enemy_bullet.y + 20 <= barrier[i][e].y + barrier[i][e].h)) {
                   //Top-right corner


                   //To destroy barrier
                   barrier[i][e].alive--;
                   //To stop the bullet
                   enemy_bullet_init = 1;
               }
           }
          
       }
   }
   //PLAYER BULLET & BARRIER COLIDES
   for(int i=0; i<4; i++) {
       for(int e=0; e<4; e++) {
           if(barrier[i][e].alive>=1 && bullet_init==0){
               if((bullet.x >= barrier[i][e].x && bullet.x <= barrier[i][e].x + barrier[i][e].w && bullet.y >= barrier[i][e].y && bullet.y <= barrier[i][e].y + barrier[i][e].h) ||
                   (bullet.x + 9 >= barrier[i][e].x && bullet.x + 9 <= barrier[i][e].x + barrier[i][e].w && bullet.y >= barrier[i][e].y && bullet.y <= barrier[i][e].y + barrier[i][e].h) ||
                   (bullet.x >= barrier[i][e].x && bullet.x <= barrier[i][e].x + barrier[i][e].w && bullet.y + 20 >= barrier[i][e].y && bullet.y + 20 <= barrier[i][e].y + barrier[i][e].h) ||
                   (bullet.x + 9 >= barrier[i][e].x && bullet.x + 9 <= barrier[i][e].x + barrier[i][e].w && bullet.y + 20 >= barrier[i][e].y && bullet.y + 20 <= barrier[i][e].y + barrier[i][e].h)){
                   //To destroy barrier
                   barrier[i][e].alive--;
                   //To stop the bullet
                   bullet_init = 1;
               }
           }
       }
   }


   //PLAYER BULLET && SPACESHIP UFO
   if(bullet_init==0){
       if((bullet.x >= spaceship.x && bullet.x <= spaceship.x + spaceship.w && bullet.y >= spaceship.y && bullet.y <= spaceship.y + spaceship.h) ||
           //Top-left corner
           (bullet.x + 9 >= spaceship.x && bullet.x + 9 <= spaceship.x + spaceship.w && bullet.y >= spaceship.y && bullet.y <= spaceship.y + spaceship.h) ||
           //Top-right corner
           (bullet.x >= spaceship.x && bullet.x <= spaceship.x + spaceship.w && bullet.y + 20 >= spaceship.y && bullet.y + 20 <= spaceship.y + spaceship.h) ||
           //Top-right corner
           (bullet.x + 9 >= spaceship.x && bullet.x + 9 <= spaceship.x + spaceship.w && bullet.y + 20 >= spaceship.y && bullet.y + 20 <= spaceship.y + spaceship.h)){
           //Top-right corner


           //To stop the bullet
            #ifdef _WIN32
               soloud.stopAudioSource(audio[3]);
               audio[3].setLooping(false);
               soloud.play(audio[4], 0.8f /* volumen */, 0.0f /* pan */);
            #endif
          
           bullet_init = 1;
           spaceship_init = 1;
           spaceship.x=-50;
           ufoscore=rand()%6;
           switch(ufoscore){
               case 0:{
                   ufoscore=50;
                   break;
                }
               case 1:{
                   ufoscore=100;
                   break;
                }
               case 2:{
                   ufoscore=150;
                   break;
                }
               case 3:{
                   ufoscore=200;
                   break;
                }
               case 4:{
                   ufoscore=250;
                   break;
                }
               case 5:{
                   ufoscore=300;
                   break;
                }
           }
           numscore = numscore + ufoscore;
           ufoscore=0;
       }
   }
}
//TO CALCULATE UFO MOVEMENT AND APPARIENCE
void UfoMechanics(){
   //The average wait is less than 1min by choosing 3600 ticks, cause game takes 60 ticks x sec
   if(rand()%7200==0 || esat::IsKeyPressed('L')){
       spaceship_init=0;
       #ifdef _WIN32
        soloud.play(audio[3], 0.5f /* volumen */, 0.0f /* pan */);
        audio[3].setLooping(true);
       #endif
   }
   //When it inits it starts moving and animating
   if(spaceship_init==0){
       spaceship.x+=3;
       spaceship.animation+=2;
   }
   //If it reaches the border it "dies" and comes back to -50
   if(spaceship.x >= 800){
       #ifdef _WIN32
           soloud.stopAudioSource(audio[3]);
           audio[3].setLooping(false);
       #endif
       spaceship_init=1;
       spaceship.x=-50;
   }
}


//TO DRAW INSCREEN TEXTS
void ScreenTexts(){
   if(numscore > numhiscore){
       numhiscore = numscore;
   }
   //DRAWS
   esat::DrawBegin();
   esat::DrawClear(0,0,0);


   esat::DrawText(20,50,"Score: ");
   esat::DrawText(140,50,score);
   esat::DrawText(300,50,"Level");
   esat::DrawText(415,50,level);
   esat::DrawText(540,50, "Hi-Score:");
   esat::DrawText(700,50, hiscore);
   //CREDITS TO THE AUTHOR
   esat::DrawSetTextSize(30); //TO MAKE SMALLER TEXT
   esat::DrawText(660,710, "Credits:");
   esat::DrawText(750,710, credits);
   esat::DrawSetTextSize(50); // TO RETURN NORMAL SIZE
  
   esat::DrawEnd();
}


//WHEN PLAYER WINS IT GIVES HIM CHANCE TO GO TO NEXT LEVEL
void GameWinned(){
   //DRAWS
   esat::DrawBegin();
   esat::DrawClear(0,0,0);


   esat::DrawText(20,50,"You have won Space Invaders :)");
   esat::DrawText(20,100,"Score: ");
   esat::DrawText(300,100,"Level");
   esat::DrawText(415,100,level);
   esat::DrawText(140,100,score);


   //CREDITS TO THE AUTHOR
   esat::DrawSetTextSize(20); //TO MAKE SMALLER TEXT
   esat::DrawText(580,710,"Author: Pau Leandrez Gomez");
   esat::DrawText(610,730,"github.com/ByPauu");
   esat::DrawSetTextSize(50); // TO RETURN NORMAL SIZE


   mouse.x = esat::MousePositionX();
   mouse.y = esat::MousePositionY();
   if(mouse.x>=300 && mouse.y>=300 && mouse.x<=580 && mouse.y<=408){
       esat::DrawSprite(continue2,300,300);
       if(esat::MouseButtonDown(0)){
           ContinueGameplay();
           numlevel++;
       }
   }else{
       esat::DrawSprite(continue1,300,300);
   }
   esat::DrawEnd();
}


//WHEN PLAYER DIES IT GIVES YOU OP TO CONTINUE IF YOU HAVE MORE CREDITS
void GameLost(){
   //DRAWS
   esat::DrawBegin();
   esat::DrawClear(0,0,0);


   esat::DrawText(20,50,"You have lost Space Invaders :(");
  
   esat::DrawText(20,100,"Score: ");
   esat::DrawText(140,100,score);
   esat::DrawText(300,100,"Level");
   esat::DrawText(415,100,level);
   esat::DrawText(600,100,"Credits");
   esat::DrawText(740,100,credits);


   //CREDITS TO THE AUTHOR
   esat::DrawSetTextSize(20); //TO MAKE SMALLER TEXT
   esat::DrawText(580,710,"Author: Pau Leandrez Gomez");
   esat::DrawText(610,730,"github.com/ByPauu");
   esat::DrawSetTextSize(50); // TO RETURN NORMAL SIZE


   mouse.x = esat::MousePositionX();
   mouse.y = esat::MousePositionY();
   if(numcredits>=1){
       if(mouse.x>=300 && mouse.y>=300 && mouse.x<=580 && mouse.y<=408){
           esat::DrawSprite(continue2,300,300);
           if(esat::MouseButtonDown(0)){
               RestartGameplay();
           }
       }else{
           esat::DrawSprite(continue1,300,300);
       }
   }else{
       esat::DrawText(20,300,"You have no more credits");
   }
   esat::DrawEnd();
}


//TO CALCULATE NUMBER OF CREDITS SCORE AND LEVEL INTO TEXT
void GetCharScore(){
   snprintf(score, sizeof(score),  "%04d", numscore);
   snprintf(level, sizeof(level),  "%02d", numlevel);
   snprintf(credits, sizeof(credits), "%d", numcredits);
   snprintf(hiscore, sizeof(hiscore), "%04d", numhiscore);
}


//TO DRAW EVERY ANIMATIO OF THE COIN ON THE BEGGINING
void CreditGameplayHUD(){
   mouse.x = esat::MousePositionX();
   mouse.y = esat::MousePositionY();
   if(coin.animation<15){
       esat::DrawSprite(coin1,300,300);
   }
   if(coin.animation<30 && coin.animation>=15){
       esat::DrawSprite(coin2,300,300);
   }
   if(coin.animation<45 && coin.animation>=30){
       esat::DrawSprite(coin3,300,300);
   }
   if(coin.animation<60 && coin.animation>=45){
       esat::DrawSprite(coin4,300,300);
   }
   if(coin.animation<75 && coin.animation>=60){
       esat::DrawSprite(coin5,300,300);
   }
   if(coin.animation<90 && coin.animation>=75){
       esat::DrawSprite(coin6,300,300);
   }
   if(coin.animation>=90){
       coin.animation=0;
   }
   //TO CALCULATE COLIDES BETWEEN COIN AND MOUSE
   if(mouse.x>=300 && mouse.y>=300 && mouse.x<=370 && mouse.y<=370){
       if(esat::MouseButtonDown(0)){
           if(numcredits<5){
               numcredits++;
           }
       }
   }
   //TO CALCULATE COLIDES BETWEEN START BUTTON AND MOUSE
   if(mouse.x>=388 && mouse.y>=300 && mouse.x<=567 && mouse.y<=407){
       esat::DrawSprite(boton2,388,300);
       if(esat::MouseButtonDown(0)){
           if(numcredits>=1){
               creditinsert=1;
           }
       }
   }else{
       esat::DrawSprite(boton1,388,300);
   }
   esat::DrawText(325,280,credits);
   esat::DrawText(370,280,"Credits");
   esat::DrawText(150,50,"Welcome to Space Invaders :)");
   //CREDITS TO THE AUTHOR
   esat::DrawSetTextSize(20); //TO MAKE SMALLER TEXT
   esat::DrawText(580,710,"Author: Pau Leandrez Gomez");
   esat::DrawText(610,730,"github.com/ByPauu");
   esat::DrawSetTextSize(50); // TO RETURN NORMAL SIZE
}


//STARTING ANIMATION WERE PLAYER INSERTS COINS
void CreditStartHUD(){
   while(creditinsert==0){
       last_time = esat::Time();
       if(!esat::WindowIsOpened() || esat::IsSpecialKeyPressed(esat::kSpecialKey_Escape)){
               esat::WindowDestroy();
       }


       //TO ANIMATE THE FLIPPING COIN
       coin.animation++;


       //DRAWS
       esat::DrawBegin();//
       esat::DrawClear(0,0,0);
       CreditGameplayHUD();
       esat::DrawEnd();
      
       //TO CALCULATE NUMBER OF CREDITS SCORE AND LEVEL INTO TEXT
       GetCharScore();


       do{
           current_time = esat::Time();
       }while((current_time-last_time)<=1000.0/fps);
       esat::WindowFrame();
   }
}


void FullGameplay(){
   if(num_enemy >= 1 && player.alive != 0 && numcredits >= 1){
       //INPUTS
       PlayerInput();
      
       //UPDATES
       Colides();
       //TO CALCULATE NUMBER OF CREDITS SCORE AND LEVEL INTO TEXT
       GetCharScore();


       EnemyAttacks();
       UfoMechanics();
   }
   if(num_enemy >= 1 && player.alive != 0 && numcredits >= 1){
       //Movement
       if(horizontal==0){
           EnemyMovement();
       }else{
           EnemyDownMovement();
       }
   }
   if(num_enemy >= 1 && player.alive != 0 && numcredits >= 1){
       //START DRAWINGS
       esat::DrawBegin();
       esat::DrawClear(0,0,0);
       DrawSprites();
       ScreenTexts();
       esat::DrawEnd();
   }
}


void WinOrDieScreen(){
   if(num_enemy == 0 && player.alive != 0 && numcredits >= 1){
       GameWinned();
   }
   //WHEN PLAYER LOSTS
   if(player.alive == 0){
       GameLost();
   }
}


void InitSound(){
#ifdef _WIN32
    //Load audio
    soloud.init();
    //Enemy explosion
    audio[0].load("../assets/audio/backgroundsound.mp3");
    audio[0].setLooping(true);
    audio[1].load("../assets/audio/shoot.wav");
    audio[2].load("../assets/audio/invaderkilled.wav");
    audio[3].load("../assets/audio/ufo_lowpitch.wav");
    audio[4].load("../assets/audio/ufo_highpitch.wav");
    audio[5].load("../assets/audio/explosion.wav");

    soloud.play(audio[0], 0.5f /* volumen */, 0.0f /* pan */);
#endif
}


void FreeSound(){
#ifdef _WIN32
   soloud.deinit();
#endif
}
