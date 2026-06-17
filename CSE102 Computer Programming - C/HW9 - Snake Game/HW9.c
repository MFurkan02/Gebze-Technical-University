#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
#define BAIT '.'

typedef struct // structure of the Block which has a type and value for the nested blocks.
{
    char type;
    int value;
} Block ;

typedef struct // point structure of the snake which holds the values of the row and column values of the head and bodies of the snake.   
{
    int row;
    int col;
} Point ;


Block*** init_board(){ // function to initialize the board
    
    int i,j;
    int bait_i,bait_j;
    // coordinates of bait and obstacle which will be generated randomly.
    int obstacle_i,obstacle_j;
    int obstacle;
    do
    {
        bait_i = rand() % 10;
        bait_j = rand() % 10;
    } while (bait_i==0 && bait_j==0);
    // the random value generation of the bait and obstacle till both of them are not zero in case if they hit the snake's head at the beginning of the game.
    do
    {
        obstacle_i = rand() % 10;
        obstacle_j = rand() % 10;
    } while ( obstacle_i==0 &&  obstacle_j==0);

    Block*** board = (Block***)malloc(10* sizeof(Block**)); 
    for (int i = 0; i < 10; i++) {
        board[i] = (Block**)malloc(10 * sizeof(Block*));
        for (int j = 0; j < 10; j++) {
            board[i][j] = (Block*)malloc(sizeof(Block)); // dynamically allocating memory for every dimension of the board.
            board[i][j][0].type = 'e';  // Initializing all blocks as empty at first.
            board[i][j][0].value = 0;
        }
    }
    board[bait_i][bait_j][0].type='b'; // changing the type of the block to bait that has the randomly generated values of bait.
    int k;
    int obstacle_dim = 1 + (rand() % 3); // picking how many nested blocks will be on the 3rd dimension.Picking between 1-3 because each of them will have a value too.
    board[obstacle_i][obstacle_j]=(Block*)realloc(board[obstacle_i][obstacle_j],obstacle_dim*sizeof(Block)); // dynamically allocating memory for the picked obstacle dimension value. 
    for(k=0;k<obstacle_dim;k++){
        board[obstacle_i][obstacle_j][k].type='o';  // changing the type of the block to obstacle that has the randomly generated values of obstacle.
        board[obstacle_i][obstacle_j][k].value=1 + rand() % 3; // value of the obstacle that every nested block will have
    }
    return board; // returning the initialized board.
    
} 

void draw_board(Block*** board,Point *Snake,int length){
    int i,j,k,d; // variables for the for loop of all the dimensions.
    int flag=0; 
    char empty=' '; // empty block to be printed for empty block types. 
    int obs=0;
    printf("----------------------\n"); // printing the horizontal edges of the game board.
    for ( i = 0; i < 10; i++)
    {
        printf("|"); // printing the vertical edges of the game board for the edges at the beginning.
        for (j = 0; j < 10; j++)
        {
            for(k=0;k<length;k++){ // looking through the coordinates of the body of the snake up to the current length of the snake to print it.
                if(Snake[k].row==i && Snake[k].col==j){
                    flag=1; // changing the flag if a snake coordinate is encountered to not to print empty in the below if condition.
                    if(k==0){ // if the encountered snake body is head head of the snake 'O' will be printed.
                        printf("O ");
                    }
                    else{ // otherwise body of the snake 'X' will be printed.
                        printf("X ");
                    }
                }
            }
            if(flag==0){ // if a snake coordinate is not encountered.
                if(board[i][j][0].type=='b'){ // if the type of the block is bait then BAIT('.') will be printed.
                    printf("%c ",BAIT);
                }
                else if(board[i][j][0].type=='e'){ // if the type of the block is empty then ' ' will be printed.
                    printf("%c ",empty);
                }
                else if(board[i][j][0].type=='o'){ // if the type of the block is obstacle then values of the obstacles will be printed.
                    for(d=0;d<4;d++){
                        obs+=board[i][j][d].value;
                    }
                    if((obs/10)>=1){
                        printf("%d",obs);
                    }
                    else{ 
                        printf("%d ",obs);
                    }
                }
            }
            obs=0;
            flag=0; // resetting the flag value for the next row.

        }
        printf("|\n");  // printing the vertical edges of the game board for the edges at the end.
    }
    printf("----------------------\n"); // printing the horizontal edges at the end of the game board.
}


