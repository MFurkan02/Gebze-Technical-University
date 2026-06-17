package furcontainer;

public class JavaVector<T> implements JavaContainer<T> {
    private int size;
    private int capacity;
    private T[] vect;

    /**
     * Constructor that takes the capacity to create a vector(array) with that capacity and setting the size to 0. 
     * @param vcapacity
     */
    public JavaVector(int vcapacity) {
        size = 0; // at first there is no element in vector.
        capacity = vcapacity;
        vect = (T[]) new Object[capacity];
    }

    /**
     * Add() method to add a new element to our vector.
     * It first checks if the element to be added is in the vector or not using equals() method.
     * If the element is already in the vector an exception will be thrown.
     * If the element is not in the vector first capacity will be checked using checkCapacity() method then
     * the element will be added to the end and the size will be incremented.  
     */
    public void Add(T obj) {
        checkCapacity(); 
        vect[size++] = obj;
        System.out.print(obj);
        System.out.println(" is added to the container!");
    }

    /**
     * Remove() method to remove a element from our vector.
     * It first checks if the element to be removed is in the vector or not using equals() method.
     * If the element to be removed is not in the vector an exception will be thrown.
     * If the element is in the vector then vector will be shifted left after the element to be removed to remove the element. 
     * after that the size will be decremented.  
     */
    public void Remove(T obj) {
        if(!equals2(obj)){
            throw new IllegalArgumentException("Element to be deleted does not exist in this vector!\n");
        }
        var found=0;
        if(equals2(obj)){ 
            for(int i=0;i<size;++i){
                if(vect[i].equals(obj) || found==1 ){
                    found=1;
                    vect[i] = vect[i+1];
                }
            }
            System.out.print(obj);
            System.out.println(" is removed from the container!");
        }
        size-=1;   
    }

    /**
     * Size() method to return the size of the vector.
     */ 
    public int Size() {
        return size;
    }

    /**
     * method that will return an iterator using Iterator class.
     */
    public Iterator<T> getIterator() {
        return new Iterator<>(vect);
    }

    /**
     * checkCapacity() method to check the capacity of the vector and 
     * if the capacity <= size + 1 , new capacity will be capacity *= 2
     * and a new temp vector will be created to copy our vector
     * then our new capacity vector will be the same as temp vector.
     */
    private void checkCapacity() {
        if (capacity <= size + 1){ 
            capacity = 2 * capacity;
            T[] temp_vect = (T[]) new Object[capacity] ; 
            for(int i=0 ; i<size ; ++i){
                temp_vect[i] = vect[i]; 
            } 
            vect = temp_vect ; 
        }
    }

    /**
     *  ToString() method to print the elements of the vector by using our iterator from Iterator class to iterate through the vector 
     *  and a result String to concetanate the result.
     *  result String will be returned after all the merging operations to the result String.
     */
    @Override
    public String toString() { 
        String string;
        string = "JavaVector = { " ;
        Iterator it = getIterator() ;
        while (it.hasNext()) { 
            string += it.next();
            if(it.hasNext()) 
                string += " , ";
        }
        string += " }\n" ;
        return string;
    }

    /**
     *  boolean overridden method 'equals' to return true or false if the two vectors are equal or not.
     *  It first checks if obj and our class are the same then it checks sizes are the same then every element of our array(vector) and others is equal
     *  then if all of them are equal and ordered it returns true.
     */ 
    @Override
    public boolean equals(Object obj) {
    if (this == obj)
        return true;
    if (obj == null)
        return false;
    JavaVector other = (JavaVector) obj;
    if (size != other.size)
        return false;
    for (int i = 0; i < size; i++) {
        if (vect[i] != other.vect[i])
            return false;
    }
        return true;
    }
        
    
    /**
     *  boolean method 'equals2(equals to)' to return true or false if the element is in the vector or not.
     *  It uses enhanced for loop to iterate through the vector
     *  using keyword 'var' to determine its type and compares the current index value with the element to be found in vector.
    */  
    public boolean equals2(Object obj) {
        if (obj != null) {
            for(var elm : vect){
                if (elm!=null && elm.equals(obj) ) {
                    return true;
                }
            }
        }
        return false;
    }
}
