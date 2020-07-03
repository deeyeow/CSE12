/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        3/16/2020
                                                        cs12wi20fi
                                Final Assessment

File Name:      Heap.java
Description:    This program will implement an array-based polymorphic Heap 
		data structure.
******************************************************************************/

/**
 * Represents a Heap data structure that is implemented using an array. Items
 * are removed from the Heap in alphabetical order. Class contains all methods
 * the heap may require, including insertion of new items, removal of existing
 * items, and restructuring methods to maintain heap-order.
 */
public class Heap extends Base {

        // data fields
        private int occupancy; 
	private int capacity;
        
	//declare heap array
	private Base[] heapArray;

        // debug flag
        private static boolean debug;

        // debug messages
        private static final String ALLOCATE = "Allocating Heap";
        private static final String AND = " and ";
        private static final String INSERT = "Inserting ";
	private static final String FULL = "Heap is full, could not insert!!";
	private static final String SWAP = "Swapping ";

        /**
         * Constructor for a heap. Initializes all declared variables to the
	 * appropriate values.
	 *
	 * @param  The name of the heap
	 *
         * @return Nothing
         */
        public Heap (int size) {

                // initialize data fields
		this.capacity = size;
		this.occupancy = 0;

		// create new heap array of size
		this.heapArray = new Base[size];

		// print debug message
		if (debug)
		{
			System.err.println(ALLOCATE);
		}
        }


        /**
         * Sets the debug flag to false
         * @return Nothing
         */
        public static void debugOff () {

		debug = false;
        }


        /**
         * Sets the debug flag to true
         * @return Nothing
         */
        public static void debugOn () {

		debug = true;
        }


        /**
         * Inserts a Base into the heap at the appropriate location.
	 *
	 * @param  An element to be inserted into the heap in the form of a 
	 *         Base
	 *
         * @return Boolean indicating if the insert was successful
         */
        public boolean insert (Base element) {

		// check if heap is already full
		if (occupancy >= capacity)
		{
			// print debug message
			if (debug)
			{
				System.err.println(FULL);
			}

			return false;
		}

		// insert at final leaf in heap, or last position in array
		heapArray[occupancy] = element;

		// increment occupancy
		occupancy++;

		// reheap-up to preserve heap-order
		reheapUp();

		// print debug message
		if (debug)
		{
			System.err.println(INSERT +
			element.getName());
		}

		return true;
        }


        /**
         * Removes the most important object (stored at the root). The root
	 * element is replaced by the element at the final leaf, and then
	 * reheap-down is performed to find its correct location. Then returns 
	 * a pointer to the removed element.
	 *
         * @return A pointer to the element removed from the heap.
         */
        public Base remove () {

		// remove will never be called when heap is empty (from Driver)
		// if heap only has root, do not need to call reheap-down
		if (occupancy == 1)
		{
			// decrement occupancy
			occupancy--;

			return heapArray[0];
		}
		
		// else, heap is populated with >= 2 elements, need to call
		// reheap-down

		// store root element
		Base tempRemove = heapArray[0];

		// replace root with final leaf
		heapArray[0] = heapArray[occupancy - 1];

		// remove final leaf
		heapArray[occupancy - 1] = null;

		// decrement occupancy
		occupancy--;

		// reheap-down to preserve heap-order
		reheapDown();

		// return removed root
                return tempRemove;
        }


        /**
         * Starting at the "final leaf", this method should move the inserted
	 * element up the tree until it satisfies the "heap-order", swapping
	 * places with its parent until it is less important than its parent and
	 * more important than its children.
	 *
         * @return Nothing.
         */
	 private void reheapUp()
	 {
	 	// temp local elements
		Base curr, parent;

		// get location of inserted element
		int currIndex = occupancy - 1;

		// keep iterating if "final leaf" is not root node yet
		while (currIndex > 0)
		{
			// get curr and parent elements
			curr = get(currIndex);
			parent = getParent(currIndex);

			// check heap-order
			if (curr.isGreaterThan(parent))
			{
				// swap if not in correct heap-order
				swap(getParentIndex(currIndex), currIndex);
			}

			// break if heap-order is correct
			else
			{
				break;
			}

			// traverse up the heap
			currIndex = getParentIndex(currIndex);
		}
	 }


        /**
         * Starting at the root, this method should move the former "final leaf"
	 * element down the tree until it satisfies the "heap-order", swapping
	 * places with its most important child until it is less important than
	 * its parent and more important than its children.
	 *
         * @return Nothing.
         */
	private void reheapDown()
	{
		// get location of root
		int currIndex = 0;

		// keep iterating if root node still has children
		// if has right child, must have left child, so can check only
		// left
		while (hasLeft(currIndex))
		{
			// temp local variable
			int smallerChildIndex = getLeftChildIndex(currIndex);

			// get left and right children
			Base leftChild = getLeftChild(currIndex);
			Base rightChild = getRightChild(currIndex);

			// compare to get index of the smaller child
			if (hasRight(currIndex)
				&& rightChild.isGreaterThan(leftChild))
			{
				smallerChildIndex = 
					getRightChildIndex(currIndex);
			}

			// get smaller child and current elements
			Base smallerChild = get(smallerChildIndex);
			Base curr = get(currIndex);

			// check heap-order
			if (smallerChild.isGreaterThan(curr))
			{
				// swap if not in correct heap-order
				swap(smallerChildIndex, currIndex);
			}

			// break if heap-order is correct
			else
			{
				break;
			}

			// traverse down the heap
			currIndex = smallerChildIndex;
		}
	}


