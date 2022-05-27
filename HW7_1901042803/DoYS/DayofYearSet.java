/**
 * This class stores an array of DayofYear objects with no duplicates, contains an
 * inner class called DayofYear. Requires the DoYSExceptions class 
 * to be imported for some specific exceptions
 */
package DoYS;

import java.util.ArrayList;
import DoYS.DoYSExceptions.WrongDate;

public class DayofYearSet{
    private DayofYear[] _data;
    private static int _daysAlive;

    static public class DayofYear{
        private int _day, _month;

        /*Stores the number of days in each month*/
        static private int[] _daysInMonth = 
                             {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        //Constructors

        /**
         * Default Constructor
         */
        public DayofYear(){
            _day = 1;
            _month = 1;
        }

        /**
         * A constructor that takes two integers as day and month.
         * Throws an exception in case the day and month values are invalid
         */
        public DayofYear(int day, int month) throws WrongDate {
            if (!(checkDate(day, month))) 
                throw new WrongDate(String.format("%d, %d", day, month), 
                                    getClass().getCanonicalName());

            _day = day;
            _month = month;
        }

        /**
         * Modifies the day and month values of the object. 
         * Returns true on success, and false if the given values are invalid.
         */
        public boolean setDate(int day, int month){
            if (!(checkDate(day, month))) return false;
            
            else{
                _day = day;
                _month = month;
                return true;
            }
        }

        /**
         * Return the day
         */
        public int getDay() { return _day; }

        /**
         * Return the month
         */
        public int getMonth() { return _month; }

        /**
         * Check the if the given day and month are valid.
         * Return true if the values are valid.
         */
        public static boolean checkDate(int day, int month)
        {
            if (month > 12 || month < 0 || 
                day < 0 || day > _daysInMonth[month-1])   
                return false;

            else return true;
        }

        /**
         * Create a copy of the object and return a reference to it
         */
        public Object clone(){
            DayofYear temp = new DayofYear();

            try{
                temp = new DayofYear(getDay(), getMonth());
            }

            catch(WrongDate ignored) {}/**Ignore the exception because the
                                        *day and month values are already valid
                                        */

            return (Object) temp;
        }
    
        /**
         * Return a string representation of the object
         */
        @Override
        public String toString(){
            return new String(String.format("%d, %d", _day, _month));
        }
    }


    /**
     * Default Constructor
     */
    public DayofYearSet() { _data = null; }

    /**
     * A constructor that takes an ArrayList of DayofYear as an argument
     */
    public DayofYearSet(ArrayList<DayofYear> source){
        _data = new DayofYear[source.size()];
        
        for(int i = 0; i < source.size(); ++i)
            _data[i] = source.get(i);
    }

    @Override
    public String toString() {
        String temp = new String();

        try{
            temp = temp.concat(String.format("Size of the set: %d\n", size()));
            temp = temp.concat(String.format("Days in this set: (%s)", get(0)));
            
            for (int i = 1; i < size(); ++i)
                temp = temp.concat(String.format(", (%s)", get(i)));
        }
        
        catch(ArrayIndexOutOfBoundsException outofBounds)
        {
            System.out.printf("%s: Out of bounds detected.\n", getClass().getCanonicalName());
        }

        catch(NullPointerException ExcEmpty)
        {
            System.out.printf("%s: Printing an empty set.\n", getClass().getCanonicalName());
        }

        return temp;
    }

    @Override
    public boolean equals(Object target) {
        if (!(target instanceof DayofYearSet))  return false;   /*If the sizes are different, then they're definitely inequal*/
        
        DayofYearSet tempTaregt = (DayofYearSet) target;

        if (tempTaregt.size() != size())  return false;

        for (int i = 0; i < size(); ++i)
            if (find(tempTaregt.get(i)) == -1 || tempTaregt.find(get(i)) == -1)  return false; /*if any member of a set was not in the other set, return false*/

        return true;    /*All members of each set exists in the other set*/
    }

    /**
     * Takes a DayofYear object, and adds it to the set.
     * Returns true if the object was added, and false if an
     * object with same date existed in the set and the object 
     * is not added
     */
    public boolean add(DayofYear target){
        DayofYear[] temp;

        if (find(target) != -1)   return false;  /*If the object existed in the target set, then abort function (NO DUPLICATES ARE ALLOWED)*/

        temp = new DayofYear[size() + 1];    /*Dynamically allocate the new set with one extra slot*/

        for (int i = 0; i < size(); ++i)
            temp[i] = (DayofYear)_data[i].clone();    /*Copy the old set to the temporary one*/

        temp[size()] = target;    /*Add the new object*/

        _data = temp; /*Assign the temporary se to the old one*/

        addToDaysAlive(1);    /*Increase the total number of live set members*/

        return true;
    }

