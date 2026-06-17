#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Size 50



void printf_product(FILE* receipt,int dish,int amount){

    FILE *fp;
    char ch;
    int flag = 0;
    int counter=0;
    int cch=0;
    fp = fopen("menu.txt", "r");
    

    if (fp == NULL) {
        printf("Unable to open file.");
    }
    if (amount==0 || amount==1){ // if amount is either 1 or 0 (1* or 0* will not be printed ," " will be printed instead.
        fprintf(receipt,"%s"," ");
    }
    else{
        fprintf(receipt," %d* ",amount); // else amount* will be printed (2* , 3* ,etc.).
    }   
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == ' ') {
            flag = 1; // setting the flag when a space is encountered so the prices will not be printed to the file.
        } else if (ch == '\n') {
            counter++;
            flag = 0; // reset the flag at the end of each line to print the products(dishes).
        } else if (flag==0 && counter==dish) {
            cch++;
            fprintf(receipt,"%c", ch); // print the character if the flag is not set.(flag=0)
        }
        
    }
    if (cch<=7){ // if character size of the dish is lower than 7 (Kebap,Sütlaç,Pizza) an extra tab will be printed to provide the indent of the prices. 
        fprintf(receipt,"%s","\t\t"); 
    }
    else{
    fprintf(receipt,"%s","\t");  // printing tab afer each product is printed to the file.
    }    
    fclose(fp);
    
}
float prices(int dish,int amount){

    FILE *fp;
    FILE* receipt;
    fp = fopen("menu.txt", "r");
    int c;
    float price;
    int counter=0;

    while ((c = fgetc(fp)) != EOF) {
        if(c=='\n'){ // if a newline encountered counter will be incremented.
            counter++;
        }
        
        if (c >= '0' && c <= '9') { // if the character is a number(0-9).
            if((counter)==dish){ // if counter is equal to the entered dish number 5--> (İskender).
            ungetc(c, fp); // the number will be unget to get the whole float price.
            fscanf(fp, "%f", &price); //price will be read.
            if(amount==0)
            price = price; // if amount is zero price will be the same instead of 0.
            else{
                price=amount*price; // else price will be multiplied by the given amount.
            }
            
        }
    }
}
    fclose(fp);
     return price; //price will be returned each time the function called in part1 to get the total price. 
}


   


void printf_prices(FILE* receipt,int dish,int amount){

    FILE *fp;
    fp = fopen("menu.txt", "r");
    int c;
    float price;
    int counter=0;

    while ((c = fgetc(fp)) != EOF) {
        if(c=='\n'){ // if a newline encountered counter will be incremented.
            counter++;
        }
        
        if (c >= '0' && c <= '9') { // if the character is a number(0-9).
            if(counter==dish){ // if counter is equal to the entered dish number 5--> (İskender).
            ungetc(c, fp); // the number will be unget to get the whole float price.
            fscanf(fp, "%f", &price); //price will be read.
            if(amount!=0 && amount!=1){
            price = amount*price; // if amount is not zero price will be multiplied by the given amount.
            fprintf(receipt,"%s","\t\t\t\t"); // price will be printed after printing tabs.
            fprintf(receipt,"%.2f\n", price);
            }
            else if(amount==1 || amount==0){
            price=price; // if amount is zero or one price will be the same.
            fprintf(receipt,"%s","\t\t\t\t"); // instead this time 5 tabs will be printed before the price.
            fprintf(receipt,"%.2f\n", price);
            }

    
            }
        }
    }
    fclose(fp);
}

void image_of_receipt(){
    FILE* receipt;
    receipt=fopen("receipt.txt","r");
    printf("Displaying the image of the receipt...\n");
    char ch; //After everything is written to receipt file everytihng on the receipt file will be printed to the user.
    while (fscanf(receipt,"%c",&ch)!=EOF){
             printf("%c",ch);
    }
    fclose(receipt);
}



