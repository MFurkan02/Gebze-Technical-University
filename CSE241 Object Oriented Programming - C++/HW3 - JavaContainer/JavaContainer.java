package furcontainer;

/**
 * JavaContainer interface that has the Add,Remove,Size,getIterator methods.
 */

public interface JavaContainer<T> {

    void Add(T obj);

    void Remove(T obj);

    int Size();

    Iterator<T> getIterator();

}