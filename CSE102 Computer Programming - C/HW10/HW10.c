#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 25
typedef struct Book { 
    char isbn[MAX];
    char title[MAX];
    char author[MAX];
    int year;
    char rstatus[MAX];
    struct Book * next;
} Book ;


typedef struct Student { 
    char name[MAX];
    char ID[MAX];
    struct Book * borrowed;
    struct Student * next;
} Student ;


void BorrowedList(Student *S,char* ID){
    Student *S1=S; // temporary Student pointer.
    while (S1!=NULL) 
    {
        if(strcmp(S1->ID,ID)==0){ // if the searched ID matches with a student ID in the linked list 
            printf("Borrowed Book of the student %s : \n",S1->name); // the name of the student and
            printf("ISBN of the borrowed book : %s\n",S1->borrowed->isbn); // the isbn of his/her borrowed book will be printed. 
            break;
        }
        S1=S1->next; // passing to the next Student until it is NULL which means the Student linked list came to an end.
    }   
}

void writetofile(Book *B){ // function to write the details of the books to file. 
    FILE *bfp; // file pointer
    bfp=fopen("books.txt","w"); // opening the file on writing mode.
    Book *NB;
    NB=B; // temp book pointer
    while (NB!=NULL)
    {
        fprintf(bfp,"%s,%s,%s,%d,%s\n",NB->isbn,NB->title,NB->author,NB->year,NB->rstatus); // writing the details of every books to file
        NB=NB->next; // passing to the next book until it is not null.
    }
    fclose(bfp); // closing the file.
    
}

void writetofile2(Student *S){ // function to write the details of the students to file. 
    FILE *sfp; // file pointer
    sfp=fopen("students.txt","w"); // opening the file on writing mode.
    Student *NS;
    NS=S; // temp student pointer
    while (NS!=NULL)
    {   
        if(NS->borrowed==NULL){
            fprintf(sfp,"%s,%s,None\n",NS->name,NS->ID); // writing the details of every students to file
        }
        else fprintf(sfp,"%s,%s,%s\n",NS->name,NS->ID,NS->borrowed->isbn); // writing the details of every students to file
        NS=NS->next; // passing to the next student until it is not null.
    }
    fclose(sfp); // closing the file.
    
}



void addBook(Book** B,char* isbn, char* title, char* author, int publicationYear, int method){

    Book* NB;
    NB=*B;

    if(method==1){ // insert to beginning
            Book *B1=(Book *)malloc(sizeof(Book)); // creating a new book linked list for the book that will be added.
            strcpy(B1->isbn,isbn); // then copying the values that will be added to the values of the new book.
            strcpy(B1->author,author);
            strcpy(B1->title,title);
            strcpy(B1->title,title);
            B1->year = publicationYear;
            strcpy(B1->rstatus,"Available"); // making the status Available at first.
            B1->next=*B; // to change the first book pointer we use a pointer to pointer to add to the beginning. 
            *B=B1;    
            //NB->next=B;
            writetofile(*B); // writing the updated book to the file after adding. 
    }
    else if (method==0) // insert to end
    {
        while(NB->next!=NULL){
            
            NB=NB->next; // passing to the last Book pointer.
        }
        FILE *lib1;
        lib1=fopen("books.txt","a"); 
        Book *B1=(Book *)malloc(sizeof(Book)); // creating a new book linked list for the book that will be added.
        strcpy(B1->isbn,isbn); // then copying the values that will be added to the values of the new book.
        strcpy(B1->author,author);
        strcpy(B1->title,title);
        strcpy(B1->title,title);
        B1->year = publicationYear;
        strcpy(B1->rstatus,"Available");  // making the status Available at first.
        B1->next=NULL; // making the next book of new book NULL.
        NB->next=B1; // after passing to the last book pointer the next which is NULL will be the new book.
        fprintf(lib1,"%s,%s,%s,%d,%s\n",B1->isbn,B1->title,B1->author,B1->year,B1->rstatus); // appending the new book values to the file.
        fclose(lib1);
    }
}


