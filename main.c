#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

//for undo and redo
char all_boards[8][8][1000];
char all_dead_1_array[15][1000];
char all_dead_2_array[15][1000];
int all_dead1[1000];
int all_dead2[1000];
int counter,max_counter;

//for save and load
int g_flag;
char turn;

//to end game
int end_flag;

//change if checkmate or stalemate to accept only end or undo
int c_s_flag;

int dead1,dead2;
char dead_1_array[15],dead_2_array[15];

void fillboard(char board[8][8]);
void printboard(char board[8][8]);
int convert(char c_1);
int if_inrange(int n_1,int n_2,char c_1,char c_2);

void scan_valid_move1(char board[8][8]);
void scan_valid_move2(char board[8][8]);

int pawn_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int rook_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int knight_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int bishop_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int queen_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int king_valid_move1(char board[8][8],int r1,int c1,int r2,int c2);
int white_correct_move(char board[8][8],int r1,int c1,int r2,int c2);

int pawn_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int rook_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int knight_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int bishop_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int queen_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int king_valid_move2(char board[8][8],int r1,int c1,int r2,int c2);
int black_correct_move(char board[8][8],int r1,int c1,int r2,int c2);

int is_under_attack_by_black(char board[8][8],int r,int c);
int is_under_attack_by_white(char board[8][8],int r,int c);
int is_white_king_safe(char board[8][8],int r1,int c1,int r2,int c2);
int is_black_king_safe(char board[8][8],int r1,int c1,int r2,int c2);

int is_white_valid_move(char board[8][8],int r1,int c1,int r2,int c2);
int is_black_valid_move(char board[8][8],int r1,int c1,int r2,int c2);

int is_black_king_checked(char board[8][8]);
int is_white_king_checked(char board[8][8]);

int one_valid_move_for_black(char board[8][8]);
int one_valid_move_for_white(char board[8][8]);

void save_to_all_boards(char board[8][8]);
void copy_from_all_boards(char board[8][8]);

