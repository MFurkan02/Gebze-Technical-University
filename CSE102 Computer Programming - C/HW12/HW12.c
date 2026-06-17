#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node { 
   void* data; 
   struct Node* next;
} Node;

 typedef struct {
    char type[20]; 
    int ivals[1];
    double svals[1];
} Asset1; 

typedef struct { 
    char type[20]; 
    double svals[2];
    int ivals[2];
} Asset2; 

typedef struct { 
    char type[20]; 
    char string1[50];
    char string2[50];
} Asset3;

typedef struct { 
    char type[20]; 
    double value1;
    float value2; 
    double value3;
} Asset4;

void display(Node *head){ // displaying the linked list according to the asset type of each.
    Node *current=head;
    while (current != NULL) {
        if (strcmp(((Asset1*)current->data)->type, "Asset1") == 0) {

            printf("%s,%d,%.2lf\n",((Asset1*)current->data)->type,((Asset1*)current->data)->ivals[0], ((Asset1*)current->data)->svals[0]);
        
        }
        else if (strcmp(((Asset2*)current->data)->type, "Asset2") == 0) {

            printf("%s,%d,%d,%.2lf,%.2lf\n",((Asset2*)current->data)->type,((Asset2*)current->data)->ivals[0], ((Asset2*)current->data)->ivals[1],((Asset2*)current->data)->svals[0], ((Asset2*)current->data)->svals[1]);
        
        }   
        else if (strcmp(((Asset3*)current->data)->type, "Asset3") == 0) {
            
            printf("%s,%s,%s\n",((Asset3*)current->data)->type,((Asset3*)current->data)->string1, ((Asset3*)current->data)->string2);
        
        }
        else if (strcmp(((Asset4*)current->data)->type, "Asset4") == 0) {
            
            printf("%s,%.2lf,%.2f,%.2lf\n",((Asset4*)current->data)->type,((Asset4*)current->data)->value1, ((Asset4*)current->data)->value2,((Asset4*)current->data)->value3);
        
        }
        current = current->next;
    }
}


void fillLinkedList(struct Node** head){

    int num_asset ;
    int count=0;
    int count3=1;
    num_asset = (rand() % 11) + 10 ; // random asset number
    printf("Selected number of assets is %d\n",num_asset);
    Node *current= *head;
    while (count<num_asset)
    {
        int pick = rand() % 4 + 1; // picking the asset type randomly
        Node *new=(Node *)malloc(sizeof(Node)); // allocating memory for the new node.
        new->next=NULL;

        if(pick==1){ // if the type is 1.
            Asset1 * A1 =(Asset1 *)malloc(sizeof(Asset1)); // allocating memory for the structure and filling its datas.
            strcpy(A1->type,"Asset1"); 
            int ivals1= rand() % 100 ;
            A1->ivals[0]=ivals1;
            double dvals1= rand() % 100 ;
            A1->svals[0]=dvals1;
            new->data=A1; // then making data of the new node the filled new structure
        }

        else if(pick==2){  // if the type is 2.
            Asset2 * A2 =(Asset2 *)malloc(sizeof(Asset2)); // allocating memory for the structure and filling its datas.
            strcpy(A2->type,"Asset2");
            int ivals1= rand() % 100 ;
            A2->ivals[0]=ivals1;
            int ivals2= rand() % 100 ;
            A2->ivals[1]=ivals2;
            double dvals1= (rand() % 10000) / 100.00;
            A2->svals[0]=dvals1;
            double dvals2= (rand() % 10000) / 100.00 ;
            A2->svals[1]=dvals2;
            new->data=A2;
        }

        else if(pick==3){  // if the type is 3.
            Asset3 *  A3 =(Asset3 *)malloc(sizeof(Asset3)); // allocating memory for the structure and filling its datas.
            strcpy(A3->type,"Asset3");
            sprintf(A3->string1,"random%d",count3);
            count3++;
            sprintf(A3->string2,"random%d",count3);
            count3++;
            new->data=A3; // then making data of the new node the filled new structure
        }

        else {  // if the type is 4.
            Asset4 * A4 =(Asset4 *)malloc(sizeof(Asset4)); // allocating memory for the structure and filling its datas.
            strcpy(A4->type,"Asset4");
            double val1= (rand() % 10000) / 100.00 ;
            float val2= (rand() % 10000) / 100.00 ;
            double val3= (rand() % 10000) / 100.00;
            A4->value1=val1;
            A4->value2=val2;
            A4->value3=val3;
            new->data=A4; // then making data of the new node the filled new structure
        }
        count++;
        if(current==NULL){ // if the list is NULL
            *head=current=new; // assigning the new node to head and current.
        }
        else{ // if the list is not NULL
            current->next=new; // assigning next of the current to new node.
            current=current->next; // passing to the next.
        }
    }
}

