/**
 * Contains the possible Exceptions in DayofYearSet class
 */
package DoYS;

public class DoYSExceptions {

    /**
     * This exception is thrown for invalid day and month values
     */
    static public class WrongDate extends Exception {
        /**
         * Takes the Date string, containing day and month values,
         * and the path of the function that threw it.  
         */  
        public WrongDate(String Date, String path) {  
            System.out.printf("%s: Date %s is invalid\n", path, Date); 
        }
    }
}