void part1(){

        FILE* menu;  
        FILE* receipt;
        char ch,st;
        int dish,amount;
        float total;
        int p1;
        float discount=0;

        char dish1;
        int x;
    
       
        time_t t ;
        struct tm *tmp ; // needed structure and variables to print the time using <time.h>
        char MY_TIME[Size];
        time( &t ); 
        tmp = localtime( &t );
        
        // using strftime to display time
        strftime(MY_TIME, sizeof(MY_TIME), "%d.%m.2023/%H:%M\n", tmp); // printing the time using <time.h>

        menu = fopen("menu.txt", "r");
        receipt= fopen("receipt.txt","w+"); // Opening the files.

        fprintf(receipt,"%s\t\t\t","220104004964"); // Printing the student number.
        fprintf(receipt,"%s",MY_TIME); // Printing the time.
        fprintf(receipt,"%s","------------------------------------------------------------\n");
        fprintf(receipt,"%s"," Product\t\t\t\t\tPrice (TL)\n");  // Printing the headers and signs.
        fprintf(receipt,"%s","------------------------------------------------------------\n\n");

        printf("Yemek Listesi : \n");
        int co=1;
        int flag=-1;
        float price;
        while(fscanf(menu,"%c",&dish1)!=EOF){ //character by character
            if (dish1=='0' || dish1=='1' || dish1=='2'|| dish1=='3'|| dish1=='4'|| dish1=='5'|| dish1=='6'|| dish1=='7' || dish1=='8'|| dish1=='9' || dish1=='.'){
                continue; // not to read the prices of the products in the menu file.(0-9)
            }
            if(dish1=='\n' && flag==-1){
                flag=0; // not to read the first line (Product Prices(TL)).
            }
            if(dish1=='\n' && flag==0){
                printf("\n");
                if (co!=0)
                printf("%d. ",co); // Printing 1. , 2. etc. if newline is read from menu txt file.
                co++;
                
                
            }
            if(dish1!='\n' && flag==0){
            printf("%c",dish1); // Printing the products if it is not newline.
            }
        }
   
        printf("\n");
        total = 0;
        amount=-1;
        while (amount!=0)  // While the wanted serving is not zero product and amount will be asked.
         {

            printf("Please choose a product (1-%d): ",co-1); // co is the dish counter -1 is because of the last line counted.  
            scanf("%d",&dish);
            
            printf("How many servings do you want? ");
            scanf("%d",&amount);
            
            if (amount<0 || dish<=0 || dish>(co-1) ){ // if dish is bigger than the product or amount and dish is smaller than 0.
                printf("Please enter a valid product or amount number!\n");
            }
            else{
            printf_product(receipt,dish,amount); //printing the product using printf_product() function.
            printf_prices(receipt,dish,amount);  //printing the prices using printf_prices() function.
            total+=prices(dish,amount); // getting the prices of the dished using prices() function.
            }
            

        }
        
        printf("Are you student? (Y/N): "); // Asking if the customer is a student or not.
        scanf(" %c",&st);
        

        fprintf(receipt,"%s"," Total:\t\t\t\t\t\t"); // Printing the total price without discount to txt file.
        fprintf(receipt,"%.2f\n",total); //\t\t\t\t\t\t
        
        double discount2=0;
        if (total > 150){
            discount2 = -1*(total*0.1);
            fprintf(receipt,"%s"," %10 Discount:\t\t\t\t\t");   // It will be printed to the receipt txt file.
            fprintf(receipt,"%.2f\n",discount2);  
        }
        if(st=='Y'){
            discount = -1*(total*(12.5/100));      // Discount if customer is a student.
            fprintf(receipt,"%s"," Student Discount:\t\t\t\t");   // It will be printed to the receipt txt file.
            fprintf(receipt,"%.2f\n",discount);     
        }
        else if(st=='N'){
            total = total; // Total will be the same if customer is not a student.
        }
        else{
            printf("Please enter a valid answer!\n"); // If an answer different than Y or N is entered.
            printf("Are you student? (Y/N): "); // Asking if the customer is a student or not.
            scanf(" %c",&st);
        }
        fprintf(receipt,"%s","------------------------------------------------------------\n");
        fprintf(receipt,"%s"," Price Payable:\t\t\t\t\t");  // Printing the prices to the receipt txt.
        total = total+discount+discount2;  // price payable after the discounts. 
        fprintf(receipt," %.2f\n",total); // fprinting the payable price.
        float VAT=total*1.18;
        fprintf(receipt,"%s"," Price + VAT:\t\t\t\t\t");
        fprintf(receipt," %.2f\n",VAT);
        fclose(receipt);
        image_of_receipt(); // Output the image of the receipt.
        fclose(menu);

}