void serializeLinkedList(struct Node* head){  
    FILE *fp;
    fp=fopen("linkedlist.bin","wb");
    Node *current=head;
    while (current!=NULL)
    {
        // for each data of the linked list comparing the type of the structure then writing to the binary file according to its type.
        if(strcmp(((Asset1*)current->data)->type,"Asset1")==0){
            fwrite(current->data,sizeof(Asset1),1,fp);
        }
        else if(strcmp(((Asset2*)current->data)->type,"Asset2")==0){
            fwrite(current->data,sizeof(Asset2),1,fp);
        }
        else if(strcmp(((Asset3*)current->data)->type,"Asset3")==0){
            fwrite(current->data,sizeof(Asset3),1,fp);
        }
        else if(strcmp(((Asset4*)current->data)->type,"Asset4")==0){
            fwrite(current->data,sizeof(Asset4),1,fp);
        }
        current=current->next;
    }
    fclose(fp);
    printf("Linked List has been written to the binary file and serialized!\n");
}

void deserializeLinkedList(struct Node** head){
    FILE *fp;
    fp=fopen("linkedlist.bin","rb");
    Node *current=*head;
    while (!feof(fp))
    {
        Node *new=(Node *)malloc(sizeof(Node)); // allocating memory for new node.
        new->next=NULL;
        Asset1 *A1=(Asset1 *)malloc(sizeof(Asset1)); // allocating memory for the Asset1.
        fread(A1, sizeof(Asset1), 1, fp);
        if(strcmp(A1->type,"Asset1")==0){ // if the read structure is Asset1.
            new->data = A1; // assigning the data of the list to A1.
        }
        else{
            fseek(fp, -sizeof(Asset1), SEEK_CUR);
            free(A1);
            Asset2 *A2=(Asset2 *)malloc(sizeof(Asset2)); // allocating memory for the Asset2.
            fread(A2, sizeof(Asset2), 1, fp);
            if(strcmp(A2->type,"Asset2")==0){ // if the read structure is Asset2.
                new->data = A2;  // assigning the data of the list to A2.
            }
            else{
                fseek(fp, -sizeof(Asset2), SEEK_CUR);
                free(A2);
                Asset3 *A3=(Asset3 *)malloc(sizeof(Asset3)); // allocating memory for the Asset3.
                fread(A3, sizeof(Asset3), 1, fp);
                if( strcmp(A3->type,"Asset3")==0){  // if the read structure is Asset3.
                    new->data = A3;  // assigning the data of the list to A3.
                }
                else{
                    fseek(fp, -sizeof(Asset3), SEEK_CUR);
                    free(A3);
                    Asset4 *A4=(Asset4 *)malloc(sizeof(Asset4)); // allocating memory for the Asset4.
                    fread(A4, sizeof(Asset4), 1, fp);
                    if(strcmp(A4->type,"Asset4")==0){ // if the read structure is Asset4.
                        new->data = A4;   // assigning the data of the list to A4.
                    }
                    else{
                        break;
                    }

                }
            }

        }
        if(current==NULL){ // if the list is NULL.
            *head=new;  // assigning the new node to head and current.
            current=new;
        }
        else{  // if the list is not NULL
            current->next=new;  // assigning next of the current to new node.
            current=current->next;   // passing to the next.
        }
    }
    fclose(fp);
    printf("Linked List has been read from the binary file and deserialized!\n");
}