void update(Block*** board,Point **Snake,int *length,int moves,int tail_i,int tail_j){
    int i,j,k,flag=0;
    int bait_i,bait_j;
    // coordinates of bait and obstacle which will be generated randomly.
    int obstacle_i,obstacle_j;
    int obstacle;
    int num_of_obs=0; // to count the number of obstacles to not create more than 3 obstacles to board.
    //printf("Current move is %d\n",moves);
    //printf("Tail coordinates3= %d-%d\n",tail_i,tail_j);
    /*
    for ( i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++){
            if(board[i][j][0].type=='b' && (*Snake[0]).row==i && (*Snake[0]).col==j){
            // checking if the head of the snake encountered with the bait by checking through the board and its type.
                flag=1; // changing the flag value.
                board[i][j][0].type='e'; // changing the type of the eaten bait location to empty.
                (*length)++; // incrementing the length of the snake after snake eats the bait.
                
                //Snake=new;
                // after shifting the snake then since the snake will get bigger after eating the bait.
                // then another entry will be added to the snake dynamic array and its position will be determined according to the former entry of the snake dynamic array.
            }
            else if(board[i][j][0].type=='o' && (*Snake[0]).row==i && (*Snake[0]).col==j && board[i][j][0].value<(*length)){
                board[i][j][0].type='e'; // changing the type of the captured obstacle location to empty.
                //num_of_obs--;
            }
        }
    }*/
    if(board[(*Snake)[0].row][(*Snake)[0].col]->type=='b'){
                //printf("yummy!\n");
                flag=1; // changing the flag value.
                board[(*Snake)[0].row][(*Snake)[0].col][0].type='e'; // changing the type of the eaten bait location to empty.
                (*length)++; // incrementing the length of the snake after snake eats the bait.
                //printf("nooo\n");
                *Snake=realloc(*Snake,sizeof(Point)*(*length));
                 //printf("yooo\n");
                (*Snake)[(*length-1)].row=tail_i;
                (*Snake)[(*length-1)].col=tail_j; 
                do // the random value generation of the bait till the type of the block is empty to put the bait on an empty block.
                {
                    bait_i = rand() % 10;
                    bait_j = rand() % 10;
                } while (board[bait_i][bait_j][0].type!='e');
                board[bait_i][bait_j][0].type='b';   // after generating the dimension values changing the type of that block to bait.
    }
    for ( i = 0; i < 10; i++) 
    {
            for(j = 0; j < 10; j++){ // checking through the type of the all dimensions to count the current number of obstacle on the board.
                if(board[i][j][0].type=='o'){
                    num_of_obs++; // incrementing the num_of_obs counter if an obstacle is encountered.
                }
            }
    }

    if(moves>0 && moves%5==0 && num_of_obs<3){ // since in every 5 moves a new obstacle will be added with the condition not being more than 3.
        // the random value generation of the obstacle till type of the block is empty to put the obstacle on an empty block.
        do
        {
            obstacle_i = rand() % 10;
            obstacle_j = rand() % 10;
        } while ( board[obstacle_i][obstacle_j][0].type!='e');

        int obstacle_dim = 1 + (rand() % 3); // picking how many nested blocks will be on the 3rd dimension.Picking between 1-3 because each of them will have a value too.
        board[obstacle_i][obstacle_j]=(Block*)realloc(board[obstacle_i][obstacle_j],obstacle_dim*sizeof(Block)); // dynamically allocating memory for the picked obstacle dimension value. 
        for(k=0;k<obstacle_dim;k++){
            board[obstacle_i][obstacle_j][k].type='o';  // changing the type of the block to obstacle that has the randomly generated values of obstacle.
            board[obstacle_i][obstacle_j][k].value=1 + rand() % 3; // value of the obstacle that every nested block will have (1-3)
        }
    }
    else if(moves%5==0 && num_of_obs==3){
        int pick= 1 + (rand() % 3) ;
        int current=0;
        for (i = 0; i < 10; i++)
        {
            for(j=0;j<10;j++){
                if(board[i][j][0].type=='o'){
                    current++;
                    if(current==pick){
                        board[i][j][0].value++;
                    }
                }
            }
        }
        
    }
    /*
    if(flag){
                printf("YEAH\n");
                *Snake=realloc(*Snake, (*length) * sizeof(Point));
                (*Snake)[(*length-1)].row=tail_i;
                (*Snake)[(*length-1)].col=tail_j; 
                do // the random value generation of the bait till the type of the block is empty to put the bait on an empty block.
                {
                    bait_i = rand() % 10;
                    bait_j = rand() % 10;
                } while (board[bait_i][bait_j][0].type!='e');
                board[bait_i][bait_j][0].type='b';   // after generating the dimension values changing the type of that block to bait.
    }*/
    if(!flag){
            //printf("shift!\n");
            for (i = ((*length)-1); i > 0 ; i--){ 
                // loop to go through the body of the snake except its head.
                ((*Snake)[i]) = ((*Snake)[i-1]); // shifting the body of the snake.
            }
    }
} 

