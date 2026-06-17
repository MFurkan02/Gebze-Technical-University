#include <stdio.h>
#include <math.h>

void part1(int year) {


if ((year % 4 == 0 && year % 100 != 0  ) || (year % 400 == 0 && year % 4000 != 0) )  {
    // If the year is divisible by 4 and not divisible by 100 or it is divisible by 400 and not divisible by 4000 year is a leap year.
    // https://airandspace.si.edu/stories/editorial/science-leap-year#:~:text=The%20rule%20is%20that%20if,skipped%20is%20the%20year%202100.
    //  "The rule is that if the year is divisible by 100 and not divisible by 400, leap year is skipped. The year 2000 was a leap year, for example, but the years 1700, 1800, and 1900 were not.  The next time a leap year will be skipped is the year 2100."
    // This is the condition of being a leap year.  
    printf("%d is a leap year.\n",year);

   }
else {
    // If it is another condition for example divisible by 100 it will print this year is not a leap year. 
    printf("%d is not a leap year.\n",year);

   }
   

}


int int_digits(float res){
    int i=0;

    if ((int)res==0) // If the number is 0.123 or 0.0002345 it will directly say it has 1 integer digit.Since it will not get in while loop.
        i=1;
    while ((int)res > 0){
        i++;                // While the integer side of the number is not equal to zero the number will be divised by 10. 
        res = res /  10;
    }
    return i;
}

int float_digit(double res) {
    int fl=0;
    int ch;			     // I tried finding float digits not using the sprinf format but it was not reliable.
                                    // It found some of the digits true but some of them was wrong.Using the below funtion.
                                    // Also tried using only integer part to solve but for examples like 7.35 m=7 n=1 the result was 735000.0e-5 which is not good looking.
                                    // Since it was not stated not to use sprintf I used since it was more reliable.
    int cond=0;
    if ((int)res == res) {	// If number do not have a floating digit.
        fl=0;
    }
    else{
    char c[50]; //size of the number 
    sprintf(c, "%.6g", res);  // Converting the float number to string to reach the number of floating point above.

    int count; 

        // incrementing the count till the end of the string
        for (count = 0; c[count] != '\0'; ++count){
            if (c[count] == '.') {
                cond=1;
                fl=-1;    // After reaching the floating point of the string which is the side after the dot.
                           
            }
            if (cond==1){
                fl++;    // It will increment fl counter.

            }

        }
    }
    return fl;
}

/*
int float_digit(double res) {
    int fl=0;                           // 2.463 -> 24.63 -> 246.3 -> 2463.0 
    while (res != (long int)res )   // I tried finding float digits not using the sprinf format but it was not reliable.
                                    // It found some of the digits true but some of them was wrong.Using this funtion.
      {
        fl = fl + 1;
        res = res * 10;
      }
        return fl;

}
*/

void part2(char fou,int m,int n,char operation,double op1,double op2) {

// fou = format of output 
int iresult;    // Result variables.
float dresult ;
    
switch (operation) {
	
    case '+' :
        if (fou=='I') {
            iresult = op1 + op2; // Addition calculation.
            printf("%d + %d = %d\n",(int)op1,(int)op2,iresult); // Printing the addition result.
            break;
        }
        else if (fou=='S') {
            dresult = op1 + op2;

            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g + %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g + %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");			// Printing 0 to the start of the result to reach the wanted m.

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            
    
            
            break;
        }

    case '-':
		if (fou=='I') {
            iresult = (int)op1 - (int)op2; // Substraction calculation.
            printf("%d - %d = %d\n",(int)op1,(int)op2,iresult); // Printing the substraction result.
            break;
        }
        else if (fou=='S') {
            dresult = op1 - op2;
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g - %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g - %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
         }
	case '/':
		if (fou=='I') {
            iresult = op1 / op2;  // Division calculation.
            printf("%d / %d = %d\n",(int)op1,(int)op2,iresult); // Printing the division result.
            break;
        }
        else if (fou=='S') {
            dresult = (float)op1 / (float)op2;
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <=  n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g / %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g / %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
        }
	case '*':
		if (fou=='I') {
            iresult = (int)op1 * (int)op2;  // Multiplication calculation.
            printf("%d * %d = %d\n",(int)op1,(int)op2,iresult); // Printing the multiplication result.
            break;
        }
        else if (fou=='S') {
            dresult = op1 * op2;
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {
                
                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <=  n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g * %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g * %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
        }
	case '%':
		if (fou=='I') {
            iresult = (int)op1 % (int)op2;     // Mod calculation.
            printf("%d %% %d = %d\n",(int)op1,(int)op2,iresult);   // Printing the mod result.
            break;
        }
        else if (fou=='S') {
            dresult = fmod(op1,op2);
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <=  n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g %% %g = ",op1,op2); // Mod operation can only be operated with integer numbers in C.
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%g %% %g = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
        }
	case '!':
        int i;
        iresult=1;
        dresult = 1.0;
        op2=1;
		if (fou=='I') {
            for(i=1;i<=(int)op1;i++){
                
                iresult= iresult*i ; // Factorial calculation.

            }
            printf("%d! = %d\n",(int)op1,iresult);  // Printing the factorial result.
            break;
        }
        else if (fou=='S') {
            for(i=1;i<=(int)op1;i++){
                
                dresult= dresult*i ;

            }
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%d! = ",(int)op1); // Factorial operation can only be operated with integer numbers in C.
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) >= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%d! = ",(int)op1);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
        }
    case '^':
		if (fou=='I') {
            iresult = pow((int)op1,(int)op2);  // Power calculation.
            printf("%d ^ %d = %d\n",(int)op1,(int)op2,iresult);   // Printing the power result.
            break;
        }
        else if (fou=='S') {
            dresult = pow(op1,op2);
            if (n >= m || (float_digit(dresult) + int_digits(dresult)) > m  ) {

                printf("Error! Either n is greater than m or total digits of the result is greater than m!\n"); // m=3 n=5 or result= 94.237 and m=4 n=2.
    
                }

            else if ( float_digit(dresult) <= n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%lg ^ %lg = ",op1,op2); 
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * pow(10,-1);     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe%d\n",n,dresult,c);
        }

        else if ( float_digit(dresult) > n) { // If floating point digit is lower than the wanted (n) the number will be divised by 10.
                                                  // For example if res is 12.397 and n=4 to increase the floating point
                                                  // The number will be divised by 10.
            printf("%lg ^ %lg = ",op1,op2);
            int c = 0;
            while (float_digit(dresult) != n ) {
                dresult = dresult * 10 ;     // While the floating point is not equal to n.
                c++;                                // Result will be multiplied with 10^-1 for example n=3 result = 12.3
                                                    // It will become 1.23 then 0.123 then stop.
                                                    // c will be the 0.123e(c) which is  0.123e3.    
            }

            int ii = int_digits(dresult);
                while((ii + float_digit(dresult)) < m ) {
                        m--;
                        printf("0");

                    }
            printf("%.*fe-%d\n",n,dresult,c);
        }
            break;
    }






	default:
            printf("Error! Operator is not correct.\n");  // Operator is different from the given operators for example '?' or '&'
                                                          // It will print an error message.
            break;
    }   
  
}





