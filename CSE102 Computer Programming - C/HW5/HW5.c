#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100

char case_conversion(char* ch){
    int i;
    char lower_case_letters[26]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'} ;
    char upper_case_letters[26]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'} ;
        for(i=0;i<27;i++){
            if (*ch==lower_case_letters[i]){ // if the entered character is a lower-case letter it will become an upper-case letter. 
                *ch=upper_case_letters[i];  // letter case conversion.
            }
    }
    return *ch; // if the entered character is an upper_case it will be returned directly to be used in the part1() fucntion.
}

void part1(){

    FILE* let; // file pointer argument
    char letters[26]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'} ;
    char fname[MAX] ;// defining the filename that will be input by the user with size MAX(100 defined) above.
    printf("Enter the file name : ");
    scanf("%s",fname);

    let = fopen(fname,"r");

    int counter[26] = {} ; // counter array for frequency.
    char ch;
    int i;
    while(fscanf(let,"%c",&ch)!=EOF){ // assigning char ch to the every character on the txt file.
        ch=case_conversion(&ch); // to return the lower-case letters of the current index(i) from the case conversion function using pointer argument from case conversion function. 
            for(i=0;i<27;i++){
                if(ch==letters[i]){ // if the character on the txt file is one of the upper-case letters or lower_case letters from the other function.  
                    counter[i]++; // current index of the counter array will be incremented. 
            }
        }
    }
    
    printf("Letter Frequency:\n");
    int j;
    for(j=0;j<26;j++){ // printing the letters and its frequencies in a tabular format.
    printf("%c = %d\n",letters[j],counter[j]);
    }
    fclose(let); // Closing the file.
}
typedef enum  {RED,GREEN,BLUE,YELLOW,ORANGE} Color;
 // RED = {1,0,0}
 // GREEN = {0,1,0}
 // BLUE = {0,0,1}        // r-b r-g b-y r-y
 // YELLOW = {0.5,0.5,0}
 // ORANGE = {0.5,0.4,0.2}

