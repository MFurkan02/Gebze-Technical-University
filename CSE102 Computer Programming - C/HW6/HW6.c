#include <stdio.h>
#define MAX 100 // defining max size of the arrays

void part2(); // mentioning the function that will be used for part1() function.
void part3();
void part4();
void part5(double store[MAX][3]);
void part6(double store[MAX][3],int index);


int comma(){ // number of comma counter to find if a feauture is added or not to file to use in other functions.
    FILE* products;
    products = fopen("products.txt","r+");
    char ch;
    int count=0;
    while (fscanf(products,"%c",&ch)!=EOF){ // scanning through the products file char by char.
        if(ch=='\n'){ // if newline stop to finish the scanning in the first line.
            break;
        }
        if(ch==','){ // counting the comma in the first line.
            count++;
        }
    }
    return count;
    fclose(products);   
}


int product_ID(){ // number of products in products.txt
    FILE* products;
    products = fopen("products.txt","r+"); // opening the file on read+ mode.
    char name[8] = {} ;     // string of the name.
    char brand[5] = {} ;   // string of the brand. 
    int ID=0; // counter.
    int pid;
    char type;
    double price;
    char features2[MAX];
    fscanf(products,"%*[^\n],\n");
    while(fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,features2)!=EOF){
        char name[8] = {} ; // resetting the string for the next name in the file.
        char brand[5] = {} ; // resetting the string for the next brand in the file.
        // incrementing the counter after every id encountered.
        ID++; // getting the number of products in the products file to use for add product and other functions.
    }   
    return ID;
}

int stocks_ID(){ // number of stocks in stocks.txt
    FILE* stocks;
    stocks = fopen("stocks.txt","r+");
    int ID=0;
    int sID;
    int pID;
    char branch[15];
    int current_stock;
    fscanf(stocks,"%*[^\n],\n");
    while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){
        char branch[15]= {} ; // resetting the string for the next branch in the file.
        ID++; // incrementing the counter after every id encountered.
    }
    return ID;
}



int strcmp(char str1[],char str2[]){ // our string compare function instead the string.h one.
    int i=0;   // index
    int flag=1; // flag to control two string are equal or not.
    while(str1[i]!='\0' && str2[i]!='\0'){  // loop continues till the end of string.
        if (str1[i] != str2[i]){  // if index of 1st string and 2nd string is not equal flag is one and loop breaks.
            flag=1;
            break;
        }
        else{
            i++;    // else index will be incremented and flag will be zero.
            flag=0;
        }
    }   
    return flag; // after the loop finishes flag will be returned 0 if they are equal , 1 they are not equal
}


void part1(){
    int choice;
    double store[MAX][3]; 
    printf("Welcome operator, please select an option to continue:\n\n"); // THE MENU
    printf("1- File Operations\n2- Query products\n3- Check stock status\n4- Stock control by brand\n5- Export report and exit\n");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        part2();
        break;
    case 2:
        part3();
        break;
    case 3:
        part4();
        break;
    case 4:
        part5(store); 
        break;
    case 5:
        printf("Exporting the report and exiting from the program...\n"); // exit from the program.
        break;
    default:
        printf("Invalid input! Going back to menu...\n"); // if any input other than (1-5) is entered.
        part1();
        break;
    }
}

