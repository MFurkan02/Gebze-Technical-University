#include <stdio.h>
#include <stdlib.h>

void part1(int xy,int Dx,int Dy,int Cx,int Cy){
    int i,j;

    printf("-----------------\n");
    for(i=0;i<=xy;i++){  // Y axis
        
        for(j=0;j<=xy;j++){ // X axis

            if (i==Dy && j==Dx){
                printf("|D");
            }
            else if (i==Cy && j==Cx){
                printf("|C");
            }
            else if(j==xy){
                printf("|\n");
                break;
            }
            else {
                printf("| ");
            }
        }

    }
    printf("-----------------\n");




}


void part2(char loc,int xy,int Dx,int Dy,int Cx,int Cy){
    #define UP 'w'
    #define DOWN 's'
    #define RIGHT 'd'
    #define LEFT 'a'

   

    if (loc == 'w') { // && Cy>0 || Cy<xy
        Cy = Cy - 1;
        part1(xy,Dx,Dy,Cx,Cy);
    }
    else if (loc == 's') {  // && Cy>0 || Cy<xy
        Cy = Cy + 1;
        part1(xy,Dx,Dy,Cx,Cy);
    }
    else if (loc == 'd') {  // && Cx>0 || Cx<xy
        Cx = Cx + 1;
        part1(xy,Dx,Dy,Cx,Cy);

    }
    else if (loc == 'a') {  // && Cy>0 || Cy<xy
        Cx = Cx - 1;
        part1(xy,Dx,Dy,Cx,Cy);
    }
    else{
        printf("Wrong move!\n");
    }


    
}

int part3(int sta,int Dx,int Dy,int Cx,int Cy){

    if (Dx == Cx && Dy == Cy){
        sta = 1 ;                       // If the X coordinate of the door and X coordinate of the player is the same 
    }                                   // and If the Y coordinate of the door and Y coordinate of the player is the same
                                        // Status of the game will be 1 which means the game is finished.
    else{
        sta = 0 ;
    }                                   // Otherwise status is 0 which means player have not reached the door yet.

    return sta;
}


void part4(int choice){

    
    printf("Welcome to to the 2D puzzle game !\n");
    printf("1. New Game\n");
    printf("2. Help\n");
    printf("3. Exit\n");
    
    printf("Enter a number between 1 to 3 to implement the related action : ");
    scanf("%d",&choice);




        

        switch(choice){

        

        case 1:
            int xy,Dx,Dy,Cx,Cy,sta;
            char loc;
            
            printf("Enter the size of the room (5-10) : ");
            scanf("%d",&xy);
           
           
            // coordinate = (rand() % (upper(xy) - lower(1) + 1)) + lower(1)
            Dx = (rand() % ((xy-1) - 1 + 1)) + 1 ; // X coordinate of the door
            Dy = (rand() % ((xy-1) - 1 + 1)) + 1 ; // Y coordinate of the door
            Cx = (rand() % ((xy-1) - 1 + 1)) + 1 ; // X coordinate of the player
            Cy = (rand() % ((xy-1) - 1 + 1)) + 1 ; // Y coordinate of the player
        
            part1(xy,Dx,Dy,Cx,Cy);
            //int status = part3(sta,Dx,Dy,Cx,Cy);

            
            while ( part3(sta,Dx,Dy,Cx,Cy) == 0)
            {   
                
                printf("Please enter a direction to go (w,a,s,d) : ");
                scanf(" %c",&loc);
                part2(loc,xy,Dx,Dy,Cx,Cy);

            }


            break;
        case 2:
            printf("The character is able to move one space in any of the four cardinal directions: up, down, left, and right. Diagonal moves are not allowed. The user will input their desired move using the following keys: 'a' for left, 'd' for right, 'w' for up, and 's' for down. These moves should be defined at the beginning of the codewith  direction  names  as  the  keys.Any  control  or  selection  with  [a,  d,  w,  s]  characters will  result  in  a missgrading. The game will prompt the user for a new move after each move is made until the game is over.The current state of the room should be printed after each move, except when the character reaches the door.  If  the  character  attempts  to  move  through  a  wall,  a  warning  message  should  be  displayed.  The game ends when the character reaches the door, and a message should be displayed to notify the user that the game is over and how many moves were made during the game.\n");
            break;
        case 3:
            printf("Exiting from the game...\nHave a good day!\n");
            break;
        default:
            printf("Oops! Please enter a number between 1-3!\n");
            printf("Enter a number between 1 to 3 to implement the related action : ");
            scanf("%d",&choice);
            part4(choice);
            break;
    
   
    }



    
}

int main(){
  
    int choice; 
    part4(choice);
    return 0;
}