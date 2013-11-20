#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_rotozoom.h"
#include <string>
#include <conio.h>
#include <fstream>
#include <vector>

using namespace std;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int sq_size=80;
const int MENU_WIDTH = SCREEN_WIDTH-SCREEN_HEIGHT;

int  players=0,AI_turn=-1;                          //no of players and color of AI(for 1 player chess only)
int next_move[10],prev_move[5]={0};
int xyz[10];

int mouse_x,mouse_y,num,turn=1;
int wkx=7,wky=4,bkx=0,bky=4,move_count,piece_left;
int game[8][8],moves[60];
int vPawnM[8][8],vRookM[8][8],vBishopM[8][8],vKnightM[8][8],vQueenM[8][8],vKingM[8][8];
int vPawnE[8][8],vRookE[8][8],vBishopE[8][8],vKnightE[8][8],vQueenE[8][8],vKingE[8][8];
fstream file("Game.txt",ios::in | ios::out | ios::trunc);
ifstream obj("Values.txt");
bool wlcastle=true,wrcastle=true,blcastle=true,brcastle=true;
bool quit=false,selected=false,insuff_piece=false;

SDL_Surface *screen = NULL;
SDL_Surface *black = NULL;
SDL_Surface *white = NULL;
SDL_Surface *select = NULL;
SDL_Surface *cover = NULL;
SDL_Surface *message = NULL;
SDL_Surface *restart = NULL;
SDL_Surface *choose = NULL;
SDL_Surface *rook = NULL;
SDL_Surface *knight = NULL;
SDL_Surface *bishop = NULL;
SDL_Surface *queen = NULL;
SDL_Surface *whi = NULL;
SDL_Surface *bla = NULL;
SDL_Surface *back = NULL;
SDL_Surface *attack = NULL;
SDL_Surface *checkm = NULL;
SDL_Surface *stalem = NULL;
SDL_Surface *move_50 = NULL;
SDL_Surface *insuff = NULL;
SDL_Surface *thatsdraw = NULL;
SDL_Surface *bq = NULL;
SDL_Surface *bk = NULL;
SDL_Surface *bp = NULL;
SDL_Surface *bb = NULL;
SDL_Surface *br = NULL;
SDL_Surface *bn = NULL;
SDL_Surface *wq = NULL;
SDL_Surface *wk = NULL;
SDL_Surface *wp = NULL;
SDL_Surface *wb = NULL;
SDL_Surface *wr = NULL;
SDL_Surface *wn = NULL;
SDL_Surface *choice_a = NULL;
SDL_Surface *choice_b = NULL;

SDL_Event event;
TTF_Font *font = NULL;
SDL_Color textWhite = { 255, 255, 255 };
SDL_Color textRed = { 255, 0, 0 };
SDL_Color textGreen = { 0, 128, 0 };

//SDL_Color bg = { 255, 0, 255 };

