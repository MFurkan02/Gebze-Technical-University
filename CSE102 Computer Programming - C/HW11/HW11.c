#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 25


void option3(){
    printf("Listing Family Trees : \n");
    
    printf("Select an operation : \n\n");

    printf("1-Add new person\n");
    printf("2-Remove a person\n");
    printf("3-Print a Person's nuclear family\n");
    printf("4-Search relatives of given person\n");
    printf("5-Return to the main menu\n");
    //printf("%s has been selected.\n");

    /*
    2
    person[MAX];
    printf("Enter the name of the person you want to remove : ");
    scanf("%s",person);
    printf("Warning : Alice has children, removing Alice will also remove her children.\n");
    printf("Do you still want to continue?");
    char ans[MAX];
    scanf("%s",ans);
    if(strcmp("Yes",ans)==0){
        printf("%s and her children have been removed from the family tree.\n");
    }
    */
    /*
    3
    printf("Enter the name of the person : %s");
    scanf("%s");
    printf("Printing nuclear family of %s :\n");
    printf("Father : %s");
    printf("Mother : %s");
    printf("Siblings : %s");
    printf("Children : %s");
    /*
    4
    printf("Enter the name of the person\n");
    scanf("%s\n");

    printf("Select type of relatives :\n");
    printf("1-Parents\n");
    printf("2-Spouse\n");
    printf("3-Children\n");
    printf("4-Grandparents\n");
    printf("5-Cousins\n");
    int choice;
    scanf("%d",choice);
    */
}

void menu(){
    printf("*****************************");
    printf("Family Tree System Menu :\n");
    printf("1-Add Family Tree\n");
    printf("2-Remove Family Tree\n");
    printf("3-Load Family Tree\n");
    printf("4-Exit\n");
    int choice;
    printf("> ");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        int choice1;
        printf("1-Add Node\n");
        printf("2-Save Family Tree\n");
        printf("3-Exit\n");
        printf("> ");
        scanf("%d",&choice1);
        if(choice1==1){
            char name[MAX];
            char mother[MAX];
            char father[MAX];
            int age;
            printf("Enter the name of the person : ");
            scanf("%s",name);
            printf("Enter the age of the person : ");
            scanf("%d",&age);
            printf("Enter the name of the mother : ");
            scanf("%s",mother);
            printf("Enter the name of the father : ");
            scanf("%s",father);
        }
        else if (choice1==2)
        {
            printf("Tree has been saved!\n");
        }
        else{
            printf("Exiting from the program...Have a nice day!\n");
        }
        break;
    case 2:
        break;
    case 3:
        option3();
        break;
    case 4:
        printf("Exiting from the program...Have a nice day!\n");
        break;
    default:
        break;
    }

}



int main(){
    menu();
}