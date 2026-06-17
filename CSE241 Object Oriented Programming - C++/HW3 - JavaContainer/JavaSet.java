package furcontainer;

public class JavaSet<T> implements JavaContainer<T> {
    private int size;
    private int capacity;
    private T[] set;

    /**
     * Constructor that takes the capacity to create a set(array) with that capacity and setting the size to 0. 
     * @param jcapacity
     */
    public JavaSet(int jcapacity) {
        size = 0 ; // at first there is no element in set.
        capacity = jcapacity;
        set = (T[]) new Object[capacity];
    }

    /**
     * Add() method to add a new element to our set.
     * It first checks if the element to be added is in the set or not using equals() method.
     * If the element is already in the set an exception will be thrown.
     * If the element is not in the set first capacity will be checked using checkCapacity() method then
     * the element will be added to the end and the size will be incremented.  
     */
    public void Add(T obj) {
        if(equals2(obj)){
           throw new IllegalArgumentException("The element you are trying to add to set is already in the set!");
        }
            checkCapacity();
            set[size++] = obj;
            System.out.print(obj);
            System.out.println(" is added to the container!");
        }
    
    /*
     * Remove() method to remove a element from our set.
     * It first checks if the element to be removed is in the set or not using equals() method.
     * If the element to be removed is not in the set an exception will be thrown.
     * If the element is in the set then set will be shifted left after the element to be removed to remove the element. 
     * after that the size will be decremented.  
     */
    public void Remove(T obj) {
        if(!equals2(obj)){ 
            throw new IllegalArgumentException("Element to be deleted does not exist in this set!\n");
        }
        int found=0;
        //if(equals2(obj)){ 
            for(int i=0;i<size;++i){
                if(set[i].equals(obj) || found==1 ){
                    found=1;
                    set[i] = set[i+1];
                }
            }
            System.out.print(obj);
            System.out.println(" is removed from the container!");
        //}
        size-=1;    
    }
    

    /**
     * Size() method to return the size of the set.
     */ 
    public int Size() {
        return size;
    }

    /**
     * method that will return an iterator using Iterator class.
     */   
    public Iterator<T> getIterator() {
        return new Iterator<>(set);
    }

    /**
     * checkCapacity() method to check the capacity of the set and 
     * if the capacity <= size + 1 , new capacity will be capacity *= 2
     * and a new temp set will be created to copy our set
     * then our new capacity set will be the same as temp set.
     */
    private void checkCapacity() {
        if (capacity <= size + 1){ 
            capacity = 2 * capacity;
            T[] temp_set= (T[]) new Object[capacity] ; 
            for(int i=0 ; i<size ; ++i)
                temp_set[i] = set[i];
            set = temp_set ;
        }
    }

    /**
     *  ToString() method to print the elements of the set by using our iterator from Iterator class to iterate through the set 
     *  and a result String to concetanate the result.
     *  result String will be returned after all the merging operations to the result String.
     */
    @Override
    public String toString() {
        String string;
        string = "JavaSet = { " ;
        Iterator it = getIterator() ;
        while (it.hasNext()) { // printing all the datas of the vector or set classes using iterator.Stopping when there is no next element.
            string += it.next();
            if(it.hasNext()) // if it is not the last element it will not print the comma ','
                string += " , ";
        }
        string += " }\n" ;
        return string;
    }

    /**
     *  boolean overridden method 'equals' to return true or false if the two sets are equal or not.
     *  It first checks if obj and our class are the same then it checks sizes are the same then every element of our array(set) and others is equal
     *  then if all of them are equal it returns true.Unless vector elements does not have to be ordered.
     */ 
    @Override
    public boolean equals(Object obj) {
        if(this==obj)
            return true;
        JavaSet other = (JavaSet) obj;
        if(Size()!=other.Size())
            return false;
        for(var elm : other.set){
            if(elm!=null && !equals2(elm)){
                return false;
            }
        }
        for(var elm : set){
            if(elm!=null && !other.equals2(elm)){
                return false;
            }
        }
        return true;
    }
        
    
    /**
     *  boolean method 'equals2(equals to)' to return true or false if the element is in the set or not.
     *  It uses enhanced for loop to iterate through the set
     *  using keyword 'var' to determine its type and compares the current index value with the element to be found in set.
    */ 
    public boolean equals2(Object obj) {
        if (obj != null) {
            for (var value : set) {
                if ( ( value!=null && obj.equals(value)) ) {
                    return true;
                }
            }
        }
        return false;
    }
}