SDL_Surface *load_image(string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
bool init();
int load_files();
void clean_up();
void board();
void pieces();
SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
void resizeImage( SDL_Surface*& img, const double newwidth, const double newheight );
bool detect(int,int,int [8][8],int);
void init_game();
void draw_piece(int,int);
int gen_moves(int [8][8],int,int,int []);
bool enemy(int,int,int,int [8][8]);
void draw_moves(int);
bool position_check(int,int,int,int [8][8]);
int m_pawn(int,int,int,int [8][8],int []);
int m_rook (int,int,int,int [8][8],int []);
int m_knight (int,int,int,int [8][8],int []);
int m_bishop (int,int,int,int [8][8],int []);
int m_queen (int,int,int,int [8][8],int []);
int m_king (int,int,int,int [8][8],int []);
bool valid_moves(int,int,int,int []);
void walk(int,int);
void deselect(int,int,int);
void update_game(int,int[]);
void board_piece(int,int);
void redraw(int,int);
bool m_enpasant(int,int,int,int[]);
int m_castle(int [],int);
void menu(int);
void welcome();
void piece_square_values();
bool promotion();
bool capture();
bool insuffice();
bool search_piece(int,int,int,int,int,int [],int [8][8]);
void read_file(int [8][8]);



/*
bool human_promotion(int game[8][8],int turn,int next_move[5]);
bool AI_promotion(int game[8][8],int turn,int next_move[5]);
void pulkit_update_game(int game[8][8],int turn,int next_move[]);
void update_top_n_moves(float arr[6][50],int top_n,float score,int check_further,int next_move[5],int next_next_move[5]);
float highest_of_top_n(float arr[6][50],int top_n,int next_move[5]);
void initialize_top_n_moves(float a[6][50],int n);
void AI_func(int game[8][8],int AI_turn,int next_move[5]);
float AI_depth(int game[8][8],int AI_turn,int depth,int top_n,int check_further,int next_move[5]);
bool pulkit_AI_promotion(int game[8][8],int turn,int next_temp_move[6]);
*/
void AI_func(int game[8][8],int AI_turn,int next_move[10],int prev_move[10]);
int AI_depth(int game[8][8],int turn,int depth,int alpha,int next_move[10]);
void AI_initialize_top_move(int a[10],int depth);
void AI_update_top_move(int top_move[10],int depth,int next_move[10]);
void AI_initialize_moves(int moves[]);
void AI_copy_game(int a[8][8],int b[8][8]);
void AI_intialize_next_move(int next_move[10]);
bool AI_detect(int game[8][8],int x,int y,int turn=0);
bool AI_enemy(int game[8][8],int x,int y,int color);
void AI_update_game(int game[8][8],int turn,int next_move[]);
int AI_gen_moves(int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_pawn(int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_rook (int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_knight (int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_bishop (int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_queen (int game[8][8],int x,int y,int turn,int moves[]);
int AI_m_king (int game[8][8],int x,int y,int turn,int moves[]);
int AI_filter_moves(int game[8][8],int moves[],int pos,int turn);
bool AI_position_check(int game[8][8],int x,int y, int turn);
bool AI_is_castle(int game[8][8],int turn,int next_move[5]);
bool AI_is_enpasant(int game[8][8],int turn,int next_move[10]);
void AI_normal_move(int game[8][8],int turn,int next_move[10]);
float AI_move_score(int game[8][8],int turn);
bool AI_search_piece(int srow,int scol,int erow,int ecol,int to_find,int store[2],int game[8][8]);
void AI_piece_square_values();
void AI_read_file(int store[8][8]);
int AI_abs(int n);





void copy_game(int a[8][8],int b[8][8]);
float move_score(int [8][8],int);
void start_and_initialize();
void write_moves_to_file(int,int [4]);
int human_func(int game[8][8],int,int []);
void check_everything(int []);
void draw_updated_game(int,int []);
bool is_enpasant(int game[8][8],int,int []);
bool is_castle(int game[8][8],int,int []);
bool check_mate(int);
bool stale_mate(int,int [8][8]);
bool move_50_rule(int,int);
void copy_prev_move(int []);
int filter_moves(int [8][8],int [],int,int);
void print_game();
void normal_move(int [8][8],int,int []);
void update_game(int [8][8],int,int []);
void end_game();

int main(int argc, char* args[])
{
    if( init() == false )return 1;

    if( load_files() != 0 )return 2;

    if(file.good())
        file<<"Everything Loaded\n";

    start_and_initialize();

    while(!quit)
    {
        if(turn==AI_turn)
        {
            AI_func(game,turn,next_move,xyz);

            if(file.good())
            {
                file<<"Move AI:"<<endl;
                file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<endl;



            }
        }
        else
        {
            AI_move_score(game,-turn);

            //file<<game[0][0]<<endl;
            int temp=human_func(game,turn,next_move);

                   //0 for restart
            //file<<game[0][0]<<endl;
            if(file.good())
            {
                file<<"Move Human:"<<endl;
                file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<endl;
            }                                             //-1 for exit
            if(temp==0)                                     // 1 otherwise
            {
                start_and_initialize();
                continue;
            }
            else if(temp==-1)quit=true;

            else if(temp==-2)
            {
                if(file.good())
                {
                    file<<"SDL Error found..Exiting the game";
                    quit=true;
                }
            }
        }





        if(!quit)
        {

            //file<<"im here"<<endl;


            //write_moves_to_file(turn,next_move);        //Debugging purpose only
            //file<<game[0][0]<<endl;
            update_game(game,turn,next_move);                //logical part
            //file<<game[3][3]<<endl;

            draw_updated_game(turn,next_move);          //graphical part
            //file<<game[0][0]<<endl;
            check_everything(next_move);                //It also changes turn
            //file<<game[0][0]<<endl;
        }
    }

    clean_up();

    return 0;
}


SDL_Surface *load_image(string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
}


bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;
    if( TTF_Init() == -1 )
        return false;


    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
        return false;

    SDL_WM_SetCaption( "Chess", NULL );

    return true;
}


int load_files()
{
    black = load_image( "80/blacksquare.png" );
    white = load_image( "80/whitesquare.png" );
    select = load_image( "80/selected.png" );
    back = load_image( "80/back.png" );
    cover=load_image( "80/cover.png" );
    bq= load_image( "80/bq.png" );
    bk= load_image( "80/bk.png" );
    bp= load_image( "80/bp.png" );
    br= load_image( "80/br.png" );
    bn= load_image( "80/bn.png" );
    bb= load_image( "80/bb.png" );
    wq= load_image( "80/wq.png" );
    wk= load_image( "80/wk.png" );
    wp= load_image( "80/wp.png" );
    wr= load_image( "80/wr.png" );
    wn= load_image( "80/wn.png" );
    wb= load_image( "80/wb.png" );


    font = TTF_OpenFont( "arabella.ttf", 32 );

    message = TTF_RenderText_Blended( font, "Turn : ", textWhite );
    whi = TTF_RenderText_Blended( font, "The Great Whites", textGreen );
    bla = TTF_RenderText_Blended( font, "The Dark Knights", textGreen );
    restart = TTF_RenderText_Blended( font, "Press 'R' to restart", textWhite );
    attack = TTF_RenderText_Blended( font, "Are Under Attack", textRed );
    thatsdraw = TTF_RenderText_Blended( font, "Thats a Draw", textGreen );
    choose = TTF_RenderText_Blended( font, "Choose your piece :", textWhite );
    rook = TTF_RenderText_Blended( font, "A) Rook", textWhite );
    knight = TTF_RenderText_Blended( font, "B) Knight", textWhite );
    bishop = TTF_RenderText_Blended( font, "C) Bishop", textWhite );
    queen = TTF_RenderText_Blended( font, "D) Queen", textWhite );
    checkm = TTF_RenderText_Blended( font, "Emerge Victorious", textWhite );
    stalem = TTF_RenderText_Blended( font, "Stalemate", textWhite );
    move_50 = TTF_RenderText_Blended( font, "50 move rule", textWhite );
    insuff = TTF_RenderText_Blended( font, "Insufficient Pieces ", textWhite );
    choice_a = TTF_RenderText_Blended( font, "A)  Player vs Computer", textWhite );
    choice_b = TTF_RenderText_Blended( font, "B)  Player vs Player", textWhite );

    piece_square_values(); // Reads all the piece square values from file into respective arrays

    if( black == NULL||white == NULL ||select == NULL ||back == NULL ||cover == NULL  )
        return 52;
    if(bq == NULL ||bk == NULL ||bp == NULL ||br == NULL ||bn == NULL ||bb == NULL )
        return 2;
    if(wq == NULL ||wk == NULL ||wp == NULL ||wr == NULL ||wn == NULL ||wb == NULL )
        return 3;
    if( font == NULL )
        return 4;
    if(message == NULL ||whi == NULL ||bla == NULL ||restart == NULL ||attack == NULL ||thatsdraw == NULL )
        return 5;
    if(choose == NULL ||rook == NULL ||knight == NULL ||bishop == NULL ||queen == NULL ||checkm == NULL )
        return 6;
     if(stalem == NULL ||move_50 == NULL ||insuff == NULL ||choice_a == NULL ||choice_b == NULL )
        return 7;

    return 0;
}


void board()
{
    resizeImage(white,sq_size,sq_size);
    resizeImage(black,sq_size,sq_size);
    resizeImage(select,sq_size,sq_size);

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if((i%2!=0&&j%2!=0)||(i%2==0&&j%2==0))
            apply_surface(i*sq_size,j*sq_size,white,screen);
            else
            apply_surface(i*sq_size,j*sq_size,black,screen);
        }
    }
}


void pieces()
{
    //Pawns
    for(int i=0;i<8;i++)
    {
        apply_surface(i*sq_size,sq_size*1,bp,screen);
        apply_surface(i*sq_size,sq_size*6,wp,screen);
    }

    //Blacks
    apply_surface(0,0,br,screen);
    apply_surface(sq_size,0,bn,screen);
    apply_surface(sq_size*2,0,bb,screen);
    apply_surface(sq_size*3,0,bq,screen);
    apply_surface(sq_size*4,0,bk,screen);
    apply_surface(sq_size*5,0,bb,screen);
    apply_surface(sq_size*6,0,bn,screen);
    apply_surface(sq_size*7,0,br,screen);
    //Whites
    apply_surface(0,sq_size*7,wr,screen);
    apply_surface(sq_size,sq_size*7,wn,screen);
    apply_surface(sq_size*2,sq_size*7,wb,screen);
    apply_surface(sq_size*3,sq_size*7,wq,screen);
    apply_surface(sq_size*4,sq_size*7,wk,screen);
    apply_surface(sq_size*5,sq_size*7,wb,screen);
    apply_surface(sq_size*6,sq_size*7,wn,screen);
    apply_surface(sq_size*7,sq_size*7,wr,screen);
}


void resizeImage( SDL_Surface*& img, const double newwidth, const double newheight )
{
    double zoomx = newwidth  / (float)img->w;
    double zoomy = newheight / (float)img->h;

    SDL_Surface* sized = zoomSurface( img, zoomx, zoomy, SMOOTHING_OFF );

    // Copy transparency data.
    // matchColorKeys( img, sized );

    SDL_FreeSurface( img );

    img =  sized;
}


bool detect(int x,int y,int game[8][8],int turn=0)
{
    if(y*sq_size>=SCREEN_WIDTH-MENU_WIDTH||y<0)return false;
    if(x*sq_size>=SCREEN_HEIGHT||x<0)return false;

    if(turn==0)
        if(game[x][y]==0)return false;

    if(turn==1)
        if(game[x][y]<=0)return false;

    if(turn==-1)
        if(game[x][y]>=0)return false;

    return true;
}


void init_game()
{
    for(int i=0;i<8;i++)        //Empty
        for(int j=0;j<8;j++)
            game[i][j]=0;

    for(int i=0;i<8;i++)        //Pawns
    {
        game[1][i]=-1;
        game[6][i]=1;
    }
    //Blacks
    game[0][0]=-2;game[0][7]=-2;  //Rook
    game[0][1]=-3;game[0][6]=-3;  //Knight
    game[0][2]=-4;game[0][5]=-4;  //Bishop
    game[0][3]=-5;               //Queen
    game[0][4]=-6;               //King

    //Whites
    game[7][0]=2;game[7][7]=2;  //Rook
    game[7][1]=3;game[7][6]=3;  //Knight
    game[7][2]=4;game[7][5]=4;  //Bishop
    game[7][3]=5;               //Queen
    game[7][4]=6;               //King

    wlcastle=true,wrcastle=true,blcastle=true,brcastle=true,quit=false;
    selected=false,insuff_piece=false;
    turn=1,piece_left=32,move_count=0;
}


void draw_piece(int x,int y)
{
    if(y*sq_size>=SCREEN_WIDTH-MENU_WIDTH||y<0)return;
    if(x*sq_size>=SCREEN_HEIGHT||x<0)return ;

    if(game[x][y]==1)apply_surface(y*sq_size,x*sq_size,wp,screen);
    else if(game[x][y]==-1)apply_surface(y*sq_size,x*sq_size,bp,screen);
    else if(game[x][y]==2)apply_surface(y*sq_size,x*sq_size,wr,screen);
    else if(game[x][y]==-2)apply_surface(y*sq_size,x*sq_size,br,screen);
    else if(game[x][y]==3)apply_surface(y*sq_size,x*sq_size,wn,screen);
    else if(game[x][y]==-3)apply_surface(y*sq_size,x*sq_size,bn,screen);
    else if(game[x][y]==4)apply_surface(y*sq_size,x*sq_size,wb,screen);
    else if(game[x][y]==-4)apply_surface(y*sq_size,x*sq_size,bb,screen);
    else if(game[x][y]==5)apply_surface(y*sq_size,x*sq_size,wq,screen);
    else if(game[x][y]==-5)apply_surface(y*sq_size,x*sq_size,bq,screen);
    else if(game[x][y]==6)apply_surface(y*sq_size,x*sq_size,wk,screen);
    else if(game[x][y]==-6)apply_surface(y*sq_size,x*sq_size,bk,screen);
}


int gen_moves(int game[8][8],int x,int y,int moves[])
{
    int count=0;
    int backup[8][8];
    int turn;

    if(game[x][y]>=0)
    {
        turn=1;
    }
    if(game[x][y]<0)
    {
        turn=-1;
    }

    if(game[x][y]==1*turn)count=m_pawn(x,y,turn,game,moves);

    else if(game[x][y]==2*turn)count=m_rook(x,y,turn,game,moves);

    else if(game[x][y]==3*turn)count=m_knight(x,y,turn,game,moves);

    else if(game[x][y]==4*turn)count=m_bishop(x,y,turn,game,moves);

    else if(game[x][y]==5*turn)count=m_queen(x,y,turn,game,moves);

    else if(game[x][y]==6*turn)count=m_king(x,y,turn,game,moves);


    copy_game(game,backup);
    count=filter_moves(backup,moves,count,turn);

    return count;
}


void draw_moves(int num)
{
    for(int i=0;i<num;i++)
    {
        if(moves[i*2]*sq_size>=SCREEN_WIDTH-MENU_WIDTH||moves[i*2]*sq_size<0)continue;
        if(moves[i*2+1]*sq_size>=SCREEN_HEIGHT||moves[i*2+1]*sq_size<0)continue;

        apply_surface(moves[i*2+1]*sq_size,moves[i*2]*sq_size,select,screen);
        if(detect(moves[i*2],moves[i*2+1],game))
            draw_piece(moves[i*2],moves[i*2+1]);
    }
}


bool position_check(int x,int y, int turn,int game[8][8])
{
    if(x<0||x>7||y<0||y>7)return false;

    int temp,temp1,temp2;
    int moves[60];

    temp=m_rook(x,y,turn,game,moves);


    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*2)||game[temp1][temp2]==(-1*turn*5))
            return true;
    }

    temp=m_knight(x,y,turn,game,moves);
    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*3))
            return true;
    }

    temp=m_bishop(x,y,turn,game,moves);
    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*4)||game[temp1][temp2]==(-1*turn*5))
            return true;
    }

    for(int i=0;i<3;i++)
    {
        temp1=x-1+i;
        if(temp1<0)continue;
        else if(temp1>7)break;
        for(int j=0;j<3;j++)
        {
            temp2=y-1+j;
            if(temp2<0)continue;
            else if(temp2>7)break;

            if(game[temp1][temp2]==(-1*turn*6))
                return true;
        }
    }

    if(turn>0)
    {
        temp1=x-1;
        temp2=y-1;
        if(temp1>=0&&temp2>=0)
            if(game[temp1][temp2]==-1)
                return true;

        temp1=x-1;
        temp2=y+1;
        if(temp1>=0&&temp2<=7)
            if(game[temp1][temp2]==-1)
                return true;
    }
    else
    {
        temp1=x+1;
        temp2=y+1;
        if(temp1<=7&&temp2<=7)
            if(game[temp1][temp2]==1)
                return true;

        temp1=x+1;
        temp2=y-1;
        if(temp1<=7&&temp2>=0)
            if(game[temp1][temp2]==1)
                return true;
    }

    return false;
}