        /**
         * Swaps any two specified elements.
	 *
	 * @param The index of the specified elements.
	 *
         * @return Nothing.
         */
	 private void swap(int index1, int index2)
	 {
	 	Base temp1 = heapArray[index1];
		Base temp2 = heapArray[index2];
		heapArray[index1] = temp2;
		heapArray[index2] = temp1;

		// print debug message
		if (debug)
		{
			System.err.println(SWAP + temp1.getName() + AND +
			temp2.getName());
		}
	 }


        /**
         * Checks whether the specified element at the index has a left child.
	 *
	 * @param The index of the specified element.
	 *
         * @return A boolean indicating if the element has a left child.
         */
	 private boolean hasLeft(int index)
	 {
		int leftIndex = getLeftChildIndex(index);

		// if out of array bounds, or doesn't exist
		if (leftIndex >= capacity || get(leftIndex) == null)
		{
			return false;
		}

		return true;
	 }


        /**
         * Checks whether the specified element at the index has a right child.
	 *
	 * @param The index of the specified element.
	 *
         * @return A boolean indicating if the element has a right child.
         */
	 private boolean hasRight(int index)
	 {
	 	int rightIndex = getRightChildIndex(index);

		// if out of array bounds, or doesn't exist
		if (rightIndex >= capacity || get(rightIndex) == null)
		{
			return false;
		}

		return true;
	 }


        /**
         * Gets the reference to a specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return A reference to the specified element.
         */
	 private Base get(int index)
	 {
		return heapArray[index];
	 }


        /**
         * Gets index of the parent of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return The index of the parent of the specified element.
         */
	 private int getParentIndex(int index)
	 {
		// formula to get parent
		return (index - 1) / 2;
	 }


        /**
         * Gets the parent of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return A reference to the parent of the specified element.
         */
	 private Base getParent(int index)
	 {
	 	int ans = getParentIndex(index);

		// if out of array bounds
		if (ans < 0)
		{
			return null;
		}

		return get(getParentIndex(index));
	 }


        /**
         * Gets index of the left child of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return The index of the left child of the specified element.
         */
	 private int getLeftChildIndex(int index)
	 {
		// formula to get left child
		return index * 2 + 1;
	 }


        /**
         * Gets the left child of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return A reference to the left child of the specified element.
	 * Returns null otherwise.
         */
	 private Base getLeftChild(int index)
	 {
	 	int ans = getLeftChildIndex(index);

		// if out of array bounds
		if (ans >= capacity)
		{
			return null;
		}

		return get(ans);
	 }


        /**
         * Gets index of the right child of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return The index of the right child of the specified element.
         */
	 private int getRightChildIndex(int index)
	 {
		// formula to get right child
		return index * 2 + 2;
	 }


        /**
         * Gets the right child of the specified element in the heap.
	 *
	 * @param The index of the specified element.
	 *
         * @return A reference to the right child of the specified element.
	 * Returns null otherwise.
         */
	 private Base getRightChild(int index)
	 {
	 	int ans = getRightChildIndex(index);

		// if out of array bounds
		if (ans >= capacity)
		{
			return null;
		}

		return get(ans);
	 }


        /**
         * Indicates whether the heap is currently empty or not.
	 *
         * @return A boolean indicating if the heap is empty or not.
         */
	public boolean isEmpty()
	{
		if (occupancy <= 0)
		{
			return true;
		}

		return false;
	}


        /**
         * Indicates whether the heap is currently full or not.
	 *
         * @return A boolean indicating if the heap is full or not.
         */
	public boolean isFull()
	{
		if (occupancy >= capacity)
		{
			return true;
		}

		return false;
	}


        /**
         * Creates a string with the contents of the heap. This method first
         * adds the general information of this heap, then adds all elements
	 * to the return string 
         *
         * @return  String representation of this heap
         */
        public String write () {
		
		String str = "The heap has " + occupancy + " items.";

		// iterate for every element in heap
		for (int i = 0; i < occupancy; i++)
		{
			// store curr element
			Base tempElement = heapArray[i];

			// print statements
			str += "\nAt index " + i + ":  name:  ";
			str += tempElement.getName() + " with studentnum:  ";
			str += tempElement.getStudentNum() + ".";
		}

                return str;
        }

}
