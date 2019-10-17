#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdio.h>
#include "bkg.c"
#include "bkgTiles.c"
#include "tetromino.c"

UWORD spritePalette[] =
  {
   0, RGB_WHITE, RGB(31, 0, 0), RGB_ORANGE,
   0, RGB_WHITE, RGB_BLUE, RGB_BLUE
  };

UWORD bkgPalette[] =
  {
   0, RGB_WHITE, RGB_BLUE, RGB_PURPLE
  };

enum TETRO{I,O,T,S,Z,J,L};

//starting coordinates
const INT8 strX = 16;
const INT8 strY = 16;

//tetromino properties
int currX = strX;
int currY = strY;
enum TETRO currTetro = T;
int tetroState = 0;
int coor[4][2];

void perfDelay(UINT8 numloops){
  UINT8 i;
  for(i = 0; i < numloops; i++){
    wait_vbl_done();
  }
}

void startTetro(){
  //populate the coors array with the state 0 coordinates of
  //currTetro
  UINT8 i;
  if(currTetro == T && tetroState == 0){
    for(i = 0; i < 4; i++){
      if(i == 3){
	coor[i][0] = 1;
	coor[i][1] = 1;
      }
      else{
	coor[i][0] = i;
	coor[i][1] = 0;
      }
    }
  }
}

/*
 * valid move returns true
 */
UBYTE checkMove(int newX, int newY){
  //X left-bound: 16 right-bound: 96
  // for now the right-bound will be 96 - 24(3 sprites: width of the tetro)
  //Y top-bound: 16 bottom-bound: 152
  // for now the bottom bound will be 152 - 8(1 sprite)

  //we must take in account of the width of the tetro as well as the height
  // newX + width
  // newY + height
  return ((newX >= 16 && newX <= 72) && (newY >= 16 && newY <= 144));
}

void drawTetro(int x, int y){
  //T tetromino
  int i;
  
  //each tetro consists of 4 tiles
  for(i = 0; i < 4; i++){
    set_sprite_tile(i, 0); //get tile
    set_sprite_prop(i, 0); //set color palette

    move_sprite(i, (x + (coor[i][0] * 8)), (y + (coor[i][1] * 8)) );
  }
}

void rotate(){
  if(currTetro == T || currTetro == J || currTetro == L){
    tetroState++;
    
  }
  else if(currTetro == Z || currTetro == S || currTetro == I){
    tetroState = (tetroState < 1)? tetroState++ : 0;
  }
  drawTetro(currX, currY);
}

void moveTetro(){
  if(joypad() == J_LEFT){
    if(checkMove(currX - 8, currY)){
      currX -= 8;
      drawTetro(currX, currY);
    }
  }
  else if(joypad() == J_RIGHT){
    if(checkMove(currX + 8, currY)){
      currX += 8;
      drawTetro(currX, currY);
    }
  }
  else if(joypad() == J_UP){
    //rotate
    if(checkMove(currX, currY - 8)){
      currY -= 8;
      drawTetro(currX, currY);
    }
  }
  else if(joypad() == J_DOWN){
    //move down to the floor
    rotate();
  }
}

int gravBase = 1;
int gravRate = 25;
void gravity(){
  //to the floor
  UBYTE grav = 1;
  if(grav){
    if(gravBase % gravRate == 0){
      if(checkMove(currX, currY + 8)){
	currY += 8;
	drawTetro(currX, currY);
	//delay(rate);
      }
      else{
	//means it hit the floor so move on to the next tetro
	grav = 0;
      }
      gravBase = 1;
    }
    else{
      gravBase++;
    }
  }
}

void main(){
  //set background
  // setpalette
  VBK_REG = 1;
  set_bkg_palette(0, 1, bkgPalette);
  VBK_REG = 0;
  
  set_bkg_data(0, 18, bkgTiles);
  set_bkg_tiles(0, 0, 20, 18, bkg);

  set_sprite_data(0, 1, tetromino);
  set_sprite_palette(0, 2, spritePalette);

  startTetro();
  drawTetro(currX, currY);
  
  SHOW_BKG;
  SHOW_SPRITES;
  DISPLAY_ON;

  while(1){
    wait_vbl_done();
    
    moveTetro();

    gravity();
    
    delay(100);
    //perfDelay(rate);
  }
}