void deleteBook(Book* B,char* isbn){
    Book* prev; // prev and current book variables.
    Book *current;
    prev=B;
    current=B;
    while (current!=NULL) // passing to the next book until it is NULL or value is found.
    {
        if(strcmp(current->isbn,isbn)==0){ // if the isbn of the book that will be deleted is found.
            prev->next=current->next; // to delete it the next book of the previos book will be the next book of the current book(book that will be deleted).
            break;
        }
        prev=current; // passing the current to prev then current will pass to its next.
        current=current->next;
    }
    writetofile(B); // after everything is done the updated book will be written to the file.
}
 

void updateBook(Book* B,char* isbn, char* feature,char* value){

    Book* B1=B;
    while (B1!=NULL)
    {
        // if the searhed isbn is matched with an isbn of the book then 
        // according to the feature that will be updated
        // that feature will be changed with the entered value from the user.
        if(strcmp(B1->isbn,isbn)==0){ 
            if(strcmp(feature,"title")==0){
                strcpy(B1->title,value);
            }
            else if(strcmp(feature,"author")==0){
                strcpy(B1->author,value);
            }
            else if(strcmp(feature,"publication year")==0){
               B1->year=atoi(value); // converting string to integer and assigning B->year to the value.
            }
        }
        B1=B1->next; // passing to the next book until it is null.
    }
    writetofile(B); // after everything is done the updated book will be written to the file.
} 

Book* sortTitle(Book* head) {
    // Count the number of books
    int count = 0;
    Book* current = head;
    while (current != NULL) { // first counting the books in the list for sorting them in for loop with bubble sort.
        count++;
        current = current->next; // iterating through the next book until it is NULL.
    }

    // Perform bubble sort by title
    for (int i = 0; i < count - 1; i++) {
        current = head; // the temp head book will be the current book.
        for (int j = 0; j < count - 1 - i; j++) {
            if (strcmp(current->title, current->next->title) > 0) {
                // Swap books
                Book* temp = current->next; // swapping the books if current is bigger than its next.
                current->next = current->next->next;
                temp->next = current;

                if (j == 0) { // If the first book is swapped then updating the head of the list.
                    head = temp;
                } else {
                    Book* prev = head;
                    while (prev->next != current) { // Updating the pointer of the next book of previous book to the new book.
                        prev = prev->next;
                    }
                    prev->next = temp;
                }

                current = temp; // the updating the current book pointer after swapping.
            }
            current = current->next; // iterating through the next book to the end.
        }
    }

    return head;
}

Book* sortAuthor(Book* head) {
    // Count the number of books
    int count = 0;
    Book* current = head;
    while (current != NULL) { // first counting the books in the list for sorting them in for loop with bubble sort.
        count++;
        current = current->next; // iterating through the next book until it is NULL.
    }

    // Perform bubble sort by author
    for (int i = 0; i < count - 1; i++) {
        current = head; // the temp head book will be the current book.
        for (int j = 0; j < count - 1 - i; j++) {
            if (strcmp(current->author, current->next->author) > 0) {
                // Swap books 
                Book* temp = current->next; // swapping the books if current is bigger than its next.
                current->next = current->next->next;
                temp->next = current;

                if (j == 0) { // If the first book is swapped then updating the head of the list.
                    head = temp;
                } else {
                    Book* prev = head;
                    while (prev->next != current) {  // Updating the pointer of the next book of previous book to the new book.
                        prev = prev->next;
                    }
                    prev->next = temp;
                }

                current = temp; // the updating the current book pointer after swapping.
            }
            current = current->next; // iterating through the next book to the end.
        }
    }

    return head;
}