// Function 3 //

void part3(float exam1,float exam2,float exam3,float assignment1,float assignment2) {

float final;



final = (((exam1 + exam2 + exam3) / 3) * 0.6) + (((assignment1 + assignment2) / 2)* 0.4) ; // Calculating the final grade.

if (final >= 60) {

        printf("Final grade : %.1f Passed!\n",final); // If final grade is greater equal than 60 then the final grade will
                                                      // be printed with the Passed message.
    }
    else {

        printf("Final grade : %.1f Failed!\n",final); // If final grade is lower 60 then the final grade will
                                                      // be printed with the Failed message.

    }
}





int main() {

    // PART 1

    int year; // The variable needed for the part1 function.
    printf("------------------------------PART 1 ------------------------------\n");
    printf("****************************\n");
    printf("Please enter a year : ");    // Taking the year input from the user.
    scanf("%d",&year);
    part1(year);

    // PART 2

    char fou ;
    int m;
    int n;
    char operation;     // All the variables needed for the part2 function.
    double op1;
    double op2;
    
    printf("------------------------------PART 2 ------------------------------\n");	
    printf("****************************\n");

    printf("Enter the format of output(S OR I) : ") ;   // Taking all the inputs needed for the part2 function from the user.
    scanf(" %c", &fou);

   

    if (fou=='S') {
     printf("Enter m and n values : "); // If the format is Scientific m and n values will be taken from the user.
     scanf("%d %d",&m,&n);
    }
    
    printf("Enter the operation (+, -,/,*,%%,!,^): ");  // Taking all the inputs needed for the part2 function from the user.
    scanf(" %c", &operation);
    
    if (operation != '!') {
    	
    printf("Enter the first operand: ");   // Taking all the inputs needed for the part2 function from the user.
    scanf("%lg",&op1);
    
    
    printf("Enter the second operand: ");  // If the operator is factorial 2nd operand will not be taken from the user since it only needs 1 operand.
    scanf("%lg",&op2);
    }
    
    else if (operation == '!') {
    
    printf("Enter the operand: ");   // Taking all the inputs needed for the part2 function from the user.
    //op1 = (int)op1;
    scanf("%lg",&op1);
    op2=1;
    }
    part2(fou,m,n,operation,op1,op2);

    

    // PART 3

    float exam1;
    float exam2;
    float exam3;
    float assignment1;      // All the variables needed for the part3 function.
    float assignment2;
    float final;
    printf("------------------------------PART 3 ------------------------------\n");	
    printf("****************************\n");

    printf("Enter 3 exam grades of student : ");    // Taking the exam grade inputs from the user.
    scanf("%f %f %f",&exam1,&exam2,&exam3); 

    printf("Enter 2 assignment grades of student : ");  // Taking the assignment grade inputs from the user.
    scanf("%f %f",&assignment1,&assignment2);
    
    if (exam1<0 || exam1>100 || exam2<0 || exam2>100 || exam3<0 || exam3>100 || assignment1<0 || assignment1>100 || assignment2<0 || assignment2>100 )
	// If the exam or assignment is not valid ex: 125 or -23 or etc.
	printf("Please enter a valid exam or assignment result!\n");
    else{	
			
    part3(exam1,exam2,exam3,assignment1,assignment2);  

    }
    

    return 0 ;

}