int m_pawn(int x,int y,int turn,int game[8][8],int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp[2];

    if(turn==1)
    {
        if(!detect(x-1,y,game))
        {
            moves[count++]=x-1;
            moves[count++]=y;

            if(x==6)
            {
                if(!detect(x-2,y,game))
                {
                    moves[count++]=x-2;
                    moves[count++]=y;
                }
            }
        }

        if(enemy(x-1,y-1,-turn,game))
        {
            moves[count++]=x-1;
            moves[count++]=y-1;
        }

        if(enemy(x-1,y+1,-turn,game))
        {
            moves[count++]=x-1;
            moves[count++]=y+1;
        }

        if(m_enpasant(x,y,turn,temp))
        {
            moves[count++]=temp[0];
            moves[count++]=temp[1];
        }

    }
    else if(turn==-1)
    {
        if(!detect(x+1,y,game))
        {
            moves[count++]=x+1;
            moves[count++]=y;

            if(x==1)
            {
                if(!detect(x+2,y,game))
                {
                    moves[count++]=x+2;
                    moves[count++]=y;
                }
            }
        }

        if(enemy(x+1,y-1,-turn,game))
        {
            moves[count++]=x+1;
            moves[count++]=y-1;
        }

        if(enemy(x+1,y+1,-turn,game))
        {
            moves[count++]=x+1;
            moves[count++]=y+1;
        }
        if(m_enpasant(x,y,turn,temp))
        {
            moves[count++]=temp[0];
            moves[count++]=temp[1];
        }
    }
    return count/2;
}