Book* sortYear(Book* head) {
    // Count the number of books
    int count = 0;
    Book* current = head;
    while (current != NULL) { // first counting the books in the list for sorting them in for loop with bubble sort.
        count++;
        current = current->next; // iterating through the next book until it is NULL.
    }

    // Perform bubble sort by publication year
    for (int i = 0; i < count - 1; i++) {
        current = head; // the temp head book will be the current book.
        for (int j = 0; j < count - 1 - i; j++) {
            if (current->year > current->next->year) {
                // Swap books
                Book* temp = current->next; // swapping the books if current is bigger than its next.
                current->next = current->next->next;
                temp->next = current;

                if (j == 0) { // If the first book is swapped then updating the head of the list.
                    head = temp; 
                } else {
                    Book* prev = head;
                    while (prev->next != current) {  // Updating the pointer of the next book of previous book to the new book.
                        prev = prev->next;
                    }
                    prev->next = temp;
                }

                current = temp; // the updating the current book pointer after swapping.
            }
        }
            current = current->next; // iterating through the next book to the end.
    }

    return head;
}


void filterAndSortBooks(Book* B, int filterChoice, char* filter, int sortChoice) {

    // NOTE : I DID NOT CHANGE THE TEXT FILE NOT TO LOSE THE BOOKS THAT DOES NOT MATCH THE FILTER CRITERIA.

    Book *filteredBooks = NULL;
    Book *currentBook = B;

    // Filtering books
    while (currentBook != NULL) {
        if (filterChoice == 0) { // Filter by author
            if (strcmp(currentBook->author, filter) == 0) {
                Book *filBook = (Book *)malloc(sizeof(Book)); // creating a new book for the filtered book.
                *filBook = *currentBook; // filBook to current book that matches the filter
                filBook->next = NULL; // next book of filbook is null

                // Add the filtered book to the list
                if (filteredBooks == NULL) { // if the main filtered books linked list is NULL 
                    filteredBooks = filBook; // then it will be the same with filBook since there is not any book in filteredBooks.
                } else { // if it is not NULL then passing till to the end of the filteredBooks linked list to add to the end.
                    Book *temp = filteredBooks;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = filBook;
                }
            }
        } else if (filterChoice == 1) { // Filter by publication year  
        // same process as the filter by author
            int year = atoi(filter);
            if (currentBook->year == year) {
                Book *filBook = (Book *)malloc(sizeof(Book));
                *filBook = *currentBook;
                filBook->next = NULL;

                // Adding the filtered book to the list same process as the filter by author
                if (filteredBooks == NULL) {
                    filteredBooks = filBook;
                } else {
                    Book *temp = filteredBooks;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = filBook;
                }
            }
        }

        currentBook = currentBook->next; // passing to the next book in the library till it is NULL.
    }

    // Sorting books
    if (sortChoice == 0) { // Sorting by title using the function I created above.
        filteredBooks = sortTitle(filteredBooks);
    } else if (sortChoice == 1) { // Soringt by author using the function I created above.
         filteredBooks = sortAuthor(filteredBooks);
    } else if (sortChoice == 2) { // Sorting by publication year using the function I created above.
         filteredBooks = sortYear(filteredBooks);
    }

    // Print filtered and sorted books
    Book *temp = filteredBooks;
    while (temp != NULL) {
        printf("ISBN: %s | Author: %s | Title: %s | Publication Year: %d | Reserve Status: %s\n",
               temp->isbn, temp->author, temp->title, temp->year, temp->rstatus);
        temp = temp->next;
    }

    // Free memory for filtered books
    while (filteredBooks != NULL) {
        Book *nextBook = filteredBooks->next;
        free(filteredBooks);
        filteredBooks = nextBook;
    }
}





void reverseBookList(Book** B) {
    Book* prev = NULL; // pointer to the previous book.
    Book* current = *B; // pointer to the current book which is a pointer to a pointer.
    Book* next = NULL; // pointer to the next book.
    while (current != NULL) { // passing through until current book is not null.
        next = current->next; // store the next book to the next book linked list.
        current->next = prev; // reversing the list by pointing the current book's next to the previous book.
        prev = current; // moving the pointers forward then current becomes previous and next becomes current.
        current = next;
    }
    *B = prev; // updating the first book pointer to the first of the list which is the last book since it is reversed
    writetofile(*B); // writing to file the new updated book linked list.
}



