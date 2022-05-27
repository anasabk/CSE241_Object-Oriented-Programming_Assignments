import java.util.ArrayList;

import DoYS.DayofYearSet;
import DoYS.DayofYearSet.DayofYear;
import DoYS.DoYSExceptions.WrongDate;
import java.io.File;  // Import the File class
import java.io.FileWriter;
import java.io.IOException;  // Import the IOException class to handle errors

public class Driver {
    public static void main(String[] wow){
        try {
            ArrayList<DayofYear> initial1 = new ArrayList<DayofYear>();

            initial1.add(new DayofYear(15, 3));
            initial1.add(new DayofYear(24, 7));
            initial1.add(new DayofYear(3, 11));
            initial1.add(new DayofYear(8, 7));
            initial1.add(new DayofYear(30, 4));

            DayofYearSet testSet1 = new DayofYearSet(initial1),
                     testSet2 = new DayofYearSet(),
                     testSet3 = new DayofYearSet(),
                     testSet4 = new DayofYearSet();


            File resultFile;
            FileWriter resultFileWriter;
            boolean tempResult;

            resultFile = new File("TestResults.txt");
            resultFileWriter = new FileWriter(resultFile);

            if (resultFile.createNewFile()) {
                System.out.println("\nFile created: " + resultFile.getName() + "\n");
            } 

            else {
                System.out.println("\nFile already exists.\n");
            }

            resultFileWriter.write(String.format("\nTestSet 1:\n%s\n\n", testSet1));
            System.out.printf("TestSet 1:\n%s\n\n", testSet1);


            testSet2.add(30, 4);
            testSet2.add(23, 5);
            testSet2.add(2, 12);
            testSet2.add(12, 9);
            testSet2.add(24, 7);

            resultFileWriter.write(String.format("TestSet 2:\n%s\n\n", testSet2));
            System.out.printf("TestSet 2:\n%s\n\n", testSet2);


            testSet3.add(2, 56);
            testSet3.add(8, 7);
            testSet3.add(32, 8);
            testSet3.add(2, 1);
            testSet3.add(17, 12);

            resultFileWriter.write(String.format("TestSet 3:\n%s\n\n", testSet3));
            System.out.printf("TestSet 3:\n%s\n\n", testSet3);


            testSet4.add(22, 10);
            testSet4.add(13, 2);
            testSet4.add(2, 12);
            testSet4.add(15, 9);
            testSet4.add(4, 3);

            resultFileWriter.write(String.format("TestSet 4:\n%s\n\n", testSet4));
            System.out.printf("TestSet 4:\n%s\n\n", testSet4);


            resultFileWriter.write(String.format("testSet1 equals testSet2: %s\n", testSet1.equals(testSet2)));
            System.out.printf("testSet1 equals testSet2: %s\n\n", testSet1.equals(testSet2));


            resultFileWriter.write("\nadd function test:\n\n");
            System.out.printf("\nadd function test:\n\n");

            tempResult = testSet3.add(new DayofYear(12, 3));
            resultFileWriter.write(String.format("Day 12/03 is added to testSet3: %s\n", tempResult));
            System.out.printf("Day 12/03 is added to testSet3: %s\n", tempResult);

            tempResult = testSet1.add(25, 13);
            resultFileWriter.write(String.format("Day 25/13 is added to testSet1: %s\n\n", tempResult));
            System.out.printf("Day 25/13 is added to testSet1: %s\n\n", tempResult);


            resultFileWriter.write("\nremove function test:\n");
            System.out.printf("\nremove function test:\n\n");

            testSet2.remove(3);
            resultFileWriter.write(String.format("testSet2 had it's 3rd member removed:\n%s\n", testSet2));
            System.out.printf("testSet2 had it's 3rd member removed:\n%s\n\n", testSet2);

            testSet3.remove(5);
            resultFileWriter.write(String.format("testSet3 had it's 5th member (does not exist) removed:\n%s\n\n", testSet3));
            System.out.printf("testSet3 had it's 5th member (does not exist) removed:\n%s\n\n", testSet3);


            resultFileWriter.write("\nSize function test:\n");
            System.out.printf("\nSize function test:\n");

            resultFileWriter.write(String.format("The size of TestSet3 is: %s\n", testSet3.size()));
            System.out.printf("The size of TestSet3 is: %s\n", testSet3.size());

            resultFileWriter.write(String.format("The size of TestSet1 is: %s\n\n", testSet1.size()));
            System.out.printf("The size of TestSet1 is: %s\n\n", testSet1.size());


            resultFileWriter.write("\nUnion set test:\n\n");
            System.out.printf("\nUnion set test:\n\n");

            resultFileWriter.write(String.format("The union set of TestSet1 and TestSet3:\n%s\n\n", testSet1.union(testSet3)));
            System.out.printf("The union set of TestSet1 and TestSet3:\n%s\n\n", testSet1.union(testSet3));

            resultFileWriter.write(String.format("The union set of TestSet1 and TestSet2:\n%s\n\n", testSet1.union(testSet2)));
            System.out.printf("The union set of TestSet1 and TestSet2:\n%s\n\n", testSet1.union(testSet2));


            resultFileWriter.write("\nDifference set test:\n\n");
            System.out.printf("\nDifference set test:\n\n");

            resultFileWriter.write(String.format("The difference set of TestSet1 and TestSet3:\n%s\n\n", testSet1.difference(testSet3)));
            System.out.printf("The difference set of TestSet1 and TestSet3:\n%s\n\n", testSet1.difference(testSet3));

            resultFileWriter.write(String.format("The difference set of TestSet1 and TestSet2:\n%s\n\n", testSet1.difference(testSet2)));
            System.out.printf("The difference set of TestSet1 and TestSet2:\n%s\n\n", testSet1.difference(testSet2));


            resultFileWriter.write("\nIntersection set test:\n\n");
            System.out.printf("\nIntersection set test:\n\n");

            resultFileWriter.write(String.format("The Intersection set of TestSet1 and TestSet3:\n%s\n", testSet1.intersection(testSet3)));
            System.out.printf("The Intersection set of TestSet1 and TestSet3:\n%s\n", testSet1.intersection(testSet3));

            resultFileWriter.write(String.format("The Intersection set of TestSet1 and TestSet2:\n%s\n\n", testSet1.intersection(testSet2)));
            System.out.printf("The Intersection set of TestSet1 and TestSet2:\n%s\n\n", testSet1.intersection(testSet2));


            resultFileWriter.write("\nComplement set test:\n\n");
            System.out.printf("\nComplement set test:\n\n");

            resultFileWriter.write(String.format("The Complement set of TestSet2: \n%s\n", testSet2.complement()));
            System.out.printf("The Complement set of TestSet2: Printed to TestResults.txt file\n");

            resultFileWriter.write(String.format("The Complement set of TestSet1: \n%s\n", testSet1.complement()));
            System.out.printf("The Complement set of TestSet1: Printed to TestResults.txt file\n");

            resultFileWriter.write(String.format("\nThe total number of live DayofYear objects is: %d\n", DayofYearSet.getDaysAlive()));
            System.out.printf("\nThe total number of live DayofYear objects is: %d\n\n", DayofYearSet.getDaysAlive());

            resultFileWriter.write("De Morgan rule test:\n");
            resultFileWriter.write(String.format("for TestSet1 and TestSet2: %s\n", 
                (testSet1.union(testSet2)).complement().equals(testSet1.complement().intersection(testSet2.complement()))));

            resultFileWriter.write(String.format("for TestSet1 and TestSet3: %s\n", 
                (testSet1.union(testSet3)).complement().equals(testSet1.complement().intersection(testSet3.complement()))));

            resultFileWriter.write(String.format("for TestSet2 and TestSet3: %s\n", 
                (testSet2.union(testSet3)).complement().equals(testSet2.complement().intersection(testSet3.complement()))));

            resultFileWriter.write(String.format("for TestSet1 and TestSet4: %s\n", 
                (testSet1.union(testSet4)).complement().equals(testSet1.complement().intersection(testSet4.complement()))));

            resultFileWriter.write(String.format("for TestSet4 and TestSet3: %s\n", 
                (testSet4.union(testSet3)).complement().equals(testSet4.complement().intersection(testSet3.complement()))));

            resultFileWriter.close();
        }

        catch (IOException fileException) {
            System.out.println("File not created\n");
            fileException.printStackTrace();
            return;
        }

        catch(WrongDate excDate){

        }
    }
}
