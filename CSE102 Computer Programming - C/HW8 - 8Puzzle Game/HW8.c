#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define UP 'U' // define of the moves.
#define LEFT 'L'
#define RIGHT 'R'
#define DOWN 'D' 

typedef struct Puzzle  // structure of 8-Puzzle.
    {
        int board[3][3]; // board array of the Puzzle structure.
        int move; // move number of the Puzzle structure.
        int best_score; // best score of the Puzzle structure.
        char empty; // character for the '-' character of the Puzzle structure.
        int last_pick; // the last pick of the PC to not to do the same move again in auto finish function.
    } Puzzle ;

Puzzle create_board(Puzzle *Puzzle){
    int nums[9]={1,2,3,4,5,6,7,8,9}; // the first situation of the board before it is mixed.
    int temp;
    int i,j;
    srand(time(NULL));
    for (i = 0; i < 9; i++)
    {
        j=rand() % 9; // picking a number form 1 to 9
        temp=nums[i]; // then changing the position of picked index of the nums array to mix the array.
        nums[i]=nums[j];
        nums[j]=temp;
    }
    int l,g,conv=0;
    for ( l = 0; l < 8; l++) // controlling if the board is solvable or not by checking the inversion number.
    {
        for(g=l+1;g<9;g++){
            if(nums[l]>nums[g] && nums[l]!=9 && nums[g]!=9){
                conv++;
            }
        }
    }

   // printf("Conversion number is %d\n",conv);
    int n=0; // the index of nums array.
    int r,c; // the indices of board array.
    for ( r = 0; r < 3; r++)
    {
        for ( c= 0; c < 3; c++)
        {
                Puzzle->board[r][c]=nums[n];  // equalling the puzzle board to the nums array which was mixed.
                n++; 
        }
    }
    if(conv%2!=0){ // if inversion number is odd which means the board is not solvable.
        if ( Puzzle->board[0][0] == 9 || Puzzle->board[0][1] ==9){ // if the empty char '_' which is 9 for my code is in 00 or 01 index of the board
             //printf("Old is %d-%d\n",Puzzle->board[1][0],Puzzle->board[1][1]); // then instead of it 10 and 11 indices will be changed to make 
             temp = Puzzle->board[1][0] ;                                     // inversion number even to make it solvable.
            Puzzle->board[1][0] = Puzzle->board[1][1] ;
            Puzzle->board[1][1] = temp;
        }
        else { // if the empty char '_' which is 9 for my code is in 10 or 11 index of the board
            //printf("Old %d-%d\n",Puzzle->board[0][0],Puzzle->board[0][1]);  // then this time indices 00 and 01 will be changed to make the inversion even.
            temp = Puzzle->board[0][0] ;
            Puzzle->board[0][0] = Puzzle->board[0][1] ;
            Puzzle->board[0][1] = temp;
        }
        
    }

    return *Puzzle;
}

    

int possibles(Puzzle *Puzzle,int nums[4],char direct[4]){ // the function to let PC choose only the possible number and direction in auto_finish function.
    int i,j;
    int index=0;
     for(i=0;i<3;i++){
       for(j=0;j<3;j++){
           if(Puzzle->board[i][j]==9){ // if the index of the board is the '_' character which is 9 it will look towards the left,right,up and down to check if it can move there without getting out of index.
                if (i<2) { // then if it can move there the char and direct array will be that number and direct which PC will choose from. 
                    nums[index]=Puzzle->board[i+1][j];
                    direct[index]=UP;
                    index++;
                }
                if (j<2){
                    nums[index]=Puzzle->board[i][j+1];
                    direct[index]=LEFT;
                    index++;
                }
                if (i>0) {
                    nums[index]=Puzzle->board[i-1][j];
                    direct[index]=DOWN;
                    index++;
                }
                if (j>0) {
                    nums[index]=Puzzle->board[i][j-1];
                    direct[index]=RIGHT;
                    index++;
                }

           }
        }
    }
    return index; // returning the index so that rand function will choose up to that index.
}



int finished(Puzzle *Puzzle){ // controlling if the game is finished or not.
    int finished=0;
    if(Puzzle->board[0][0]==1 && Puzzle->board[0][1]==2 && Puzzle->board[0][2]==3 && Puzzle->board[1][0]==4 && Puzzle->board[1][1]==5 && Puzzle->board[1][2]==6 && Puzzle->board[2][0]==7 && Puzzle->board[2][1]==8){
        finished=1;
    } // checking if the board is like 12345678 then changing the finshed to 1 and returning if it is like that.
    return finished; // returning if the game is finished or not.
}


int finished2(Puzzle *Puzzle){ // function for auto_finish function to finish much qiuckly to not pick 1,2 or 3 if the first row is 123.
    int finished2=0;
    if(Puzzle->board[0][0]==1 && Puzzle->board[0][1]==2 && Puzzle->board[0][2]==3) finished2=1; // if first row is 123 then finished2 will be 1.
    return finished2; // returning if the first row is 123 or not.
}