void part2(){
    // submenu of part2
    printf("1. Add a new product\n2. Delete a product\n3. Update a product\n4. Add feature to products\n5. Add a new stock entry\n6. Delete a stock entry\n7. Update a stock entry\n8. Back to main menu\n");
    FILE* products;
    FILE* stocks;
    products = fopen("products.txt","a+");
    stocks = fopen("stocks.txt","a+");
    fscanf(stocks,"%*[^\n],\n"); // skipping the first line of both files.
    fscanf(products,"%*[^\n],\n");
    char featurename[15];
    int sID,pID,current_stock;
    char branch[MAX][15];
    int choice;
    int flag;
    char features2[MAX];
    char nf[MAX][20];
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        int pid;
        char type;
        double price=0;
        printf("Enter the type(1 character) : ");
        scanf(" %c",&type);
        char name[8] = {} ;
        printf("Enter the name(maximum 8 characters) : ");
        scanf("%s",name);
        char brand[5] = {} ;
        printf("Enter the brand(maximum 5 characters) : ");
        scanf("%s",brand);
        printf("Enter the price(double) : "); // getting the variables for the new product.
        scanf("%lf",&price);
        if (comma()>4){
            rewind(products); // if there is a feature added to the  product it will also ask the feature value from the user.
            fscanf(products,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%s",featurename);
            printf("Enter the %s : ",featurename); // asking the feature value.
            scanf("%s",features2);
            fprintf(products,"\n%d,%c,%s,%s,%.2f,%s",product_ID()+1,type,name,brand,price,features2); // printing the new products to the file with the new added feature.
        }
        else{
            fprintf(products,"\n%d,%c,%s,%s,%.2f",product_ID()+1,type,name,brand,price); // printing the new products to the file without feature since it has not added yet.
        }
        fclose(products);
        part1();
        break;
    case 2: // delete product
            int ID;
            printf("Enter the pID of the product to be deleted\n");
            scanf("%d",&ID);
            int index = 0;
            int pid2[MAX];
            char type2[MAX];
            char name2[MAX][8];
            char brand2[MAX][6];
            double price2[MAX];
            char feature2[MAX][15];
            int com=comma();
            if (com>4){
                rewind(products); // if there is a feature added to the  product it will also ask the feature value from the user.
                fscanf(products,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,]%s",featurename);
            }
            while (fscanf(products, "%d,%c,%[^,],%[^,],%lf%[^\n]\n", &pid, &type, name, brand, &price,features2) != EOF) {
                if(pid<ID){ // if the wanted product to be deleted is lower than the pID value on the file.
                    pid2[index] = pid; // all of them will be stored directly.
                    type2[index] = type;
                    for (int i = 0; i < 8; i++) {
                        if (name[i] != '\0') {
                        name2[index][i] = name[i];
                        }
                    }
                    char name[8] = {} ;
                        for (int i = 0; i < 6; i++) {
                            if (brand[i] != '\0') {
                            brand2[index][i] = brand[i];
                            }   
                        }

                    price2[index] = price;
                    
                    for (int i = 0; i < 15; i++) {
                            if (features2[i] != '\0') {
                            feature2[index][i] = features2[i];
                            }   
                        }
                    char brand[5] = {} ;
                    index++;
                }
                else if (pid>ID){ // if the wanted product to be deleted is greater than the pID value on the file.
                    pid2[index] = pid-1; // pID value will be minus one since for example if 3 is the wanted to be deleted pID, 4 will be the new 3. 
                    type2[index] = type; // rest of them will be stored directly.
                    for (int i = 0; i < 8; i++) {
                        if (name[i] != '\0') {
                        name2[index][i] = name[i];
                        }
                    }
                    char name[8] = {} ;
                        for (int i = 0; i < 6; i++) {
                            if (brand[i] != '\0') {
                            brand2[index][i] = brand[i];
                            }   
                        }
                        for (int i = 0; i < 10; i++) {
                            if (features2[i] != '\0') {
                            feature2[index][i] = features2[i];
                            }   
                        }
                    price2[index] = price;
                    index++;
                    char brand[5] = {} ;
                    }
            }
            products = fopen("products.txt", "w+"); // after storing all of them the file will be opened on 'w' to be reseted.
            if (com>4)
            {
                fprintf(products,"pID,Type,Name,Brand,Price%s\n",featurename); // and the first line and the arrays that holds all the values will be printed to the file.
            }
            else{
                fprintf(products,"pID,Type,Name,Brand,Price\n"); // and the first line and the arrays that holds all the values will be printed to the file.
            }
            for (int i = 0; i < index; i++) {
                if(com>4){
                    fprintf(products,"%d,%c,%s,%s,%.2f%s\n", pid2[i], type2[i], name2[i], brand2[i], price2[i],feature2[i]);
                }
                else{
                    fprintf(products,"%d,%c,%s,%s,%.2f\n", pid2[i], type2[i], name2[i], brand2[i], price2[i]);
                }
            }
            fclose(products);
            part1();
            break;
    case 3: // update product 
            int ID2;
            printf("Enter the pID of the product to be updated\n");
            scanf("%d",&ID2);
            char enter[MAX];
            char enter2[MAX];
            char enter3[MAX];
            char type3;
            double price3;
            char newf[20];
            int flag=0;
            if (comma()>4){
                rewind(products); // if there is a feature added to the  product it will also ask the feature value from the user.
                fscanf(products,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%s",featurename);
                printf("Enter the feature to be updated (type,name,brand,price,%s) : ",featurename); // getting the feature to be updated.
                scanf("%s",enter);
                flag=1;
            }
            else{
                printf("Enter the feature to be updated (type,name,brand,price) : "); // getting the feature to be updated.
                scanf("%s",enter);
            }
            if( strcmp(enter,"name")==0){ // if feature is the name.
            printf("Enter the name to update : ");
            scanf("%s",enter2);
            }
            else if( strcmp(enter,"brand")==0 ){ // if feature is the brand.
            printf("Enter the brand to update : ");
            scanf("%s",enter3);
            }
            else if( strcmp(enter,"type")==0  ){ //  if feature is the type.
            printf("Enter the type to update : ");
            scanf(" %c",&type3);
            }
            else if( strcmp(enter,"price")==0  ){ // // if feature is the price.
            printf("Enter the price to update : ");
            scanf("%lf",&price3);
            }
            else if(strcmp(enter,featurename)==0){
                printf("Enter the %s to update : ",featurename);
                scanf("%s",newf);
            }

            while (fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,features2) != EOF) {
                    pid2[index] = pid; // all of the values will be stored in the arrays.
                    type2[index] = type;
                    for (int i = 0; i < 9; i++) {
                            if (name[i] != '\0') {
                            name2[index][i] = name[i];
                            }
                    }
                    for (int i = 0; i < 6; i++) {
                            if (brand[i] != '\0') {
                            brand2[index][i] = brand[i];
                            }
                    }
                    price2[index] = price;
                    for (int i = 0; i < 10; i++) {
                            if (features2[i] != '\0') {
                                nf[index][i] = features2[i];
                            }   
                    }
                    // updated ones
                    if(pid==ID2 && strcmp(enter,"type")==0){ // if feature is the type , type index will be changed with the wanted value.
                        
                        type2[index] = type3;
                    }
                        
                    else if(pid==ID2 && strcmp(enter,"price")==0){ // if feature is the price , price index will be changed with the wanted value.
                        price2[index] = price3;     
                    }
                    else if(pid==ID2 && strcmp(enter,"name")==0){ // if feature is the name , name index will be changed with the wanted value.
                        for (int i = 0; i < 9; i++) {
                            
                            name2[index][i] = enter2[i];
                            
                        }
                    }
                    else if(pid==ID2 && strcmp(enter,"brand")==0){ // if feature is the brand , brand index will be changed with the wanted value.
                                for (int i = 0; i < 6; i++) {
                                    
                                    brand2[index][i] = enter3[i];
                                }
                    }
                    else if(pid==ID2 && strcmp(enter,featurename)==0){ // if feature is the brand , brand index will be changed with the wanted value.
                                for (int i = 0; i < 20; i++) {
                                    
                                    nf[index][i] = newf[i];
                                }
                    }
                    char brand[6] = {} ;
                    char name[9] = {} ; // resetting the character arrays after each loop.
                    char feature2[10]={};
                    index++; // incrementing the index.
            }
            //rewind(products); 
            //if(comma()>4){ // if a feature is added it will be printed too.
             //   fscanf(products,"%*[^,],%*[^,],%*[^,],%*[^,],%*[^,]%[^\n]",featurename);
            //}
            fclose(products);
            products = fopen("products.txt", "w+"); // opening the file on 'w' mode to reset the file and print the values from the arrays to the file.
            if(flag==1){
                fprintf(products, "pID,Type,Name,Brand,Price,%s\n",featurename);
            }
            else{
                fprintf(products,"pID,Type,Name,Brand,Price\n");
            }
            //fprintf(products, "pID,Type,Name,Brand,Price,%s\n",featurename);
            for (int i = 0; i < index; i++) {
                if(flag==1){ // same process as the delete product case.
                    //printf("ENterde\n");
                    if(i==(ID2-1) && (strcmp(enter,featurename)==0)) {
                        
                        fprintf(products,"%d,%c,%s,%s,%.2f,%s\n", pid2[i], type2[i], name2[i], brand2[i], price2[i],nf[i]);
                    }
                    else{
                        fprintf(products,"%d,%c,%s,%s,%.2f%s\n", pid2[i], type2[i], name2[i], brand2[i], price2[i],nf[i]);
                    }
                }
                else{
                    fprintf(products,"%d,%c,%s,%s,%.2f\n", pid2[i], type2[i], name2[i], brand2[i], price2[i]);
                }
            } 
            fclose(products);
            part1();
            break;
    case 4: // new feature
            char feature[MAX];
            printf("Enter the name of the new feature: ");
            scanf("%s", feature); // getting the name of the new feature.

            while (fscanf(products, "%d,%c,%[^,],%[^,],%lf\n", &pid, &type, name, brand, &price) != EOF) {
                pid2[index] = pid; // storing every value to arrays.
                type2[index] = type;
                for (int i = 0; i < 8; i++) {
                    if (name[i] != '\0') {
                    name2[index][i] = name[i];
                    }
                }
                char name[8] = {} ;
                    for (int i = 0; i < 6; i++) {
                        if (brand[i] != '\0') {
                        brand2[index][i] = brand[i];
                        }   
                    }
            price2[index] = price;
            index++;
            // resetting the string for the next name in the file.
            char brand[5] = {} ;
            }
            fclose(products);

            products = fopen("products.txt", "w");  // opening the file on 'w' mode to reset the file and print the values from the arrays to the file. 
            fprintf(products, "pID,Type,Name,Brand,Price,%s\n",feature);
            for (int i = 0; i < index; i++) { // printing the values to file with feature values being 'None'.
                fprintf(products, "%d,%c,%s,%s,%.2f,None\n", pid2[i], type2[i], name2[i], brand2[i], price2[i]);
            }
            
            fclose(products);

            part1(); // back to menu
            break;

    case 5: 
        int sid=5;
        int pID;
        char branch[15];
        int current_stock;
        printf("Enter the pID(integer) : ");
        scanf("%d",&pID);
        printf("Enter the name of branch(maximum 15 characters) : ");
        scanf("%s",branch);
        printf("Enter the stock value(integer) : ");
        scanf("%d",&current_stock);
        fprintf(stocks,"\n%d,%d,%s,%d",stocks_ID()+1,pID,branch,current_stock); // adding the new stock entry.
        fclose(stocks);
        part1();
        break;
    case 6: // delete stock
        int sid2;
        printf("Enter the sID of the stock to be deleted\n");
        scanf("%d",&sid2);
        int pID3[MAX];
        char branch2[MAX][15];
        int stock[MAX];
        int sid3[MAX];
        index=0;
          while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){ // same process with th delete product
            if(sID<sid2){
                    sid3[index] = sID; // storing the values in arrays directly if wanted id is lower.
                    pID3[index] = pID;
                        for (int i = 0; i < 15; i++) {
                            //if (branch[i] != '\0') {
                            branch2[index][i] = branch[i];
                            //}   
                        }
                    stock[index] = current_stock;
                    index++;
                    char branch[15] = {} ;
            }
            else if (sID>sid2){ // // storing the values in arrays directly except pID is minus 1(same as delete product) if wanted id is greater.
                    sid3[index] = sID-1;
                    pID3[index] = pID;
                        for (int i = 0; i < 15; i++) {
                            //if (branch[i] != '\0') {
                            branch2[index][i] = branch[i];
                            //}   
                        }
                    stock[index] = current_stock;
                    index++;
                    char branch[15] = {} ;
            }
        }
            fclose(stocks);
            stocks = fopen("stocks.txt", "w");
            fprintf(stocks,"sID,pID,branch,current_stock\n"); // printing the values except the ID that has been deleted.
            for (int i = 0; i < index; i++) {
                fprintf(stocks,"%d,%d,%s,%d\n",sid3[i],pID3[i],branch2[i],stock[i]);
            }
            fclose(stocks);
            part1();
    case 7: // update stock
        int Usid;
        int cstock;
        printf("Enter the sID of the stock to be updated\n");
        scanf("%d",&Usid);

        printf("Enter the feature to be updated (pID,branch,current_stock) : ");
        scanf("%s",enter);
        int uID;
        if( strcmp(enter,"pID")==0){
        printf("Enter the pID to update : ");
        scanf("%d",&uID);
        }
        else if( strcmp(enter,"branch")==0 ){
        printf("Enter the branch to update : ");
        scanf("%s",enter3);
        }
        else if( strcmp(enter,"current_stock")==0){
        printf("Enter the current_stock to update : ");
        scanf("%d",&cstock);
        }
        while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){ // same process as update product.
                    sid3[index] = sID;
                    pID3[index] = pID;
                        for (int i = 0; i < 16; i++) {
                      
                            branch2[index][i] = branch[i]; // storing all values in arrays.
                         
                        }
                    stock[index] = current_stock;
                    
                    // if wanted to be changed feature, that feature value will be changed of that index.
                    if(sID==Usid && strcmp(enter,"pID")==0){
                        
                        pID3[index] = uID;
                    }

                    else if(sID==Usid && strcmp(enter,"branch")==0) {
                        for (int i = 0; i < 16; i++) {
                            branch2[index][i] = enter3[i];
                              
                        }
                    }

                    else if(sID==Usid && strcmp(enter,"current_stock")==0){
                        
                        stock[index] = cstock;
                    }


                    index++;
                    char branch[15] = {} ;
        }
            fclose(stocks);
            stocks = fopen("stocks.txt", "w");
            fprintf(stocks,"sID,pID,branch,current_stock\n"); // printing the values in the arrays to the file.
            for (int i = 0; i < index; i++) {
                fprintf(stocks,"%d,%d,%s,%d\n",sid3[i],pID3[i],branch2[i],stock[i]);
            }
            fclose(stocks);
            part1();
            break;
    case 8:
        part1(); // back to menu
        break;
    default:
        printf("Invalid input! Going back to menu...\n");
        part1();
        break;
    }




}