int m_rook (int x,int y,int turn,int game[8][8],int moves[])
{

    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;
        if(temp1>7)
            break;
        if(!detect(temp1,temp2,game))
        {
            //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                //continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                   // continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;
        if(temp1<0)
            break;
        if(!detect(temp1,temp2,game))
        {
            //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
               // continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                    //continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp2--;
        if(temp2<0)
            break;
        if(!detect(temp1,temp2,game))
        {
           // if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
           //    continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
               // if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                //    continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp2++;
        if(temp2>7)
            break;
        if(!detect(temp1,temp2,game))
        {
           // if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
           //     continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
               // if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                 //   continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }
    return count/2;
}


int m_knight (int x,int y,int turn,int game[8][8],int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=y-2;
    if(temp1>=0)
    {
        temp2=x-1;
        if(temp2>=0)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
        temp2=x+1;
        if(temp2<=7)
        {
           // if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
    }

    temp1=y-1;
    if(temp1>=0)
    {
        temp2=x-2;
        if(temp2>=0)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
        temp2=x+2;
        if(temp2<=7)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
    }

    temp1=y+1;
    if(temp1<=7)
    {
        temp2=x-2;
        if(temp2>=0)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
        temp2=x+2;
        if(temp2<=7)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
    }

    temp1=y+2;
    if(temp1<=7)
    {
        temp2=x-1;
        if(temp2>=0)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
        temp2=x+1;
        if(temp2<=7)
        {
            //if(!future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
            {
                if(turn<0)
                {
                    if(game[temp2][temp1]>=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
                else if(turn>0)
                {
                    if(game[temp2][temp1]<=0)
                    {
                        moves[count++]=temp2;
                        moves[count++]=temp1;
                    }
                }
            }
        }
    }
    return count/2;
}


int m_bishop (int x,int y,int turn,int game[8][8],int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;temp2++;
        if(temp1>7||temp2>7)
            break;
        if(!detect(temp1,temp2,game))
        {
           // if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
              //  continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                  //  continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;temp2--;
        if(temp1<0||temp2<0)
            break;
        if(!detect(temp1,temp2,game))
        {
            //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
              //  continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                 //   continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;temp2--;
        if(temp1>7||temp2<0)
            break;
        if(!detect(temp1,temp2,game))
        {
            //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
              //  continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                  //  continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;temp2++;
        if(temp1<0||temp2>7)
            break;
        if(!detect(temp1,temp2,game))
        {
            //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
               // continue;
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(enemy(temp1,temp2,-turn,game))
            {
                //if(future(kcheck,temp,count,x,y,temp1,temp2,turn,kx,ky,kc,tc))
                 //   continue;
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    return count/2;
}



int m_queen (int x,int y,int turn,int game[8][8],int moves[])
{
    int temp[60],temps=2;
    temp[0]=x;
    temp[1]=y;

    int temp1=m_rook(x,y,turn,game,moves);
    temp1=temp1*2;
    for(int i=2;i<temp1;i++)
        temp[temps++]=moves[i];

    temp1=m_bishop(x,y,turn,game,moves);
    temp1*=2;
    for(int i=2;i<temp1;i++)
        temp[temps++]=moves[i];

    for(int i=0;i<temps;i++)
        moves[i]=temp[i];

    return temps/2;
}



int m_king (int x,int y,int turn,int game[8][8],int moves[])
{
    int temp[60],temps=2;
    temp[0]=x;
    temp[1]=y;

    int temp1,temp2;
    for(int i=0;i<3;i++)
    {
        temp1=y-1+i;
        if(temp1<0)continue;
        else if(temp1>7)break;
        for(int j=0;j<3;j++)
        {
            temp2=x-1+j;
            if(temp1==y&&temp2==x)continue;
            if(temp2<0)continue;
            else if(temp2>7)break;

            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    temp[temps++]=temp2;
                    temp[temps++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    temp[temps++]=temp2;
                    temp[temps++]=temp1;
                }
            }
        }
    }

    temps=m_castle(temp,temps);  //Checks if castling moves are possible

    for(int i=0;i<temps;i++)
        moves[i]=temp[i];
    return temps/2;
}



bool enemy(int x,int y,int color,int game[8][8])
{
    if(y*sq_size>=SCREEN_WIDTH-MENU_WIDTH||y<0)return false;
    if(x*sq_size>=SCREEN_HEIGHT||x<0)return false;

    if(color>0)
    {
        if(game[x][y]>0)return true;
    }
    else if(color<0)
        if(game[x][y]<0)return true;
    return false;
}



bool valid_moves(int x,int y,int num,int moves[])
{
    for(int i=1;i<num;i++)
        if(moves[i*2]==x&&moves[i*2+1]==y)return true;
    return false;
}



void deselect(int x,int y,int num)
{
    for(int i=1;i<num;i++)
    {
        if(moves[i*2]==x&&moves[i*2+1]==y)continue;
        board_piece(moves[i*2],moves[i*2+1]);
    }
}



void board_piece(int x,int y)
{
     if(x*sq_size>=SCREEN_HEIGHT||x<0)return;
    if(y*sq_size>=SCREEN_WIDTH-MENU_WIDTH||y<0)return ;

    if((x%2!=0&&y%2!=0)||(x%2==0&&y%2==0))
        apply_surface(y*sq_size,x*sq_size,white,screen);
    else
        apply_surface(y*sq_size,x*sq_size,black,screen);
}



void redraw(int x,int y)
{
    int temp1,temp2;
    for(int i=1;i<num;i++)
    {
        temp1=i*2;
        temp2=temp1+1;
        if(moves[temp1]==x&&moves[temp2]==y)continue;
        if(game[moves[temp1]][moves[temp2]]!=0)
            draw_piece(moves[temp1],moves[temp2]);
    }
}



bool m_enpasant(int x,int y,int turn,int arr[2])
{

    bool enpasant=false;

    //Check if the prev move was a double step pawn

    if(game[prev_move[2]][prev_move[3]]==1||game[prev_move[2]][prev_move[3]]==-1)
    {
        if(abs(prev_move[2]-prev_move[0])==2)
            enpasant=true;
    }

    //Check if the pawn is on the fifth line and another pawn is adjacent to you
    if(enpasant)
    {
        if(turn>0)
        {
            if(x==3)
            {
                if(y-prev_move[3]==1)
                {
                    arr[0]=x-1;
                    arr[1]=y-1;
                    return true;

                }
                if(prev_move[3]-y==1)
                {
                    arr[0]=x-1;
                    arr[1]=y+1;
                    return true;
                }
            }
        }
        else
        {
            if(x==4)
            {
                if(y-prev_move[3]==1)
                {
                    arr[0]=x+1;
                    arr[1]=y-1;
                    return true;

                }
                if(prev_move[3]-y==1)
                {
                    arr[0]=x+1;
                    arr[1]=y+1;
                    return true;
                }
            }

        }

    }
    return false; //No enpasant move available

}



int m_castle(int temp[],int pos)
{
    if(wrcastle)
    {
        if(game[7][5]==0&&game[7][6]==0) //squares between rook and king are empty
        {
            //Check if the pieces king is moving through are not in attack
            if(!position_check(7,4,turn,game)&&!position_check(7,5,turn,game)&&!position_check(7,6,turn,game))
            {
                temp[pos++]=7;
                temp[pos++]=6;
            }
        }
    }

    if(wlcastle)
    {
        if(game[7][3]==0&&game[7][2]==0&&game[7][1]==0) //squares between rook and king are empty
        {
            //Check if the pieces king is moving through are not in attack
            if(!position_check(7,4,turn,game)&&!position_check(7,3,turn,game)&&!position_check(7,2,turn,game)&&!position_check(7,1,turn,game))
            {
                temp[pos++]=7;
                temp[pos++]=2;
            }
        }
    }

    if(brcastle)
    {
        if(game[0][5]==0&&game[0][6]==0) //squares between rook and king are empty
        {
            //Check if the pieces king is moving through are not in attack
            if(!position_check(0,4,turn,game)&&!position_check(0,5,turn,game)&&!position_check(0,6,turn,game))
            {
                temp[pos++]=0;
                temp[pos++]=6;
            }
        }
    }

    if(blcastle)
    {
        if(game[0][3]==0&&game[0][2]==0&&game[0][1]==0) //squares between rook and king are empty
        {
            //Check if the pieces king is moving through are not in attack
            if(!position_check(0,4,turn,game)&&!position_check(0,3,turn,game)&&!position_check(0,2,turn,game)&&!position_check(0,1,turn,game))
            {
                temp[pos++]=0;
                temp[pos++]=2;
            }
        }
    }

    return pos;
}


bool capture()
{
    int temp=0;

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(game[i][j]!=0)
                temp++;

    if(temp==piece_left)return false;
    else
    {
        piece_left=temp;
        return true;
    }
}


void menu(int col=0)
{
    apply_surface( 640, 0, back, screen );
    apply_surface( 650, 100, message, screen );
    apply_surface( 650, 350, restart, screen );
    if(col==0)return;
    if(col==1)apply_surface( 650, 150, whi, screen );
    else if(col==-1)apply_surface( 650, 150, bla, screen );

}


bool insuffice()
{

    if(piece_left>=5)return false;

    int size=0,knights=0,bishops=0,b=0,w=0;

    vector<int> rem;

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
                if(game[i][j]!=0)rem.push_back(game[i][j]);
                if(game[i][j]==3||game[i][j]==-3)knights++;
                if(game[i][j]==4||game[i][j]==-4)bishops++;
                if(game[i][j]>0)w++;
                if(game[i][j]<0)b++;
        }

    size=rem.size();

    if(size==2||(size==3&&(knights==1||bishops==1))||(b==2&&w==2&&knights==2))return true;
    return false;
}


void copy_game(int a[8][8],int b[8][8])
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            b[i][j]=a[i][j];
}


/*
     //PASTE YOUR AI CODE HERE
    //Start of AI code









bool pulkit_AI_promotion(int game[8][8],int turn,int next_temp_move[6])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    if(game[source_x][source_y]==turn*1)                        //check for pawn
    {
        if(destination_x==0||destination_x==7)                  //check for last row
        {
            //return true;
            //game[source_x][source_y]=0;
            //game[destination_x][destination_y]=next_move[4];     //promotes the pawn
        }
    }
    return false;
}


void pulkit_update_game(int game[8][8],int turn,int next_move[])
{
    if(!is_castle(game,turn,next_move))
        if(!is_enpasant(game,turn,next_move))
            normal_move(game,turn,next_move);
}


void update_top_n_moves(float arr[6][50],int top_n,float score,int check_further,int next_move[5],int next_next_move[5])
{
    int min=100000,pos=-1;
    for(int i=0;i<top_n*2;i+=2)
    {
        if(arr[2][i+1]<min)
        {
            min=arr[2][i+1];
            pos=i;
        }
    }
    if(score>min&&pos!=-1)
    {
        arr[0][pos]=next_move[0];
        arr[0][pos+1]=next_move[1];
        arr[1][pos]=next_move[2];
        arr[1][pos+1]=next_move[3];

        arr[2][pos]=next_move[4];
        arr[2][pos+1]=score;

        if(check_further!=0)
        {
            arr[3][pos]=next_next_move[0];
            arr[3][pos+1]=next_next_move[1];
            arr[4][pos]=next_next_move[2];
            arr[4][pos+1]=next_next_move[3];

            arr[5][pos]=next_move[4];
            //arr[5][pos+1]=score;
        }
    }
}


float highest_of_top_n(float arr[6][50],int top_n,int next_move[5])
{
    int max=-100000,pos=-1;
    for(int i=0;i<top_n*2;i+=2)
    {
        if(arr[2][i+1]>max)
        {
            max=arr[2][i+1];
            pos=i;
        }
    }

    if(pos!=-1)
    {
        next_move[0]=arr[0][pos];
        next_move[1]=arr[0][pos+1];
        next_move[2]=arr[1][pos];
        next_move[3]=arr[1][pos+1];
        next_move[4]=arr[2][pos];
    }
    return max;
}


void initialize_top_n_moves(float a[6][50],int n)
{
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<30;j++)
        {
            a[i][j]=-50000;
        }
    }
}



void AI_func(int game[8][8],int AI_turn,int next_move[5])
{
    int depth=4;
    int check_further=1;
    int top_n=15;
    AI_depth(game,AI_turn,depth,top_n,check_further,next_move);
}



float AI_depth(int game[8][8],int AI_turn,int depth,int top_n,int check_further,int next_move[5])              // returns the highest score
{
    int game_copy[8][8];
    int next_next_move[5];
    int next_temp_move[5];
    float top_n_moves[6][50];
    float score;
    int moves[60];
    int num;
    int temp;

    //file<<"Ai turn : "<<AI_turn<<" Depth : "<<depth<<endl;


    initialize_top_n_moves(top_n_moves,top_n);          //intializes top5 to -500
    copy_game(game,game_copy);              //can be improved......it inserts value of game into game_copy

    //if(stale_mate(AI_turn,game_copy))
        //return -60000;



    if(check_further==0)                              // if check_futher is false
    {
        if(AI_turn==1)                                  //for white
        {
            //file<<"\nai gen moves check 0\n";
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]>0)
                    {
                        num=gen_moves(game_copy,x,y,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];

                        //file<<"\nai gen moves\n"<<AI_turn<<" "<<check_further<<" "<<depth<<endl;


                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];

                            pulkit_update_game(game_copy,AI_turn,next_move);
                           // file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<"   ";

                            score=move_score(game_copy,AI_turn);

                            update_top_n_moves(top_n_moves,top_n,score,check_further,next_move,next_next_move);
                            copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
        if(AI_turn==-1)                                //for black
        {
          //  file<<"\nai gen moves check 0\n";
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]<0)
                    {
                        num=gen_moves(game_copy,x,y,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        //file<<"\nai gen moves\n"<<AI_turn<<" "<<check_further<<" "<<depth<<endl;


                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];

                            pulkit_update_game(game_copy,AI_turn,next_move);
                          //  file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<"   ";

                            score=move_score(game_copy,AI_turn);

                            update_top_n_moves(top_n_moves,top_n,score,check_further,next_move,next_next_move);
                            copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
        return highest_of_top_n(top_n_moves,top_n,next_move);
    }
    else                                    // if check_futher is true
    {
        if(AI_turn==1)                      //if white
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]>0)                //check for white
                    {
                        num=gen_moves(game_copy,x,y,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                      //  file<<"\nai gen moves\n";
                      //file<<"\nai gen moves\n"<<AI_turn<<" "<<check_further<<" "<<depth<<endl;


                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];

                            pulkit_update_game(game_copy,AI_turn,next_move);
                           // file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<"   "<<endl<<endl;

                            score=move_score(game_copy,AI_turn);

                            temp=AI_depth(game_copy,(-1)*AI_turn,depth,top_n,0,next_next_move);



                            score=score-temp;

                            if(temp<=-50000)
                            {
                                return score;
                            }



                            update_top_n_moves(top_n_moves,top_n,score,check_further,next_move,next_next_move);
                            copy_game(game,game_copy);

                        }
                    }
                }
            }
        }
        else
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]<0)                //check for black
                    {
                        num=gen_moves(game_copy,x,y,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                       // file<<"\nai gen moves\n"<<AI_turn<<" "<<check_further<<" "<<depth<<endl;


                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];

                            pulkit_update_game(game_copy,AI_turn,next_move);

                            //file<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<"   "<<endl<<endl;

                            score=move_score(game_copy,AI_turn);

                            temp=AI_depth(game_copy,(-1)*AI_turn,depth,top_n,0,next_next_move);


                            score=score-temp;

                            if(temp<=-50000)
                            {
                                return score;
                            }




                            update_top_n_moves(top_n_moves,top_n,score,check_further,next_move,next_next_move);
                            copy_game(game,game_copy);

                        }
                    }
                }
            }
        }
    }

    if(depth==1)
    {
        return highest_of_top_n(top_n_moves,top_n,next_move);
    }
    else
    {
        for(int i=0;i<top_n;i++)
        {
            if(top_n_moves[2][i*2+1]<=-20000)
                continue;
            next_temp_move[0]=top_n_moves[0][i*2];
            next_temp_move[1]=top_n_moves[0][i*2+1];
            next_temp_move[2]=top_n_moves[1][i*2];
            next_temp_move[3]=top_n_moves[1][i*2+1];
            next_temp_move[4]=top_n_moves[2][i*2];

            pulkit_update_game(game_copy,AI_turn,next_temp_move);

            next_temp_move[0]=top_n_moves[3][i*2];
            next_temp_move[1]=top_n_moves[3][i*2+1];
            next_temp_move[2]=top_n_moves[4][i*2];
            next_temp_move[3]=top_n_moves[4][i*2+1];
            next_temp_move[4]=top_n_moves[5][i*2];

            pulkit_update_game(game_copy,(-1)*AI_turn,next_temp_move);

            top_n_moves[2][i*2+1]+=AI_depth(game_copy,AI_turn,depth-1,top_n,check_further,next_next_move);

            copy_game(game,game_copy);
        }
    }
    return highest_of_top_n(top_n_moves,top_n,next_move);
}






    //End of AI code

*/











void welcome()
{

    bool leave= false;

    resizeImage(cover,SCREEN_WIDTH-MENU_WIDTH,SCREEN_HEIGHT);
    resizeImage(back,MENU_WIDTH,SCREEN_HEIGHT);
    apply_surface(0,0,cover,screen);
    apply_surface( 640, 0, back, screen );
    apply_surface( 640, 250, choice_a, screen );
    apply_surface( 640, 350, choice_b, screen );
    if( SDL_Flip( screen ) == -1 )return ;

     while(!leave)
     {
         if( event.type == SDL_QUIT )break;
         if( SDL_PollEvent( &event ) )
         {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_a:
                        players=1;
                        //choose_color();
                        leave=true;
                        break;
                    case SDLK_b:
                        players=2;
                        leave=true;

                    default : continue;
                }
            }
         }
     }

}


float AI_move_score(int game[8][8],int turn)
{

    const int kingWt=20000;
    const int queenWt=900;
    const int rookWt=500;
    const int knightWt=320;
    const int bishopWt=330;
    const int pawnWt=100;
    const int pawnStr=50;

    float materialScore;
    //float mobility;
    float answer;

    int wK=0,bK=0,wQ=0,bQ=0,wR=0,bR=0,wN=0,bN=0,wB=0,bB=0,wP=0,bP=0;
    int bIso=0,bBack=0,bDou=0,bPass=0,wIso=0,wBack=0,wDou=0,wPass=0;
    int pos_arr[2];

    int psv=0;

    bool begin_end_game=false;

    //Counting number of each piece on board


    for(int i=0;i<8;i++)
    {
        bool bdpawn=false;
        bool wdpawn=false;

        for(int j=0;j<8;j++)
        {
            switch(game[i][j])
            {
                    case 0 : break;
                    case 1 :wP++;break;
                    case 2 : wR++;break;
                    case 3 : wN++;break;
                    case 4 : wB++;break;
                    case 5 : wQ++;break;
                    case 6 : wK++;break;
                    case -1 : bP++;break;
                    case -2 : bR++;break;
                    case -3 : bN++;break;
                    case -4 : bB++;break;
                    case -5 : bQ++;break;
                    case -6 : bK++;break;
                    default : continue;
            }

            if(game[j][i]==-1) //Checking number of Black doubled pawns
            {
                if(bdpawn)bDou+=2;
                else bdpawn=true;
            }

            if(game[j][i]==1) //Checking number of White doubled pawns
            {
                if(wdpawn)wDou+=2;
                else wdpawn=true;
            }
        }
    }

    //Checking if end game has begun

    if(!begin_end_game)
    {
        //Both have lost queens and only 4 pieces remain other than king and pawns
        if(wQ==0&&bQ==0&&(wR+wB+wN<=4)&&(bR+bB+bN<=4))begin_end_game=true;

        //The side that has lost queen has only 2 other pieces remaining other than king and pawns

        if(wQ==1&&bQ==0&&(wR+wB+wN<=2))begin_end_game=true;
        if(wQ==0&&bQ==1&&(bR+bB+bN<=2))begin_end_game=true;

        //Both have queens but only 2 pieces remain with each side other than king and pawns
        if(wQ==1&&bQ==1&&(wR+wB+wN<=2)&&(bR+bB+bN<=2))begin_end_game=true;
    }

    // Adding the piece square values for each position

    for(int i=0;i<8;i++)
    {

        for(int j=0;j<8;j++)
        {
            if(!begin_end_game)
                switch(game[i][j])
                {
                        case 0 : break;
                        case 1 : psv+=vPawnM[i][j];break;
                        case 2 : psv+=vRookM[i][j];break;
                        case 3 : psv+=vKnightM[i][j];break;
                        case 4 : psv+=vBishopM[i][j];break;
                        case 5 : psv+=vQueenM[i][j];break;
                        case 6 : psv+=vKingM[i][j];break;
                        case -1 : psv-=vPawnM[7-i][j];break;
                        case -2 : psv-=vRookM[7-i][j];break;
                        case -3 : psv-=vKnightM[7-i][j];break;
                        case -4 : psv-=vBishopM[7-i][j];break;
                        case -5 : psv-=vQueenM[7-i][j];break;
                        case -6 : psv-=vKingM[7-i][j];break;
                        default : continue;
                }
            else
            {
                switch(game[i][j])
                {
                    case 0 : break;
                    case 1 : psv+=vPawnE[i][j];break;
                    case 2 : psv+=vRookE[i][j];break;
                    case 3 : psv+=vKnightE[i][j];break;
                    case 4 : psv+=vBishopE[i][j];break;
                    case 5 : psv+=vQueenE[i][j];break;
                    case 6 : psv+=vKingE[i][j];break;
                    case -1 : psv-=vPawnE[7-i][j];break;
                    case -2 : psv-=vRookE[7-i][j];break;
                    case -3 : psv-=vKnightE[7-i][j];break;
                    case -4 : psv-=vBishopE[7-i][j];break;
                    case -5 : psv-=vQueenE[7-i][j];break;
                    case -6 : psv-=vKingE[7-i][j];break;
                    default : continue;
                }

            }
        }
    }

    //Counting number of isolated pawns,backward pawns and passed pawns..

/*
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {

             if(game[i][j]==1)
             {
                 if(j>0&&j<7)
                 {
                     if(!AI_search_piece(0,j-1,7,j-1,1,pos_arr,game)&&!AI_search_piece(0,j+1,7,j+1,1,pos_arr,game))wIso++; // Isolated Pawns
                     if(!AI_search_piece(i+1,j-1,7,j+1,1,pos_arr,game)&&game[i][j+1]!=1&&game[i][j-1]!=1)wBack++; // Backward Pawns
                     if(!AI_search_piece(0,j-1,i-1,j+1,-1,pos_arr,game))wPass++; // Passed Pawns
                 }
                 if(j==0)
                 {
                     if(!AI_search_piece(0,j+1,7,j+1,1,pos_arr,game))wIso++; // Isolated Pawns
                     if(!AI_search_piece(i+1,j,7,j+1,1,pos_arr,game)&&game[i][j+1]!=1)wBack++; // Backward Pawns
                     if(!AI_search_piece(0,j,i-1,j+1,-1,pos_arr,game))wPass++; // Passed Pawns
                 }
                 if(j==7)
                 {
                     if(!AI_search_piece(0,j-1,7,j-1,1,pos_arr,game))wIso++; // Isolated Pawns
                     if(!AI_search_piece(i+1,j-1,7,j,1,pos_arr,game)&&game[i][j-1]!=1)wBack++; // Backward Pawns
                     if(!AI_search_piece(0,j-1,i-1,j,-1,pos_arr,game))wPass++; // Passed Pawns
                 }

             }
             if(game[i][j]==-1)
             {
                 if(j>0&&j<7)
                 {
                    if(!AI_search_piece(0,j-1,7,j-1,-1,pos_arr,game)&&!AI_search_piece(0,j+1,7,j+1,-1,pos_arr,game))bIso++; // Isolated Pawns
                    if(!AI_search_piece(i+1,j-1,7,j+1,1,pos_arr,game))bPass++; // Passed Pawns
                    if(!AI_search_piece(0,j-1,i-1,j+1,-1,pos_arr,game)&&game[i][j+1]!=-1&&game[i][j-1]!=-1)bBack++; // Backward Pawns

                 }
                 if(j==0)
                 {
                     if(!AI_search_piece(0,j+1,7,j+1,-1,pos_arr,game))bIso++; // Isolated Pawns
                     if(!AI_search_piece(i+1,j,7,j+1,1,pos_arr,game))bPass++; // Passed Pawns
                     if(!AI_search_piece(0,j,i-1,j+1,-1,pos_arr,game)&&game[i][j+1]!=-1)bBack++; // Backward Pawns
                 }
                 if(j==7)
                 {
                     if(!AI_search_piece(0,j-1,7,j-1,-1,pos_arr,game))bIso++; // Isolated Pawns
                     if(!AI_search_piece(i+1,j-1,7,j,1,pos_arr,game))bPass++; // Passed Pawns
                     if(!AI_search_piece(0,j-1,i-1,j,-1,pos_arr,game)&&game[i][j-1]!=-1)bBack++; // Backward Pawns
                 }
             }

        }
    }
*/
    materialScore = kingWt  * (wK-bK)
                  + queenWt * (wQ-bQ)
                  + rookWt  * (wR-bR)
                  + knightWt* (wN-bN)
                  + bishopWt* (wB-bB)
                  + pawnWt  * (wP-bP)
                  - pawnStr * (wIso-bIso + wBack-bBack + wDou-bDou)
                  + pawnStr*2 * (wPass-bPass);



    answer=(materialScore+psv)*turn;

    file<<"Score : "<<answer<<" Turn : "<<turn<<endl;

    return answer;
}


bool search_piece(int srow,int scol,int erow,int ecol,int to_find,int store[2],int game[8][8])
{


    for(int i=srow;i<=erow;i++)
    {
        for(int j=scol;j<=ecol;j++)
        {

            if(game[i][j]==to_find)
            {
                store[0]=i;
                store[1]=j;
                return true;
            }

        }
    }



    return false;
}


void piece_square_values()
{

    read_file(vPawnM);
    read_file(vKnightM);
    read_file(vBishopM);
    read_file(vRookM);
    read_file(vQueenM);
    read_file(vKingM);

    read_file(vPawnE);
    read_file(vKnightE);
    read_file(vBishopE);
    read_file(vRookE);
    read_file(vQueenE);
    read_file(vKingE);

}


void read_file(int store[8][8])
{

    string line;

    getline(obj,line);
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            obj>>store[i][j];
        }
    }
    obj.get();
}


void clean_up()
{
    //Free the image
    SDL_FreeSurface( black );
    SDL_FreeSurface( white );
    SDL_FreeSurface( bq );
    SDL_FreeSurface( bk );
    SDL_FreeSurface( bp );
    SDL_FreeSurface( br );
    SDL_FreeSurface( bb );
    SDL_FreeSurface( bn );
    SDL_FreeSurface( wq );
    SDL_FreeSurface( wk );
    SDL_FreeSurface( wr );
    SDL_FreeSurface( wp );
    SDL_FreeSurface( wn );
    SDL_FreeSurface( wb );
    SDL_FreeSurface( select );
    SDL_FreeSurface( back );
    SDL_FreeSurface( cover );

    SDL_FreeSurface( message );
    SDL_FreeSurface( whi );
    SDL_FreeSurface( bla );
    SDL_FreeSurface( restart );
    SDL_FreeSurface( attack );
    SDL_FreeSurface( thatsdraw );
    SDL_FreeSurface( choose );
    SDL_FreeSurface( rook );
    SDL_FreeSurface( knight );
    SDL_FreeSurface( bishop );
    SDL_FreeSurface( queen );
    SDL_FreeSurface( checkm );
    SDL_FreeSurface( stalem );
    SDL_FreeSurface( move_50 );
    SDL_FreeSurface( insuff );
    SDL_FreeSurface( choice_a );
    SDL_FreeSurface( choice_b );

    file<<"\n\n\n Game Ends";
    file.close();
    obj.close();


    TTF_CloseFont( font );
    TTF_Quit();

    SDL_Quit();
}


void start_and_initialize()
{
     welcome();
     board();
     pieces();
     init_game();
     menu(turn);

     if(file.good())
        file<<"Board and Pieces Drawn , Values Reset , Players chosen\n";

     if( SDL_Flip( screen ) == -1 )return;
}


int human_func(int game[8][8],int turn ,int next_move[])
{
    int x=0,y=0;
    int prev_x,prev_y;

    while(1)
    {

        if( event.type == SDL_QUIT )return -1; // Exit the game by returning -1

        if( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_r:
                        if(file.good())
                            file<<"\n\n\n\n\nGame Restarted...\n\n\n\n\n";
                        return 0;
                        break;  // Restart the game by returning 0

                    default : break;
                }
            }

            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if( event.button.button == SDL_BUTTON_LEFT )
                {
                    mouse_x = event.button.x;
                    mouse_y = event.button.y;

                    mouse_x=(mouse_x/80)*80;
                    mouse_y=(mouse_y/80)*80;


                    prev_x=x;       //Storing last clicked row and column
                    prev_y=y;
                    x=mouse_y/80;   //Current row and column
                    y=mouse_x/80;

                    if(x>7||y>7)continue; //Ensuring clicks on the chess board



                    if((detect(x,y,game,turn)&&!selected)) // If the piece clicked is valid and not selected
                    {

                        num=gen_moves(game,x,y,moves);   // Generates all valid moves for that piece

                        draw_moves(num);      // Draws all valid moves for that piece
                        selected=true;


                        if( SDL_Flip( screen ) == -1 )return -2;
                        continue;
                    }
                    else if(selected&&valid_moves(x,y,num,moves)) // If the piece is selected and a valid move has been made
                    {
                        selected=false;

                        deselect(x,y,num); // Deselect the selected pieces
                        redraw(x,y);

                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        next_move[2]=x;
                        next_move[3]=y;

                        return 1;

                    }

                    else // Invalid moves for the current selected piece or random moves when not selected
                    {

                        selected=false;
                        board_piece(moves[0],moves[1]);
                        draw_piece(moves[0],moves[1]);
                        deselect(x,y,num);         //Deselect any selected pieces and redraw them
                        redraw(x,y);

                        if(detect(x,y,game,turn)&&(x!=prev_x||y!=prev_y)) // If the piece clicked is valid and not selected
                        {
                            num=gen_moves(game,x,y,moves);   // Generates all valid moves for that piece
                            draw_moves(num);      // Draws all valid moves for that piece
                            selected=true;
                        }

                        if( SDL_Flip( screen ) == -1 )return -2;
                        continue;
                    }

                }

            }

        }

    }

}


void draw_updated_game (int turn,int next_move[])
{

    for(int i=0;i<8;i++) //Redraw the source row
    {
        board_piece(next_move[0],i);
        draw_piece(next_move[0],i);
    }

    for(int i=0;i<8;i++)// Redraw the destination row
    {
        board_piece(next_move[2],i);
        draw_piece(next_move[2],i);
    }

    redraw(next_move[0],next_move[1]); // Redraw the selected pieces

    if( SDL_Flip( screen ) == -1 )return;

}


bool is_castle(int game[8][8],int turn,int next_move[])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];
    bool flag=false ;

    if(game[source_x][source_y]==6)             //check for white king
    {
        if(source_y-destination_y==2)           //left side castle
        {
            game[7][0]=0;
            game[7][3]=2;
            game[7][4]=0;
            game[7][2]=6;
            flag=true;
        }
        if(destination_y-source_y==2)           //right side castle
        {
            game[7][7]=0;
            game[7][5]=2;
            game[7][4]=0;
            game[7][6]=6;
            flag=true;
        }
        wlcastle=false;
        wrcastle=false;

        wkx=destination_x;                      //updating the white king position
        wky=destination_y;
    }
    else if(game[source_x][source_y]==-6)         //check for black king
    {
        if(source_y-destination_y==2)            //left side castle
        {
            game[0][0]=0;
            game[0][3]=-2;
            game[0][4]=0;
            game[0][2]=-6;
            flag=true;
        }
        if(destination_y-source_y==2)            //right side castle
        {
            game[0][7]=0;
            game[0][5]=-2;
            game[0][4]=0;
            game[0][6]=-6;
            flag=true;
        }
        blcastle=false;
        brcastle=false;

        bkx=destination_x;                      //updating the white king position
        bky=destination_y;
    }

    if(source_x==0&&source_y==0)blcastle=false;                 // check if the move is a rook move
    if(source_x==0&&source_y==7)brcastle=false;
    if(source_x==7&&source_y==0)wlcastle=false;
    if(source_x==7&&source_y==7)wrcastle=false;

    return flag;
}


