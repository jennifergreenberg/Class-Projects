/*	Jessica To
	masc0428	*/
	
package data_structures;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;

public class Hashtable<K,V> implements DictionaryADT<K,V>{
	private int tableSize, maxSize, currentSize;
	private int modificationCounter;
	private LinkedListDS<DictionaryNode<K,V>>[] list;
	
	public Hashtable(int n){
		maxSize = n;
		tableSize = (int) (maxSize * 1.3f);
		currentSize = 0;
		modificationCounter = 0;
		list = new LinkedListDS[tableSize];
		for (int i=0; i<tableSize; i++)
			list[i] = new LinkedListDS<DictionaryNode<K,V>>();
	}
	
	// Returns true if the dictionary has an object identified by
	// key in it, otherwise false.
	public boolean contains(K key){
		return list[getHashCode(key)].contains(new DictionaryNode<K,V>(key,null));
	}

	// Adds the given key/value pair to the dictionary.  Returns
	// false if the dictionary is full, or if the key is a duplicate.
	// Returns true if addition succeeded.
	public boolean insert(K key, V value){
		if(isFull()) return false;
		if(list[getHashCode(key)].contains(new DictionaryNode<K,V>(key,null)))
			return false;
		list[getHashCode(key)].addLast(new DictionaryNode<K,V>(key, value));
		currentSize++;
		modificationCounter++;
		return true;
	}

	// Deletes the key/value pair identified by the key parameter.
	// Returns true if the key/value pair was found and removed,
	// otherwise false.
	public boolean remove(K key){
		if(contains(key)){
			list[getHashCode(key)].remove(new DictionaryNode<K,V>(key,null));
			currentSize--;
			modificationCounter++;
			return true;
		}
		modificationCounter++;
		return false;
	}

	// Returns the value associated with the parameter key.  Returns
	// null if the key is not found or the dictionary is empty.
	public V getValue(K key){
		DictionaryNode<K,V> tmp = list[getHashCode(key)].find(
		new DictionaryNode<K,V>(key,null));
		if(tmp == null) return null;
		return tmp.value;
	}

	// Returns the key associated with the parameter value.  Returns
	// null if the value is not found in the dictionary.  If more
	// than one key exists that matches the given value, returns the
	// first one found.
	public K getKey(V value){
		for(int i = 0; i < tableSize; i++)
			for(DictionaryNode<K,V> n : list[i])
				if(((Comparable<V>) n.value).compareTo(value)==0)
					return n.key;
		return null;
	}

	// Returns the number of key/value pairs currently stored
	// in the dictionary
	public int size(){
		return currentSize;
	}

	// Returns true if the dictionary is at max capacity
	public boolean isFull(){
		if(currentSize == maxSize) return true;
		return false;
	}

	// Returns true if the dictionary is empty
	public boolean isEmpty(){
		if(currentSize == 0) return true;
		return false;
	}

	// Returns the Dictionary object to an empty state.
	public void clear(){
		for(int i=0; i<tableSize; i++)
			list[i].makeEmpty();
		currentSize=0;
		modificationCounter=0;
	}

	// Returns an Iterator of the keys in the dictionary, in ascending
	// sorted order.  The iterator must be fail-fast.
	public Iterator<K> keys(){
		return new KeyIteratorHelper<K>();
	}

	// Returns an Iterator of the values in the dictionary.  The
	// order of the values must match the order of the keys.
	// The iterator must be fail-fast.
	public Iterator<V> values(){
		return new ValueIteratorHelper<V>();
	}
	
////////////////////////////////////////////////////
	
	private int getHashCode(K key){
		return (key.hashCode() & 0x7FFFFFFF ) % tableSize;
	}
	
	private class DictionaryNode<K,V> implements
	Comparable<DictionaryNode<K,V>>{
		K key;
		V value;
		public DictionaryNode(K key, V value){
			this.key = key;
			this.value = value;
		}
		public int compareTo(DictionaryNode<K,V> node){
			return ((Comparable<K>) key).compareTo((K)node.key);
		}
	}
	
	
	abstract class IteratorHelper<E> implements Iterator<E>{
		protected DictionaryNode[] nodes;
		protected int index;
		protected long modCounter;
		
		public IteratorHelper(){
			nodes = new DictionaryNode[currentSize];
			index = 0;
			modCounter = modificationCounter;
			int j = 0;
			for(int i=0; i < tableSize; i++)
				for(DictionaryNode n : list[i])
					nodes[j++] = n;
			nodes = shellSort(nodes);
		}
		
		public boolean hasNext(){
			if(modCounter != modificationCounter)
				throw new ConcurrentModificationException();
			return index < currentSize;
		}
		
		public abstract E next();
		
		public void remove(){
			throw new UnsupportedOperationException();
		}
		
		private DictionaryNode<K,V>[] shellSort(DictionaryNode<K,V>[] array){
		int in, out, h=1;
		DictionaryNode tmp = null;
		int size = array.length;
			while(h <= currentSize/3)
				h = h*3+1;
			while(h > 0){
				for(out=h; out<size; out++){
					tmp = array[out];
					in = out;
					while(in > h-1 && (array[in-h].compareTo(tmp)>0 
								|| array[in-h].compareTo(tmp)==0)){
						array[in] = array[in-h];
						in -= h;
					}
				array[in]=tmp;
				}
			h = (h-1)/3;
			}
		return array;	
		}
	}
	
	class KeyIteratorHelper<K> extends IteratorHelper<K>{
		
		public KeyIteratorHelper(){
			super();
		}
		
		public K next(){
			return (K) nodes[index++].key;
		}
		
	}
	
	class ValueIteratorHelper<V> extends IteratorHelper<V>{
		public ValueIteratorHelper(){
			super();
		}
		
		public V next(){
			return (V) nodes[index++].value;
		}
	}
}