void part3(){
    FILE* products;
    FILE* stocks;
    products = fopen("products.txt","a+");
    char line[MAX];
    fscanf(products,"%[^\n]\n",line); // first line.
    
    int pid;
    char type;
    char name[8] = {};
    char brand[6] = {} ;
    double price=0;
    printf("1. List all products\n2. Filter products by brand, type, price, or a user-defined feature\n3. Back to main menu.\n"); // Menu of part3.
    int choice;
    scanf("%d",&choice);
     switch (choice)
    {
    case 1:
        char features[10];
        printf("%s\n",line); // printing the first line first.
        while(fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,features)!=EOF){ // scanning through the products file.
                printf("%d,%c,%s,%s,%.2f%s\n",pid,type,name,brand,price,features); // printing all the products line by line.
                char name[8]= {}; // resetting the name string array after each line.
                char brand[6]= {}; // resetting the brand string array after each line.
                char features[10]={};
        }
        fclose(products);
        part1(); // finally going back to menu.
        break;
    case 2: // filter
        char filter1[MAX];
        char filter2[MAX];
        char filter3[MAX];
        char filter4[MAX];
        int nof;
        printf("How many filter do you want to use among type,brand,user-defined(1-3)?\n"); // asking the number of filters user wants to use.
        scanf("%d",&nof);
        if(nof==1){ // if filter number is 1.Asking for the filters.
            printf("Enter the filter(s) you want to use : ");
            scanf("%[^,],%s",filter4,filter1);
            //printf("Filter 4 is %s!!\n",filter4);
            //printf("filter is %s of %s\n",filter4,filter1);
            //printf("its %c\n",filter1[0]);
        }
        if(nof==2){ // if filter number is 2.Asking for the filters.
            printf("Enter the filter(s) you want to use : ");
            scanf("%*[^,],%[^,],%*[^,],%s",filter1,filter2);
            //printf("filter is %s and %s\n",filter1,filter2);
        }
        if(nof==3){ // if filter number is 3.Asking for the filters.
            printf("Enter the filter(s) you want to use : ");
            scanf("%*[^,],%[^,],%*[^,],%[^,],%*[^,],%s",filter1,filter2,filter3);
            //printf("filters are %s and %s and %s\n",filter1,filter2,filter3);
        }
        char fch;
        printf("Do you want to use price filter?(Y-N)"); // asking the user for the price filter.
        scanf(" %c",&fch);
        float price1,price2;
        int flag=0;
        
        int ids[MAX];
        char types[MAX];
        char names[MAX][8];
        char brands[MAX][6];
        double prices[MAX];
        char nfeatures[MAX][15];
        int ind=0;
        if (fch=='Y'){
            printf("Please enter minimum and maximum price values : "); // if price filter is wanted min and max values will be asked.
            scanf("%f %f",&price1,&price2);
        }
        while(fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,features)!=EOF){ // scanning through the products file.
            if(nof==1 && type==filter1[0] && strcmp("\ntype",filter4)==0 ){ // if filter no is 1 it will be compared if filter matches.
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
            else if(nof==1 && strcmp(brand,filter1)==0 && strcmp("\nbrand",filter4)==0 ){ // for brand
    
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
            else if(nof==1 && strcmp(&features[1],filter1)==0){ // for feature
        
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
            else if(nof==2 && type==filter1[0] && strcmp(brand,filter2)==0 ){//&& fch=='Y'){ // // if filter no is 1 it will be compared if filter match.
             
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
            else if(nof==2 && type==filter2[0] && strcmp(brand,filter1)==0 ){//&& fch=='Y'){ // // if filter no is 1 it will be compared if filter match.
             
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
             else if(nof==3 && type==filter1[0] && strcmp(brand,filter2)==0 && strcmp(&features[1],filter3)==0 ){ // // if filter no is 3 it will be compared if filters match.
             
                if (fch=='Y' && price>price1 && price<price2){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
                else if (fch=='N'){
                    ids[ind]=pid;
                    types[ind]=type;
                    prices[ind]=price;
                    printf("%d,%c,%s,%s,%.2f%s\n",ids[ind],types[ind],name,brand,prices[ind],features);
                    flag=1;
                }
            }
  
        ind++;
            
        }
        if(flag==0){
            printf("No product found for these filters!\n");
        }
        fclose(products);
        part1(); // finally going back to menu.
        break;
    case 3: 
        part1();
        break; // back to menu
    default:
        printf("Invalid input! Going back to menu...\n");
        part1();
        break;
    }

}

void part4(){
    printf("1. Query the stock of a given product in a specified branch by using the product ID and branch name\n"); // menu of part4.
    printf("2. List the stock of all products in a specified branch\n");
    printf("3. List the out-of-stock products in a specified branch\n");
    printf("4. Back to main menu\n");
    FILE* stocks;
    stocks= fopen("stocks.txt","r+");
    FILE* products;
    products=fopen("products.txt","r+");
    fscanf(products,"%*[^\n],\n"); // skipping the first line of the files.
    fscanf(stocks,"%*[^\n],\n");
    int choice;
    int sID,pID,current_stock;
    char branch[15];
    char sbranch[15];
    char name[8] = {} ;     // string of the name.
    char brand[5] = {} ;   // string of the brand. 
    int ID=0; // counter.
    int pid;
    char type;
    double price;
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        int id;
        printf("Enter the name of branch(maximum 15 characters) : ");
        scanf("%s",sbranch);
        printf("Enter the product ID(integer) : ");
        scanf("%d",&id);
        int stockp[MAX];
        int index=0;
        while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){  // scanning through the products file.
            int res=strcmp(branch,sbranch);
            if (res==0 && id==pID){
            
                printf("Stock of pID %d in branch %s is ",pID,branch);
                stockp[index]=current_stock; // storing the stock of the given pID and branch.
                printf("%d.\n",stockp[index]); // printing the stock value that has been stored to the array.
                index++; // incrementing the index.
            }
            
        }
        part1(); // finally going back to menu.
        break;
    case 2:
        printf("Enter a branch(maximum 15 characters) : ");
        scanf("%s",sbranch);
        index=0;
        while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){ // scanning through the products file.
            int res=strcmp(branch,sbranch); // if branch matches with the entered one.
            if (res==0){
                printf("Stock of pID %d is ",pID); // printing the pID.
                stockp[index]=current_stock; // storing the stock value.
                printf("%d.\n",stockp[index]); // printing the current index of the array.
                index++;
            }
            
        }
        part1(); // finally going back to menu.
        break;
    case 3:
        printf("Enter the name of branch(maximum 15 characters) : ");
        scanf("%s",sbranch);
        char name[8] = {} ;     // string of the name.
        char brand[5] = {} ;   // string of the brand.
        int flag=0;
        char features[MAX];
        int find;
        int store1[MAX]; // storing the pID.
        char store2[MAX]; // storing the 
        char store3[MAX][8];
        char store4[MAX][5];
        index=0;
        while(fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock)!=EOF){ // scanning through the stocks file.
            for(int j=0;j<product_ID();j++){ // loop to go through the products file for every line of stocks file. 
                fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,features); // through the products file searching if pIDs match with the pID in stocks file. 
                int res=strcmp(branch,sbranch); // if branch name matches.
                if (res==0 && current_stock==0 && pID==pid){ // if branch name , stock and pIDs match.
                    printf("Out-of-stock product of this branch is ");
                    store1[index]=pid; // storing the pid
                    store2[index]=type; // storing the type
                    printf("%d,%c,%s,%s.\n",store1[index],store2[index],name,brand); // printing the arrays of the matching filters.
                    flag=1; // flag to not print no out of stock product message.
                    j=-1; // to go to the next loop.
                    index++;  // incrementing the index of the array.
                }
                else if (res==0 && current_stock==0 && pID!=pid && flag!=1){ // if branch name and stock matches but pIDs do not match.
                    flag=2;
                    find=pID;
                }
                char name[8] = {} ;     // resetting the string array of name for the next name in the file.
                char brand[5] = {} ;     // resetting the string array of brand for the next brand in the file.
            }
               char branch[15] = {}; //  // resetting the string branch of branch for the next branch in the file.
               rewind(products); // rewind reading the products file for the other lines after the for loop.
               fscanf(products,"%*[^\n],\n");
        }
                      if(flag==2){ // if branch name and stock matches but pIDs do not match printing message with the flag.
                        printf("There is an out-of-stock product in this branch but there is no pID of %d in products file!\n",find);
                      }
                      if(flag==0){ // if branch does not match in file with 0 stock value printing message.
                        printf("There is no out-of-stock product in this branch.\n");
                      }
        fclose(products);
        fclose(stocks);
        part1(); // finally going back to menu.
        break;
    case 4:
        part1(); // back to menu
        break;
    default:
        printf("Invalid input! Going back to menu...\n");
        part1();
        break;
    }
}



