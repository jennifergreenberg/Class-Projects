/*	Jessica To
	masc0428	*/
	
package data_structures;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;

public class BinarySearchTree<K,V> implements DictionaryADT<K,V>{
	
	private Node<K,V> root, parent;
	private int currentSize, modificationCounter;
	private K k;
	private boolean wasLeft;
	
	public BinarySearchTree(){
		root = parent = null;
		currentSize = 0;
		modificationCounter = 0;
	}
	
	// Returns true if the dictionary has an object identified by
	// key in it, otherwise false.
	public boolean contains(K key){
		if(findValue(key,root) == null)
			return false;
		return true;
	}
	
	// Adds the given key/value pair to the dictionary.  Returns
	// false if the dictionary is full, or if the key is a duplicate.
	// Returns true if addition succeeded.
	public boolean insert(K key, V value){
		if(isFull()) return false;
		if(contains(key)) return false;
		if(root==null)
			root = new Node<K,V>(key,value);
		else
			insert(key,value,root,null,false);
		currentSize++;
		modificationCounter++;
		return true;
	}
	
	// Deletes the key/value pair identified by the key parameter.
	// Returns true if the key/value pair was found and removed,
	// otherwise false.
	public boolean remove(K key){
		if(isEmpty()) return false;
		if(!contains(key)) return false;
		if(currentSize==1 && ((Comparable<K>)key).compareTo(root.key)==0)
			root=null;
		else{
			Node<K,V> node = find(key,root);
			remove(node,parent);}
		currentSize--;
		modificationCounter++;
		return true;
	}
	
	// Returns the value associated with the parameter key.  Returns
	// null if the key is not found or the dictionary is empty.
	public V getValue(K key){
		return findValue(key, root);
	}
	
	// Returns the key associated with the parameter value.  Returns
	// null if the value is not found in the dictionary.  If more
	// than one key exists that matches the given value, returns the
	// first one found.
	public K getKey(V value){
		k=null;
		findKey(root, value);
		return k;
	}
	
	// Returns the number of key/value pairs currently stored
	// in the dictionary
	public int size(){
		return currentSize;
	}
	
	// Returns true if the dictionary is at max capacity
	public boolean isFull(){
		return false;
	}
	
	// Returns true if the dictionary is empty
	public boolean isEmpty(){
		if(currentSize == 0) return true;
		return false;
	}
	
	// Returns the Dictionary object to an empty state.
	public void clear(){
		root = null;
		currentSize = 0;
		modificationCounter = 0;
	}
	
	// Returns an Iterator of the keys in the dictionary, in ascending
	// sorted order.  The iterator must be fail-fast.
	public Iterator<K> keys(){
		return new KeyIteratorHelper();
	}
	
	// Returns an Iterator of the values in the dictionary.  The
	// order of the values must match the order of the keys.
	// The iterator must be fail-fast.
	public Iterator<V> values(){
		return new ValueIteratorHelper();
	}
	
///////////////////////////////////////////////////////////////////	
	private void insert(K key, V value, Node<K,V> n,Node<K,V> parent,
	boolean wasLeft){
		if(n==null){
			if(wasLeft) parent.leftChild = new Node<K,V>(key,value);
			else parent.rightChild = new Node<K,V>(key,value);
		}
		else if(((Comparable<K>)key).compareTo((K)n.key)<0)
			insert(key,value,n.leftChild,n,true);
		else
			insert(key,value,n.rightChild,n,false);
	}
	
	private void remove(Node<K,V> n, Node<K,V> parent){
		if(n.leftChild==null && n.rightChild==null){
				if(wasLeft) parent.leftChild=null;
				else parent.rightChild=null;}
			else if(n.leftChild==null || n.rightChild==null){
				if(n==root){
					if(n.leftChild==null){
						root=n.rightChild;
						n.rightChild = null;}
					else{ root=n.leftChild;
						n.leftChild = null;}
				}
				else if(n.leftChild==null && wasLeft){
					parent.leftChild = n.rightChild;
					n.rightChild = null;}
				else if(n.leftChild==null && !wasLeft){ 
					parent.rightChild = n.rightChild;
					n.rightChild = null;}
				else if(n.rightChild==null && wasLeft){
					parent.leftChild = n.leftChild;
					n.leftChild = null;}
				else{ parent.rightChild = n.leftChild;
					n.leftChild = null;}
			}
			else{
				Node<K,V> rightNode, leftNode, successor;
				leftNode = n.leftChild;
				rightNode = successor = n.rightChild;
				if(successor.leftChild == null){
					n.key=successor.key;
					n.value=successor.value;
					n.rightChild=successor.rightChild;}
				else{
					while(successor.leftChild != null){
						rightNode=successor;
						successor=successor.leftChild;}
					if(successor.rightChild == null){
						n.key=successor.key;
						n.value=successor.value;
						rightNode.leftChild=null;}
					else{
						n.key=successor.key;
						n.value=successor.value;
						rightNode.leftChild=successor.rightChild;}
			}	
		}
	}
	
	private Node<K,V> find(K key, Node<K,V> n){
		if(n==null) return null;
		if(((Comparable<K>)key).compareTo(n.key)<0){
			wasLeft = true;
			parent = n;
			return find(key,n.leftChild);}
		else if(((Comparable<K>)key).compareTo(n.key)>0){
			wasLeft = false;
			parent = n;
			return find(key,n.rightChild);}
		else 
			return n;
	}
	
	private V findValue(K key, Node<K,V> n){
		if(n==null) return null;
		if(((Comparable<K>)key).compareTo(n.key)<0)
			return findValue(key, n.leftChild);
		else if(((Comparable<K>)key).compareTo(n.key)>0)
			return findValue(key, n.rightChild);
		else
			return (V) n.value;
	}
	
	private void findKey(Node<K,V> n, V value){
		if(n==null) return;
		if(((Comparable<V>)value).compareTo(n.value)==0){
			k=n.key;
			return;}
		findKey(n.leftChild,value);
		findKey(n.rightChild,value);
	}
	
////////////////////////////////////////////////////////////////////
	private class Node<K,V>{
	private K key;
	private V value;
	private Node<K,V> leftChild, rightChild;
	
	public Node(K k, V v){
		key = k;
		value = v;
		leftChild = rightChild = null;
	}
	}
	
	abstract class IteratorHelper<E> implements Iterator<E>{
		protected int index, index2;
		protected int modCount;
		Node<K,V>[] array;
		
		public IteratorHelper(){
			index = index2 = 0;
			modCount = modificationCounter;
			array = new Node[currentSize];
			fill(root);
		}
		
		public boolean hasNext(){
			if(modCount != modificationCounter)
				throw new ConcurrentModificationException();
			return index < currentSize;	
		}
		
		public void remove(){
			throw new UnsupportedOperationException();
		}
		
		private void fill(Node<K,V> n){
			if(n!=null){
				fill(n.leftChild);
				array[index2++]=n;
				fill(n.rightChild);
			}
		}
	}
	
	class KeyIteratorHelper<K> extends IteratorHelper<K>{
		public KeyIteratorHelper(){
			super();
		}
		public K next(){
			if(!hasNext()) throw new NoSuchElementException();
			return (K) array[index++].key;
		}
	}
	
	private class ValueIteratorHelper<V> extends IteratorHelper<V>{
		public ValueIteratorHelper(){
			super();
		}
		public V next(){
			if(!hasNext()) throw new NoSuchElementException();
			return (V) array[index++].value;
		}
	}
}