bool AI_promotion(int game[8][8],int turn,int next_move[])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    if(game[source_x][source_y]==turn*1)                        //check for pawn
    {
        if(destination_x==0||destination_x==7)                  //check for last row
        {
            game[source_x][source_y]=0;
            game[destination_x][destination_y]=next_move[4];     //promotes the pawn

            return true;
        }
    }
    return false;
}


bool human_promotion(int game[8][8],int turn,int next_move[])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];
    bool leave=false;

    if(game[source_x][source_y]==turn*1)                        //check for pawn
    {
        if(destination_x==0||destination_x==7)                  //check for move in last rows
        {

            apply_surface( 640, 0, back, screen );
            apply_surface( 640, 100, choose, screen );          //give options for promotion
            apply_surface( 640, 150, rook, screen );
            apply_surface( 640, 200, knight, screen );
            apply_surface( 640, 250, bishop, screen );
            apply_surface( 640, 300, queen, screen );

            //apply_surface(x,y,select,screen);

            game[source_x][source_y]=0;                         //delete the pawn

            if( SDL_Flip( screen ) == -1 )return -2;

            while(!leave)
            {
                if( SDL_PollEvent( &event ) )
                {
                    if( event.type == SDL_KEYDOWN )
                    {
                        switch( event.key.keysym.sym )
                        {

                            case SDLK_a:
                                game[destination_x][destination_y]=turn*2;          //promotes pawn
                                leave=true;
                                break;
                            case SDLK_b:
                                game[destination_x][destination_y]=turn*3;
                                leave=true;
                                break;
                            case SDLK_c:
                                game[destination_x][destination_y]=turn*4;
                                leave=true;
                                break;
                            case SDLK_d:
                                game[destination_x][destination_y]=turn*5;
                                leave=true;
                                break;
                            default :
                                break;
                        }
                    }
                }
            }
        }
    }
    return leave;
}


