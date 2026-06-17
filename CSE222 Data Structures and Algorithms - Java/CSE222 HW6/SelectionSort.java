public class SelectionSort extends SortAlgorithm {

	public SelectionSort(int input_array[]) {
		super(input_array);
	}

    /*
    Example Sorting Steps for the Average Case :
    {4, 2, 6, 5, 1, 8, 7, 3} -> { 1 , 2 ,6 ,5 ,4, 8 ,7, 3} -> {1 ,2 ,3 ,5, 4, 8, 7, 6} -> {1 ,2 ,3 ,4 ,5 ,8 ,7, 6} -> {1,2,3,4,5,6,7,8}    
     */


    @Override
    public void sort() {
        // fill this method
        for(int i=0;i<arr.length-1;i++){ // loop to iterate until the element previous than the last element.
            int current_minimum=i; // setting our current minimum to the current index temporarily.
            for(int j=i+1;j<arr.length;j++){ // loop to iterate until the last element from the current index + 1 to find the minimum element 
                comparison_counter++; // incrementing our comparison counter before the comparison.
                if(arr[j] < arr[current_minimum] ){ // comparing the current element with the element in the current_minimum index
                    // then if our current element is smaller than the current minimum value we update our current minimum index value
                    // with the current index value.
                    current_minimum = j;
                }
            }
            // after we found the minimum which is after our current element we swap our current value with the minimum index value.
            swap(i,current_minimum);
        }
    }

    @Override
    public void print() {
    	System.out.print("Selection Sort\t=>\t");
    	super.print();
    }
}
