import java.util.Map;
import java.util.Iterator;

public class decryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text = "";
	private String cipher_text;
	
	public decryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		this.map = _map; // our map is the given '_map'
		this.key = _key; // our key is the given '_key'
		this.cipher_text = text; // our cipher_text is 'text'
	}

	public void decrypt() {
		// do not edit this method
		generate_keystream();
		generate_plain_text();
	}
	
	private void generate_keystream() {
		for(int i=0; i<cipher_text.length() ; i++){ // loop to iterate until the length of cipher_text
			keystream += key.charAt(i % key.length() ); // adding the character at the 'current index mod length of the key' of key string to the keystream string.
		}
	}
	
	private void generate_plain_text() {
		// You must use map.get(x).keySet() with an iterator in this method
		
		for(int i=0;i<cipher_text.length();i++){ // loop for iterating through until the length of the cipher_text. 
			Iterator<Character> iter = map.get(keystream.charAt(i)).keySet().iterator(); 
			// getting the iterator of the keyset of the current index character of keystream
			while(iter.hasNext()){ // iterating through that keySet until it does not have a next.
				char ch = iter.next(); // getting the current character in the keySet.
				if(map.get(keystream.charAt(i)).get(ch).equals(cipher_text.charAt(i))){ // map.get(keystream.charAt(i)).get(iter.next()).equals(cipher_text.charAt(i))
					// checking if in the map of the current character of keystream 
					// checking if the value of the current character of the iterator equals to the current index character of cipher_text
					// then if it equals we add the current character in the keySet() which is current character of the iterator to plain_text.
					plain_text+=ch;
				}
			}
		}
	}

	public String get_keystream() {
		return keystream; // getting the generated keystream String.
	}
	
	public String get_plain_text() {
		return plain_text; // getting the generated plain_text String.
	}
}