void move(Point* Snake,int length,int *tail_i,int *tail_j){
    char direction;
    printf("> "); // prompting the user to enter a move.
    scanf(" %c",&direction);
    int flag=1;
    if(length>1){ 
    // if snake has a body which means its length is greater than 1 then it will be checked if it tried to go through the same direction with its body.
        if(Snake[0].row==Snake[1].row){ // if the snake is vertical which means head of the snake and its body which is the closest to its head is in the same row.
            if((Snake[0].col+1)==Snake[1].col && direction==RIGHT){
                printf("Wrong move!Snake can not go through the same direction with its own body!\n"); 
                // Error message to warn player to not go through the same direction(DOWN) with the body of the snake.
                flag=0;
                move(Snake,length,tail_i,tail_j); // then calling the move function again since this was a wrong move.
            }
            else if((Snake[0].col-1)==Snake[1].col && direction==LEFT){
                 printf("Wrong move!Snake can not go through the same direction with its own body!\n"); 
                // Error message to warn player to not go through the same direction(UP) with the body of the snake.
                flag=0;
                move(Snake,length,tail_i,tail_j); // then calling the move function again since this was a wrong move.
            }
        }
        else if(Snake[0].col==Snake[1].col){ // if the snake is horizontal which means head of the snake and its body which is the closest to its head is in the same column.
            if((Snake[0].row+1)==Snake[1].row && direction==DOWN){
                printf("Wrong move!Snake can not go through the same direction with its own body!\n"); 
                // Error message to warn player to not go through the same direction(RIGHT) with the body of the snake.
                flag=0;
                move(Snake,length,tail_i,tail_j); // then calling the move function again since this was a wrong move.
            }
            else if((Snake[0].row-1)==Snake[1].row && direction==UP){
                 printf("Wrong move!Snake can not go through the same direction with its own body!\n"); 
                // Error message to warn player to not go through the same direction(LEFT) with the body of the snake.
                flag=0;
                move(Snake,length,tail_i,tail_j); // then calling the move function again since this was a wrong move.
            }
        }
    }
    if(flag){ 
        int i;
        *tail_i=Snake[length-1].row;
        *tail_j=Snake[length-1].col;
        Snake[1]=Snake[0];
        switch (direction)
        {
            case UP:
                Snake[0].row--; // if the direction is UP then the head of the snake will move upwards by decreasing the row of the head.
                break;
            case DOWN:
                Snake[0].row++; // if the direction is DOWN then the head of the snake will move downwards by incrementing the row of the head.
                break;
            case RIGHT:
                Snake[0].col++; // if the direction is RIGHT then the head of the snake will move to the right by incrementing the column of the head.
                break;
            case LEFT:
                Snake[0].col--;  // if the direction is LEFT then the head of the snake will move to the left by decreasing the column of the head.
                break;
            default:
                printf("Please enter either 'w' (up) or 'a' (left) or 's' (down) or 'd' (right) ");
                // If there is a direction different than 'w','a','s' or 'd' is entered an error will be displayed an move function will be called again.
                move(Snake,length,tail_i,tail_j);
                break;
        }
    }

} 