void searchBooks(Book* B,int searchChoice, char* searchValue){
    /*
    • searchChoice: The user's choice for searching the books. 0 for searching by ISBN number, 1 for searching by author, 2 for searching by title. 
    • searchValue: The value based on the search criteria. 
    */
    int found=0;
    Book *B1=B; // temporary book pointer
    while (B1!=NULL)
    {
        if(searchChoice==0){ 
            if(strcmp(B1->isbn,searchValue)==0){ // if isbn of the book is matched the search value then it will be printed.
                printf("Book is found!\nThe details of the book is below : \nISBN : %s\nAuthor : %s\nTitle : %s\nPublication Year : %d\n",B1->isbn,B1->author,B1->title,B1->year);
                found=1; // changing the found value to not print 'There is no books matching this criteria!' from below.
            }
        }
         else if(searchChoice==1){
            if(strcmp(B1->author,searchValue)==0){ // if author of the book is matched with the search value then it will be printed.
                printf("Book is found!\nThe details of the book is below\nISBN : %s\nAuthor : %s\nTitle : %s\nPublication Year : %d\n",B1->isbn,B1->author,B1->title,B1->year);
                found=1; // changing the found value to not print 'There is no books matching this criteria!' from below.
            }
        }
        else if(searchChoice==2){
            if(strcmp(B1->title,searchValue)==0){ // if title of the book is matched with the search value then it will be printed.
                printf("Book is found!\nThe details of the book is below\nISBN : %s\nAuthor : %s\nTitle : %s\nPublication Year : %d\nReserve Status : %s\n",B1->isbn,B1->author,B1->title,B1->year,B1->rstatus);
                found=1; // changing the found value to not print 'There is no books matching this criteria!' from below.
            }
        }
        B1=B1->next; // passing to the next book until it is NULL which means the book linked list came to an end.
    }
    if(found==0){ // if found is 0 which means book is not found for the searched value.
        printf("There is no books matching this criteria!\n"); // then a message will be printed that states there is no book for that criteria.
    }
        
    
   
}

void borrowBook(Book* B,Student* S,char* isbn,char *ID){
    printf("Enter the isbn of the book to be borrowed : ");
    scanf("%s",isbn);
    printf("Enter the ID of the student who will borrow the book : ");
    scanf("%s",ID);
    Book * BB=(Book *)malloc(sizeof(Book)); // the new book pointer for the borrowed book of a student.  
    Book *B1=B; // temp variables.
    Student* S1=S;
    while (B1!=NULL) 
    {
        if(strcmp(B1->isbn,isbn)==0){ // if the searhed isbn is matched with an isbn of the book then 
            if(strcmp(B1->rstatus,"Reserved")==0){ // if it is already reserved an error message will be printed.
                printf("This book is already reserved!Please try borrowing another book!\n");
                break;
            }
            else{ // if not
                printf("Book is found!\n"); // printing that book is found and
                BB=B1; // the new book will be the book that matched with the isbn.
                break;
            }   
        }
        B1=B1->next; // passing to the next book until it is NULL which means the book linked list came to an end.
    }

    while (S1!=NULL)
    {
        if(strcmp(S1->ID,ID)==0){ // if the searhed ID is matched with an ID of a student then 
            strcpy(BB->rstatus,"Reserved"); // the status of the book will be reserved and
            S1->borrowed=BB; // the borrowed book pointer of the student will be that searched book. 
            //break;
        }
        S1=S1->next; // passing to the next student until it is NULL which means the student linked list came to an end.
    }
    writetofile(B); // writing the updated linked lists to the file.
    writetofile2(S);

} 


