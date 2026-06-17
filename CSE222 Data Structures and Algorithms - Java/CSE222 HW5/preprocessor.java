public class preprocessor {
	private String initial_string;
	private String preprocessed_string;
		
	public preprocessor(String str) {
		this.initial_string = str; // our initial_string is the given str.
	}

	public void preprocess() {
		// do not edit this method
		capitalize();
		clean();
	}
	
	private void capitalize() {
		initial_string = initial_string.toUpperCase(); // capitalizing all the letters in initial_string using the String method toUpperCase().
	}

	private void clean() {
		preprocessed_string=""; // first setting our preprocessed string to empty string.
		for(int i=0; i<initial_string.length() ; i++){ // iterating through the initial_string
			char ch = initial_string.charAt(i); // getting the current char in the initial_string
			if(Character.isLetter(ch)  ){ // checking if the current char is a letter
					if(ch!='Ş' && ch!='Ç' && ch!='Ö' && ch!='Ü' && ch!='Ğ' && ch!='İ' && ch!='É') // if it is a letter then omitting the Turkish characters and 'é'
						preprocessed_string+= ch; // then adding the character meets these requirements to preprocessed string.
			}
		}
	}
	
	public String get_preprocessed_string() {
		return preprocessed_string; // getting the preprocessed string.
	}
}