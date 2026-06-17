public class MergeSort extends SortAlgorithm {
	
	public MergeSort(int input_array[]) {
		super(input_array);
	}
	
    /*
    Example Sorting Steps for the Average Case :

    Initial Array: {4, 2, 6, 5, 1, 8, 7, 3}

    Split 1: {4, 2, 6, 5} - {1, 8, 7, 3}

    Split 2: {4, 2} - {6, 5} - {1, 8} - {7, 3}

    Split 3: {4} - {2} - {6} - {5} - {1} - {8} - {7} - {3}

    Merge Step 1: {2, 4} - {5, 6} - {1, 8} - {3, 7}

    Merge Step 2: {2, 4, 5, 6} - {1, 3, 7, 8}

    Merge Step 3 (Final Merge): {1, 2, 3, 4, 5, 6, 7, 8}
    
    */

    private void merge(int left_arr[] , int right_arr[] , int main_arr[]){
        int left = left_arr.length;
        int right =  right_arr.length  ;

        int l=0; // indice for left array to iterate until the left length.
        int r=0; // indice for right array to iterate until the right length.
        int i=0; // indice for our main_array to iterate and fill it with either the left or the right.

        while(l < left && r < right){ // while loop to iterate until both l and r variables not smaller than length of their respective arrays.
            comparison_counter++;
            if(left_arr[l] < right_arr[r]){ // comparing both left and right arrays and accordingly adding the smaller element to our main_array.
                main_arr[i++] = left_arr[l++]; // then incrementing the variables after adding it to main_array.
            }
            else { // if element in the rigth array is smaller.
                main_arr[i++] = right_arr[r++]; 
            }
        }

        // after the while loop this loop is for the remaining element in one of the arrays to add to our main_array. 
        
        while( l < left )
            main_arr[i++] = left_arr[l++]; 
        
        while( r < right )
            main_arr[i++] = right_arr[r++];

    }

    private void sort(int main_arr[]){
        
        if(main_arr.length <= 1) // returning after there is no array that has a length > 1 left.
            return;

        int mid = ( main_arr.length ) / 2; // finding our middle value.
        int left_arr[] = new int[mid]; // creating the left array for array starting from beginning to mid.
        int right_arr[] = new int[main_arr.length - mid]; // creating the right array for array starting from mid to end.

        // adding the elements to both arrays.
        for(int i=0; i < mid ; i++){
            left_arr[i] = main_arr[i];
        }
        int j=0;
        for(int i=mid; i < main_arr.length ; i++){
            right_arr[j] = main_arr[i];
            j++;
        }
        /* OR WE CAN CREATE IT BY USING ONLY ONE FOR LOOP
        int j=0;
        for(int i=0;i<main_arr.length ; i++){
            if(i < mid)
                left_arr[i] = main_arr[i]
            else{
                right_arr[j] = main_arr[i];
                j++;
            }
        }
        */
        
        sort(left_arr); // sorting the left
        sort(right_arr); // and right arrays ->
        merge(left_arr,right_arr,main_arr);     // by merging them into smaller arrays.
    }
    
    @Override
    public void sort() {
    	sort(arr);
    }
    
    @Override
    public void print() {
    	System.out.print("Merge Sort\t=>\t");
    	super.print();
    }
}