int check_status(Block*** board,Point* snake,int length){
    int status=0;
    int i;
    if(snake[0].row<0 || snake[0].col<0 || snake[0].row>9 || snake[0].col>9){ 
        // checking if the head of the snake hit the wall by checking if the head of the snake exceeded 0th or 9th index. 
        status=1; // status will be 1 meaning the game is over.
        printf("Game over!You have hit the wall!\n");
        // a message will be printed to state that the game is over and the reason of it being over.
    }
    else if(board[snake[0].row][snake[0].col][0].type=='o' && board[snake[0].row][snake[0].col][0].value>length){ 
        // if the head of the snake hit an obstacle that has x number of blocks, where x is greater than the length of the snake
        status=1; // status will be 1 meaning the game is over.
        printf("Game over!You hit an obstacle that has greater number of blocks than the length of the snake!\n");
        // a message will be printed to state that the game is over and the reason of it being over.
    }
    else{
        for (i = 1; i < length; i++)
        {
            if(snake[0].row==snake[i].row && snake[0].col==snake[i].col){ 
                // checking if head of the snake hit its own body by checking in a loop through the dynamic array of point structure snake.
                status=1; // status will be 1 meaning the game is over.
                printf("Game over!Head of the snake hit its own body!\n");
                // a message will be printed to state that the game is over and the reason of it being over.
            }
        }
    }
    return status;

}


void play(Block*** board){
    Point* snake = (Point*)malloc(1*sizeof(Point)); // allocating memory for the snake.
    // after that head of the snake will be on the top left corner by making the first row and column of the first index of the snake dynamic array 0.
    snake[0].row = 0; 
    snake[0].col = 0;
    int length=1; // length of the snake will be 1 at the beginning.
    int status=0;
    int moves=0; // status and number of moves made.
    int tail_i,tail_j;
    //draw_board(board,snake,length); // drawing the board before the first move.
        while (!status) // while loop to play the game until check status is 1.
        {   
            draw_board(board,snake,length); // drawing the board after the move is made.
            if(length>1){
                update(board,&snake,&length,moves,tail_i,tail_j); // updating the board by using the function.
            }
            move(snake,length,&tail_i,&tail_j); // moving the snake by using the function.
            status=check_status(board,snake,length); // checking the status after the move.
            if(board[snake[0].row][snake[0].col][0].type=='b'){
                //printf("here\n");
                update(board,&snake,&length,moves,tail_i,tail_j); // updating the board by using the function.
            }
            moves++;




            //status=check_status(board,snake,length); // checking the status after the move.
            //printf("Tail coordinates2= %d-%d\n",tail_i,tail_j);
            //update(board,snake,&length,moves,tail_i,tail_j); // updating the board by using the function.
            //draw_board(board,snake,length); // drawing the board after the move is made.
            //printf("length is %d\n",length);
        }
    // Freeing the dynamically allocated memory by the board dimension by dimension.
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            free(board[i][j]);
        }
        free(board[i]);
    }
    free(board);
    free(snake); // freeing the memory that has been allocated by the snake after the game ends.
} 



int main(){
    srand(time(NULL)); // seed to generate random numbers each time.
    Block*** board=init_board(); // initializing the board using init_board() function.
    play(board); // playing the initialized board using the move(),update() and check_status() funtions inside play() function.
}
