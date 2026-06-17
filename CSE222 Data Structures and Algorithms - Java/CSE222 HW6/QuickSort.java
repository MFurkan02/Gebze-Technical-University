public class QuickSort extends SortAlgorithm {

	public QuickSort(int input_array[]) {
		super(input_array);
	}

    /*
    Example Sorting Steps for the Average Case :

    Initial Array: {4, 2, 6, 5, 1, 8, 7, 3}

    Partition Step 1: {2, 1, 3}, 4, 6, 5, 8, 7

    Partition Step 2: {1}, 2, {3}, 4, 6, 5, 8, 7

    Partition Step 3 (Final): {1}, {2}, {3}, 4, 6, 5, 8, 7

    Partition Step 4: {1}, {2}, {3}, {4}, 6, 5, 8, 7

    Partition Step 5: {1}, {2}, {3}, {4}, 5, 6, 8, 7

    Partition Step 6: {1}, {2}, {3}, {4}, 5, 6, 7, 8

    */
	
    private int partition(int main_arr[],int low , int high){
        // fill this method
        int pivot = main_arr[high]; // setting our pivot value to the element in the high index of our array.
        int right = low-1; // setting our right value to low-1.

        for(int left=low;left<high;left++){ // loop to iterate from low to high in order to swap the left value smaller than the pivot with the rigth one.
            comparison_counter++; // incrementing the comparison counter before the comparison.
            if(arr[left] < pivot){ // comparing if left element is smaller than the pivot.
                swap(++right, left); // then if it is we swap the incremented right value with the left value which is the current index.
            }
        }
        swap(++right, high); // after iterating from low to high is done we finally swap the incremented right value with the element in the index high.

        
        return right; // and finally returning the right value which will be the new pivot for the upcoming arrays.
    } 

    private void sort(int main_arr[],int low , int high){
        if(low < high){ // if the low value is still lower than the high value
            int pivot = partition(main_arr,low,high); // we find our pivot value and also sorting our array while finding that. 
            sort(main_arr,low,pivot-1); // and sorting our array using low to pivot array  
            sort(main_arr,pivot + 1,high); // and pivot + 1 to high array.
        }
    }

    @Override
    public void sort() {
        int low = 0; // our low value to start from the beginning of the array.
        int high = arr.length-1; // our high value which is the last index value.
    	sort(arr,low,high); // sorting our array using these values.
    }

    @Override
    public void print() {
    	System.out.print("Quick Sort\t=>\t");
    	super.print();
    }
}