    /**
     * Takes day and month integer values. It creates an object
     * with the given day and month values and adds it to the set.
     * Returns true if the object was added. Returns false if an
     * object with same date existed in the set and the object 
     * is not added, or if the day and month values were invalid
     * and then a WrongDate exception is thrown.
     */
    public boolean add(int day, int month){
        DayofYear[] temp;

        try{
            if (find(day, month) != -1)   
                return false;  /*If the object existed in the target set
                                 then return false*/
        }
        
        catch(WrongDate excDate){
            return false;
        }

        temp = new DayofYear[size() + 1]; /*Dynamically allocate the new set 
                                            with one extra slot*/

        for (int i = 0; i < size(); ++i)
            temp[i] = (DayofYear)_data[i].clone(); /*Make a copy of the old set*/

        try{
            temp[size()] = new DayofYear(day, month);    /*Add the new object*/
        }
        
        catch(WrongDate excDate){
            return false;
        }

        _data = temp; /*Assign the temporary set to the old one*/

        addToDaysAlive(1);    /*Increase the total number of live set members*/

        return true;
    }


    /**
     * Remove the element with the given index from the list
     */
    public void remove(int index){
        DayofYear[] temp;

        if (index > size() - 1 || size() == 0) 
            return;    /*If the target index is off the set 
                        or if the set is empty, abort*/

        temp = new DayofYear[size() - 1];  /*Dynamically allocate the temporary set
                                             with one slot less*/

        /*i is the index of the old set, j is the index of the new one*/
        for (int i = 0, j = 0; (i < size()); ++i, ++j)
        {
            if (index != i) temp[j] = get(i); /*Copy the member to the temporary set
                                                if it wasn't the target*/

            else --j; /*If the target was found, skip it and decrement the 
                        temporary set index to prevent skipping cells*/
        }

        _data = temp; /*Assign the temporary set to the old one*/

        addToDaysAlive(-1);
    }

    /**
     * Return the number of days the set
     */
    public int size() {
        if(_data != null) return _data.length;
        else return 0;
    }

    /**
     * Search the set for the target, return it's index if found
     */
    public int find(DayofYear target)
    {
        for (int i = 0; i < size(); ++i)
            if (_data[i].getDay() == target.getDay() &&
                _data[i].getMonth() == target.getMonth())   return i;

        return -1;
    }

    /**
     * Search the set for the target with the given day and month values,
     * return it's index if found. Throws WrongDate exception 
     * if the day and month are invalid.
     */
    public int find(int day, int month) throws WrongDate
    {
        var target = new DayofYear();

        try{
            target = new DayofYear(day, month);
        }
        
        catch(WrongDate excDate){
            throw excDate;
        }

        for (int i = 0; i < size(); ++i)
            if (_data[i] == target)   return i;

        return -1;
    }

    /**
     * Return the element in the given index
     */
    public DayofYear get(int index){ return _data[index]; }

    /**
     * Add num to the total number of days in all the sets
     */
    public void addToDaysAlive(int num) { _daysAlive += num; }

    /**
     * Return the number for the live DayofYear objects in all the sets
     */
    static public int getDaysAlive() { return _daysAlive; }


    //Set Operators

    /**
     * Return the union set of set2 and the caller set
     */
    public DayofYearSet union(DayofYearSet set2){
        
        var temp = new DayofYearSet();

        for(int i = 0; i < size(); ++i)
            temp.add(get(i));

        for(int i = 0; i < set2.size(); ++i)
            temp.add(set2.get(i));

        return temp;
    }

    /**
     * Return the difference set of set2 and the caller set
     */
    public DayofYearSet difference(DayofYearSet set2){
        
        var temp = new DayofYearSet();

        for(int i = 0; i < size(); ++i)
            if(set2.find(get(i)) == -1) temp.add(get(i));

        for(int i = 0; i < set2.size(); ++i)
            if(find(set2.get(i)) == -1) temp.add(set2.get(i));

        return temp;
    }

    /**
     * Return the intersection set of set2 and the caller set
     */
    public DayofYearSet intersection(DayofYearSet set2){
        var temp = new DayofYearSet();

        for(int i = 0; i < size(); ++i)
            if(set2.find(get(i)) != -1) temp.add(get(i));

        for(int i = 0; i < set2.size(); ++i)
            if(find(set2.get(i)) != -1) temp.add(set2.get(i));

        return temp;
    }
    
    /**
     * Return the complement set of the caller set
     */
    public DayofYearSet complement(){
        var temp = new DayofYearSet();

        try{
            /*This loop iteratively goes through each day in a year, if the day exists in the base set, don't add it to the result*/
            for (int month = 1; DayofYear.checkDate(1, month); ++month)
            for (int day = 1; DayofYear.checkDate(day, month); ++day)
            if (find(day, month) == -1) temp.add(day, month);
        }

        catch(WrongDate excDate){
            System.out.printf("Complement set function is using invalid dates");
        }

        return temp;
    }
}