Color euclidean_distance(Color Color1,Color Color2){
    
    double colors[5][3] = {{1, 0, 0},{0, 1, 0}, {0, 0, 1} , {0.5, 0.5, 0} ,  {0.5, 0.4, 0.2} } ; // Vectors of all colors starting from red.
   
    double store[5] ={}; // to store all the distances every color vector has to the average vector (vector3). 

    double Vector1[3] = {}; // Vector of Color1.

    double Vector2[3] = {};  // Vector of Color2.

    double Vector3[3] = {} ; // Average vector which is to become (Vector1 + Vector2 / 2) for every index. 

    

    if (Color1==RED){ // if the color picked from color mixer function is red.
        Vector1[0]=1; // Assigning the first vector to the vector of red.
        Vector1[1]=0;
        Vector1[2]=0;
    }
    else if(Color1==GREEN){ // if the color picked from color mixer function is green.
        Vector1[0]=0;        // Assigning the first vector to the vector of green.
        Vector1[1]=1;
        Vector1[2]=0;
    }
    else if(Color1==BLUE){  // if the color picked from color mixer function is blue.
        
        Vector1[0]=0;  // Assigning the first vector to the vector of blue.
        Vector1[1]=0;
        Vector1[2]=1;
    }
    else if(Color1==YELLOW){ // if the color picked from color mixer function is yellow.
        
        Vector1[0]=0.5; // Assigning the first vector to the vector of yellow.
        Vector1[1]=0.5;
        Vector1[2]=0;
    }
    else if(Color1=ORANGE){  // if the color picked from color mixer function is orange.

        Vector1[0]=0.5; // Assigning the first vector to the vector of orange.
        Vector1[1]=0.4;
        Vector1[2]=0.2;
    }

     if (Color2==RED){ // if the color picked from color mixer function is red.
        Vector2[0]=1;
        Vector2[1]=0; // Assigning the second vector to the vector of red.
        Vector2[2]=0;
    }
    else if(Color2==GREEN){ // if the color picked from color mixer function is green.
        
        Vector2[0]=0;
        Vector2[1]=1;   // Assigning the second vector to the vector of green.
        Vector2[2]=0;
    }
    else if(Color2==BLUE){  // if the color picked from color mixer function is blue.
        
        Vector2[0]=0;
        Vector2[1]=0;       // Assigning the second vector to the vector of blue.
        Vector2[2]=1;
    }
    else if(Color2==YELLOW){  // if the color picked from color mixer function is yellow.
        
        Vector2[0]=0.5;
        Vector2[1]=0.5;         // Assigning the second vector to the vector of yellow.
        Vector2[2]=0;
    } 
    else if(Color2=ORANGE){   // if the color picked from color mixer function is orange.
        Vector2[0]=0.5;
        Vector2[1]=0.4;     // Assigning the second vector to the vector of orange.
        Vector2[2]=0.2;
    }

    int f;
    for (f = 0; f <3; f++)
    {
        // Calculating the average vector(Vector3) of color1 and color2 vector for every index.
        Vector3[f] = (Vector1[f] + Vector2[f]) / 2.0 ; 
    }

    int i,j;
    double dist=0.0; 
    for (j = 0; j < 5; j++)
    {
        for ( i = 0; i <3; i++)
        {
            dist +=(Vector3[i] - colors[j][i]) *  (Vector3[i] - colors[j][i])   ;  
            // Calculating the euclidean distance of the Vector3 to the all color vectors respectively(red,green,blue,yellow,orange).
        }
        store[j]=sqrt(dist); // storing the every distance color vectors had to Vector3 after taking its square. 
        dist=0.0; // resetting the distance value for the next color vector distance calculation.
    }
    Color Colors;
    //int k;
    Colors = RED ;
    int index=Colors; 
    double min=store[index]; // setting the min element of array to the first index(RED (0) ) to find the min distance in the store array.
    Color Colors1=GREEN;
    Color Colors2=ORANGE;
    for (Colors1 = GREEN; Colors1 <= ORANGE; Colors1=Colors1 + 1) // indexes after RED which is from GREEN to ORANGE
    {
        if(store[index] > store[Colors1]){ // if the current index is bigger than our selected min value 
            index=Colors1;                // changing our min value to the current index.(which is a color enumaration value(k)) --> min = store[index]
        }
    
    }
    return  index; // finally returning the index(color enumaration value) of the min distance value which is for example
                   // if it is 0 since enum RED value is 0 too RED will be retured to the color mixer function.
                   // or if it is 3 since enum YELLOW value is 3 too YELLOW will be retured to the color mixer function.
}



