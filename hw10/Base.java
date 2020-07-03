/*

                                                        Darren Yau
                                                        CSE 12, W20
                                                        3/16/2020
                                                        cs12wi20fi
                                Final Assessment

File Name:      Base.java
Description:    This program serves to support the creation of a polymorphic
		generic container. For the purposes of this assignment, this
		will be a UCSDStudent.

******************************************************************************/

/**
 * Represents an abstract class to support the creation of a UCSDStudent.
 */
public abstract class Base {


        /**
         * Getter method for the name of the UCSDStudent.
	 *
         * @return The name of the UCSDStudent
         */
        public String getName () {
		return null;
	}
        

	/**
         * Getter method for the studentnum of the UCSDStudent.
	 *
         * @return The studentnum of the UCSDStudent
         */
	public long getStudentNum() {
		return 0;
	}
        

        /**
         * Checks if an object is greater than the current object.
	 *
	 * @param  A Base that is to be compared to a UCSDStudent.
	 *
         * @return A boolean indicating if the Base is greater than the current
	 *         object
         */
	public boolean isGreaterThan (Base base) {
                return true;
        }
}

