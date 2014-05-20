/*	Jessica To
	masc0428	*/
	
package data_structures;
import java.util.TreeMap;
import java.util.Map.Entry;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;

public class BalancedTree<K,V> implements DictionaryADT<K,V>{
	TreeMap<K,V> treemap;

	public BalancedTree(){
		treemap = new TreeMap<K,V>();
	}
	
	// Returns true if the dictionary has an object identified by
	// key in it, otherwise false.
	public boolean contains(K key){
		return treemap.containsKey(key);
	}
	
	// Adds the given key/value pair to the dictionary.  Returns
	// false if the dictionary is full, or if the key is a duplicate.
	// Returns true if addition succeeded.
	public boolean insert(K key, V value){
		return treemap.put(key,value)==null;
	}
	
	// Deletes the key/value pair identified by the key parameter.
	// Returns true if the key/value pair was found and removed,
	// otherwise false.
	public boolean remove(K key){
		if(treemap.remove(key)==null) return false;
		return true;
	}
	
	// Returns the value associated with the parameter key.  Returns
	// null if the key is not found or the dictionary is empty.
	public V getValue(K key){
		return treemap.get(key);
	}
	
	// Returns the key associated with the parameter value.  Returns
	// null if the value is not found in the dictionary.  If more
	// than one key exists that matches the given value, returns the
	// first one found.
	public K getKey(V value){
		for(Entry<K,V> map : treemap.entrySet())
			if(((Comparable<V>)value).compareTo(map.getValue())==0)
				return map.getKey();
		return null;
	}
	
	// Returns the number of key/value pairs currently stored
	// in the dictionary
	public int size(){
		return treemap.size();
	} 
	
	// Returns true if the dictionary is at max capacity
	public boolean isFull(){
		return false;
	}
	
	// Returns true if the dictionary is empty
	public boolean isEmpty(){
		return treemap.isEmpty();
	}
	
	// Returns the Dictionary object to an empty state.
	public void clear(){
		treemap.clear();
	}
	
	// Returns an Iterator of the keys in the dictionary, in ascending
	// sorted order.  The iterator must be fail-fast.
	public Iterator<K> keys(){
		return treemap.keySet().iterator();
	}
	
	// Returns an Iterator of the values in the dictionary.  The
	// order of the values must match the order of the keys.
	// The iterator must be fail-fast.
	public Iterator<V> values(){
		return treemap.values().iterator();
	}
	
}