void insertion_white(char w);
void insertion_black(char w);
int check_white_promotion_char(char board[8][8],int r1,int c1,int r2,int c2,char c_3);
int check_black_promotion_char(char board[8][8],int r1,int c1,int r2,int c2,char c_3);
void copy_board(char board[8][8],char fboard[8][8]);
int is_empty0(char w);
int is_black(char w);
int is_white(char w);
int main()
{
    printf("\t\t\tCHESS GAME\n");
    char board[8][8];
    counter=-1,max_counter=-1,g_flag=1,end_flag=1,c_s_flag=1;
    //load or a new game
    printf("To Start A New Game type 1\nTo Load The Saved Game type 2\n");
    do
    {
        //array to scan input
        char scan[500000];
        for(int i=0; i<10; i++)
            scan[i]=0;
        fgets(scan,500000,stdin);
        if((scan[0]=='2')&&(scan[1]=='\n'))
        {
            FILE* f=fopen("game1","r");
            if(f == NULL)
            {
                system("cls");
                printf("\t\t\tCHESS GAME\n");
                printf("you do not have a saved game type 1 to start a new game\n");
                continue;
            }
            fread(board,sizeof(char),64,f);
            fread(dead_1_array,sizeof(char),15,f);
            fread(dead_2_array,sizeof(char),15,f);
            fread(&dead1,sizeof(int),1,f);
            fread(&dead2,sizeof(int),1,f);
            // get whose turn
            turn=fgetc(f);
            fclose(f);
        }
        else if(((scan[0]=='1')||(scan[0]=='1'))&&(scan[1]=='\n'))
        {
            turn='W';
            dead1=0,dead2=0;
            for (int i=0; i<15; i++)
            {
                dead_1_array[i]='\0';
                dead_2_array[i]='\0';
            }
            fillboard(board);
        }
        else
        {
            system("cls");
            printf("\t\t\tCHESS GAME\n");
            printf("To Start A New Game type 1\nTo Load The Saved Game type 2\n");
            continue;
        }
        break;
    }
    while(1);
    //save basic board and print it
    save_to_all_boards(board);
    printboard(board);
    while(1)
    {
        if(turn=='W')
        {
            scan_valid_move1(board);
            // check if user ended game
            if(!end_flag)
            {
                printf("game over\n");
                break;
            }
        }
        if(turn=='B')
        {
            scan_valid_move2(board);
            // check if user ended game
            if(!end_flag)
            {
                printf("game over\n");
                break;
            }
        }
    }
    return 0;
}
void fillboard(char board[8][8])
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            board[i][j]=' ';
    //white and black
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<8; j++)
        {
            if ((2+i+j)%2==0)
                board[2+i][j]=178;
            else
                board[2+i][j]=176;
        }
    }
    //pawns
    for(int i=0; i<8; i++)
    {
        board[1][i]='P';
    }
    for(int i=0; i<8; i++)
    {
        board[6][i]='p';
    }
    //rest pieces
    board[0][0]='R';
    board[0][1]='N';
    board[0][2]='B';
    board[0][3]='Q';
    board[0][4]='K';
    board[0][5]='B';
    board[0][6]='N';
    board[0][7]='R';
    board[7][0]='r';
    board[7][1]='n';
    board[7][2]='b';
    board[7][3]='q';
    board[7][4]='k';
    board[7][5]='b';
    board[7][6]='n';
    board[7][7]='r';
}
void printboard(char board[8][8])
{
    system("cls");
    printf("       A    B    C    D    E    F    G    H      \n\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("8    %c%c%c%c%c",178,178,board[0][0],178,178);
        printf("%c%c%c%c%c",176,176,board[0][1],176,176);
        printf("%c%c%c%c%c",178,178,board[0][2],178,178);
        printf("%c%c%c%c%c",176,176,board[0][3],176,176);
        printf("%c%c%c%c%c",178,178,board[0][4],178,178);
        printf("%c%c%c%c%c",176,176,board[0][5],176,176);
        printf("%c%c%c%c%c",178,178,board[0][6],178,178);
        printf("%c%c%c%c%c    8",176,176,board[0][7],176,176);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("7    %c%c%c%c%c",176,176,board[1][0],176,176);
        printf("%c%c%c%c%c",178,178,board[1][1],178,178);
        printf("%c%c%c%c%c",176,176,board[1][2],176,176);
        printf("%c%c%c%c%c",178,178,board[1][3],178,178);
        printf("%c%c%c%c%c",176,176,board[1][4],176,176);
        printf("%c%c%c%c%c",178,178,board[1][5],178,178);
        printf("%c%c%c%c%c",176,176,board[1][6],176,176);
        printf("%c%c%c%c%c    7",178,178,board[1][7],178,178);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("6    %c%c%c%c%c",178,178,board[2][0],178,178);
        printf("%c%c%c%c%c",176,176,board[2][1],176,176);
        printf("%c%c%c%c%c",178,178,board[2][2],178,178);
        printf("%c%c%c%c%c",176,176,board[2][3],176,176);
        printf("%c%c%c%c%c",178,178,board[2][4],178,178);
        printf("%c%c%c%c%c",176,176,board[2][5],176,176);
        printf("%c%c%c%c%c",178,178,board[2][6],178,178);
        printf("%c%c%c%c%c    6",176,176,board[2][7],176,176);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("5    %c%c%c%c%c",176,176,board[3][0],176,176);
        printf("%c%c%c%c%c",178,178,board[3][1],178,178);
        printf("%c%c%c%c%c",176,176,board[3][2],176,176);
        printf("%c%c%c%c%c",178,178,board[3][3],178,178);
        printf("%c%c%c%c%c",176,176,board[3][4],176,176);
        printf("%c%c%c%c%c",178,178,board[3][5],178,178);
        printf("%c%c%c%c%c",176,176,board[3][6],176,176);
        printf("%c%c%c%c%c    5",178,178,board[3][7],178,178);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("4    %c%c%c%c%c",178,178,board[4][0],178,178);
        printf("%c%c%c%c%c",176,176,board[4][1],176,176);
        printf("%c%c%c%c%c",178,178,board[4][2],178,178);
        printf("%c%c%c%c%c",176,176,board[4][3],176,176);
        printf("%c%c%c%c%c",178,178,board[4][4],178,178);
        printf("%c%c%c%c%c",176,176,board[4][5],176,176);
        printf("%c%c%c%c%c",178,178,board[4][6],178,178);
        printf("%c%c%c%c%c    4",176,176,board[4][7],176,176);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("3    %c%c%c%c%c",176,176,board[5][0],176,176);
        printf("%c%c%c%c%c",178,178,board[5][1],178,178);
        printf("%c%c%c%c%c",176,176,board[5][2],176,176);
        printf("%c%c%c%c%c",178,178,board[5][3],178,178);
        printf("%c%c%c%c%c",176,176,board[5][4],176,176);
        printf("%c%c%c%c%c",178,178,board[5][5],178,178);
        printf("%c%c%c%c%c",176,176,board[5][6],176,176);
        printf("%c%c%c%c%c    3",178,178,board[5][7],178,178);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("2    %c%c%c%c%c",178,178,board[6][0],178,178);
        printf("%c%c%c%c%c",176,176,board[6][1],176,176);
        printf("%c%c%c%c%c",178,178,board[6][2],178,178);
        printf("%c%c%c%c%c",176,176,board[6][3],176,176);
        printf("%c%c%c%c%c",178,178,board[6][4],178,178);
        printf("%c%c%c%c%c",176,176,board[6][5],176,176);
        printf("%c%c%c%c%c",178,178,board[6][6],178,178);
        printf("%c%c%c%c%c    2",176,176,board[6][7],176,176);
        printf("\n");
        printf("     %c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
        printf("1    %c%c%c%c%c",176,176,board[7][0],176,176);
        printf("%c%c%c%c%c",178,178,board[7][1],178,178);
        printf("%c%c%c%c%c",176,176,board[7][2],176,176);
        printf("%c%c%c%c%c",178,178,board[7][3],178,178);
        printf("%c%c%c%c%c",176,176,board[7][4],176,176);
        printf("%c%c%c%c%c",178,178,board[7][5],178,178);
        printf("%c%c%c%c%c",176,176,board[7][6],176,176);
        printf("%c%c%c%c%c    1",178,178,board[7][7],178,178);
        printf("\n");
        printf("     %c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("%c%c%c%c%c",176,176,176,176,176);
        printf("%c%c%c%c%c",178,178,178,178,178);
        printf("\n");
    printf("\n       A    B    C    D    E    F    G    H      \n\n");
    if(dead1>0)
    {
        printf("White dead pieces : ");
        for(int i=0; i<dead1; i++)
            printf("%c ",dead_1_array[i]);
        printf("\n");
    }
    if(dead2>0)
    {
        printf("Black dead pieces : ");
        for(int i=0; i<dead2; i++)
            printf("%c ",dead_2_array[i]);
        printf("\n\n");
    }
}
void scan_valid_move1(char board[8][8])
{
    //scanning
    int n_1,n_2;
    char c_1,c_2,c_3;
    int r1,c1,r2,c2;
    int flag=1,g_flag=1;
    char scan[500000];
    do
    {
        //check if game ends or result to a check
        if(is_white_king_checked(board))
        {
            if(one_valid_move_for_white(board))
            {
                printf("white king is checked\n");
            }
            else
            {
                printf("\t\t\tBLACK WON\n");
                //only accept undo or end
                c_s_flag=0;
            }
        }
        else
        {
            if(!one_valid_move_for_white(board))
            {
                printf("\t\t\tSTALEMATE\n");
                //only accept undo or end
                c_s_flag=0;
            }
        }
        //scan user input
        printf("White Turn\nU : for undo\nR : for redo\nS : for save\nE : for end\n");
        if(c_s_flag)
            printf("enter your move: ");
        else
            printf("you can undo or end game : ");
        //assign first 10 places to zero
        for(int i=0; i<10; i++)
            scan[i]=0;
        fgets(scan,500000,stdin);
        //re assign flag to 1
        flag=1;
        if((scan[4]=='\n' || scan[5]=='\n')&&c_s_flag)
        {
            c_1=scan[0],c_2=scan[2],c_3=scan[4];
            n_1=scan[1]-'0',n_2=scan[3]-'0';
            //check if in the board
            int flag0=if_inrange(n_1,n_2,c_1,c_2);
            if (!flag0)
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='u'||scan[0]=='U')&&scan[1]=='\n')
        {
            flag=0;
            if(counter>0)
            {
                c_s_flag=1;
                counter--;
                copy_from_all_boards(board);
            }
            else
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='r'||scan[0]=='R')&&scan[1]=='\n')
        {
            flag=0;
            if(max_counter>counter)
            {
                counter++;
                copy_from_all_boards(board);
            }
            else
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='s'||scan[0]=='S')&&scan[1]=='\n'&&c_s_flag)
        {
            flag=0;
            FILE* f=fopen("game1","w");
            if(f == NULL)
                {
                    printboard(board);
                    continue;
                }
            fwrite(board,sizeof(char),64,f);
            fwrite(dead_1_array,sizeof(char),15,f);
            fwrite(dead_2_array,sizeof(char),15,f);
            fwrite(&dead1,sizeof(int),1,f);
            fwrite(&dead2,sizeof(int),1,f);
            // store who made save
            fputc('W',f);
            fclose(f);
            g_flag=0;
        }
        else if((scan[0]=='e'||scan[0]=='E')&&scan[1]=='\n')
        {
            //do not make any move
            flag=0;
            // to end game
            end_flag=0;
        }
        else
            {
                    printboard(board);
                    continue;
            }
        if(flag)
        {
            // converting index
            r1=8-n_1;
            r2=8-n_2;
            c1=convert(c_1);
            c2=convert(c_2);
            // check promotion char
            if(!check_white_promotion_char(board,r1,c1,r2,c2,c_3))
                {
                    printboard(board);
                    continue;
                }
            //check if valid
            if(!is_white_valid_move(board,r1,c1,r2,c2))
                {
                    printboard(board);
                    continue;
                }
        }
        break;
    }
    while(1);
    if(flag)
    {
        //dead pieces
        if(is_black(board[r2][c2]))
        {
            insertion_black(board[r2][c2]);
            dead2++;
        }
        //moving the character
        board[r2][c2]=board[r1][c1];
        //returning the place to black or white
        if ((r1+c1)%2==0)
            board[r1][c1]=178;
        else
            board[r1][c1]=176;
        // make promotion
        if (board[r2][c2]=='p' && r2==0)
            board[r2][c2]=tolower(c_3);
        save_to_all_boards(board);
    }
    // do not change turn if game is saved or ended
    if(g_flag&&end_flag)
    {
        turn='B';
    }
    //print board again if game not ended
    if(end_flag)
        printboard(board);
    if(!g_flag)
        printf("Game saved\n");
}
void scan_valid_move2(char board[8][8])
{
    //scanning
    int n_1,n_2;
    char c_1,c_2,c_3;
    int r1,c1,r2,c2;
    int flag=1,g_flag=1;
    char scan[500000];
    do
    {
        //check if game ends or result to a check
        if(is_black_king_checked(board))
        {
            if(one_valid_move_for_black(board))
            {
                printf("black king is checked\n");
            }
            else
            {
                printf("\t\t\tWHITE WON\n");
                //only accept undo or end
                c_s_flag=0;
            }
        }
        else
        {
            if(!one_valid_move_for_black(board))
            {
                printf("\t\t\tSTALEMATE\n");
                //only accept undo or end
                c_s_flag=0;
            }
        }
        //scan user input
        printf("Black Turn\nU : for undo\nR : for redo\nS : for save\nE : for end\n");
        if(c_s_flag)
            printf("enter your move: ");
        else
            printf("you can undo or end game : ");
        //assign first 10 places to zero
        for(int i=0; i<10; i++)
            scan[i]=0;
        fgets(scan,500000,stdin);
        //re assign flag to 1
        flag=1;
        if((scan[4]=='\n' || scan[5]=='\n')&&c_s_flag)
        {
            c_1=scan[0],c_2=scan[2],c_3=scan[4];
            n_1=scan[1]-'0',n_2=scan[3]-'0';
            //check if in the board
            int flag0=if_inrange(n_1,n_2,c_1,c_2);
            if (!flag0)
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='u'||scan[0]=='U')&&scan[1]=='\n')
        {
            flag=0;
            if(counter>0)
            {
                c_s_flag=1;
                counter--;
                copy_from_all_boards(board);
            }
            else
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='r'||scan[0]=='R')&&scan[1]=='\n')
        {
            flag=0;
            if(max_counter>counter)
            {
                counter++;
                copy_from_all_boards(board);
            }
            else
                {
                    printboard(board);
                    continue;
                }
        }
        else if((scan[0]=='s'||scan[0]=='S')&&scan[1]=='\n'&&c_s_flag)
        {
            flag=0;
            FILE* f=fopen("game1","w");
            if(f == NULL)
                {
                    printboard(board);
                    continue;
                }
            fwrite(board,sizeof(char),64,f);
            fwrite(dead_1_array,sizeof(char),15,f);
            fwrite(dead_2_array,sizeof(char),15,f);
            fwrite(&dead1,sizeof(int),1,f);
            fwrite(&dead2,sizeof(int),1,f);
            // store who made save
            fputc('B',f);
            fclose(f);
            g_flag=0;
        }
        else if((scan[0]=='e'||scan[0]=='E')&&scan[1]=='\n')
        {
            //do not make any move
            flag=0;
            // to end game
            end_flag=0;
        }
        else
            {
                    printboard(board);
                    continue;
            }
        if(flag)
        {
            // converting index
            r1=8-n_1;
            r2=8-n_2;
            c1=convert(c_1);
            c2=convert(c_2);
            // check promotion char
            if(!check_black_promotion_char(board,r1,c1,r2,c2,c_3))
                {
                    printboard(board);
                    continue;
                }
            //check if valid
            if(!is_black_valid_move(board,r1,c1,r2,c2))
                {
                    printboard(board);
                    continue;
                }
        }
        break;
    }
    while(1);
    if(flag)
    {
        //dead pieces
        if(is_white(board[r2][c2]))
        {
            insertion_white(board[r2][c2]);
            dead1++;
        }
        //moving the character
        board[r2][c2]=board[r1][c1];
        //returning the place to black or white
        if ((r1+c1)%2==0)
            board[r1][c1]=178;
        else
            board[r1][c1]=176;
        // make promotion
        if (board[r2][c2]=='P' && r2==7)
            board[r2][c2]=toupper(c_3);
        save_to_all_boards(board);
    }
    // do not change turn if game is saved or ended
    if(g_flag&&end_flag)
    {
        turn='W';
    }
    //print board again if game not ended
    if(end_flag)
        printboard(board);
    if(!g_flag)
        printf("Game saved\n");
}
int if_inrange(int n_1,int n_2,char c_1,char c_2)
{
    if (n_1<1||n_1>8)
        return 0;
    if (n_2<1||n_2>8)
        return 0;
    if (c_1!='A'&&c_1!='B'&&c_1!='C'&&c_1!='D'&&c_1!='E'&&c_1!='F'&&c_1!='G'&&c_1!='H'&&c_1!='a'&&c_1!='b'&&c_1!='c'&&c_1!='d'&&c_1!='e'&&c_1!='f'&&c_1!='g'&&c_1!='h')
        return 0;
    if (c_2!='A'&&c_2!='B'&&c_2!='C'&&c_2!='D'&&c_2!='E'&&c_2!='F'&&c_2!='G'&&c_2!='H'&&c_2!='a'&&c_2!='b'&&c_2!='c'&&c_2!='d'&&c_2!='e'&&c_2!='f'&&c_2!='g'&&c_2!='h')
        return 0;
    return 1;
}
int convert(char c_1)
{
    //turning the input of the user to the index of the 2D array
    int i;
    switch(c_1)
    {
    case 'a':
    case 'A':
        i=0;
        break;
    case 'b':
    case 'B':
        i=1;
        break;
    case 'c':
    case 'C':
        i=2;
        break;
    case 'd':
    case 'D':
        i=3;
        break;
    case 'e':
    case 'E':
        i=4;
        break;
    case 'f':
    case 'F':
        i=5;
        break;
    case 'g':
    case 'G':
        i=6;
        break;
    case 'h':
    case 'H':
        i=7;
        break;
    }
    return i;
}
int pawn_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    //normal move
    if (r1-r2==1&&c1==c2&&is_empty0(board[r2][c2]))
        flag=1;
    //start move
    else if (r1==6&&r1-r2==2&&c1==c2&&is_empty0(board[r2][c2])&&is_empty0(board[r2+1][c2]))
        flag=1;
    //eat move
    else if(r1-r2==1&&(c1-c2==1||c2-c1==1)&&is_black(board[r2][c2]))
        flag=1;
    return flag;
}
int pawn_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    //normal move
    if (r2-r1==1&&c1==c2&&is_empty0(board[r2][c2]))
        flag=1;
    //start move
    else if (r1==1&&r2-r1==2&&c1==c2&&is_empty0(board[r2][c2])&&is_empty0(board[r2-1][c2]))
        flag=1;
    //eat move
    else if(r2-r1==1&&(c1-c2==1||c2-c1==1)&&is_white(board[r2][c2]))
        flag=1;
    return flag;
}
int rook_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    int flag1=1;
    //direction & validity
    if(r1>r2)
    {
        for(int i=r2+1; i<r1; i++)
        {
            if(is_empty0(board[i][c1])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r2>r1)
    {
        for(int i=r1+1; i<r2; i++)
        {
            if(is_empty0(board[i][c1])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(c1>c2)
    {
        for(int i=c2+1; i<c1; i++)
        {
            if(is_empty0(board[r1][i])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else
    {
        for(int i=c1+1; i<c2; i++)
        {
            if(is_empty0(board[r1][i])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    //move
    if (((r1-r2==0)||(c1-c2==0))&&flag1&&(is_empty0(board[r2][c2])||is_black(board[r2][c2])))
        flag=1;
    return flag;
}
int knight_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    //move
    if (abs(r1-r2)==2&&abs(c1-c2)==1&&(is_empty0(board[r2][c2])||is_black(board[r2][c2])))
        flag=1;
    //move
    else if (abs(r1-r2)==1&&abs(c1-c2)==2&&(is_empty0(board[r2][c2])||is_black(board[r2][c2])))
        flag=1;
    return flag;
}
int bishop_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    int flag1=1;
    if(r1>r2&&c1>c2)
    {
        for(int i=r2+1,j=c2+1; i<r1&&j<c1; i++,j++)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r1>r2&&c2>c1)
    {
        for(int i=r2+1,j=c2-1; i<r1&&j>c1; i++,j--)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r1<r2&&c1>c2)
    {
        for(int i=r2-1,j=c2+1; i>r1&&j<c1; i--,j++)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else
    {
        for(int i=r2-1,j=c2-1; i>r1&&j>c1; i--,j--)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    //normal move
    if (abs(r1-r2)==abs(c1-c2)&&flag1&&(is_empty0(board[r2][c2])||is_black(board[r2][c2])))
        flag=1;
    return flag;
}
int queen_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    if((abs(r1-r2)==0)||(abs(c1-c2)==0))
    {
        if(!rook_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if((abs(r1-r2))==(abs(c1-c2)))
    {
        if(!bishop_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int king_valid_move1(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    if(((abs(r1-r2)==0)&&(abs(c1-c2)==1))||((abs(c1-c2)==0)&&(abs(r1-r2)==1)))
    {
        if(!rook_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if(((abs(r1-r2))==(abs(c1-c2)))&&((abs(c1-c2))==1))
    {
        if(!bishop_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int rook_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    int flag1=1;
    //direction & validity
    if(r1>r2)
    {
        for(int i=r2+1; i<r1; i++)
        {
            if(is_empty0(board[i][c1])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r2>r1)
    {
        for(int i=r1+1; i<r2; i++)
        {
            if(is_empty0(board[i][c1])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(c1>c2)
    {
        for(int i=c2+1; i<c1; i++)
        {
            if(is_empty0(board[r1][i])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else
    {
        for(int i=c1+1; i<c2; i++)
        {
            if(is_empty0(board[r1][i])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    //move
    if (((r1-r2==0)||(c1-c2==0))&&flag1&&(is_empty0(board[r2][c2])||is_white(board[r2][c2])))
        flag=1;
    return flag;
}
int knight_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    //move
    if (abs(r1-r2)==2&&abs(c1-c2)==1&&(is_empty0(board[r2][c2])||is_white(board[r2][c2])))
        flag=1;
    //move
    else if (abs(r1-r2)==1&&abs(c1-c2)==2&&(is_empty0(board[r2][c2])||is_white(board[r2][c2])))
        flag=1;
    return flag;
}
int bishop_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=0;
    int flag1=1;
    if(r1>r2&&c1>c2)
    {
        for(int i=r2+1,j=c2+1; i<r1&&j<c1; i++,j++)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r1>r2&&c2>c1)
    {
        for(int i=r2+1,j=c2-1; i<r1&&j>c1; i++,j--)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else if(r1<r2&&c1>c2)
    {
        for(int i=r2-1,j=c2+1; i>r1&&j<c1; i--,j++)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    else
    {
        for(int i=r2-1,j=c2-1; i>r1&&j>c1; i--,j--)
        {
            if(is_empty0(board[i][j])==0)
            {
                flag1=0;
                break;
            }
        }
    }
    //normal move
    if (abs(r1-r2)==abs(c1-c2)&&flag1&&(is_empty0(board[r2][c2])||is_white(board[r2][c2])))
        flag=1;
    return flag;
}
int queen_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    if((abs(r1-r2)==0)||(abs(c1-c2)==0))
    {
        if(!rook_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if((abs(r1-r2))==(abs(c1-c2)))
    {
        if(!bishop_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int king_valid_move2(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    if(((abs(r1-r2)==0)&&(abs(c1-c2)==1))||((abs(c1-c2)==0)&&(abs(r1-r2)==1)))
    {
        if(!rook_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if(((abs(r1-r2))==(abs(c1-c2)))&&((abs(c1-c2))==1))
    {
        if(!bishop_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int white_correct_move(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    //check if the peace move correct
    if (board[r1][c1]=='p')
    {
        if(!pawn_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='r')
    {
        if(!rook_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='n')
    {
        if(!knight_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='b')
    {
        if(!bishop_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='q')
    {
        if(!queen_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else if(board[r1][c1]=='k')
    {
        if(!king_valid_move1(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int black_correct_move(char board[8][8],int r1,int c1,int r2,int c2)
{
    // return 1 if valid and zero if not valid
    int flag=1;
    //check if the peace move correct
    if (board[r1][c1]=='P')
    {
        if(!pawn_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='R')
    {
        if(!rook_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='N')
    {
        if(!knight_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='B')
    {
        if(!bishop_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if (board[r1][c1]=='Q')
    {
        if(!queen_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else if(board[r1][c1]=='K')
    {
        if(!king_valid_move2(board,r1,c1,r2,c2))
            flag=0;
    }
    else
        flag=0;
    return flag;
}
int is_empty0(char w)
{
    if (!is_black(w)&&!is_white(w))
        return 1;
    return 0;
}
int is_black(char w)
{
    if(w=='P'||w=='R'||w=='N'||w=='B'||w=='Q'||w=='K')
        return 1;
    return 0;
}
int is_white(char w)
{
    if(w=='p'||w=='r'||w=='n'||w=='b'||w=='q'||w=='k')
        return 1;
    return 0;
}
void copy_board(char board[8][8],char fboard[8][8])
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            fboard[i][j]=board[i][j];
        }
    }
}
int is_under_attack_by_black(char board[8][8],int r,int c)
{
    //returns 1 if under attack by black and zero if not
    int flag=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if (black_correct_move(board,i,j,r,c))
            {
                flag=1;
                break;
            }
        }
        if(flag)
            break;
    }
    return flag;
}
int is_under_attack_by_white(char board[8][8],int r,int c)
{
    //returns 1 if under attack by white and zero if not
    int flag=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if (white_correct_move(board,i,j,r,c))
            {
                flag=1;
                break;
            }
        }
        if(flag)
            break;
    }
    return flag;
}
int is_white_king_safe(char board[8][8],int r1,int c1,int r2,int c2)
{
    //returns one if safe
    char fboard[8][8];
    copy_board(board,fboard);
    //moving the character
    fboard[r2][c2]=fboard[r1][c1];
    //returning the place to black or white
    if ((r1+c1)%2==0)
        fboard[r1][c1]=178;
    else
        fboard[r1][c1]=176;
    //find white king
    int rk,ck,flagg=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(fboard[i][j]=='k')
            {
                rk=i;
                ck=j;
                flagg=1;
                break;
            }
        }
        if(flagg)
            break;
    }
    int flag=1;
    if (is_under_attack_by_black(fboard,rk,ck))
        flag=0;
    return flag;
}
int is_black_king_safe(char board[8][8],int r1,int c1,int r2,int c2)
{
    //returns one if safe
    char fboard[8][8];
    copy_board(board,fboard);
    //moving the character
    fboard[r2][c2]=fboard[r1][c1];
    //returning the place to black or white
    if ((r1+c1)%2==0)
        fboard[r1][c1]=178;
    else
        fboard[r1][c1]=176;
    //find black king
    int rk,ck,flagg=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(fboard[i][j]=='K')
            {
                rk=i;
                ck=j;
                flagg=1;
                break;
            }
        }
        if(flagg)
            break;
    }
    int flag=1;
    if (is_under_attack_by_white(fboard,rk,ck))
        flag=0;
    return flag;
}
int is_white_valid_move(char board[8][8],int r1,int c1,int r2,int c2)
{
    //returns one if a valid move for white
    //check if a correct move
    if(!white_correct_move(board,r1,c1,r2,c2))
        return 0;
    //check that white king is not in danger
    if(!is_white_king_safe(board,r1,c1,r2,c2))
        return 0;
    return 1;
}
int is_black_valid_move(char board[8][8],int r1,int c1,int r2,int c2)
{
    //returns one if a valid move for black
    //check if a correct move
    if(!black_correct_move(board,r1,c1,r2,c2))
        return 0;
    //check that white king is not in danger
    if(!is_black_king_safe(board,r1,c1,r2,c2))
        return 0;
    return 1;
}
int is_black_king_checked(char board[8][8])
{
    //find black king
    int rk,ck,flagg=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(board[i][j]=='K')
            {
                rk=i;
                ck=j;
                flagg=1;
                break;
            }
        }
        if(flagg)
            break;
    }
    int flag=0;
    if (is_under_attack_by_white(board,rk,ck))
        flag=1;
    return flag;
}
int is_white_king_checked(char board[8][8])
{
    //find white king
    int rk,ck,flagg=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(board[i][j]=='k')
            {
                rk=i;
                ck=j;
                flagg=1;
                break;
            }
        }
        if(flagg)
            break;
    }
    int flag=0;
    if (is_under_attack_by_black(board,rk,ck))
        flag=1;
    return flag;
}
int one_valid_move_for_black(char board[8][8])
{
    // returns one if there is at least one valid move for black
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(is_black(board[i][j]))
            {
                for (int k=0; k<8; k++)
                {
                    for (int h=0; h<8; h++)
                    {
                        if(is_black_valid_move(board,i,j,k,h))
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int one_valid_move_for_white(char board[8][8])
{
    // returns one if there is at least one valid move for white
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(is_white(board[i][j]))
            {
                for (int k=0; k<8; k++)
                {
                    for (int h=0; h<8; h++)
                    {
                        if(is_white_valid_move(board,i,j,k,h))
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int check_white_promotion_char(char board[8][8],int r1,int c1,int r2,int c2,char c_3)
{
    //returns one if no promotion and c_3=\n
    //returns one if promotion and c_3 is right
    if((board[r1][c1]=='p'&&r2==0)&&(c_3=='r'||c_3=='R'||c_3=='b'||c_3=='B'||c_3=='n'||c_3=='N'||c_3=='q'||c_3=='Q'))
        return 1;
    if ((!(board[r1][c1]=='p'&&r2==0))&&c_3=='\n')
        return 1;
    return 0;
}
int check_black_promotion_char(char board[8][8],int r1,int c1,int r2,int c2,char c_3)
{
    //returns one if no promotion and c_3=\n
    //returns one if promotion and c_3 is right
    if((board[r1][c1]=='P'&&r2==7)&&(c_3=='r'||c_3=='R'||c_3=='b'||c_3=='B'||c_3=='n'||c_3=='N'||c_3=='q'||c_3=='Q'))
        return 1;
    if ((!(board[r1][c1]=='P'&&r2==7))&&c_3=='\n')
        return 1;
    return 0;
}
void save_to_all_boards(char board[8][8])
{
    counter++;
    max_counter=counter;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            all_boards[i][j][counter]=board[i][j];
        }
    }
    for (int i=0; i<15; i++)
        all_dead_1_array[i][counter]=dead_1_array[i];
    for (int i=0; i<15; i++)
        all_dead_2_array[i][counter]=dead_2_array[i];
    all_dead1[counter]=dead1;
    all_dead2[counter]=dead2;
}
void copy_from_all_boards(char board[8][8])
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            board[i][j]=all_boards[i][j][counter];
        }
    }
    for (int i=0; i<15; i++)
        dead_1_array[i]=all_dead_1_array[i][counter];
    for (int i=0; i<15; i++)
        dead_2_array[i]=all_dead_2_array[i][counter];
    dead1=all_dead1[counter];
    dead2=all_dead2[counter];
}
void insertion_white(char w)
{
    //insert char in right place
    if(w=='q')
    {
        //shift and put at beginning
        for (int i=0;i<dead1;i++)
            dead_1_array[dead1-i]=dead_1_array[dead1-1-i];
        dead_1_array[0]='q';
    }
    else if(w=='r')
    {
        //finding place for rook
        int index=0;
        while(dead_1_array[index]=='q')
            index++;
        //shift
        for (int i=0;i<dead1-index;i++)
            dead_1_array[dead1-i]=dead_1_array[dead1-1-i];
        dead_1_array[index]='r';
    }
    else if(w=='b')
    {
        //finding place for bishop
        int index=0;
        while(dead_1_array[index]=='q')
            index++;
        while(dead_1_array[index]=='r')
            index++;
        //shift
        for (int i=0;i<dead1-index;i++)
            dead_1_array[dead1-i]=dead_1_array[dead1-1-i];
        dead_1_array[index]='b';
    }
    else if(w=='n')
    {
        //finding place for bishop
        int index=0;
        while(dead_1_array[index]=='q')
            index++;
        while(dead_1_array[index]=='r')
            index++;
        while(dead_1_array[index]=='b')
            index++;
        //shift
        for (int i=0;i<dead1-index;i++)
            dead_1_array[dead1-i]=dead_1_array[dead1-1-i];
        dead_1_array[index]='n';
    }
    else
        dead_1_array[dead1]='p';
}
void insertion_black(char w)
{
    //insert char in right place
    if(w=='Q')
    {
        //shift and put at beginning
        for (int i=0;i<dead2;i++)
            dead_2_array[dead2-i]=dead_2_array[dead2-1-i];
        dead_2_array[0]='Q';
    }
    else if(w=='R')
    {
        //finding place for rook
        int index=0;
        while(dead_2_array[index]=='Q')
            index++;
        //shift
        for (int i=0;i<dead2-index;i++)
            dead_2_array[dead2-i]=dead_2_array[dead2-1-i];
        dead_2_array[index]='R';
    }
    else if(w=='B')
    {
        //finding place for bishop
        int index=0;
        while(dead_2_array[index]=='Q')
            index++;
        while(dead_2_array[index]=='R')
            index++;
        //shift
        for (int i=0;i<dead2-index;i++)
            dead_2_array[dead2-i]=dead_2_array[dead2-1-i];
        dead_2_array[index]='B';
    }
    else if(w=='N')
    {
        //finding place for bishop
        int index=0;
        while(dead_2_array[index]=='Q')
            index++;
        while(dead_2_array[index]=='R')
            index++;
        while(dead_2_array[index]=='B')
            index++;
        //shift
        for (int i=0;i<dead2-index;i++)
            dead_2_array[dead2-i]=dead_2_array[dead2-1-i];
        dead_2_array[index]='N';
    }
    else
        dead_2_array[dead2]='P';
}
