import java.util.Map;

public class encryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text;
	private String cipher_text = "";
	
	public encryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		this.map = _map; // our map is the given '_map'
		this.key = _key; // our key is the given '_key'
		this.plain_text = text; // our plain_text is 'text'
	}
	
	public void encrypt() {
		// do not edit this method
		generate_keystream();
		generate_cipher_text();
	}
	
	private void generate_keystream() {
		for(int i=0; i<plain_text.length() ; i++){ // loop to iterate until the length of plain_text
			keystream += key.charAt(i % key.length() ); // adding the character at the 'current index mod length of the key' of key string to the keystream string.
		}
	}
	
	private void generate_cipher_text() {
		for(int i=0;i<keystream.length();i++){ // loop to iterate through until the length of the keystream.
			// adding the current character of keystream in the inner map of current character of the plain_text. 
			cipher_text+= map.get(plain_text.charAt(i)).get(keystream.charAt(i)); 
		}
	}

	public String get_keystream() {
		return keystream; // getting the generated keystream String.
	}
	
	public String get_cipher_text() {
		return cipher_text; // getting the generated cipher_text String.
	}
}
