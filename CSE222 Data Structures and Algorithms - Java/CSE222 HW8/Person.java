import java.util.*;

public class Person {
    private String Name ;
    private int Age ;
    private List<String>hobbies ;
    private Date timestamp;

    /**
     * Constructor1 of Person class.
     * @param Name
     * @param Age
     * @param hobbies
     */
    public Person(String Name,int Age,List<String>hobbies){
        this.Name = Name;
        this.Age = Age ;
        this.hobbies = hobbies ;
        this.timestamp = new Date();
    }

    /**
     * Constructor2 of Person class.
     * @param Name
     * @param Age
     * @param hobbies
     */
    public Person(String Name,int Age,String hobbies){
        this.Name = Name;
        this.Age = Age ;
        this.hobbies = HobbiesToList(hobbies); ;
        this.timestamp = new Date();
    }

    /**
     * method for adding the hobbies taken as string to a list after splitting them from ',' and returns the list.
     * @param hobbies
     * @return
     */
    private List<String> HobbiesToList(String hobbies){
        String[] hobbiesArray = hobbies.split(",");
        List<String> temp = new ArrayList<>();
        for(String hobby : hobbiesArray)
            temp.add(hobby);
        return temp;
    }

    /**
     * getter for getting the hobby list of a person.
     * @return
     */
    public List<String> getHobbies(){
        return hobbies;
    }

    /**
     * getter for getting the name of the person.
     * @return
     */
    public String getName(){
        return Name;
    }

    /**
     * getter method for getting the timestamp of the person.
     * @return
     */
    public Date getTime(){
        return timestamp;
    }

    /**
     * overridden toString method for printing information about the person. Name , age , hobbies and timestamp.
     */
    @Override
    public String toString(){
        return "Name : " + Name + " Age : " + Age + " Hobbies = " + hobbies + " Creation Time = " + timestamp ; 
    }
}