void color_mixer(Color Color1,Color Color2,Color (*funcp) (Color,Color)){
    // passing the euclidean distance function to pointer funcp (could be seen in main function).
    // then it will be used to determine which color is the closest to 
    
    char color1,color2;
    //double Vector1[3]={}; // First vector after user picks the first color. 
    //double Vector2[3]={}; // Second vector after user picks the second color. 
    printf("Enter Color 1 (r,g,b,y,o) : "); // picking the first color.
    scanf(" %c",&color1);
    if (color1=='r'){ // if red is picked 
        Color1=RED; // Enum type color1 will be red.
    }
    else if(color1=='g'){  // if green is picked. 
        Color1=GREEN; // Enum type color1 will be green.
    }
    else if(color1=='b'){ // if blue is picked. 
        Color1=BLUE; // Enum type color1 will be blue.
    } 
    else if(color1=='y'){ // if yellow is picked. 
        Color1=YELLOW; // Enum type color1 will be yellow.
    }
    else if(color1=='o'){ // if orange is picked. 
        Color1=ORANGE; // Enum type color1 will be orange.
    }
    else{
        printf("Invalid input!\n");    // error message if anything different from above is entered. 
    }


    printf("Enter Color 2 (r,g,b,y,o) : ");  // picking the second color.
    scanf(" %c",&color2);
    if (color2=='r'){ // if red is picked. 
        Color2=RED;  // Enum type color2 will be red.
    }
    else if(color2=='g'){ // if green is picked. 
        Color2=GREEN; // Enum type color2 will be green.
    }
    else if(color2=='b'){ // if blue is picked. 
        Color2=BLUE; // Enum type color2 will be blue.
    }
    else if(color2=='y'){ // if yellow is picked. 
        Color2=YELLOW; // Enum type color2 will be yellow.
        
    }
    else if(color2=='o'){ // if orange is picked. 
        Color2=ORANGE; // Enum type color2 will be orange.
    }
    else{
        printf("Invalid input!\n");  // error message if anything different from above is entered.
    }
    int color=funcp(Color1,Color2); // assigning pointer to function (funcp) to color and getting the result. 
     //also the returned color enumaration value from the euclidean distance function.
    printf("Mixed Color: ");
    
    if(color==RED){   // if the returned color value from the euclidean distance function is red it will be printed with its vector.
        printf("RED [1.0, 0.0, 0.0]\n");
    }
    else if(color==GREEN){ // if the returned color value from the euclidean distance function is green it will be printed with its vector.
        printf("GREEN [0.0, 1.0, 0.0]\n");
    }
    else if(color==BLUE){ // if the returned color value from the euclidean distance function is blue it will be printed with its vector.
        printf("BLUE [0.0, 0.0, 1.0]\n");
    }
    else if(color==YELLOW){
        printf("YELLOW [0.5, 0.5, 0.0]\n"); // if the returned color value from the euclidean distance function is yellow it will be printed with its vector.
    }
    else if(color==ORANGE){
        printf("ORANGE [0.5, 0.4, 0.2]\n"); // if the returned color value from the euclidean distance function is orange it will be printed with its vector.
    }
}




int X_Win(char XOX[3][3]){
    int i,j;
    int status = 0;
    for (j = 0; j<3; j++){ 
        for  (i = 0; i < 3; i++){
            if (XOX[i][j]=='X'){  // if the current index of the array is 'X'.
                if(i==0 && XOX[i][j]==XOX[i+1][j] && XOX[i+1][j]==XOX[i+2][j]){
                    status=1; // if it is vertical X's
                                                // X
                                                // X 
                    //X will win the game statues will be 1. 
                }
                else if(j==0 && XOX[i][j]==XOX[i][j+1] && XOX[i][j+1]==XOX[i][j+2]  ){
                    status=1;   // if it is horizontal X's --> XXX
                                // X will win.
                }
                else if(i==0 && j==0 && XOX[i][j]==XOX[i+1][j+1] && XOX[i+1][j+1]==XOX[i+2][j+2]){
                    status=1;  // if it diagonal from top left corner to right bottom corner as X.
                }              // X will win.
                else if(XOX[0][2]=='X' && XOX[0][2]==XOX[1][1] && XOX[1][1]==XOX[2][0]){
                    status=1; // if it diagonal from top right corner to left bottom corner as X.
                }             // X will win.
            }
        }
    }

    return status;
}

int O_Win(char XOX[3][3]){
    int i,j;
    int status2 = 0;
    for (i = 0; i<3; i++){
        for  (j = 0; j < 3; j++){
            if (XOX[i][j]=='O'){ // if the current index of the array is 'O'.
                if(j==0 && XOX[i][j]==XOX[i][j+1] && XOX[i][j+1]==XOX[i][j+2]){
                    status2=1; // if it is horizontal O's --> OOO
                                // O will win.
                } 
                else if(i==0 && XOX[i][j]==XOX[i+1][j] && XOX[i+1][j]==XOX[i+2][j]){
                    status2=1; // if it is vertical O's
                                // O
                                // O
                                // O
                }
                else if(i==0 && j==0 && XOX[i][j]==XOX[i+1][j+1] && XOX[i+1][j+1]==XOX[i+2][j+2]){
                    status2=1; // if it diagonal from top left corner to right bottom corner as O.
                }              // O will win.
                 
                else if(XOX[0][2]=='O' && XOX[0][2]==XOX[1][1] && XOX[1][1]==XOX[2][0]){
                    status2=1; // if it diagonal from top right corner to left bottom corner as O.
                }             // O will win.
            } 
        }
    }


                    return status2;
}
 

