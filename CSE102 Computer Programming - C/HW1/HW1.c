#include <stdio.h> 

int main() {
    // PART 1
    
    int number1,number2,gcd;
    
    int part1(int number1,int number2); // Calling the part1 function.
    printf("---- PART 1 ----\n");
    printf("Enter the first integer number: ");  
    scanf("%d", &number1);
    
    printf("Enter the second integer number: ");  
    scanf("%d", &number2);

    gcd = part1(number1,number2);

    printf("GCD of these two integers : %d\n",gcd);

    // PART 2
    
    int num1,num2,res2;

    int part2(int num1,int num2);	// Calling the part2 function.
    
    printf("---- PART 2 ----\n");
    printf("First number: ");  
    scanf("%d", &num1);
    
    printf("Second number: ");  
    scanf("%d", &num2);
    
    printf("Result:\n");
    
    res2 = part2(num1,num2); // Getting the addition result from the function.

    printf("%7d\n%7d\n+\n-------\n%7d\n",num1,num2,res2); //Last result after the signs(+,-------) and addition.



    // PART 3 

    int n1,n2,num3,num4,num5,num6,res3;
    
    int part3(int n1,int n2);	 // Calling the part3 function.
    printf("---- PART 3 ----\n");
    printf("First number: ");  
    scanf("%d", &n1);
    
    printf("Second number: ");  
    scanf("%d", &n2);
    printf("Result:\n");
    
    res3 = part3(n1,n2); // Getting the multiplication result from the function.

    num3= (n2%10)*n1;            // Multiplication of the units digit of number2 with the number1.
    num4= ((n2/10)%10)*n1;      // Multiplication of the tens digit of number2 with the number1.
    num5= (((n2/100))%10)*(n1);   // Multiplication of the hundreds digit of number2 with the number1.
    num6=(n2/1000)*(n1);            // Multiplication of the thousands digit of number2 with the number1.
    
    int con=0;

    if ( ((9>=n1) & (n1>=-9)) || (num4==0 & num5==0 & num6==0)) {  // If the first number only has 1 digit or the second number only has 1 digit.

        printf("%7d\n%7d\n*\n-------\n%7d\n",n1,n2,res3);  // It will print the multiplication only, since on paper 123*1 or 1*123 is shown only by the result.
        con=-1;

    }

    else if (num5==0 & num6==0 && con!=-1) {
        printf("%7d\n%7d\n*\n-------\n%7d\n%6d\n+\n-------\n%7d\n",n1,n2,num3,num4,res3); // If n2(2nd number) has 2 digits ex:12 .
        con=-2;
    }
     
    else if (num6!=0){
        printf("%7d\n%7d\n*\n-------\n%7d\n%6d\n%5d\n%4d\n+\n-------\n%7d\n",n1,n2,num3,num4,num5,num6,res3); // If second number has 4 digits.(1234) 
    }
    
    else {
    printf("%7d\n%7d\n*\n-------\n%7d\n%6d\n%5d\n+\n-------\n%7d\n",n1,n2,num3,num4,num5,res3);  // If second number has 3 digits.(123) 
    }

    // PART 4 

    int numberp4,cond;
    
    int part4(int numberp4); // Calling the part4 function.
    
    printf("---- PART 4 ----\n");
    printf("Enter an integer between 1 and 10 (1 and 10 is inclusive) : ");  
    scanf("%d", &numberp4);
    
    cond=part4(numberp4);
    if (cond==0 ) {
        printf("Invalid input.\n");  // Details are inside the part4 function.

    }

    if (cond==10 ) {
        printf("The integer you entered is greater than 5.\n");
    }

    if (cond==5) {
        printf("The integer you entered is less than or equal to 5.\n");
    }


    return 0;
    
}




int part1(int number1,int number2) {

    int gcd,gcd1;

    gcd=number1 % number2 ;	// Remainder of the division of number1 and number2.
    
    if (number1>=number2 & gcd==0) {	// If the gcd is already zero for example 46 % 23 = 0 it will return the smaller number which is 23.
    					// However if it is 23 % 46 or something else gcd won't be zero and it will continue to while statement.
        return number2;
    }

    while (gcd!=0) {		// While the gcd is not zero it will apply the Euclidean algorithm.

        gcd = number1 % number2;  // For example 25 % 10 = 5
        number1=number2;	 // It will be 10 % 5 = 0 according to the Euclidean algorithm.And the gcd will be the number2 which is 5 when the gcd is zero. 
        number2=gcd;		// However in my code it will be the number1 since number1 will be assigned to number2.
	
        gcd1=number1; 
       
    }
    return gcd1;
    
    
}

int part2(int num1,int num2) {

    int res2;

    res2 = num1 + num2; //Result of the addition.
 
    return res2;
}

int part3(int n1,int n2) {

    int res3;

    res3 = n1 * n2; // Result of the multiplication.
 
    return res3;
}

int part4(int numberp4) {
    int cond;
    
    if (numberp4>5 & numberp4<=10 ) {  // If the number is between 6-10 returning the first condition to main function to print the related message.
        cond=10;
        return cond;
     }

    else if (numberp4<=5 & numberp4 >=1) {  // If the number is between 1-5 returning the second condition to main function to print the related message.
        cond=5;
        return cond;
        
    }
    else {
    	cond=0;		// If the number is not in range of 1-10 returning the third condition to main function to print the related message.
    	return cond;
        

    }
    
}
