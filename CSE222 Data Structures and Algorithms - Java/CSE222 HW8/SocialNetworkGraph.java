import java.util.*;

public class SocialNetworkGraph {
    Map<String, Person> People ;
    Map<Person, List<Person>> Friendships;
    
    /**
     * Constructor of SocialNetworkGraph.
     */
    public SocialNetworkGraph(){
        People = new HashMap<>();
        Friendships = new HashMap<>();
    }

    /**
     * Method to add person to people map.First the inputs of name , age and hobbies are taken in the Main class main method then person is created 
     * with these datas after that this person is added to people map and added to the friendship graph with an empty list for friends.
     * I did not include timestamp while taking the input since your code also did not include it and also it is hard to enter the exact date from keyboard.
     * @param Person_Name
     * @param age
     * @param hobbies
     */
    public void AddPerson(String Person_Name , int age , String hobbies ){
        Person newPerson = new Person(Person_Name, age, hobbies);
        People.put(Person_Name, newPerson);
        Friendships.put(newPerson, new ArrayList<>());
        System.out.println("Person added -> " + newPerson) ;
    }

    /**
     * Method to remove person from people map.First the input of name is taken in the Main class main method then person matches
     * with this name is removed and also it is removed from friendsip graph too both key and values.
     * An error message will be printed if the person to be deleted does not exist.
     * @param Person_Name
     * @param age
     * @param hobbies
     */
    public void RemovePerson(String Person_Name){
        Person deletePerson = People.get(Person_Name);
        if(deletePerson!=null){
            People.remove(deletePerson); // removing from people map
            Friendships.remove(deletePerson); // removing from friendship graph key.
            for (Person p : Friendships.keySet()) { // removing from other person's friendship list
                List<Person> friends = Friendships.get(p);
                for(int i=0 ; i<friends.size() ; i++){
                    if(friends.get(i).equals(deletePerson))
                        friends.remove(i);
                }
            }

            System.out.println("Person " + deletePerson + " IS DELETED !");
        }
        else {
            System.out.println("The person you are trying to remove does not exist!");
        }
    }   

    /**
     * Method to display friendships
     */
    public void displayFriendships() {
        for (Person p : Friendships.keySet()) {
            System.out.print(p.getName() + " is friends with: ");
            List<Person> friends = Friendships.get(p);
            for (int i = 0; i < friends.size(); i++) {
                System.out.print(friends.get(i).getName());
                if (i < friends.size() - 1) {
                    System.out.print(", ");
                }
            }
            System.out.println();
        }
    }
    
    /**
     * Method to add friendship between two person.It gets the 2 person from their names then if both of these persons exist
     * it adds friendship between them for both of them one by one.
     * @param person1_name
     * @param person2_name
     */
    public void AddFriendship(String person1_name , String person2_name){
        
        Person person1 = People.get(person1_name);
        Person person2 = People.get(person2_name);

        if (person1 != null && person2 != null) {
            Friendships.get(person1).add(person2);
            Friendships.get(person2).add(person1);
            System.out.println("Friendship added between " + person1_name + " and " + person2_name);
        } else {
            System.out.println("Friendship can not be added ! One or both persons not found in the network.");
        }
    }

    /**
     * Method to remove friendship between two person.It gets the 2 person from their names then if both of these persons exist
     * it removes the friendship between them for both of them one by one.
     * @param person1_name
     * @param person2_name
     */
    public void RemoveFriendship(String person1_name , String person2_name){

        Person person1 = People.get(person1_name);
        Person person2 = People.get(person2_name);
        
        if (person1 != null && person2 != null) {
            Friendships.get(person1).remove(person2);
            Friendships.get(person2).remove(person1);
            System.out.println("Friendship removed between " + person1_name + " and " + person2_name);
        } else {
            System.out.println("Friendship can not be deleted! One or both persons not found in the network.");
        }

    }

    /**
     * Breadth First Search algorithm of the friendship graph.
     * @param start
     * @param visited
     * @param cluster
     */
    private void bfs(Person start, Set<Person> visited, List<Person> cluster) {
        Queue<Person> queue = new LinkedList<>();
        queue.add(start);
        visited.add(start);

        while (!queue.isEmpty()) {
            Person current = queue.poll();
            cluster.add(current);

            for (Person neighbor : Friendships.get(current)) {
                if (!visited.contains(neighbor)) {
                    queue.add(neighbor);
                    visited.add(neighbor);
                }
            }
        }
    }



    /**
     * Finds and prints the shortest path between two persons in the friendship graph using BFS.More detailed comments are inside the method.
     * @param startName The name of the starting person.
     * @param endName The name of the ending person.
     */
    public void FindShortestPath(String startName, String endName) {
        // Getting the Person objects for the given names
        Person start = People.get(startName);
        Person end = People.get(endName);

        // Check if both persons exist in the graph
        if (start == null || end == null) {
            System.out.println("One or both persons not found in the graph.");
            return;
        }

        // Initialize a queue for BFS, a map to keep track of previous nodes, and a set for visited nodes
        Queue<Person> queue = new LinkedList<>();
        Map<Person, Person> prev = new HashMap<>();
        Set<Person> visited = new HashSet<>();

        // Start BFS from the starting person
        queue.add(start);
        visited.add(start);

        // Perform BFS
        while (!queue.isEmpty()) {
            Person current = queue.poll();

            // If the current person is the end person then print the path and return
            if (current.equals(end)) {
                printPath(start, end, prev);
                return;
            }

            // Iterate over the neighbors of the current person
            for (Person neighbor : Friendships.getOrDefault(current, new ArrayList<>())) {
                // If the neighbor has not been visited, adding it to the queue and mark it as visited
                if (!visited.contains(neighbor)) {
                    queue.add(neighbor);
                    visited.add(neighbor);
                    // Keeping track of the path by storing the previous node for each neighbor
                    prev.put(neighbor, current);
                }
            }
        }

        // If the queue is empty and the end person was not found, print that no path was found
        System.out.println("No path found between " + startName + " and " + endName);
    }

