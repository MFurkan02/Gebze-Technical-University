import java.util.HashMap;
import java.util.Map;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.Iterator;

public class alphabet {
	private Set<Character> english_alphabet = new LinkedHashSet<Character>();
	private Map<Character, Map<Character, Character>> map = new HashMap<Character,  Map<Character, Character>>();
	
	public alphabet() {
		// do not edit this method
		fill_english_alphabet();
		fill_map();
	}
	
	private void fill_english_alphabet() {
		// do not edit this method
		for(char c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ".toCharArray()) {
		    english_alphabet.add(c);
		}
	}
	
	private void fill_map() {
		// You must use the "english_alphabet" variable in this method, to fill the "map" variable.
		// You can define 1 or 2 iterators to iterate through the set items.
		
		int curr=0; // variable to iterate the iterator to the current character.
		char ch =' ';
		Iterator<Character> iter1 ; // defining the 1st 
		Iterator<Character> iter2 ; // and 2nd iterators.
		for(char c : english_alphabet){ // loop to iterate to put the letter to the outer map -> map.put('A',inner) ... map.put('Z',inner);
			iter1 = english_alphabet.iterator(); // resetting the both iterators
			iter2 = english_alphabet.iterator(); // to the start of the set for every character in the map.

			Map<Character,Character> inner = new HashMap<Character,Character>(); // map which will be the inner map of our map(table) for every letter in english_alphabet. 
			
			for(int i=0;i<curr;i++){ // loop to iterate until our current letter. For example 1 for B, 5 for F etc.
									// for starting from that letter in the inner map. A='B' or A='F' etc.
				iter1.next();
			}

			for(char c2 : english_alphabet){ // loop for the keys of the inner map -> inner.put('A',B) ... inner.put('D',E) ... inner.put('Z',A).
				if(iter1.hasNext()){ // if the 1st iterator has next letter character to be put will be that letter.
					ch=iter1.next();
				}
				else if(!iter1.hasNext()){ //  if the 1st iterator do not 'have next letter then character to be put will be the letter of next of iter2.
					ch=iter2.next(); 	   // for example for outer map 'H'=> 'S='Z' then from 'H'=> 'T'='A' iter2.next() will be used to fill the map.
				}
				inner.put(c2, ch); // putting the current alphabet letter key 'c2' to value ch(iter.next()). 
			}

			map.put(c, inner); // after the inner map is filled completely putting that inner map to the value of key 'c' which is the
							   // current letter of the outer map key loop.
			curr++; // incrementing our curr for iter1.next iterate until there in the next loop.EX: 1 for 'B' , 24 for 'Y' , 25 for 'Z'.
		}

		// this is my 2nd solution but using ASCII table.
		/* 
		Iterator<Character> iter1 = english_alphabet.iterator();
		int i=0;
		while(iter1.hasNext()){
			Map<Character,Character> inner = new HashMap<Character,Character>();
			for (int j = 0; j < 26; j++){
                if(65 + i + j > 90) {
                  inner.put( (char)(65 + j) , (char) (65 + i + j - 26) );
				}
                else {
                  inner.put( (char)(65 + j) , (char) ( 65 + i + j) );   // if not printing the related character in the ascii table
                }                
            }
			map.put(iter1.next(), inner);
			i++;
		}*/		
	}

	public void print_map() {
		// do not edit this method
		System.out.println("*** Viegenere Cipher ***\n\n");
		System.out.println("    " + english_alphabet);
		System.out.print("    ------------------------------------------------------------------------------");
		for(Character k: map.keySet()) {
			System.out.print("\n" + k + " | ");
			System.out.print(map.get(k).values());
		}
		System.out.println("\n");
		
	}

	public Map get_map() {
		return map;
	}
}