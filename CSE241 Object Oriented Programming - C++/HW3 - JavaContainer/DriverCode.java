package furcontainer;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


/**
 * Driver class to test the implemented methods and write sets and vectors to text files.
 * It gets an input between 1-3 to either auto test,create a vector or create a set.
 * For 1 it automatically creates a vector and a set and it tries all the methods for the vector and set such as Add,Remove,ToString,Size and equals finally.
 * Finally it writes these set and vector to file using the Iterator created. 
 * For 2 and 3 it wants input to take the capacity from the user checking for validity while capacity>1 
 * After that in a menu it wants the user to try the functions Add,Remove,Size,ToString and finally if 5 is chosen
 * It will write the set or vector to file using Iterator and terminate the program.
 * As before it will check validity for the input to be between 1-5.
 */
public class DriverCode {

    public static void main(String[] args) {
        // Test each method at least 2 times and print the results
        
        System.out.println("1.Auto test vector and set.");
        System.out.println("2.Create a vector.");
        System.out.println("3.Create a set.");

        Scanner input = new Scanner(System.in);
        // Prompt the user to enter the number.
        System.out.print("Enter your choice (1-3) : ");
        // Read the user's input as a string
        Integer choice = input.nextInt();
    
        if(choice==1){
            JavaContainer<Integer> vector = new JavaVector<>(5);
            JavaContainer<Integer> set = new JavaSet<>(5);
            System.out.println("--- SET ---");
            Auto_testContainer(set);
            System.out.println("\n");
            System.out.println("--- VECTOR ---");
            Auto_testContainer(vector);
            writeContainertoFile(vector, "vector.txt");
            writeContainertoFile(set, "set.txt");

            //Equals function test

            JavaSet<Integer> eqset = new JavaSet<Integer>(5);
            JavaSet<Integer> eqset2 = new JavaSet<Integer>(5);

            JavaVector<Integer> eqvec = new JavaVector<Integer>(5);
            JavaVector<Integer> eqvec2 = new JavaVector<Integer>(5);
            System.out.println("Creating new vectors and sets to try the overriden equals method...\n");
            System.out.println("Set1 is being created");
            eqset.Add(23);
            eqset.Add(49);
            eqset.Add(97);
            System.out.println(eqset.toString());
            writeContainertoFile(eqset,"set3.txt");
            System.out.println("Set2 is being created");
            eqset2.Add(49);
            eqset2.Add(97);
            eqset2.Add(23);
            System.out.println(eqset2.toString());
            writeContainertoFile(eqset2,"set4.txt");
            System.out.println("Is Set1 equals to Set2 ? -> " + eqset.equals(eqset2) + "\n");

            System.out.println("Vector1 is being created");
            eqvec.Add(43);
            eqvec.Add(57);
            eqvec.Add(83);
            System.out.println(eqvec.toString());

            System.out.println("Vector2 is being created");
            eqvec2.Add(43);
            eqvec2.Add(57);
            eqvec2.Add(83);
            System.out.println(eqvec2.toString());
            System.out.println("Is Vector1 equals to Vector2 ? -> " + eqvec.equals(eqvec2));
            
        }
        else if(choice==2){
            System.out.print("Enter the capacity of the vector you want to create : ");
                Integer choice2 = input.nextInt();
                while(choice2<1){
                    System.out.println("Capacity can not be lower than one!");
                    choice2 = input.nextInt();
                }
            JavaContainer<String> vector = new JavaVector<> (choice2);
            CreateVector(vector);
        }
        else if(choice==3){
            System.out.print("Enter the capacity of the set you want to create : ");
            Integer choice3 = input.nextInt();
            while(choice3<1){
                System.out.println("Capacity can not be lower than one!");
                choice3 = input.nextInt();
            }
            JavaContainer<String> set = new JavaSet<>(choice3);
            CreateSet(set);
        }
        
        // Creating instances for JavaVector and JavaSet for a given capacity.


        // Test add, remove, size, getIterator, toString, and equals methods

        // Write sets to text files

        input.close();
    }