    /**
     * Method for printing the path between the shortest path.
     * @param start
     * @param end
     * @param prev
     */
    private void printPath(Person start, Person end, Map<Person, Person> prev) {
        List<String> path = new ArrayList<>();
        System.out.print("Shortest path : ");
        for (Person at = end; at != null; at = prev.get(at)) {
            path.add(at.getName());
        }
        Collections.reverse(path);
        for(int i=0 ; i<path.size() ; i++){
            if(i==path.size()-1)
                System.out.println(path.get(i));
            else{
                System.out.print(path.get(i) + " -> ");
            }
        }

    }

    /**
     * Counts and displays the clusters in the friendship graph.More detailed comments are inside the method.
     * @return the count of clusters.
     */
    public int CountClusters() {
        // Set to keep track of visited persons
        Set<Person> visited = new HashSet<>();
        // Variable to count the number of clusters
        int count = 0;
        // List to store all clusters
        List<List<Person>> clusters = new ArrayList<>();

        // Iterate over all persons in the friendship graph
        for (Person person : Friendships.keySet()) {
            // If the person is not visited  perform bfs to find the cluster
            if (!visited.contains(person)) {
                // List to store the current cluster
                List<Person> cluster = new ArrayList<>();
                // Performing bfs starting from the current person to find all connected persons
                bfs(person, visited, cluster);
                // Add the found cluster to the list of clusters
                clusters.add(cluster);
                // Incrementing the cluster count
                count++;
            }
        }

        // Variable to keep track of the cluster index for display purposes
        int clusterIndex = 1;
        // Iterate over all clusters to print their members
        for (List<Person> cluster : clusters) {
            System.out.println("Cluster " + clusterIndex + ":");
            for (Person person : cluster) {
                // Print each person in the current cluster
                System.out.println(person);
            }
            // Increment the cluster index
            clusterIndex++;
        }

        // Return the total count of clusters
        return count;
    }

    /**
     * method to suggest friends for the given person and given number of suggestions.
     * First the person with that name is found.If there is no such person or suggestion number is < 1 then error message will be printed.
     * Otherwise the hobbies will be checked first then the hobby count will be calculated
     * after that mutual friends will be calculated and according to that for all person objects and score will be calculated and then scores will be sorted
     * and first N(suggestion number) of this sorted persons will be printed.
     * @param name
     * @param suggestion_number
     */
    public void SuggestFriends(String name, int suggestion_number) {
        // Check if the person exists and suggestion number is valid
        if (People.containsKey(name) && suggestion_number > 0) {
            System.out.println("Suggested friends for " + name + ":");
    
            // List to store potential friends and their scores
            List<Person> suggestions = new ArrayList<>();
    
            // Map to store total scores and mutual friend counts for each potential friend
            Map<Person, Double> scores = new HashMap<>();
            Map<Person, Integer> mutualFriendCounts = new HashMap<>();
    
            // Iterate over all persons in the graph
            for (Person p : People.values()) {
                // Skip if it is the same person or they are already friends(contains)
                if (!p.getName().equals(name) && !Friendships.getOrDefault(People.get(name), new ArrayList<>()).contains(p)) {
                    // Counter for common hobbies
                    double hobby_count = 0;
    
                    // Calculating common hobbies
                    for (String hobby : p.getHobbies()) {
                        if (People.get(name).getHobbies().contains(hobby)) {
                            hobby_count += 0.5;
                        }
                    }
    
                    // Calculating mutual friend count
                    int mutualFriendCount = 0;
                    for (Person friend : Friendships.get(p)) {
                        if (Friendships.get(People.get(name)).contains(friend)) {
                            mutualFriendCount++;
                        }
                    }
    
                    // Calculating total score
                    double total = hobby_count + mutualFriendCount;
    
                    // Store the total score and mutual friend count for the potential friend
                    scores.put(p, total);
                    mutualFriendCounts.put(p, mutualFriendCount);
                }
            }
    
            // Sort the potential friends based on the total score
            List<Map.Entry<Person, Double>> sortedScores = new ArrayList<>(scores.entrySet());
            sortedScores.sort(Map.Entry.comparingByValue(Comparator.reverseOrder()));
    
            // Print the top suggestion_number suggestions
            for (int i = 0; i < Math.min(suggestion_number, sortedScores.size()); i++) {
                Map.Entry<Person, Double> entry = sortedScores.get(i);
                Person suggestedFriend = entry.getKey();
                double total = entry.getValue();
                int mutualFriendCount = mutualFriendCounts.get(suggestedFriend);
                System.out.println(suggestedFriend.getName() + " ( Score: " + total + ", " + mutualFriendCount + " mutual friends, " + (total-mutualFriendCount) / 0.5 + " common hobbies)");
                suggestions.add(suggestedFriend);
            }
    
            // If not enough suggestions were found
            if (suggestions.size() < suggestion_number) {
                System.out.println("Not enough suggestions available.");
            }
        } else {
            System.out.println("The person is not found or suggestion number is lower than 1!");
        }
    }
    
    

}