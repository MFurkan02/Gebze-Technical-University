#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_WORD_SIZE 35
#define MAX_WORD_COUNT 100
#define MAX 300000
#define SENTINEL '-'
#define MAX_VECTOR 400

//int voidintvoid
int read_dict(const char * file_name, char dict[][MAX_WORD_SIZE]){
    FILE* dictionary;
    dictionary=fopen(file_name,"r");
    char str[MAX_WORD_SIZE];
    int index=0,i,j;
    int numlen,vectorlen;
    fscanf(dictionary,"%*[^=]=%d%*[^=]=%d\n",&numlen,&vectorlen);
    while (fscanf(dictionary,"%s",str)!=EOF){
        for(i = 0; i < MAX_WORD_SIZE; i++){ // for loop to copy the string to current index of dict array.
            if ( (str[i]>64 && str[i]<91) || (str[i]>96 && str[i]<123) ){ // if the string is a character a to z OR A to Z it will be taken to dict array.
                    dict[index][i]=str[i]; // copying the string if it meets the condition.
            }
        }
        dict[index][i]='\0';  
        if( ((str[0]>=48) && (str[0]<=57)) || str[0]==45 ) { // if the first character of string is a number 0123456789 or minus '-' like -0.29 
            continue;                                        // the index will be the same since it is not a word.Thus it will not be in dict array.
        }
        else{
            //dict[index][i+1]='\0';
            index++; // If it is not it will be in the dict array.
        }
         
    }
    dict[index][0]='-'; // setting the last element of the array to '-' .
   
    fclose(dictionary); // Closing the file.
    return index;
}


//intvoid 
int read_text(const char  * text_file, const char  * ignore_file, char words[][MAX_WORD_SIZE]){
    FILE* ignore; // file pointer of ignore.txt
    FILE* input; // file pointer of input.txt
    ignore=fopen(ignore_file,"r"); // opening the files.
    input=fopen(text_file,"r");
    char str_ignore[MAX][MAX_WORD_SIZE];
    char str[MAX_WORD_SIZE]; // string for the ones in ignore.txt .
    char str2[MAX_WORD_SIZE]; // // string for the ones in input.txt .
    int index=0;
    int index2=0;
    int index3;
    int i,j;
    int flag=0;
    int flag2=0;
    while(fscanf(ignore,"%s",str)!=EOF){ // Reading words from the ignore file until the end of file is reached.
        for(i = 0; i < MAX_WORD_SIZE; i++){ // Copying the string into the array str_ignore
            str_ignore[index][i]=str[i];
        }
        index++; // Incrementing the index for the next string to be added to the str_ignore array.
    }
    while(fscanf(input,"%s",str2)!=EOF){ // // Reading words from the input file until the end of file is reached.
        //index3=0;
        for(i = 0; i < MAX_WORD_SIZE; i++){
            if ( (str2[i]>47 && str2[i]<58) || (str2[i]>64 && str2[i]<91) || (str2[i]>96 && str2[i]<123) || str2[i]==32 || str2[i]==39 ){

                    words[index2][i]=str2[i];
                   // index3++;
            }          
        }
            for( j = 0; j < index ; j++){ // // Checking if the string is in the ignore list.
                if(strcmp(words[index2],str_ignore[j])==0){
                        flag=1; // setting the flag to one if encountered to an ignore word.
                }
            }
                if (flag==1){
                        flag=0; // to reset the flag.
                }
                else{
                    index2++; // incrementing the index if it is not in the ignore list which means flag is still zero.
                }
    }
    words[index2][0]='-'; // setting the last element of the array to '-' .
    fclose(input); // Closing the files.
    fclose(ignore);
    
    return index2;
}
//doublevoid 