void returnBook(Book* B,Student* S,char* isbn,char *ID){
    printf("Enter the isbn of the book to be returned : ");
    scanf("%s",isbn);
    printf("Enter the ID of the student who will return the book : ");
    scanf("%s",ID);
    Book *B1=B;
    Student* S1=S;
    while (B1!=NULL)
    {
        if(strcmp(B1->isbn,isbn)==0){ // if the book is found with the isbn then loop will be stopped and B1 will be the pointer that points to the searched book.
            break;   
        }
        B1=B1->next; // passing to the next book until it is NULL which means the book linked list came to an end.
    }

    while (S1!=NULL)
    {
        if(strcmp(S1->ID,ID)==0){ // if the searhed ID is matched with an ID of a student then 
                if(S1->borrowed==B1){ // if borrowed book of the student is the same with the book that will be returned then
                    strcpy(B1->rstatus,"Available"); // the status will be changed to available and 
                    S1->borrowed=NULL; // borrowed book of the student will be null.
                }
            break;
        }
        S1=S1->next; // passing to the next student until it is NULL which means the student linked list came to an end.
    }
    writetofile(B); // writing the updated linked lists to the file.
    writetofile2(S);
}

void displayBooks(Book* B){ // function to pass through all the books and print them until it is NULL.
    Book *NB=B;
    while (NB!=NULL)
    {
        if(NB!=NULL){
           printf("ISBN : %s|Author : %s|Title : %s|Publication Year : %d|Reserve Status : %s\n",NB->isbn,NB->author,NB->title,NB->year,NB->rstatus);
            NB=NB->next; 
        }
        
    }
    
}


void displayStudents(Student* S){ // function to pass through all the students and print them until it is NULL.
    while (S!=NULL)
    {
        if(S->borrowed!=NULL){
            printf("Student Name : %s | Student ID : %s | Borrowed Book ISBN : %s\n",S->name,S->ID,S->borrowed->isbn);
        }
        else{
            printf("Student Name : %s | Student ID : %s | Borrowed Book : None\n",S->name,S->ID);
        }
        S=S->next;
    }
    
}