void part5(double store[MAX][3]){

    FILE* stocks;
    FILE* products;
    products = fopen("products.txt","r+");
    stocks= fopen("stocks.txt","r+");
    fscanf(products,"%*[^\n],\n"); // to skip the first line writings(Brand,Price etc.)
    fscanf(stocks,"%*[^\n],\n"); // to skip the first line writings.
    int pid,sID,current_stock;
    char type;
    double price;
    char branch[15];
    char name[9] = {};
    char sbrand[6]; // searched brand
    char brand[6] ;
    int index=0;
    int res;
    int pID;
    int flag=0;
    printf("Enter a brand name(maximum 5 characters) : ");
    scanf("%s",sbrand);
    char new_feature[MAX];
    while(fscanf(products,"%d,%c,%[^,],%[^,],%lf%[^\n]\n",&pid,&type,name,brand,&price,new_feature)!=EOF){ // scannning through the products file.
            for(int j=0;j<stocks_ID();j++){ // loop to go through the stocks file for every line of products file. 
            fscanf(stocks,"%d,%d,%[^,],%d",&sID,&pID,branch,&current_stock); // scannning through the stocks file.
            res=strcmp(brand,sbrand);
            if (res==0 && pid==pID){
                store[index][0]=pID; // storing the pID 
                store[index][1]=price; // storing the price
                store[index][2]=current_stock; // storing the stock value.
                index++; // incrementing the index.
                flag=1; // for the next if statement
            }
            else if(res==0 && pid!=pID && flag!=1){ // if brand matches but pIDs do not match.
                flag=2;
            }
            char branch[15] = {};
        }
        char name[9] = {};
        char brand[6] = {};
        rewind(stocks); // for the for loop rewinding the stocks file and skipping the first line.
        fscanf(stocks,"%*[^\n],\n");
    }
    if (flag==2){ // if brand matches but pIDs do not match printing message.
        printf("There is a %s brand in products file but it does not match with any pID in stocks file!\n",sbrand);
    }
    int i;
    for(i=0;i<index;i++){ // for the matched brand values printing the stored values.
        printf("pID is %d, price is %.2f, stock amount is %d.\n",(int)store[i][0],store[i][1],(int)store[i][2]);
    }
   // printf("index is %d\n",index);
    fclose(products);
    fclose(stocks);
    part6(store,index); // reporting the array with min,max,median to reports.txt using part 6 function.
    part1(); // finally going back to menu.
}