/*
void deserializeLinkedList(struct Node** head) {
    FILE *fp;
    fp = fopen("linkedlist.bin", "rb");
    Node *current = *head;
    
    while (1) {
        Node *newNode = (Node*)malloc(sizeof(Node));
        newNode->next = NULL;
        Asset1 *A1 = (Asset1*)malloc(sizeof(Asset1));
        
        if (fread(A1, sizeof(Asset1), 1, fp) != 1) {
            free(A1);
            free(newNode);
            break;
        }
        
        if (strcmp(A1->type, "Asset1") == 0) {
            newNode->data = A1;
        }
        else {
            fseek(fp, -sizeof(Asset1), SEEK_CUR);
            free(A1);

            Asset2 *A2 = (Asset2*)malloc(sizeof(Asset2));
            if (fread(A2, sizeof(Asset2), 1, fp) != 1) {
                free(A2);
                free(newNode);
                break;
            }

            if (strcmp(A2->type, "Asset2") == 0) {
                newNode->data = A2;
            }
            else {
                fseek(fp, -sizeof(Asset2), SEEK_CUR);
                free(A2);

                Asset3 *A3 = (Asset3*)malloc(sizeof(Asset3));
                if (fread(A3, sizeof(Asset3), 1, fp) != 1) {
                    free(A3);
                    free(newNode);
                    break;
                }

                if (strcmp(A3->type, "Asset3") == 0) {
                    newNode->data = A3;
                }
                else {
                    fseek(fp, -sizeof(Asset3), SEEK_CUR);
                    free(A3);

                    Asset4 *A4 = (Asset4*)malloc(sizeof(Asset4));
                    if (fread(A4, sizeof(Asset4), 1, fp) != 1) {
                        free(A4);
                        free(newNode);
                        break;
                    }

                    if (strcmp(A4->type, "Asset4") == 0) {
                        newNode->data = A4;
                    }
                    else {
                        free(A4);
                        free(newNode);
                        break;
                    }
                }
            }
        }
        
        if (current == NULL) {
            *head = newNode;
            current = newNode;
        }
        else {
            current->next = newNode;
            current = current->next;
        }
    }
    
    fclose(fp);
    printf("Linked List has been read from the binary file and deserialized!\n");
}*/

void deallocate(Node *head){
    Node *current=head;
    Node *prev=NULL;
    while (current!=NULL)
    {
        prev=current;
        current=current->next;
        free(prev->data);
        free(prev);
    }
    head=NULL;
}

int main(){
    Node *head=NULL; // initializing the list null at the beginning.
    srand(time(NULL)); // seed to generate random numbers each time.
    fillLinkedList(&head); // filling the list with the fillLinkedList function.
    printf("Displaying the filled linked list...\n");
    printf("*********************************************\n");
    display(head); // display function to display the filled link list.
    printf("*********************************************\n");
    serializeLinkedList(head); // serializing the filled linked list.
    deallocate(head);

/*
    deserializeLinkedList(&head); // deserializing the linked list.
    printf("Displaying the deserialized linked list...\n");
    display(head); // displaying the deserialized linked list.
    deallocate(head);
*/
    Node *new=NULL; // initializing a new NULL list for deserializing.
    deserializeLinkedList(&new); // deserializing the linked list.
    printf("Displaying the deserialized linked list...\n");
    printf("*********************************************\n");
    display(new); // displaying the deserialized linked list.
    printf("*********************************************\n");
    deallocate(new);
    return 0;
}
