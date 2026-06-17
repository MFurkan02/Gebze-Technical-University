package furcontainer;

/**
 * Iterator class that has 'hasNext' and 'next' methods.
 */

public class Iterator<T> {
    private int currentIndex;
    private T[] elements;

    /**
     * Constructor for iterator that takes an array and makes that setting its private array data to that array and it sets the index to 0.
     * @param obj
     */
    Iterator(T[] obj) {
        elements = obj;
        currentIndex = 0;
    }

    /**
     * boolean hasNext() method to return true or false
     * if there is another element in the container
     * according to the size value.
     */
    public boolean hasNext() {
        return currentIndex < elements.length && elements[currentIndex] != null;
    }

    /**
     * next() method to return the next element in the container and incrementing the index so the iterator.
     */
    public Object next() {
        return elements[currentIndex++];
    }
}