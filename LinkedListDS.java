/*	Jessica To	
	masc0428	
*/

package data_structures;
import java.util.Iterator;
import java.lang.Comparable;
import java.util.NoSuchElementException;
import java.lang.UnsupportedOperationException;

public class LinkedListDS<E> implements ListADT<E>{
	private Node<E> head, tail;
	private int currentSize;
	
	public LinkedListDS(){
		head = tail = null;
		currentSize = 0;
	}
	
	//  Adds the Object obj to the beginning of the list
    	public void addFirst(E obj){
		Node<E> newNode = new Node<E>(obj);
		if(head == null){
			head = tail = newNode;
			currentSize++;}
		else{
			newNode.next = head;
			head = newNode;
			currentSize++;
		}
	}
	
	//  Adds the Object obj to the end of the list
    	public void addLast(E o){
		Node<E> newNode = new Node<E>(o);
		if(head == null){
			head = tail = newNode;
			currentSize++;}
		else{
			tail.next = newNode;
			tail = newNode;
			currentSize++;
		}
	}

	//  Removes the first Object in the list and returns it.
	//  Returns null if the list is empty.
	public E removeFirst(){
		if(head == null)
			return null;
		E tmp = (E) head.data;
		if(head == tail){
			head = tail = null;
			currentSize--;
		}
		else{
		head = head.next;
		currentSize--;
		}
		return tmp;
	}

	//  Removes the last Object in the list and returns it.
	//  Returns null if the list is empty.
    	public E removeLast(){
		if(head == null)
			return null;
		Node <E> previous = null, current = head;
		while(current != tail){
			previous = current;
			current = current.next;
		}
		E tmp = (E) current.data;
		if(head == tail){
			tmp = head.data;
			head = tail = null;
			currentSize--;
			return tmp;}
		previous.next = null;
		tail = previous;
		currentSize--;
		return tmp;
	}

	//  Returns the first Object in the list, but does not remove it.
	//  Returns null if the list is empty.
    	public E peekFirst(){
		if(head == null)
			return null;
		return head.data;
	}

	//  Returns the last Object in the list, but does not remove it.
	//  Returns null if the list is empty.
    	public E peekLast(){
		if(head == null)
			return null;
		return tail.data;
	}
    
	//  Finds and returns the Object obj if it is in the list, otherwise
	//  returns null.  Does not modify the list in any way
    	public E find(E obj){
		Node<E> current = head;
		while(current != null && ((Comparable<E>)obj).compareTo(current.data) != 0)
			current = current.next;
		if(current == null)
			return null;
		return current.data;
	}

	//  Removes the first instance of thespecific Object obj from the list, if it exists.
	//  Returns true if the Object obj was found and removed, otherwise false
    	public boolean remove(E obj){
		Node<E> previous = null, current = head;
		if(this.contains(obj) == false)
			return false;
		while(current != null && ((Comparable<E>)obj).compareTo(current.data) !=0){
			previous = current;
			current = current.next;
			}
		if(head == null)		
			return false;
		if(current == head){		
			head = head.next;
			currentSize--;
			return true;
			}
		if(current == tail){		
			previous.next = null;
			currentSize--;
			return true;
			}
		previous.next = current.next;	
		currentSize--;
		return true;
	}

	//  The list is returned to an empty state.
    	public void makeEmpty(){
		head = tail = null;
		currentSize = 0;
	}

	//  Returns true if the list contains the Object obj, otherwise false
    	public boolean contains(E obj){
		Node<E> current = head;
		while(current != null && ((Comparable<E>)obj).compareTo(current.data) !=0)
			current = current.next;
		if(current == null)
			return false;
		return true;
	}

	//  Returns true if the list is empty, otherwise false
    	public boolean isEmpty(){
		if(head == null)
			return true;
		return false;
	}

	//  Returns true if the list is full, otherwise false
    	public boolean isFull(){
		return false;
	}

	//  Returns the number of Objects currently in the list.
    	public int size(){
		return currentSize;
	}

	//  Returns an Iterator of the values in the list, presented in
	//  the same order as the list.
    	public Iterator<E> iterator(){
		return new IteratorHelper();
	}

/////////////////////////////////////////////////////////

	class IteratorHelper implements Iterator<E>{
		Node<E> itrPtr;
		public IteratorHelper(){
			itrPtr = head;
		}
		public E next(){
			if(!hasNext()) throw new NoSuchElementException();
			E tmp = (E) itrPtr.data;
			itrPtr = itrPtr.next;
			return tmp;
		}
		public boolean hasNext(){
			return itrPtr != null;
		}
		public void remove(){
			throw new UnsupportedOperationException();
		}
	}

/////////////////////////////////////////////////////////

	class Node<T>{
		T data;
		Node<T> next;
		public Node(T obj){
		data = obj;
		next = null;
		}
	}
}