int main(){
    int choice; // menu choice
    // Book structure
    char isbn[MAX];
    char title[MAX];
    char author[MAX];
    int year;
    char reserve[MAX];
    Book *B=(Book *)malloc(sizeof(Book));
    // Student Structure
    char name[MAX];
    char ID[MAX];
    Student *S=(Student *)malloc(sizeof(Student));
    // file operations
    FILE *bfp;
    FILE *sfp;
    bfp=fopen("books.txt","r"); // opening both of the files.
    sfp=fopen("students.txt","r"); 
    Book *btemp=B;
    int count=0;
    while (fscanf(bfp,"%[^,],%[^,],%[^,],%d,%[^\n]\n",isbn,title,author,&year,reserve)!=EOF) // passing through the file until EOF
    {
        Book *NB=(Book *)malloc(sizeof(Book)); // for every line of the file a new book will be created and values will be copied
        strcpy(NB->isbn,isbn); 
        strcpy(NB->author,author);
        strcpy(NB->title,title);
        NB->year=year;
        strcpy(NB->rstatus,reserve);
        // then will be passed to the next book.
        if(count==0){ // if count is 0 which means btemp is NULL then btemp will be the new book and main book B will be the new book too.
            btemp=NB; 
            B=NB;
        }
        else{ // if btemp is not NULL then next of btemp will be the new book and btemp will be passed to the next book
            btemp->next=NB;
            btemp=btemp->next;
        }
        count++;
    }
    char bisbn[MAX];
    Student *temp=S;
    int count2=0;
    while (fscanf(sfp,"%[^,],%[^,],%s\n",name,ID,bisbn)!=EOF)  // passing through the file until EOF
    {
        Student *NS=(Student *)malloc(sizeof(Student)); // for every line of the file a new student will be created and values will be copied
        strcpy(NS->name,name);
        strcpy(NS->ID,ID);
        // then will be passed to the next student.
        if(count2==0){ // if count is 0 which means temp is NULL then temp will be the new student and main student S will be the new student too.
            temp=NS;
            S=NS;
        }
        else{ // if temp is not NULL then next of temp will be the new student and temp will be passed to the next student.
            temp->next=NS;
            temp=temp->next;
        }
        count2++;
    }

    while(choice!=12){
            // Menu of the game
            printf("--------------------------------------------\n");
            printf("Welcome to the Library Management System!\n");
            printf("1.Display all the books in the library\n");
            printf("2.Display all the students in the system\n");
            printf("3.Add book to the library\n");
            printf("4.Delete book from library\n");
            printf("5.List the books a student borrowed\n");
            printf("6.Update a book\n");
            printf("7.Filter and sort books\n");
            printf("8.Reverse book list\n");
            printf("9.Search a book from the library\n");
            printf("10.Borrow a book\n");
            printf("11.Return a borrowed book\n");
            printf("12.Exit\n");
            printf("Enter your choice : ");
            scanf("%d",&choice);
            switch (choice)
            {
                // switch case for all the cases
                // all the functions of cases and the values that is wanted to be entered from the user is below.
                    case 1:
                        displayBooks(B);
                        break;
                    case 2:
                        displayStudents(S);
                        break;
                    case 3:
                        char isbn2[MAX];
                        char title[MAX];
                        char author[MAX];
                        int publicationYear;
                        int method;
                        printf("Enter the isbn of the book to be added : ");
                        scanf("%s",isbn2);
                        printf("Enter the title of the book to be added : ");
                        scanf(" %[^\n]",title);
                        printf("Enter the author of the book to be added : ");
                        scanf(" %[^\n]",author);
                        printf("Enter the publication year of the book to be added : ");
                        scanf("%d",&publicationYear);
                        
                        printf("Enter the method : ");
                        scanf("%d",&method);
                        addBook(&B,isbn2,title,author,publicationYear,method);
                        break;
                    case 4:
                        char isbn3[MAX];
                        printf("Enter the isbn of the book to be removed : ");   
                        scanf("%s",isbn3);
                        deleteBook(B,isbn3);
                        break;
                    case 5:
                        char ID1[20];
                        printf("Enter the ID of the student whose borrowed books will be listed : ");
                        scanf("%s",ID1);
                        BorrowedList(S,ID1);
                        break;
                    case 6:
                        char isbn4[MAX];
                        char feature[MAX];
                        char value[MAX];
                        printf("Enter the isbn of the book to be updated : ");
                        scanf("%s",isbn4);
                        printf("Enter the feature : ");
                        scanf(" %[^\n]",feature);
                        printf("Enter the value : ");
                        scanf(" %[^\n]",value);
                        updateBook(B,isbn4,feature,value);
                        break;  
                    case 7:
                        char filter[MAX];
                        int fchoice,schoice; 
                        printf("Enter the filter choice : ");
                        scanf("%d",&fchoice);
                        printf("Enter the filter criteria : ");
                        scanf(" %[^\n]",filter);
                        printf("Enter the sort choice : ");
                        scanf("%d",&schoice);
                        filterAndSortBooks(B,fchoice,filter,schoice);
                        break;
                    case 8:
                        reverseBookList(&B);
                        break;  
                    case 9:
                        int sch;
                        char sval[MAX];
                        printf("Enter the search choice : ");
                        scanf("%d",&sch);
                        printf("Enter the search value : ");
                        scanf(" %[^\n]",sval);
                        searchBooks(B,sch,sval);
                        break;
                    case 10:
                        char isbn5[MAX];
                        char ID2[MAX];
                        borrowBook(B,S,isbn5,ID2);
                        break;
                    case 11:
                        char isbn6[MAX];
                        char ID3[MAX];
                        returnBook(B,S,isbn6,ID3);
                        break;
                    case 12:
                        printf("Exiting from the program...\nHave a nice day!\n");
                        break;
                    default:
                        break;
                }
            }

    fclose(bfp); // closing both files.
    fclose(sfp);
}