double dissimilarity(char * w1, char * w2, char dict[][MAX_WORD_SIZE], float threshold, const char * file_name ){
    
    int index,i,j,vectorlen;
    double dis=threshold;
    double emp;
    FILE* dictionary;
    dictionary=fopen(file_name,"r");
    fscanf(dictionary,"%*[^=]=%d%*[^=]=%d\n",&index,&vectorlen);
    //printf("%d %d",index,vectorlen);
    double vector1[vectorlen]; // vectorlen
    double vector2[vectorlen];
    double vector1v;
    double vector2v;
    char str[MAX_WORD_SIZE];
    //printf("Enter the threshold value for dissimilarity function(0.17-0.18 recommended value) : ");
    //scanf("%f",&threshold);
    //double dis=0;
    for(i=0;i<index+1;i++){
        fscanf(dictionary,"%s",str);
      for(j=0;j<vectorlen;j++){
        if (strcmp(w1,str)==0){
            fscanf(dictionary,"%lf",&vector1[j]);
            //fscanf(dictionary,"%lf",&vector1);
        }
        else if (strcmp(w2,str)==0 && strcmp(w1,w2)!=0 ){
            fscanf(dictionary,"%lf",&vector2[j]);
            //fscanf(dictionary,"%lf",&vector2);
        }
        else{
            fscanf(dictionary,"%lf",&emp);
        }
      }
    }
    for(j=0;j<vectorlen;j++){
        dis+= pow((vector1[j]-vector2[j]),2) ;
        //printf("%lf\n",dis);
    }
    dis=sqrt(dis);
    //printf("%lf\n",dis);
    /*
      for(j=0;j<vectorlen;j++){
        printf("Vector1_%lf ---- Vector2_%lf\n",vector1[j],vector2[j]);
      }*/

    return dis;

}




int histogram(const char words[][MAX_WORD_SIZE],const int occurrences[], const char hist[][MAX_WORD_SIZE+5+20]){
    int histog=1;
    int word_counter;
    int count=0;
    int flag=0;
    char dict[MAX][MAX_WORD_SIZE];
    int k;
    int max=occurrences[0];
    for(int o=0;o<MAX_WORD_COUNT;o++){
        if(words[o][0]!='\0'){
            word_counter++;
        }
        if(occurrences[o]>20){
            flag=1;
            if(occurrences[o]>max){
                max=occurrences[o];
            }
        }
    }
    //printf("%d is max\n",max);
    if(flag==1 && word_counter>1){
        histog=max/20;
    }
    //printf(" hist is%d\n",histog);
    if (histog==0)
    {
        histog=1;
    }
    
    return histog;
}

int in_text(const char  * text_file,const char  * ignore_file,char * w){ // to find the occurany of a word outside the input word which is the closest word to a word outside the input.txt.
    
    char words[MAX][MAX_WORD_SIZE];
    int occur;
    int len=read_text(text_file,ignore_file,words);
    for (int i = 0; i < len; i++)
    {
        if(strcmp(w,words[i])==0){
            occur++;
        }
    }
    return occur;
}


int in_dict(const char * file_name,char dict[][MAX_WORD_SIZE],char w1[]){ // to look if a word is in dictionary.txt it will be used to find the vectors of the word if it is.
    int len;
    len=read_dict(file_name,dict);
    int in=1;
    for(int i=0;i<len;i++){
        if(strcmp(w1,dict[i])==0){
            in=0;
        }
    }
    return in;

}