void part6(double store[MAX][3],int index){ // id min max median of stock
    FILE* report;
    report = fopen("report.txt","w");
    int i;
    double pids[MAX];
    int count[MAX];
    //int j=sizeof(store[0])/sizeof(store[0][0]);
    //int jj=0;
    int j,jj;
    int temp;
    int stocks[MAX]={};
    while(store[i][0]>0){ // getting the length of the array by using pID values being greater than 0.
        if(store[i][0]>0){
                 jj++; // incrementing the counter.
        }
       
        i++;  // incrementing the array index.
    }
    //printf("j is %d\n",j);
    for(i=0;i<index;i++){
        stocks[i]=store[i][2]; // the stock values of store array to the stock array.
    }
    fprintf(report,"pID,min,max,median\n"); // printing to the report first line.
    int k;
    for(i=0; i<index; i++)
    {
        for(k=i+1; k<index; k++) {  // sorting the stocks array for min max and median values.
            if(stocks[i]>stocks[k]){
                temp = stocks[i];
                stocks[i] = stocks[k];
                stocks[k] = temp;
            }
        }
    }
    int index2=0;    
    j=index;                                                                                                           // index
    if(j>0 && j%2==0){ // if length of array is even.Median will be the average of the two middle elements of the array. size 4 --> median avr(1,2) 
    fprintf(report,"%d,%d,%d,%.2f\n",(int)store[index2][0],stocks[0],stocks[j-1],(float)(stocks[(j/2)]+stocks[((j/2)-1)])/2 );
    }
    else if (j>0 && j%2==1)  { 
        // if length of array is odd.Median will be the middle element of the array.
        fprintf(report,"%d,%d,%d,%.2f\n",(int)store[index2][0],stocks[0],stocks[j-1],(float)stocks[(j/2)] );
    }
    fclose(report); // closing the report file.
}

int main(){
    part1(); // Menu of the file which contains all functions of the homework in it.
    return 0;
}