bool is_enpasant(int game[8][8],int turn,int next_move[5])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    if(game[source_x][source_y]==turn*1)                                        //check for pawn
    {
        if(game[destination_x][destination_y]==0&&abs(destination_y-source_y)==1) //check if destination is empty and the move is in side coloumn
        {
            game[destination_x][destination_y]=turn*1;
            game[source_x][source_y]=0;

            if(destination_y-source_y==1)               //right side enpasant
                game[source_x][source_y+1]=0;
            else                                        //left side enpasant
                game[source_x][source_y-1]=0;
            return true;
        }
    }
    return false;
}


void normal_move(int game[8][8],int turn,int next_move[5])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    game[destination_x][destination_y]=game[source_x][source_y];
    game[source_x][source_y]=0;

}


void update_game(int game[8][8],int turn,int next_move[5])
{






    if(!is_castle(game,turn,next_move))
        if(!is_enpasant(game,turn,next_move))
            if(!(turn!=AI_turn&&human_promotion(game,turn,next_move)))
                if(!(turn==AI_turn&&AI_promotion(game,turn,next_move)))
                    normal_move(game,turn,next_move);

    copy_prev_move(prev_move); // Copying previous move to use in enpasant
    print_game();            // Used for debugging


}


void check_everything(int next_move[])
{

    turn=-turn;   //Change the turn
    menu(turn);

    //check if king is in check
    int king_pos[2]={-16};
    bool kcheck=false;
    bool smate=false;
    bool draw=false;

    search_piece(0,0,7,7,turn*6,king_pos,game);
    if(position_check(king_pos[0],king_pos[1],turn,game))
    {
        kcheck=true;

        if(turn==1)
            apply_surface( 650, 450, whi, screen );
        else
            apply_surface( 650, 450, bla, screen );

        apply_surface( 650, 480, attack, screen );
        if( SDL_Flip( screen ) == -1 )return;
    }

    if(stale_mate(turn,game))
    {
        smate= true;
        menu();
        apply_surface( 650, 450, thatsdraw, screen );
        apply_surface( 650, 480, stalem, screen );
        if( SDL_Flip( screen ) == -1 )return;
    }

    if(kcheck&&smate)   // for checkmate
    {
        menu(-turn);
        apply_surface( 650, 480, checkm, screen );
        if( SDL_Flip( screen ) == -1 )return;

    }



    else if(move_50_rule(next_move[2],next_move[3]))
    {
        draw=true;
        menu();
        apply_surface( 650, 450, thatsdraw, screen );
        apply_surface( 650, 480, move_50, screen );
        if( SDL_Flip( screen ) == -1 )return;

    }

    else if(insuffice())
    {
        draw=true;
        menu();
        apply_surface( 650, 450, thatsdraw, screen );
        apply_surface( 650, 480, insuff, screen );
        if( SDL_Flip( screen ) == -1 )return;
    }

    if (smate||draw)
    {
        end_game();
        return;
    }


    if( SDL_Flip( screen ) == -1 )return;

}


bool move_50_rule(int x,int y)
{
    /* Counting the number of moves when no pawn is moved and no caputre is made (50 move rule) */

    if(move_count==100)
       return true;

    if(game[x][y]!=1&&game[x][y]!=-1&&!capture())move_count++;
    else move_count=0;

    return false;

}


bool check_mate(int turn)
{
    if(turn==1)return (position_check(wkx,wky,1,game)&&stale_mate(turn,game));
    else return (position_check(bkx,bky,-1,game)&&stale_mate(turn,game));

}


bool stale_mate(int turn,int game[8][8])
{

    int moves[60];

    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(turn==1&&game[i][j]<=0)continue;
            if(turn==-1&&game[i][j]>=0)continue;
            if(game[i][j]!=0)
            {
                if(gen_moves(game,i,j,moves)>1)
                {
                        return false;
                }

            }
        }
    }
    return true;
}


void write_moves_to_file(int turn,int next_move[])
{
    file<<endl;
    if(turn==1)
    {
        file<<"White : "<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<endl;
    }
    else
    {
        file<<"Black : "<<next_move[0]<<","<<next_move[1]<<"-->"<<next_move[2]<<","<<next_move[3]<<endl;
    }
}


void copy_prev_move(int prev_move[])
{
    for(int i=0;i<4;i++)
        prev_move[i]=next_move[i];

}


