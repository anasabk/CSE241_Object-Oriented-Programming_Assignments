#include <iostream>
#include <vector>
#include "header.h"

using std::cout;
using std::cerr;
using DayofYearNS::DayofYearSet;
using std::ofstream;
using std::vector;
using std::endl;


int main()
{
    vector<DayofYearSet::DayofYear> testVector = {DayofYearSet::DayofYear(15, 3),
                                                  DayofYearSet::DayofYear(24, 7),
                                                  DayofYearSet::DayofYear(3, 11),
                                                  DayofYearSet::DayofYear(8, 7)};

    ofstream resultFile;
    DayofYearSet testSet1, testSet2, testSet3, testSet4(testVector);

    resultFile.open("results", ifstream::out);

    if (resultFile.is_open() == false)
    {
        cerr << "Couldn't open results" << endl;
        exit(2);
    }
    
    testSet1 << "sample_input1.txt";
    cout << "testSet1:" << testSet1;

    testSet2 << "sample_input2.txt";
    cout << "testSet2:" << testSet2;

    testSet3 << "sample_input3.txt";
    cout << "testSet3:" << testSet3;

    cout << "testSet4:" << testSet4;

    cout << "testSet1 == testSet2 is " << (testSet1 == testSet2 ? "true" : "false") << endl;
    cout << "testSet2 != testSet4 is " << (testSet2 != testSet4 ? "true" : "false") << endl;


    resultFile << "\nadd function test:\n\n";
    cout << "add function test:\n\n";

    DayofYearSet::DayofYear addTest(12,3);
    testSet3.add(addTest);
    cout << "Day 12/03 is added to testSet3: Done\n";
    resultFile << "12/03 added to testSet3:" << testSet3;

    testSet1.add(25, 13);
    cout << "Day 25/13 (invalid) is added to testSet1: Done\n\n";
    resultFile << "25/13 (invalid) added to testSet1:" << testSet1;


    resultFile << "\nremove function test:\n\n";
    cout << "remove function test:\n\n";

    testSet2.remove(3);
    cout << "testSet2 had it's 3rd member removed: Done\n";
    resultFile << "3rd member removed from testSet2:" << testSet2;

    testSet4.remove(5);
    cout << "testSet4 had it's 5th member (does not exist) removed: Done\n\n";
    resultFile << "5th member (does not exist) removed from testSet4:" << testSet4;


    cout << "\nSize function test:\n";

    cout << "The size of TestSet4 is: " << testSet4.size() << endl;
    cout << "The size of TestSet1 is: " << testSet1.size() << endl << endl;


    resultFile << "\nUnion set test:\n\n";
    cout << "\nUnion set test:\n";

    cout << "The union set of TestSet1 and TestSet3: Done\n";
    resultFile << "The union set of TestSet1 and TestSet3:" << (testSet1 + testSet3);

    cout << "The union set of TestSet1 and TestSet2: Done\n\n";
    resultFile << "The union set of TestSet1 and TestSet2" << (testSet1 + testSet2);


    resultFile << "\nDifference set test:\n\n";
    cout << "\nDifference set test:\n";

    cout << "The difference set between TestSet1 and TestSet3: Done\n";
    resultFile << "The difference set between TestSet1 and TestSet3:" << (testSet1 - testSet3);
    
    cout << "The difference set between TestSet1 and TestSet2: Done\n";
    resultFile << "The difference set between TestSet1 and TestSet2:" << (testSet1 - testSet2);


    resultFile << "\nIntersection set test:\n\n";
    cout << "\nIntersection set test:\n";

    cout << "The intersection between TestSet1 and TestSet3: Done\n";
    resultFile << "The intersection between TestSet1 and TestSet3:" << (testSet1 ^ testSet3);

    cout << "The intersection between TestSet1 and TestSet2: Done\n";
    resultFile << "The intersection between TestSet1 and TestSet2:" << (testSet1 ^ testSet2);


    resultFile << "\nComplement set test:\n\n";
    cout << "\nComplement set test:\n";

    cout << "The complement set of TestSet1: Done\n";
    resultFile << "The complement set of TestSet1:" << (!testSet1);

    cout << "The complement set of TestSet2: Done\n";
    resultFile << "The complement set of TestSet2:" << (!testSet2);


    cout << "\nIndex operator test:\n";

    cout << "The 4th element of TestSet2 is " << testSet2[3].getDay() << '/' << testSet2[3].getMonth() << endl;
    //cout << "The 7th element (does not exist) of TestSet1 is: " << testSet1[6].getDay() << '/' << testSet1[6].getMonth() << endl; //Will exit with code 4

    cout << "\nThe total number of live DayofYear objects is: " << DayofYearSet::numofLiveDates() << endl << endl;

    cout << "De Morgan rule test:\n";
    cout << "for TestSet1 and TestSet2: " << (!(testSet1 + testSet2) == (!testSet1 ^ !testSet2)  ? "true" : "false") << endl;
    cout << "for TestSet1 and TestSet3: " << (!(testSet1 + testSet3) == (!testSet1 ^ !testSet3)  ? "true" : "false") << endl;
    cout << "for TestSet2 and TestSet3: " << (!(testSet2 + testSet3) == (!testSet2 ^ !testSet3)  ? "true" : "false") << endl;
    cout << "for TestSet2 and TestSet4: " << (!(testSet2 + testSet4) == (!testSet2 ^ !testSet4)  ? "true" : "false") << endl;
    cout << "for TestSet4 and TestSet1: " << (!(testSet4 + testSet1) == (!testSet4 ^ !testSet1)  ? "true" : "false") << endl;
}