int valid_move(int number,char direction,Puzzle *Puzzle){ // to control if the input move (number,direction) is valid.
    int valid=0; // control validation number. 
    int i,j;
    for ( i = 0; i < 3; i++) // loop to go through the board.
    {
        for (j = 0; j < 3 ; j++)
        {
           
            if(Puzzle->board[i][j]==number){ // if the input number ex:(5) is encountered.
                switch (direction) // the direction from the input.
                {
                case UP: // checling if up direction is '-' if it is the move is valid.
                    if(Puzzle->board[i-1][j]==9){
                        valid=1;
                    }
                    break;
                case DOWN: // checling if down direction is '-' if it is the move is valid.
                    if(Puzzle->board[i+1][j]==9){
                        valid=1;
                    }
                    break;
                case LEFT: // checling if left direction is '-' if it is the move is valid.
                    if(Puzzle->board[i][j-1]==9){
                        valid=1;
                    }
                    break;
                case RIGHT: // checling if right direction is '-' if it is the move is valid.
                    if(Puzzle->board[i][j+1]==9){
                        valid=1;
                    }
                    break;
                default:
                    break;    
                }
            }
        }
    }

    return valid; // returning if move is valid or not.

}


void print_puzzle(Puzzle *Puzzle){ // function to print the puzzle board.
    int i,j;
    Puzzle->empty='_'; // the '_' character which will be printed instead of number 9.
    FILE* puzzle;
    puzzle=fopen("puzzle.txt","a"); // opening the puzzle file to append to it. 
    for ( i = 0; i < 3; i++) //  loop to go through the board.
    {
        for (j = 0; j < 3 ; j++)
        {
            if(Puzzle->board[i][j]==9){ // if number is 9 printing '-' instead of 9 to terminal and file.
                printf("%c ",Puzzle->empty);
                fprintf(puzzle,"%c ",Puzzle->empty);
            }
            else{
                printf("%d ",Puzzle->board[i][j]); // else printing the number directly to terminal and file.
                fprintf(puzzle,"%d ",Puzzle->board[i][j]);
            }
        }
        printf("\n"); // newline after every row to terminal and file.
        fprintf(puzzle,"\n");
    }
    fprintf(puzzle,"\n"); // printing an extra newline to file for every board to have newline between them.
    fclose(puzzle); // closing the file.
}

Puzzle move(int number,char direction,Puzzle *Puzzle){ // function to move the numbers to given direction on Puzzle board.
    int i,j;
    int temp,new,temp2;
    for ( i = 0; i < 3; i++) // loop to go through the board
    {
        for ( j = 0; j < 3; j++)
        {   
            if(Puzzle->board[i][j]==9){ // if the index is 9 which is '_' and direction is down or right. 
                switch (direction){  // then the index of the board will be changed with the right or down. 
                    case DOWN: 
                        temp=Puzzle->board[i][j];
                        Puzzle->board[i][j]=Puzzle->board[i-1][j];
                        Puzzle->board[i-1][j]=temp;
                        break;
                    case RIGHT:        
                        temp=Puzzle->board[i][j]; 
                        Puzzle->board[i][j]=Puzzle->board[i][j-1];
                        Puzzle->board[i][j-1]=temp;
                        break;
                    default:
                        break;
                }
            }
            else if(Puzzle->board[i][j]==number){ // if the index is the number to be moved
                 switch (direction){  // if direction is UP or LEFT it will be moved and changed towards the location. 
                    case UP:
                        temp=Puzzle->board[i][j];
                        Puzzle->board[i][j]=Puzzle->board[i-1][j];
                        Puzzle->board[i-1][j]=temp;
                        break;
                    case LEFT:       
                        temp=Puzzle->board[i][j]; 
                        Puzzle->board[i][j]=Puzzle->board[i][j-1];
                        Puzzle->board[i][j-1]=temp;
                        break;
                    default:
                        break;
            }
        }
    }
  }
    return *Puzzle; // returning the moved Puzzle board.
}

int read_best_score(Puzzle *Puzzle){
    FILE* Best;
    int score;
    Best=fopen("best_score.txt","r"); // opening the file to read.
    fscanf(Best,"%d",&score); // scanning the score and equaling the Puzzle best score to that score in the file.
    Puzzle->best_score=score;
    printf("Best score is %d!\n",score); // printing the best score and returning the best score.
    fclose(Best);
    return Puzzle->best_score;
}