int filter_moves(int game[8][8],int moves[],int pos,int turn)
{

    if(pos<=1)return pos;

    int x=moves[0];
    int y=moves[1];
    int dx,dy;
    int source_piece=game[x][y];
    int dest_piece;
    int wkx,wky,bkx,bky;
    int king_pos[2]={-15};
    bool king=false;

    if(source_piece==6||source_piece==-6)king=true;

    for(int i=1;i<pos;i++)
    {
        dx=moves[i*2],dy=moves[i*2+1];
        dest_piece=game[moves[i*2]][moves[i*2+1]];
        game[moves[i*2]][moves[i*2+1]]=source_piece;
        game[x][y]=0;

        if(!king)
        {
            search_piece(0,0,7,7,turn*6,king_pos,game);
            //file<<"king : ";
            //file<<king_pos[0]<<","<<king_pos[1]<<endl;

            if(position_check(king_pos[0],king_pos[1],turn,game))
            {
                moves[i*2]=moves[pos*2-2];
                moves[i*2+1]=moves[pos*2-1];
                pos--;
                i--;
                                   //Remove MOve
            }

        }

        else
        {
            if(turn==1)
            {
                wkx=moves[i*2],wky=moves[i*2+1];

                if(position_check(wkx,wky,1,game))
                {
                    moves[i*2]=moves[pos*2-2];
                    moves[i*2+1]=moves[pos*2-1];
                    pos--;
                    i--;                      //Remove MOve

                }
            }
            else
            {
                bkx=moves[i*2],bky=moves[i*2+1];

                if(position_check(bkx,bky,-1,game))
                {
                    moves[i*2]=moves[pos*2-2];
                    moves[i*2+1]=moves[pos*2-1];
                    pos--;
                    i--;                        //Remove MOve

                }
            }


        }
        game[x][y]=source_piece;
        game[dx][dy]=dest_piece;

    }

    return pos;
}


void print_game()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(game[i][j]<0)
                file<<game[i][j]<<" ";
            else
                file<<" "<<game[i][j]<<" ";

        }
        file<<endl;
    }
}


void end_game()
{
    bool leave= false;
    while(!leave)
    {
         if( event.type == SDL_QUIT )
         {
             quit=true;
             break;
         }
         if( SDL_PollEvent( &event ) )
         {
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_r:
                        start_and_initialize();
                        leave=true;
                        break;
                    default : continue;

                }
            }
         }
     }
}















void AI_func(int game[8][8],int AI_turn,int next_move[10],int prev_move[10])
{
    int depth=3;
    int alpha=50000;
    //int check_further=0;
    //int top_n=15;

    //AI_wirte_move_to_file(prev_move);
    //AI_intialize_next_move(next_move);

    alpha=AI_depth(game,AI_turn,depth,alpha,next_move);

   // AI_wirte_move_to_file(next_move);
}


int AI_depth(int game[8][8],int AI_turn,int depth,int alpha,int next_move[10])
{
    int game_copy[8][8];
    int next_next_move[10];
    int top_move[10];
    int score;
    int moves[100];
    int num;

    AI_initialize_moves(moves);
    AI_intialize_next_move(next_next_move);
    AI_intialize_next_move(next_move);
    AI_initialize_top_move(top_move,depth);
    AI_copy_game(game,game_copy);

    if(depth==0)
    {
        if(AI_turn==1)                                  //for white
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]>0)
                    {
                        num=AI_gen_moves(game_copy,x,y,AI_turn,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        next_move[4]=game_copy[x][y];           // the value of source piece

                        if(next_move[0]==1)
                            next_move[4]=5;

                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];
                            next_move[5]=game_copy[next_move[2]][next_move[3]];            // the value of destination piece

                            AI_update_game(game_copy,AI_turn,next_move);

                            score=AI_move_score(game_copy,AI_turn);
                            next_move[6]=score;

                            if(score>alpha)                 //breaking condition
                            {
                                for(int q=0;q<10;q++)
                                {
                                    next_move[q]=top_move[q];
                                }
                                return score;
                            }

                            AI_update_top_move(top_move,depth,next_move);

                            AI_copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
        else if(AI_turn==-1)                                //for black
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]<0)
                    {
                        num=AI_gen_moves(game_copy,x,y,AI_turn,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        next_move[4]=game_copy[x][y];           // the value of source piece

                        if(next_move[0]==6)
                            next_move[4]=-5;

                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];
                            next_move[5]=game_copy[next_move[2]][next_move[3]];            // the value of destination piece

                            AI_update_game(game_copy,AI_turn,next_next_move);

                            score=AI_move_score(game_copy,AI_turn);
                            next_move[6]=score;

                            if(score>alpha)                 //breaking condition
                            {
                                for(int q=0;q<10;q++)
                                {
                                    next_move[q]=top_move[q];
                                }
                                return score;
                            }

                            AI_update_top_move(top_move,depth,next_move);

                            AI_copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
    }
    else                        // depth not 0
    {
        if(AI_turn==1)                                  //for white
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]>0)
                    {
                        num=AI_gen_moves(game_copy,x,y,AI_turn,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        next_move[4]=game_copy[x][y];           // the value of source piece

                        if(next_move[0]==1)
                            next_move[4]=5;

                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];
                            next_move[5]=game_copy[next_move[2]][next_move[3]];            // the value of destination piece

                            AI_update_game(game_copy,AI_turn,next_move);

                            score=AI_depth(game_copy,-AI_turn,depth-1,alpha,next_next_move);
                            next_move[6]=score;

                            if(score<alpha)
                            {
                                alpha=score;
                            }

                            AI_update_top_move(top_move,depth,next_move);

                            AI_copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
        else if(AI_turn==-1)                                //for black
        {
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    if(game_copy[x][y]<0)
                    {
                        num=AI_gen_moves(game_copy,x,y,AI_turn,moves);
                        next_move[0]=moves[0];
                        next_move[1]=moves[1];
                        next_move[4]=game_copy[x][y];           // the value of source piece

                        if(next_move[0]==6)
                            next_move[4]=-5;

                        for(int i=1;i<num;i++)
                        {
                            next_move[2]=moves[i*2];
                            next_move[3]=moves[i*2+1];
                            next_move[5]=game_copy[next_move[2]][next_move[3]];            // the value of destination piece

                            AI_update_game(game_copy,AI_turn,next_move);

                            score=AI_depth(game_copy,-AI_turn,depth-1,alpha,next_next_move);
                            next_move[6]=score;

                            if(score<alpha)
                            {
                                alpha=score;
                            }

                            AI_update_top_move(top_move,depth,next_move);

                            AI_copy_game(game,game_copy);
                        }
                    }
                }
            }
        }
    }
    for(int i=0;i<10;i++)
    {
        next_move[i]=top_move[i];
    }
    return next_move[6];
}


void AI_initialize_top_move(int a[10],int depth)         //200 rows and 10 columns...... each row will contain a move
{
    if(depth%2==0)
    {
        for(int j=0;j<10;j++)
        {
            a[j]=-50000;
        }
    }
    else
    {
        for(int j=0;j<10;j++)
        {
            a[j]=50000;
        }

    }
}

void AI_initialize_moves(int moves[])
{
    for(int i=0;i<100;i++)
    {
        moves[i]=0;
    }
}

void AI_update_top_move(int top_move[10],int depth,int next_move[10])
{
    if(depth%2==0)
    {
        if(next_move[6]>top_move[6])
        {
            for(int i=0;i<10;i++)
            {
                top_move[i]=next_move[i];
            }
        }
    }
    else
    {
        if(next_move[6]<top_move[6])
        {
            for(int i=0;i<10;i++)
            {
                top_move[i]=next_move[i];
            }
        }
    }
}



void AI_copy_game(int a[8][8],int b[8][8])         // copy the values of matrix a into matrix b
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            b[i][j]=a[i][j];
}


void AI_intialize_next_move(int next_move[10])
{
    for(int i=0;i<10;i++)
        next_move[i]=-50000;
}


bool AI_detect(int game[8][8],int x,int y,int turn)
{
    if(y>7||y<0)return false;
    if(x>7||x<0)return false;


    if(turn==0)
        if(game[x][y]==0)return false;

    if(turn==1)
        if(game[x][y]<=0)return false;

    if(turn==-1)
        if(game[x][y]>=0)return false;

    return true;
}


bool AI_enemy(int game[8][8],int x,int y,int color)
{
    if(y>7||y<0)return false;
    if(x>7||x<0)return false;

    if(color>0)
    {
        if(game[x][y]>0)return true;
    }
    else if(color<0)
        if(game[x][y]<0)return true;
    return false;
}


void AI_update_game(int game[8][8],int turn,int next_move[])
{
    //if(!AI_is_castle(game,turn,next_move))
       // if(!AI_is_enpasant(game,turn,next_move))
            AI_normal_move(game,turn,next_move);
}




int AI_gen_moves(int game[8][8],int x,int y,int turn,int moves[])
{
    int count=0;
    int backup[8][8];

    if(game[x][y]==0)
        return 0;

    if(game[x][y]==1*turn)count=AI_m_pawn(game,x,y,turn,moves);

    else if(game[x][y]==2*turn)count=AI_m_rook(game,x,y,turn,moves);

    else if(game[x][y]==3*turn)count=AI_m_knight(game,x,y,turn,moves);

    else if(game[x][y]==4*turn)count=AI_m_bishop(game,x,y,turn,moves);

    else if(game[x][y]==5*turn)count=AI_m_queen(game,x,y,turn,moves);

    else if(game[x][y]==6*turn)count=AI_m_king(game,x,y,turn,moves);

    AI_copy_game(game,backup);
    count=AI_filter_moves(backup,moves,count,turn);

    return count;
}


int AI_m_pawn(int game[8][8],int x,int y,int turn,int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    //int temp[2];

    if(turn==1)
    {
        if(!AI_detect(game,x-1,y))
        {
            moves[count++]=x-1;
            moves[count++]=y;

            if(x==6)
            {
                if(!AI_detect(game,x-2,y))
                {
                    moves[count++]=x-2;
                    moves[count++]=y;
                }
            }
        }

        if(AI_enemy(game,x-1,y-1,-turn))
        {
            moves[count++]=x-1;
            moves[count++]=y-1;
        }

        if(AI_enemy(game,x-1,y+1,-turn))
        {
            moves[count++]=x-1;
            moves[count++]=y+1;
        }
/*
        if(m_enpasant(x,y,turn,temp))
        {
            moves[count++]=temp[0];
            moves[count++]=temp[1];
        }
*/
    }
    else if(turn==-1)
    {
        if(!AI_detect(game,x+1,y))
        {
            moves[count++]=x+1;
            moves[count++]=y;

            if(x==1)
            {
                if(!AI_detect(game,x+2,y))
                {
                    moves[count++]=x+2;
                    moves[count++]=y;
                }
            }
        }

        if(AI_enemy(game,x+1,y-1,-turn))
        {
            moves[count++]=x+1;
            moves[count++]=y-1;
        }

        if(AI_enemy(game,x+1,y+1,-turn))
        {
            moves[count++]=x+1;
            moves[count++]=y+1;
        }
        /*
        if(m_enpasant(x,y,turn,temp))
        {
            moves[count++]=temp[0];
            moves[count++]=temp[1];
        }
        */
    }
    return count/2;
}



