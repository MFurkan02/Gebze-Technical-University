public class BubbleSort extends SortAlgorithm {

	public BubbleSort(int input_array[]) {
		super(input_array);
	}
	
    /*
    Example Sorting Steps for the Average Case :
    ->  {4, 2, 6, 5, 1, 8, 7, 3} -> {2, 4, 5, 1, 6, 7, 3, 8} -> {2, 4, 1, 5, 6, 3, 7, 8}  -> {2, 1, 4, 5, 3, 6, 7, 8}
    ->  {1, 2, 4, 3, 5, 6, 7, 8} -> {1, 2, 3, 4, 5, 6, 7, 8} -> {1, 2, 3, 4, 5, 6, 7, 8} -> {1, 2, 3, 4, 5, 6, 7, 8} 
     */

    @Override
    public void sort() {
    	// fill this method
        int swap_check = 0; // variable to check if in the inner loop there was a swap that occured. 
        for(int i=arr.length-1;i>0;i--){ // loop to iterate through all the elements-1. 
            for(int j=0;j<i;j++){ // 2nd loop to compare every adjacent element as if they were in a 'bubble' together 
                comparison_counter++; // incrementing the comparison counter before the comparison.
                if(arr[j+1] < arr[j] ){ // if the left of the adjacent element is greater 
                    // then we swap it with the right adjacent element.
                    swap(j, j+1);
                    swap_check = 1; // setting our swap_check variable to 1 in order to state there has been a swap and the array is not sorted 
                                    // yet for the next iterations.
                }
            }
            if(swap_check == 0) // if there were no swaps occured in the inner loop it means that the array is already sorted since there 
                return;         // there were no swaps.Hence , the sort is complete and can be returned.
            swap_check = 0; // if it was not returned which means there was a swap and swap_check was not zero then we will reset our
                            // swap_check variable to 0 in order to check the swap condition for the next iteration in the outer loop.
        }
    }
    
    @Override
    public void print() {
    	System.out.print("Bubble Sort\t=>\t");
    	super.print();
    }
}