void part3(){
    char XOX[3][3] = {{'-','-','-'},{'-','-','-'},{'-','-','-'}} ; // board array
    int i,j;
    int Xr,Xc;
    int Or,Oc;
    int move=0;
    char play;
    while (move<=9)
    {

    printf("Player 1 (X), enter your move (row,column) : "); // asking player1 of the move he/she wants to do.
    scanf("%d %d",&Xc,&Xr); // X column (Xc) --- X row (Xr)
    if(Xc>=0 && Xc<3 && Xr>=0 && Xr<3 && XOX[Xc][Xr]=='-'){
    XOX[Xc][Xr] = 'X' ; // assigning 'X' to the entered row column. 
    move++;
    for(j=0;j<3;j++){ // printing the current status of the board after the moves.
            for(i=0;i<3;i++){
                printf("%c",XOX[j][i]);
                printf(" "); // space after each element is entered to make it look good after printing.
            }
            printf("\n"); // after each row newline will be printed to print the upcoming row.
        }
    }
    else{
        printf("Please enter valid input!\n"); // if a number except 0-2 entered or if X is entered to a position where 'O' or 'X' exists.
        printf("Player 1 (X), enter your move (row,column) : "); // asking player1 of the move he/she wants to do.
        scanf("%d %d",&Xc,&Xr);
        XOX[Xc][Xr] = 'X' ;
        move++;
        for(j=0;j<3;j++){ // printing the current status of the board after the moves.
                for(i=0;i<3;i++){
                    printf("%c",XOX[j][i]);
                    printf(" ");
                }
                printf("\n");
            }
        }
    int status1=X_Win(XOX); // getting the current status of the game from X_Win function.
    if(status1==1){
        printf("Player 1 (X) wins!\n"); // if status is 1 which means X won.
        break;
    }
    else if (move==9){ // if board is full which means all 9 element is completed but there is still no winner the game will finish with a tie.
        printf("No winner! It's a tie!\n"); // Tie message will be printed.
        break;
    }
    printf("Player 2 (O), enter your move (row,column) : "); // asking player2 of the move he/she wants to do.
    scanf("%d %d",&Oc,&Or);
    if(Oc>=0 && Oc<3 && Or>=0 && Or<3 && XOX[Oc][Or]=='-'){
    XOX[Oc][Or] = 'O' ;
    move++;
    for(j=0;j<3;j++){ // printing the current status of the board after the moves.
            for(i=0;i<3;i++){
                printf("%c",XOX[j][i]);
                printf(" ");
            }
            printf("\n");
        }
    }
    else{
        printf("Please enter valid input!\n"); // if a number except 0-2 entered or if O is entered to a position where 'O' or 'X' exists.
        printf("Player 2 (O), enter your move (row,column) : "); // asking player2 of the move he/she wants to do.
        scanf("%d %d",&Oc,&Or);
        XOX[Oc][Or] = 'O' ;
        move++;
        for(j=0;j<3;j++){ // printing the current status of the board after the moves.
                for(i=0;i<3;i++){
                    printf("%c",XOX[j][i]);
                    printf(" ");
                }
                printf("\n");
        }
    }
    int status2=O_Win(XOX); // getting the current status of the game from O_Win function.
    if(status2==1){ // if status is 1 which means O won.
        printf("Player 2 (O) wins!\n"); // O win message will be printed 
        break;
    }
  }
}



int main(){
    
    Color Color1;
    Color Color2;
    char play;
    color_mixer(Color1,Color2,euclidean_distance); 
            return 0; 
        
}