void part2(){
    #define STONE 1
    #define PAPER 2
    #define SCISSORS 3

    int my_choice,your_choice;
    int status =1 ;

 // Giving play 'Y' to be in while loop at the beginning of the game.

    while (status==1 ) // In a loop until the player wants to play the game again('Y').
     {

    printf("Please make a choice!\n");  // Beginning of the game
    printf("1: Stone, 2: Paper, 3:Scissors\n");
    scanf("%d",&my_choice);  
    
    while(my_choice<1 || my_choice>3) { // If I pick a number except 1-3.
        printf("Please enter a number between 1-3!\n"); // Error message to enter 1-3.
         printf("1: Stone, 2: Paper, 3:Scissors\n"); 
          scanf("%d",&my_choice);
        }
        srand( time(0));
        your_choice = (rand() % 3) + 1; // Random number between 1-3.
        srand( time(0));  // To generate different random number in each game.

        switch (your_choice)
        {
        case STONE : // If the computer choses STONE.
            printf("You chose Stone. "); 
            if (my_choice == 1) // If I chose STONE too.
                printf("I chose Stone. It's a tie!\n");
            else if (my_choice == 2) // If I chose Paper.
                printf("I chose Paper. I won!\n");
            else if (my_choice == 3) // If I chose Scissors.
                printf("I chose Scissors. You won!\n");
            break;
        case PAPER : // If the computer choses PAPER.
            printf("You chose Paper. "); 
            if (my_choice == 1)  // If I chose PAPER too.
                printf("I chose Stone. You won!\n");
            else if (my_choice == 2)  // If I chose Paper.
                printf("I chose Paper. It's a tie!\n");
            else if (my_choice == 3) // If I chose Scissors.
                printf("I chose Scissors. I won!\n");
            break; 
        case SCISSORS : // If the computer choses SCISSORS.
            printf("You chose Scissors. ");
            if (my_choice == 1) // If I chose SCISSORS too.
                printf("I chose Stone. I won!\n");
            else if (my_choice == 2)  // If I chose Paper.
                printf("I chose Paper. You won!\n");
            else if (my_choice == 3) // If I chose Scissors.
                printf("I chose Scissors. It's a tie!\n");
            break;
        default:
            printf("Error!\n"); // Error message if computer somehow choses a number except 1-3.
            break;
        }
                char play;
                printf("Do you want to play again? (Y/N): "); // Asking if the user wants to play again.
                scanf(" %c",&play);
                if (play== 'N' ){ // If user enter 'N' the game will finish.
                    status=0;    
                }
                else if (play== 'Y' ){ // If user enter 'N' the game will finish.
                    status=1;   
                }
                else if(play!='N' || play!='Y' ){
                    printf("Please enter Y or N\n");
                    printf("Do you want to play again? (Y/N): "); // Asking if the user wants to play again.
                    scanf(" %c",&play);
                } 
    }
}

int main() {
    printf("-----------PART 1-------------\n");
    part1();
    printf("-----------PART 2-------------\n");
    part2();

      return 0;

}