void playing_as_user(Puzzle Puzzle){ // First(1) option to play as the user.
    int number,i,j;
    int score=1000; // first condition of the score.
    char direction;
    int valid=0;
    Puzzle.move=0; // making the move counter 0.
    Puzzle=create_board(&Puzzle); // creating the board from the create board function.

    
    while (finished(&Puzzle)==0){ // while loop to continue till the game finishes.
        print_puzzle(&Puzzle); // Printing the board
        printf("Enter your move (number-direction, e.g., 2-R): ");
        scanf("%d-%c",&number,&direction);  // prompting the user to enter move and the direction. 
        valid=valid_move(number,direction,&Puzzle); // checking if the move is valid or not.
        if(valid==0){ // if the move is not valid  an error message will be printed and another move will be wanted to be entered.
            printf("%d-%c is an illegal move!!! Make a legal move!!!\n",number,direction);
            Puzzle.move++; // move counter will be incremented after the move.
        }else{ // if the move is valid then the move will be made and the puzzle board will be updated.
            Puzzle=move(number,direction,&Puzzle);
            Puzzle.move++; // move counter will be incremented after the move.
        }
        if (finished(&Puzzle)==1)  // if the game is finished
        {
            print_puzzle(&Puzzle); // the final puzzle board will be printed and a congratulatory message will be printed with the number of moves made and the best socre.
            printf("Congratulations! You have finished the game. \n\n");
            printf("Total number of moves : %d\n",Puzzle.move);
            score-=10*(Puzzle.move);
            Puzzle.best_score=read_best_score(&Puzzle);
            if (score>Puzzle.best_score)  // if the score of this current game is higher than the best score
            {
                Puzzle.best_score=score;  // current score will be the best score
                FILE* Best;
                Best=fopen("best_score.txt","w");  
                fprintf(Best,"%d",score); // then the best score on the best score txt will be changed to the current score which is the new best score.
                fclose(Best);
            }
            printf("Your score : %d\n",score); // score of this game will be printed.
        }   
    }
}

void auto_finish(Puzzle *Puzzle){ // Second(2) option to make PC play the game.   
    int number,i,j;
    char direction;   
    int nums[4];
    char direct[4];
    int index,pick;
    index=possibles(Puzzle,nums,direct); // returning the possible number of moves that can PC move.
    srand(time(0));
    pick=rand() % index; // picking a number between the numbers that can be pickable.
    if(finished2(Puzzle)==1){ // if the first row is 123 then 123 or the last pick will not be picked one more according to the while loop.
        while(nums[pick]==1 || nums[pick]==2 || nums[pick]==3 || Puzzle->last_pick==nums[pick]){
            pick=rand() % index;
        }
    }
    else{ // if the first row is still not 123 then only the movable number and directions will be picked except the last pick.
        while (Puzzle->last_pick==nums[pick])
        {
            pick=rand() % index;
        }
    } 
    Puzzle->last_pick=nums[pick]; // after pick is valid last pick will be changed to that pick for the next recursion call to not pick the last pick.
    *Puzzle=move(nums[pick],direct[pick],Puzzle); // then moving the '_' character to that location and updating the board.
    printf("Computer Move : ");
    printf("%d-%c\n",nums[pick],direct[pick]); // printing the move of the computer.
    Puzzle->move++; // incrementing the number of move.
    print_puzzle(Puzzle); // printing the puzzle.
    if(finished(Puzzle)==1){ // if game is finished congratulatory message and total number of moves will be printed.
        printf("Computer finished the game!\n");
        printf("Total number of computer moves: %d\n",Puzzle->move);
    }
    else{
        auto_finish(Puzzle); // to call the function recursively if the game is not finished.    
    } 
}

int main(){
    FILE* puzzletxt;
    puzzletxt=fopen("puzzle.txt","w"); // to reset the file after every compilation to write to an empty txt file on every new compilation.
    fclose(puzzletxt);
    Puzzle Puzzle;
    int choice;
    // THE MENU OF THE 8-PUZZLE GAME //
    printf("Welcome to the 8-Puzzle Game!\n");
    printf("Please select an option:\n");
    printf("(1) Play game as a user\n(2) Finish the game with PC\n(3) Show the best score\n(4) Exit\n");
    printf("> ");
    scanf("%d",&choice);
    switch (choice)
    {
        case 1: // user
            playing_as_user(Puzzle);
            main();
            break;
        case 2: // auto finish
            Puzzle=create_board(&Puzzle); // first creating the board for the recursive auto_finish function.
            Puzzle.last_pick=0; // the last pick of PC to not pick it again which is 0 for now since it is stil not picked.
            print_puzzle(&Puzzle); // printing the first puzzle board.
            Puzzle.move=0; // making the number of moves 0 before game starts.
            auto_finish(&Puzzle); // auto_finish function.
            main();
            break;
        case 3: // Best score
            read_best_score(&Puzzle);
            main();
            break;
        case 4:
            printf("Exiting from the game...\nHave a nice day!\n");
            break;
        default:
            printf("Please enter a choice between 1-4!\n");
            main();
            break;
    }
}