int AI_m_rook (int game[8][8],int x,int y,int turn,int moves[])
{

    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;
        if(temp1>7)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;
        if(temp1<0)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp2--;
        if(temp2<0)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp2++;
        if(temp2>7)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }
    return count/2;
}


int AI_m_knight (int game[8][8],int x,int y,int turn,int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=y-2;
    if(temp1>=0)
    {
        temp2=x-1;
        if(temp2>=0)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
        temp2=x+1;
        if(temp2<=7)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
    }

    temp1=y-1;
    if(temp1>=0)
    {
        temp2=x-2;
        if(temp2>=0)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
        temp2=x+2;
        if(temp2<=7)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
    }

    temp1=y+1;
    if(temp1<=7)
    {
        temp2=x-2;
        if(temp2>=0)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
        temp2=x+2;
        if(temp2<=7)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
    }

    temp1=y+2;
    if(temp1<=7)
    {
        temp2=x-1;
        if(temp2>=0)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
        temp2=x+1;
        if(temp2<=7)
        {
            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    moves[count++]=temp2;
                    moves[count++]=temp1;
                }
            }
        }
    }
    return count/2;
}


int AI_m_bishop (int game[8][8],int x,int y,int turn,int moves[])
{
    int count=2;
    moves[0]=x;
    moves[1]=y;

    int temp1,temp2;

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;temp2++;
        if(temp1>7||temp2>7)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;temp2--;
        if(temp1<0||temp2<0)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1++;temp2--;
        if(temp1>7||temp2<0)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    temp1=x;temp2=y;
    for(int i=0;i<7;i++)
    {
        temp1--;temp2++;
        if(temp1<0||temp2>7)
            break;
        if(!AI_detect(game,temp1,temp2))
        {
            moves[count++]=temp1;
            moves[count++]=temp2;
        }
        else
        {
            if(AI_enemy(game,temp1,temp2,-turn))
            {
                moves[count++]=temp1;
                moves[count++]=temp2;
            }
            break;
        }
    }

    return count/2;
}


int AI_m_queen (int game[8][8],int x,int y,int turn,int moves[])
{
    int temp[60],temps=2;
    temp[0]=x;
    temp[1]=y;

    int temp1=AI_m_rook(game,x,y,turn,moves);
    temp1=temp1*2;
    for(int i=2;i<temp1;i++)
        temp[temps++]=moves[i];

    temp1=AI_m_bishop(game,x,y,turn,moves);
    temp1*=2;
    for(int i=2;i<temp1;i++)
        temp[temps++]=moves[i];

    for(int i=0;i<temps;i++)
        moves[i]=temp[i];

    return temps/2;
}


int AI_m_king (int game[8][8],int x,int y,int turn,int moves[])
{
    int temp[60];
    int temps=2;
    temp[0]=x;
    temp[1]=y;

    int temp1,temp2;
    for(int i=0;i<3;i++)
    {
        temp1=y-1+i;
        if(temp1<0)continue;
        else if(temp1>7)break;
        for(int j=0;j<3;j++)
        {
            temp2=x-1+j;
            if(temp1==y&&temp2==x)continue;
            if(temp2<0)continue;
            else if(temp2>7)break;

            if(turn<0)
            {
                if(game[temp2][temp1]>=0)
                {
                    temp[temps++]=temp2;
                    temp[temps++]=temp1;
                }
            }
            else if(turn>0)
            {
                if(game[temp2][temp1]<=0)
                {
                    temp[temps++]=temp2;
                    temp[temps++]=temp1;
                }
            }
        }
    }

   // temps=AI_m_castle(temp,temps);  //Checks if castling moves are possible

    for(int i=0;i<temps;i++)
        moves[i]=temp[i];

    return temps/2;
}






int AI_filter_moves(int game[8][8],int moves[],int pos,int turn)
{
    int x=moves[0];
    int y=moves[1];
    int dx,dy;
    int source_piece=game[x][y];
    int dest_piece;
    int wkx,wky,bkx,bky;
    int king_pos[2];
    bool king=false;

    if(source_piece==6||source_piece==-6)king=true;

    for(int i=1;i<pos;i++)
    {
        dx=moves[i*2],dy=moves[i*2+1];
        dest_piece=game[moves[i*2]][moves[i*2+1]];
        game[moves[i*2]][moves[i*2+1]]=source_piece;
        game[x][y]=0;

        if(!king)
        {
            AI_search_piece(0,0,7,7,turn*6,king_pos,game);

            if(AI_position_check(game,king_pos[0],king_pos[1],turn))
            {
                moves[i*2]=moves[pos*2-2];
                moves[i*2+1]=moves[pos*2-1];
                pos--;
                i--;
                                   //Remove MOve
            }

        }

        else
        {
            if(turn==1)
            {
                wkx=moves[i*2],wky=moves[i*2+1];

                if(AI_position_check(game,wkx,wky,1))
                {
                    moves[i*2]=moves[pos*2-2];
                    moves[i*2+1]=moves[pos*2-1];
                    pos--;
                    i--;                      //Remove MOve

                }
            }
            else
            {
                bkx=moves[i*2],bky=moves[i*2+1];

                if(AI_position_check(game,bkx,bky,-1))
                {
                    moves[i*2]=moves[pos*2-2];
                    moves[i*2+1]=moves[pos*2-1];
                    pos--;
                    i--;                        //Remove MOve

                }
            }


        }
        game[x][y]=source_piece;
        game[dx][dy]=dest_piece;

    }

    return pos;
}



bool AI_position_check(int game[8][8],int x,int y, int turn)
{
    int temp,temp1,temp2;
    int moves[60];

    temp=AI_m_rook(game,x,y,turn,moves);


    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*2)||game[temp1][temp2]==(-1*turn*5))
            return true;
    }

    temp=AI_m_knight(game,x,y,turn,moves);
    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*3))
            return true;
    }

    temp=AI_m_bishop(game,x,y,turn,moves);
    for(int i=1;i<temp;i++)
    {
        temp1=moves[i*2];
        temp2=moves[i*2+1];
        if(game[temp1][temp2]==(-1*turn*4)||game[temp1][temp2]==(-1*turn*5))
            return true;
    }

    for(int i=0;i<3;i++)
    {
        temp1=x-1+i;
        if(temp1<0)continue;
        else if(temp1>7)break;
        for(int j=0;j<3;j++)
        {
            temp2=y-1+j;
            if(temp2<0)continue;
            else if(temp2>7)break;

            if(game[temp1][temp2]==(-1*turn*6))
                return true;
        }
    }

    if(turn>0)
    {
        temp1=x-1;
        temp2=y-1;
        if(temp1>=0&&temp2>=0)
            if(game[temp1][temp2]==-1)
                return true;

        temp1=x-1;
        temp2=y+1;
        if(temp1>=0&&temp2<=7)
            if(game[temp1][temp2]==-1)
                return true;
    }
    else
    {
        temp1=x+1;
        temp2=y+1;
        if(temp1<=7&&temp2<=7)
            if(game[temp1][temp2]==1)
                return true;

        temp1=x+1;
        temp2=y-1;
        if(temp1<=7&&temp2>=0)
            if(game[temp1][temp2]==1)
                return true;
    }

    return false;
}



bool AI_is_castle(int game[8][8],int turn,int next_move[5])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    if(source_x-destination_x!=0)
        return false;

    if(game[source_x][source_y]==6)             //check for white king
    {
        if(source_y-destination_y==2)           //left side castle
        {
            game[7][0]=0;
            game[7][3]=2;
            game[7][4]=0;
            game[7][2]=6;
            return true;
        }
        if(destination_y-source_y==2)           //right side castle
        {
            game[7][7]=0;
            game[7][5]=2;
            game[7][4]=0;
            game[7][6]=6;
            return true;
        }
    }
    else if(game[source_x][source_y]==-6)         //check for black king
    {
        if(source_y-destination_y==2)            //left side castle
        {
            game[0][0]=0;
            game[0][3]=-2;
            game[0][4]=0;
            game[0][2]=-6;
            return true;
        }
        if(destination_y-source_y==2)            //right side castle
        {
            game[0][7]=0;
            game[0][5]=-2;
            game[0][4]=0;
            game[0][6]=-6;
            return true;
        }
    }
    return false;
}



bool AI_is_enpasant(int game[8][8],int turn,int next_move[10])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    if(game[source_x][source_y]==turn*1)                                        //check for pawn
    {
        if(game[destination_x][destination_y]==0&&AI_abs(destination_y-source_y)==1) //check if destination is empty and the move is in side coloumn
        {
            game[destination_x][destination_y]=turn*1;
            game[source_x][source_y]=0;

            if(destination_y-source_y==1)               //right side enpasant
                game[source_x][source_y+1]=0;
            else                                        //left side enpasant
                game[source_x][source_y-1]=0;
            return true;
        }
    }
    return false;
}

int AI_abs(int n)
{
    if(n<0)
        n=-n;
    return n;
}


void AI_normal_move(int game[8][8],int turn,int next_move[10])
{
    int source_x=next_move[0],source_y=next_move[1],destination_x=next_move[2],destination_y=next_move[3];

    game[destination_x][destination_y]=next_move[4];
    game[source_x][source_y]=0;
}








bool AI_search_piece(int srow,int scol,int erow,int ecol,int to_find,int store[2],int game[8][8])
{


    for(int i=srow;i<=erow;i++)
    {
        for(int j=scol;j<=ecol;j++)
        {

            if(game[i][j]==to_find)
            {
                store[0]=i;
                store[1]=j;
                return true;
            }

        }
    }



    return false;
}


void AI_piece_square_values()
{

    AI_read_file(vPawnM);
    AI_read_file(vKnightM);
    AI_read_file(vBishopM);
    AI_read_file(vRookM);
    AI_read_file(vQueenM);
    AI_read_file(vKingM);

    AI_read_file(vPawnE);
    AI_read_file(vKnightE);
    AI_read_file(vBishopE);
    AI_read_file(vRookE);
    AI_read_file(vQueenE);
    AI_read_file(vKingE);

}


void AI_read_file(int store[8][8])
{

    string line;

    getline(obj,line);
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            obj>>store[i][j];
        }
    }
    obj.get();
}











