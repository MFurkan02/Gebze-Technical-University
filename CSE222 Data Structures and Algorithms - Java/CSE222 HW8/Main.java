import java.util.Scanner;

/**
 * Main method for adding/removing people, adding/removing friendships , finding the shortest path , suggesting friends and counting clusters.
 * It takes an integer choice between 1-8 and according to the number performs the action by taking inputs from the user (name , age , etc.) for the
 * related method. 
 */
public class Main {
    public static void main(String[] args){

        SocialNetworkGraph network = new SocialNetworkGraph();
        // Adding some people for demonstration
        
        network.AddPerson("John Doe", 25, "reading,hiking,cooking");
        network.AddPerson("Jane Smith", 22, "swimming,cooking");
        network.AddPerson("Alice Johnson", 27, "hiking,painting");
        network.AddPerson("Bob Brown", 30, "reading,swimming");
        network.AddPerson("Emily Davis", 28, "running,swimming");
        network.AddPerson("Frank Wilson", 26,"reading,hiking");
        

        // Adding friendships for demonstration
        network.AddFriendship("John Doe", "Jane Smith");
        network.AddFriendship("John Doe", "Alice Johnson");
        network.AddFriendship("Jane Smith", "Bob Brown");
        network.AddFriendship("Emily Davis", "Frank Wilson");
        //network.AddFriendship("Bob Brown","Emily Davis");

        network.SuggestFriends("John Doe",2);
        // Finding shortest path for demonstration
        //network.FindShortestPath("John Doe","Emily Davis");
        network.FindShortestPath("John Doe","Bob Brown");


        // Counting clusters for demonstration
        network.CountClusters();

        System.out.println("DISPLAYING FRIENDSHIPS");
        network.displayFriendships();

        network.RemovePerson("Emily Davis");
        network.RemoveFriendship("John Doe","Jane Smith");

        System.out.println("DISPLAYING FRIENDSHIPS");
        network.displayFriendships();

        network.FindShortestPath("John Doe","Bob Brown");

        Scanner scanner = new Scanner(System.in);
        int choice = 0;
        SocialNetworkGraph sng = new SocialNetworkGraph();

        while (choice != 8) {
            System.out.println("===== Social Network Analysis Menu =====");
            System.out.println("1. Add person");
            System.out.println("2. Remove person");
            System.out.println("3. Add friendship");
            System.out.println("4. Remove friendship");
            System.out.println("5. Find shortest path");
            System.out.println("6. Suggest friends");
            System.out.println("7. Count clusters");
            System.out.println("8. Exit");
            System.out.print("Please select an option: ");
            try{
                if (scanner.hasNextInt()) {
                    choice = scanner.nextInt();
                    scanner.nextLine();
                    String timestamp1 , timestamp2 ;
                    switch (choice) {
                        case 1:
                            System.out.print("Enter name : ");
                            String name = scanner.nextLine() ;
                            System.out.print("Enter age : ");
                            int age  = scanner.nextInt() ;
                            scanner.nextLine();
                            System.out.print("Enter hobbies (comma-separated): ");                        
                            String hobbies = scanner.nextLine() ;
                            sng.AddPerson(name , age , hobbies );
                            break;
                        case 2:
                            System.out.print("Enter name of the person to be deleted : ");
                            name = scanner.nextLine() ;
                            sng.RemovePerson(name);
                            break;
                        case 3:
                            System.out.print("Enter first person's name : " );
                            name = scanner.nextLine() ;
                            /*System.out.print("Enter first person's timestamp: " );
                            timestamp1 = scanner.nextLine();*/
                            System.out.print("Enter second person's name: ");
                            String name2 = scanner.nextLine() ;
                            /*System.out.print("Enter second person's timestamp: ");
                            timestamp2 = scanner.nextLine();*/
                            sng.AddFriendship(name , name2 );
                            break;
                        case 4:
                            System.out.print("Enter first person's name : " );
                            name = scanner.nextLine() ;
                            /*System.out.print("Enter first person's timestamp: " );
                            timestamp1 = scanner.nextLine();*/
                            System.out.print("Enter second person's name: ");
                            name2 = scanner.nextLine() ;
                            /*System.out.print("Enter second person's timestamp: ");
                            timestamp2 = scanner.nextLine();*/
                            sng.RemoveFriendship( name , name2 );
                            break;
                        case 5:
                            System.out.print("Enter first person's name : " );
                            name = scanner.nextLine() ;
                            /*System.out.print("Enter first person's timestamp: " );
                            timestamp1 = scanner.nextLine();*/
                            System.out.print("Enter second person's name: ");
                            name2 = scanner.nextLine() ;
                            /*System.out.print("Enter second person's timestamp: ");
                            timestamp2 = scanner.nextLine();*/
                            sng.FindShortestPath(name,name2);
                            break;
                        case 6:
                            System.out.print("Enter person's name : ");
                            name = scanner.nextLine() ;
                            /*System.out.print("Enter person's timestamp : ");
                            /*timestamp1 = scanner.nextLine() ;*/
                            System.out.print("Enter maximum number of friends to suggest: ");
                            int suggestion_number = scanner.nextInt();
                            scanner.nextLine();
                            sng.SuggestFriends(name,suggestion_number);
                            break;
                        case 7:
                            System.out.println("Counting clusters in the social network...");
                            System.out.println("Number of clusters found : " + sng.CountClusters());
                            break;
                        case 8:
                            System.out.println("Exiting...");
                            break;
                        default:
                            System.out.println("Invalid choice. Please enter a number between 1 and 8 ! ");
                    }
                } else {
                    System.out.println("Invalid input. Please enter a number between 1 and 8.");
                    scanner.next(); // clear invalid input
                }
        }
        catch( Exception ex ){
            System.out.println("An error occured while taking the input from the user.Please try again!");
        }
    }
        scanner.close();
    }

    /*
    SocialNetworkGraph network = new SocialNetworkGraph();
        Scanner scanner = new Scanner(System.in);

        // Adding some people for demonstration
        network.addPerson("John Doe", 25, Arrays.asList("reading", "hiking", "cooking"));
        network.addPerson("Jane Smith", 22, Arrays.asList("swimming", "cooking"));
        network.addPerson("Alice Johnson", 27, Arrays.asList("hiking", "painting"));
        network.addPerson("Bob Brown", 30, Arrays.asList("reading", "swimming"));
        network.addPerson("Emily Davis", 28, Arrays.asList("running", "swimming"));
        network.addPerson("Frank Wilson", 26, Arrays.asList("reading", "hiking"));

        // Adding friendships for demonstration
        network.addFriendship("John Doe", "Jane Smith");
        network.addFriendship("John Doe", "Alice Johnson");
        network.addFriendship("Jane Smith", "Bob Brown");
        network.addFriendship("Emily Davis", "Frank Wilson");

        // Finding shortest path for demonstration
        network.findShortestPath("John Doe", "Bob Brown");

        // Counting clusters for demonstration
        network.countClusters();

        scanner.close();
     */
}