    /**
     * Auto test method to test the created vector and set as container
     * It tests Add,Remove,Size methods  
     * and ToString method according to the class instance.
     * It throws an exception if the element to be added is already in the set(only for set) and 
     * element to be removed does not exist in the set or vector. 
      * @param container
     */
    public static void Auto_testContainer(JavaContainer<Integer> container) {
        
        
            try{
                container.Add(1);
                container.Add(2);
                container.Add(3);
                container.Add(4);
                container.Add(5);
                container.Add(6);
                container.Add(7);
                container.Add(8);
                container.Add(9); // exception will occur for set since 'Element2' is already in the set.
                container.Add(10);
                container.Remove(1);
                container.Remove(6);
                //  container.remove("Element8"); // exception will occur for both since 'Element8' is not in the container.
                
                
                System.out.println("Size: " + container.Size()); // printing the size of the container.
                if(container instanceof JavaVector)
                    System.out.print(((JavaVector)container).toString()); // printing the container with ToString function using the iterator in the function.
                else if(container instanceof JavaSet)
                    System.out.print(((JavaSet)container).toString()); // printing the container with ToString function using the iterator in the function.
                // Testing equals function.
                var check=2;
                var check2=3;

                if(container instanceof JavaVector){
                    System.out.println("Is " + check  + " in the container? -> " + ((JavaVector)container).equals2(check));
                    System.out.println("Is " + check2  + " in the container? -> " + ((JavaVector)container).equals2(check2));
                }
                else if(container instanceof JavaSet){
                    System.out.println("Is " + check  + " in the container? -> " + ((JavaSet)container).equals2(check));
                    System.out.println("Is " + check2  + " in the container? -> " + ((JavaSet)container).equals2(check2));
                }
                
                
                System.out.println("Adding '3' to container if possible...");
                container.Add(3);
                System.out.println("New Size: " + container.Size()); // printing the size of the container.
                if(container instanceof JavaVector)
                    System.out.print(((JavaVector)container).toString()); // printing the container with ToString function using the iterator in the function.
                else if(container instanceof JavaSet)
                    System.out.print(((JavaSet)container).toString()); // printing the container with ToString function using the iterator in the function.
                System.out.println("Removing '11' from container if possible...");
                container.Remove(11);
                
            }
            catch(IllegalArgumentException ex){ // catching if there is an exception in add or remove functions.
                System.out.println("Exception! -> " + ex.getMessage());
            }
            // Close the scanner to release system resources
           // input.close();
        }

        /**
         * If the choice is 2 to create a vector with user input.
         * It creates the vector using the CreateVector method and 
         * adds,removes elements and returns the size and displays it according to the user input. 
         * @param vector
         */
        public static void CreateVector(JavaContainer<String> vector){
            Scanner input = new Scanner(System.in);
            try{
                
                // Read the user's input as a string
                while(true){
                    System.out.println("1.Add an element to vector");
                    System.out.println("2.Remove an element from vector");
                    System.out.println("3.Get the current size of the vector.");
                    System.out.println("4.Display the vector.(toString)");
                    System.out.println("5.Exit and write the vector to the file 'vector2.txt'.");
                    System.out.print("> ");
                    int val = input.nextInt();
                    input.nextLine();
                    switch(val){
                        case 1:
                            System.out.print("Enter the element to be added : ");
                            String obj = input.nextLine();
                            vector.Add(obj);
                            break;
                        case 2:
                            System.out.print("Enter the element to be removed : ");
                            String obj2 = input.nextLine();
                            vector.Remove(obj2);
                            break;
                        case 3:
                            System.out.println("Size of the vector is " + vector.Size());
                            break;
                        case 4:
                            System.out.println(((JavaVector)vector).toString());
                            break;
                        case 5:
                            writeContainertoFile(vector,"vector2.txt");
                            return;
                        default:
                            System.out.println("Enter a number between 1-5 !");
                    }
                }
            }
            catch(IllegalArgumentException ex){ // catching if there is an exception in add or remove functions.
                System.out.println("Exception! -> " + ex.getMessage());
            }

            input.close();
    }

    /**
         * If the choice is 3 to create a set with user input.
         * It creates the set using the CreateSet method and 
         * adds,removes elements and returns the size and displays it according to the user input. 
         * @param set
         */
    public static void CreateSet(JavaContainer<String> set){
        Scanner input = new Scanner(System.in);
        try{

            // Read the user's input as a string
            while(true){
                System.out.println("1.Add an element to set");
                System.out.println("2.Remove an element from set");
                System.out.println("3.Get the current size of the set.");
                System.out.println("4.Display the set.(toString)");
                System.out.println("5.Exit and write the vector to the file 'set2.txt'.");
                System.out.print("> ");
                int val = input.nextInt();
                switch(val){
                    case 1:
                        System.out.print("Enter the element to be added : ");
                        String obj = input.next();
                        set.Add(obj);
                        break;
                    case 2:
                        System.out.print("Enter the element to be removed : ");
                        String obj2 = input.next();
                        set.Remove(obj2);
                        break;
                    case 3:
                        System.out.println("Size of the set is " + set.Size());
                        break;
                    case 4:
                        System.out.println(((JavaSet)set).toString());
                        break;
                    case 5:
                        writeContainertoFile(set,"set2.txt");
                        return;
                    default:
                        System.out.println("Enter a number between 1-5 !");
                }
            }
        }
        catch(IllegalArgumentException ex){ // catching if there is an exception in add or remove functions.
            System.out.println("Exception! -> " + ex.getMessage());
        }
        input.close();
    }

    /**
     * Method to write the vectors or sets to files using the iterator created to iterate through the container(vector or set).
     * And writes all the elements to file line by line.
     * @param container
     * @param fileName
     */
    private static void writeContainertoFile(JavaContainer container, String fileName) {
        // Write set elements to a text file
        try (FileWriter ContainertoFile = new FileWriter(fileName)) {
            Iterator it = container.getIterator();
            while (it.hasNext()) { // uisng the iterator to write the set and vectors to file.
                ContainertoFile.write(it.next() + "\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}