int main(int argc, char *argv[]){

    char dict[MAX][MAX_WORD_SIZE];
    char words[MAX][MAX_WORD_SIZE];
    const char * file_name;
    const char  * text_file;
    const char  * ignore_file;
    char str[MAX_WORD_COUNT];
    
    
 


    // FILE NAMES CAN BE CHANGED FROM HERE FOR OTHER FILES !!!!!!!! 
    file_name="dictionary.txt";
    text_file="input.txt";
    ignore_file="ignore.txt";




    //read_dict(file_name,dict);
    //read_text(text_file,ignore_file,words);

    printf("Enter word(s) : ");
    scanf(" %[^\n]",str);

    char input[MAX_WORD_COUNT][MAX_WORD_SIZE]; // array of the input words.
    int occurances[MAX_WORD_COUNT]={0}; // array for the occurancy of the input words.
    int count=0,flag=0;
    int i =0 , j=0 , k=0;
    int loop=0;
    int word_counter=0;
    while (!loop) {
        if(str[i]==' ' || str[i]=='\0'){
            word_counter++;
            k++;
            j=-1;      // resetting the index of the word array for the next word.
            count=0; // resetting the occurancy of the word for the next word.
        }
        if(str[i]=='\0'){ // if input words came to the end while loop will be stopped by making the loop value 1.
                loop=1;
        }
        else{ // if the input word index j is neither space or end of array string will be stored in word array.
        input[k][j]=str[i];
        j++;
        }
        i++;
    }
    int length = read_text(text_file,ignore_file,words);
    for(int f=0;f<length;f++){
        for (int s = 0; s < k; s++)
        {
            if(strcmp(words[f],input[s])==0){
                occurances[s]++;
            }
        }      
    }
    char hist[MAX][MAX_WORD_SIZE+5+20]; // histogram array for the output.
    int histog=histogram(input,occurances,hist); // getting the scale
    if (histog>1){ // if scale is more than 1 printing it.
        printf("Scale: %d\n",histog);
    }
    int t,h;
    char NM[]="\t\tNO MATCHES"; // for the words that are not in input.txt or dict.txt. 
    //char closest[MAX_WORD_SIZE]={"ddf"}; // if the word doesn't appear in file_name, its closest in dictionary.
    int occur;
    int index=0;
    int vec=read_dict(file_name,dict);
    char hist2[MAX_WORD_COUNT][MAX_WORD_SIZE];
    char ch='"';
    char star='*';
    char plus='+';
    char tab='\t';
    char arrow1='-';
    char arrow2='>';
    for(k=0;k<word_counter;k++){ // creating the lines for the output histogram array.
       
        if(histog>1){

            if(occurances[k]>0){ // if the word is in text file
            	//printf("Scale: %d\n",histog);
                hist[k][0]='"'; // the apostrophe of the word.
                strcat(hist[k],input[k]); // after asking Zehra hoca it is stated that we could use these functions to append.
                strncat(hist[k],&ch,1);  // after asking Zehra hoca it is stated that we could use these functions to add character which is apostrophe.
                strncat(hist[k],&tab,1); // adding tab to the string to look nice when printing. 
                strncat(hist[k],&tab,1);
                int p=0;
                //int size=sizeof(input[k])/sizeof(input[k][0]);
                for ( p = 0; p < (occurances[k]/histog); p++) // since scale is 2 it will be divised by 2.
                {
                    strncat(hist[k],&star,1);
                    //hist2[k][p]='*';
                }
               //printf("size is %d\n",size);

            }
            
            else if(occurances[k]==0 && in_dict(file_name,dict,input[k])==1){
            	//printf("Scale: %d\n",histog);
                hist[k][0]='"'; // the apostrophe of the word.
                strcat(hist[k],input[k]); 
                strncat(hist[k],&ch,1);  // the apostrophe of the word.
                strcat(hist[k],NM); // if the word is not both in input and dictionary txt '\tNO MATHCES' string will be appended.
            }
            else{
                    double threshold;
                    printf("Enter the threshold value for dissimilarity function(6 recommended value) : ");
                    scanf("%lf",&threshold);
                    double close=1/threshold;
                    hist[k][0]='"'; // the apostrophe of the word.
                    strcat(hist[k],input[k]); // appending the input word to the hist array.
                    strncat(hist[k],&arrow1,1); // appending the arrow that points to the closest word.
                    strncat(hist[k],&arrow2,1);
                    printf("Please wait it could take up to 20-25 seconds for every vector of the words to be compared!\n");
                    printf("Scale: %d\n",histog);
                    for (int v = 0; v < vec; v++)
                    {
                        
                        double dis=dissimilarity(input[k],dict[v],dict,threshold,file_name);
                        double sim=1/dis; // similaritiy(inverse of dissimilarity).
                        printf("dissimilarity between '%s' and '%s' is %lf\n",input[0],dict[v],sim);
                        if(sim>close){
                            close=sim;
                            index=v;
                        }
                    }
                    //printf("distance is %lf\n",close);
                    int occur=in_text(text_file,ignore_file,dict[index]);
                    strcat(hist[k],dict[index]);
                    strncat(hist[k],&ch,1);  // the apostrophe of the word. 
                    if(!occur){ // if the closest word does not appear in input.txt
                        strcat(hist[k],NM);
                    }
                    else{
                            strncat(hist[k],&tab,1); // adding tab to the string to look nice when printing
                            strncat(hist[k],&tab,1);
                            for (int p = 0; p < (occur/histog); i++)
                            {
                                strncat(hist[k],&plus,1); // appending plus as many as the closest word appear in input.txt.
                            }
                            
                    }
        }
    }
  }

        if(histog==1){
            if(word_counter==1){
                if(occurances[0]>0){ // if the only word in input appears in txt.
                    printf("'%s' appears in '%s' %d times.\n",input[0],text_file,occurances[0]);
                }
                else if(occurances[0]==0 && in_dict(file_name,dict,input[0])==1){ // if the only word in input does not appear in both txt files. 
                    printf("'%s' doesn't appear in '%s'.\n",input[0],text_file);
                }
                else{
                    float threshold;
                    printf("Enter the threshold value for dissimilarity function(6 recommended value) : "); // asking for the threshold value.
                    scanf("%f",&threshold);
                    float close=1/threshold; // equalizing close value to threshold for the start.
                    float dis,sim;
                    printf("Please wait it could take up to 20-25 seconds for every vector of the words to be compared!\n");
                    for (int v = 0; v < vec; v++) // comparing every vector(200) with words(1000) to find the closest word then printing it.
                    {
                        
                        dis=dissimilarity(input[0],dict[v],dict,threshold,file_name);
                        sim=1/dis; // similaritiy(inverse of dissimilarity).
                        printf("dissimilarity between '%s' and '%s' is %lf\n",input[0],dict[v],sim);
                        if(sim>close){
                            close=sim;
                            index=v;
                        }
                    }
                    //printf("distance is %f\n",close);
                    int occur=in_text(text_file,ignore_file,dict[index]);
                    if(!occur){
                        printf("'%s' doesn't appear in '%s' but closest word '%s' doesn't appear too!\n",input[0],text_file,dict[index]);
                    }
                    else{
                        printf("'%s' doesn't appear in '%s' but '%s' appears %d times.\n",input[0],text_file,dict[index],occur);
                    }
                }
            }
            
            else if(word_counter>1){ // if scale is 1 but there is more than one word.

              for(k=0;k<word_counter;k++){ // creating the lines for the output histogram array.

                if(occurances[k]>0){ // if the word is in text file
                    hist[k][0]='"'; // the apostrophe of the word.
                    strcat(hist[k],input[k]); // after asking Zehra hoca it is stated that we could use these functions to append.
                    strncat(hist[k],&ch,1);  // after asking Zehra hoca it is stated that we could use these functions to add character which is apostrophe.
                    strncat(hist[k],&tab,1); // adding tab to the string to look nice when printing. 
                    strncat(hist[k],&tab,1);
                    int p=0;
                    
                    for ( p = 0; p < (occurances[k]); p++) // since scale is 1 it will be printed directly.
                    {
                        strncat(hist[k],&star,1);
                        
                    }
                }
                else if(occurances[k]==0 && in_dict(file_name,dict,input[k])==1){ // // if the word in input does not appear in both txt files. 
                    hist[k][0]='"'; // the apostrophe of the word.
                    strcat(hist[k],input[k]); // the input word appending to the hist array.
                    strncat(hist[k],&ch,1);  // the apostrophe of the word.
                    strcat(hist[k],NM); // if the word is not both in input and dictionary txt '\tNO MATHCES' string will be appended.
                }
                else{ // if the word is not input.txt but in the dictionary.txt.
                        float threshold;
                        printf("Enter the threshold value for dissimilarity function(6 recommended value) : ");
                        scanf("%f",&threshold);
                        //dissimilarity(input[k],dict[0],dict,threshold,file_name);
                        float close=1/threshold;
                        //printf("thr is %f\n",threshold);
                        hist[k][0]='"'; // the apostrophe of the word.
                        strcat(hist[k],input[k]); // appending the input word to the hist array.
                        strncat(hist[k],&arrow1,1); // appending the arrow that points to the closest word.
                    	strncat(hist[k],&arrow2,1);
                        // Since scale is one the closest word will not be in hist array as in the example pdf only the plus of occurany of the closest word will appear.
         
                        int v;
                        printf("Please wait it could take up to 20-25 seconds for every vector of the words to be compared!\n");
                        for (v = 0; v < vec; v++)
                        {
                            
                            double dis=dissimilarity(input[k],dict[v],dict,threshold,file_name);
                            double sim=1/dis; // similaritiy(inverse of dissimilarity).
                            printf("dissimilarity between '%s' and '%s' is %lf\n",input[0],dict[v],sim);
                            if(sim>close){
                                close=sim;
                                index=v;
                            }
                        }
                        //printf("distance is %lf\n",close);
                        int occur=in_text(text_file,ignore_file,dict[index]);
                        strcat(hist[k],dict[index]);
                        strncat(hist[k],&ch,1);  // the apostrophe of the word. 
                        if(!occur){ // if the closest word does not appear in input.txt
                            strcat(hist[k],NM);
                        }
                        else{
                                strncat(hist[k],&tab,1); // adding tab to the string to look nice when printing.
                                strncat(hist[k],&tab,1);	
                                for (int p = 0; p < (occur/histog); i++)
                                {
                                    strncat(hist[k],&plus,1); // appending plus as many as the closest word appear in input.txt.
                                }
                                
                        }
                    }
              }
            }
    
        }

    if (word_counter>1){ // printing the histogram array which will be the output.
    int q;
        for (q = 0;q < k; q++){
            printf("%s\n",hist[q]);
        }
    }

    return 0;
}
