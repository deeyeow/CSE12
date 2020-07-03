/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        2/18/2020
                                                        cs12wi20fi
                                Assignment Seven

File Name:      Tree.java
Description:    This program will implement a binary tree data structure. This
                structure will be used to allow variable assignment in the 
		calculator.

******************************************************************************/

public class Tree<Whatever extends Base> extends Base {

        /* data fields */
        private TNode root;
        private long occupancy; 
        private String treeName;

        /* debug flag */
        private static boolean debug;

        /* debug messages */
        private static final String ALLOCATE = " - Allocating]\n";
        private static final String AND = " and ";
        private static final String COMPARE = " - Comparing ";
        private static final String INSERT = " - Inserting ";
        private static final String TREE = "[Tree ";


        /**
         * Constructor for a tree. Initializes all declared variables to the
	 * appropriate values.
	 *
	 * @param  The name of the tree
	 *
         * @return Nothing
         */
        public Tree (String name) {

                /* initialize variables */
		this.treeName = name;
		this.occupancy = 0;
		this.root = null;

		if (debug)
		{
			System.err.println(TREE + this.treeName + ALLOCATE);
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
         * Returns the tree's name
         * @return name of the tree
         */
        public String getName() {
                return treeName;
        }

        /**
         * Returns if the tree is empty
         * @return boolean indicating if the tree is empty
         */
	public boolean isEmpty () {
		
		return occupancy == 0;
	}

        /**
         * Inserts a TNode into the tree at the appropriate location. Updates
	 * height and balance of each TNode accordingly.
	 *
	 * @param  An element to be inserted into the tree in the form of a 
	 *         TNode
	 *
         * @return boolean indicating if the insert was successful
         */
        public boolean insert (Whatever element) {

                /* if tree is empty */
		if (root == null)
		{
			/* set root to new TNode with element */
			root = new TNode(element);
			
			if (debug)
			{
				System.err.println(TREE + treeName + INSERT +
				element.getName() + "]");
			}

			return true;
		}

		/* if tree already has elements, need to loop through, so keep
		 * track of current node */
		TNode curr = root;
		
		/* iterate through tree until find an empty spot to insert*/
		while (curr != null)
		{
			if (debug)
			{
				System.err.println(TREE + treeName + COMPARE +
				curr.data.getName() + AND + element.getName() + 
				"]");
			}

			/* if tree contains a duplicate */
			if (curr.data.equals(element))
			{
				/* if duplicate was deleted, can copy element
				 * into TNode at location */
				if (curr.hasBeenDeleted)
				{
					/* storing parent node */
					TNode prev = curr;

					/* copying data into new TNode */
					curr = new TNode(element);
					curr.parent = prev.parent;
					curr.left = prev.left;
					curr.right = prev.right;
					curr.balance = prev.balance;
					curr.height = prev.height;

					/* setting delete flag to false */
					curr.hasBeenDeleted = false;
				}

				/* if duplicate not deleted, don't do anything
				 * to tree */
				 break;
			}

			/* if element is smaller than current TNode */
			else if (curr.data.isGreaterThan(element))
			{
				/* go left and insert if empty */
				if (curr.left == null)
				{
					if (debug)
					{
						System.err.println(TREE +
						treeName + INSERT +
						element.getName() + "]");
					}

					curr.left = new TNode(element);
					curr.left.parent = curr;
					//curr.left.height = curr.height + 1;

					break;
				}

				/* current TNode's left child is occupied,
				 * update curr and keep looping down tree */
				else
				{
					curr = curr.left;
					continue;
				}
			}

			/* if element is larger than current TNode */
			else
			{
				/* go right and insert if empty*/
				if (curr.right == null)
				{
					if (debug)
					{
						System.err.println(TREE +
						treeName + INSERT +
						element.getName() + "]");
					}

					curr.right = new TNode(element);
					curr.right.parent = curr;
					//curr.right.height = curr.height + 1;

					break;
				}

				/* current TNode's right child is occupied,
				 * update curr and keep looping down tree */
				 else
				 {
					curr = curr.right;
					continue;
				 }
			}
		}

		/* update balance and height by looping back up tree */
		while (curr != null)
		{
			/* initialize local variables to keep track of and
			 * compare heights */
			long leftHeight = 0, rightHeight = 0;

			/* if current TNode has left child, update leftHeight */
			if (curr.left != null)
			{
				leftHeight = curr.left.height + 1;
			}

			/* if current TNode has right child, update rightHeight
			 * */
			if (curr.right != null)
			{
				rightHeight = curr.right.height + 1;
			}

			/* find max height and update current TNode's height */
			curr.height = Math.max(leftHeight, rightHeight);

			/* also update current TNode's balance */
			curr.balance = leftHeight - rightHeight;

			/* loop up the tree */
			curr = curr.parent;
		}

                return true;
        }

        /**
         * Looks up a specified element, and returns a pointer to that element
	 * if found in the tree.
	 *
	 * @param  An element to be looked up in the tree
	 *
         * @return a pointer to the element if it is found in the tree.
	 *         Otherwise returns null.
         */
        public Whatever lookup (Whatever element) {

                /* if tree is empty */
		if (root == null)
		{
			return null;
		}

		/* if tree already has elements, need to loop through, so keep
		 * track of current node */
		TNode curr = root;
		
		/* iterate through tree until no more TNodes to search */
		while (curr != null)
		{
			if (debug)
			{
				System.err.println(TREE + treeName + COMPARE +
				curr.data.getName() + AND + element.getName() + 
				"]");
			}

			/* if tree contains a duplicate */
			if (curr.data.equals(element))
			{
				/* if duplicate was deleted, no TNode to be
				 * found in tree */
				if (curr.hasBeenDeleted)
				{
					return null;
				}

				/* if duplicate not deleted, can return TNode */
				else
				{
					return curr.data;
				}
			}

			/* if element is smaller than current TNode */
			else if (curr.data.isGreaterThan(element))
			{
				/* go left and check if empty */
				if (curr.left == null)
				{
					return null;
				}

				/* current TNode's left child is occupied,
				 * update curr and keep looping down tree */
				else
				{
					curr = curr.left;
					continue;
				}
			}

			/* if element is larger than current TNode */
			else
			{
				/* go right and check if empty*/
				if (curr.right == null)
				{
					return null;
				}

				/* current TNode's right child is occupied,
				 * update curr and keep looping down tree */
				 else
				 {
					curr = curr.right;
					continue;
				 }
			}
		}
                return null;    // not there
        }

        /**
         * Removes a specified TNode in the tree by setting its hasBeenDeleted 
	 * flag to true. Then returns a pointer to the removed element.
	 *
	 * @param  An element to be removed in the tree
	 *
         * @return a pointer to the element if it is removed from the tree.
	 *         Otherwise returns null.
         */
        public Whatever remove (Whatever element) {

                /* if tree is empty */
		if (root == null)
		{
			return null;
		}

		/* if tree already has elements, need to loop through, so keep
		 * track of current node */
		TNode curr = root;
		
		/* iterate through tree until no more TNodes to search */
		while (curr != null)
		{
			if (debug)
			{
				System.err.println(TREE + treeName + COMPARE +
				curr.data.getName() + AND + element.getName() + 
				"]");
			}

			/* if tree contains a duplicate */
			if (curr.data.equals(element))
			{
				/* if duplicate was deleted, no TNode to be
				 * found in tree */
				if (curr.hasBeenDeleted)
				{
					return null;
				}

				/* if duplicate not deleted, can return TNode
				 * and "delete" */
				else
				{
					/* set delete flag to true */
					curr.hasBeenDeleted = true;

					/* decrement occupancy */
					occupancy--;

					return curr.data;
				}
			}

			/* if element is smaller than current TNode */
			else if (curr.data.isGreaterThan(element))
			{
				/* go left and check if empty */
				if (curr.left == null)
				{
					return null;
				}

				/* current TNode's left child is occupied,
				 * update curr and keep looping down tree */
				else
				{
					curr = curr.left;
					continue;
				}
			}

			/* if element is larger than current TNode */
			else
			{
				/* go right and check if empty*/
				if (curr.right == null)
				{
					return null;
				}

				/* current TNode's right child is occupied,
				 * update curr and keep looping down tree */
				 else
				 {
					curr = curr.right;
					continue;
				 }
			}
		}
                return null;    // not there
        }

        /**
         * Creates a string representation of this tree. This method first
         * adds the general information of this tree, then calls the
         * recursive TNode function to add all nodes to the return string 
         *
         * @return  String representation of this tree 
         */
        public String toString () {
                String string = "Tree " + treeName + ":\noccupancy is ";
                string += occupancy + " elements.";

                if(root != null)
                        string += root.writeAllTNodes();

                return string;
        }

        private class TNode {

                public Whatever data;
                public TNode left, right, parent;
                public boolean hasBeenDeleted;

                /* left child's height - right child's height */
                public long balance;
                /* 1 + height of tallest child, or 0 for leaf */
                public long height;

                public TNode (Whatever element) {

                        /* Initialize all variables to null or 0 */
			this.data = element;
			this.left = null;
			this.right = null;
			this.parent = null;
			this.hasBeenDeleted = false;

			this.balance = 0;
			this.height = 0;

			/* increment occupancy when creating new node */
			occupancy++;
                }

                /**
                 * Creates a string representation of this node. Information
                 * to be printed includes this node's height, its balance,
                 * and the data its storing.
                 *
                 * @return  String representation of this node 
                 */

                public String toString () {
                        return "at height:  " + height + "  with balance: " +
                                balance + "  " + data;
                }

                /**
                 * Writes all TNodes to the String representation field. 
                 * This recursive method performs an in-order
                 * traversal of the entire tree to print all nodes in
                 * sorted order, as determined by the keys stored in each
                 * node. To print itself, the current node will append to
                 * tree's String field.
                 */
                public String writeAllTNodes () {
                        String string = "";
                        if (left != null)
                                string += left.writeAllTNodes ();
                        if (!hasBeenDeleted) 
                                string += "\n" + this;          
                        if (right != null)
                                string += right.writeAllTNodes ();

                        return string;
                }
        }
}
