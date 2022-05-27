#include <iostream>
#include <algorithm>
#include <vector>
#include "GTUVector.h"
#include "GTUArray.h"
#include "GTUSet.h"
#include "GTUIterators.h"

using std::cout;
using std::endl;

template<typename T>
void print(T i) { cout << i << ", "; }

int main()
{
    cout << "\nGTUVector Test:\n";
    cout << "GTUVector<int> Vector1 = {1, 2, 3, 4};\n";
    GTU::GTUVector<int> Vector1 = {2, 4, 1, 3};
    for(auto temp = Vector1.begin(); temp != Vector1.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nVector1.size(): " << Vector1.size() << endl;
    cout << "Vector1.capacity(): " << Vector1.capacity() << endl;

    cout << "\nauto Vector2(Vector1);\n";
    auto Vector2(Vector1);
    for(auto temp = Vector2.begin(); 
        temp != Vector2.end(); 
        ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nVector3 = Vector1;\n";
    auto Vector3 = Vector1;
    for(auto temp = Vector3.begin(); temp != Vector3.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nauto Vector4(move(Vector1));\n";
    auto Vector4(move(Vector1));
    for(auto temp = Vector4.begin(); temp != Vector4.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nVector5 = move(Vector4);\n";
    auto Vector5 = move(Vector4);
    for(auto temp = Vector5.begin(); temp != Vector5.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nVector5.erase(Vector5.begin());\n";
    Vector5.erase(Vector5.begin());
    for(auto temp = Vector5.begin(); temp != Vector5.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nVector5[2]: " << Vector5[2];

    cout << "\n\nVector5.add(5);\n";
    Vector5.add(5);
    for(auto temp = Vector5.cbegin(); temp != Vector5.cend(); ++temp)
        std::cout << *temp << ", ";

    // cout << "\n\nstd::swap Vector5 and Vector4:\nVector4:\n";
    // std::sort(Vector5.begin(), Vector5.begin());
    // std::for_each(Vector5.begin(), Vector5.end(), print<int>);

    cout << "\n\nstd::find 3 in Vector4: " 
         << *(std::find(Vector5.begin(), Vector5.end(), 3));
    
    cout << "\n\nprint the data std::for_each element in Vector4:\n";
    std::for_each(Vector5.begin(), Vector5.end(), print<int>);

    cout << "\n\nVector4.clear();\n";
    Vector5.clear();
    for(auto temp = Vector5.begin(); temp != Vector5.end(); ++temp)
        std::cout << *temp << ", ";



    cout << "\n\n\n\nGTUArray Test:\n";
    cout << "GTUArray<char, 4> Array1 = {'a', 'b', 'c', 'd'};\n";
    GTU::GTUArray<char, 4> Array1 = {'e', 'c', 'b', 'd'};
    for(auto temp = Array1.begin(); temp != Array1.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nArray1.size(): " << Array1.size();

    cout << "\n\nauto Array2(Array1);\n";
    auto Array2(Array1);
    for(auto temp = Array2.begin(); temp != Array2.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nauto Array3 = Array1;\n";
    auto Array3 = Array1;
    for(auto temp = Array3.begin(); temp != Array3.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nauto Array4(move(Array1));\n";
    auto Array4(move(Array1));
    for(auto temp = Array4.begin(); temp != Array4.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nArray5 = move(Array4);\n";
    auto Array5 = move(Array4);
    for(auto temp = Array5.begin(); temp != Array5.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nArray5[3]: " << Array5[3];

    // cout << "\n\nstd::swap Array5 and Array5:\nArray5:\n";
    // std::sort(Array5.begin(), Array5.begin());
    // std::for_each(Array5.begin(), Array5.end(), print<char>);

    cout << "\n\nstd::find 'b' in Array5: " 
         << *(std::find(Array5.begin(), Array5.end(), 'b'));
    
    cout << "\n\nprint the data std::for_each element in Array5:\n";
    std::for_each(Array5.begin(), Array5.end(), print<char>);

    cout << "\n\nArray5.fill('e');\n";
    Array5.fill('e');
    for(auto temp = Array5.begin(); temp != Array5.end(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nArray5.clear();\n";
    Array5.clear();
    for(auto temp = Array5.begin(); temp != Array5.end(); ++temp)
        std::cout << *temp << ", ";



    cout << "\n\n\n\nGTUSet Test:\n";
    cout << "GTUSet<int> Set1 = {10, 30, 50, 20, 40};\n";
    GTU::GTUSet<int> Set1 = {10, 30, 50, 20, 40};
    for(auto temp = Set1.cbegin(); temp != Set1.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet1.add(10);\n";
    Set1.add(10);
    for(auto temp = Set1.cbegin(); temp != Set1.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet1.add(11);\n";
    Set1.add(11);
    for(auto temp = Set1.cbegin(); temp != Set1.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet1.erase(++Set1.begin());\n";
    Set1.erase(++Set1.begin());
    for(auto temp = Set1.cbegin(); temp != Set1.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nauto Set2(Set1);\n";
    auto Set2(Set1);
    for(auto temp = Set2.cbegin(); temp != Set2.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet3 = Set2;\n";
    auto Set3 = Set2;
    for(auto temp = Set3.cbegin(); temp != Set3.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nauto Set4(move(Set3));\n";
    auto Set4(move(Set3));
    for(auto temp = Set4.cbegin(); temp != Set4.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet5 = move(Set4);\n";
    auto Set5 = move(Set4);
    for(auto temp = Set5.cbegin(); temp != Set5.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nUnion:\nSet6 = {60, 70, 90, 80, 100}\nSet7 = Set6 + Set5;\n";
    GTU::GTUSet<int> Set6 = {60, 70, 90, 80, 100},
                     Set7 = Set6 + Set5;
    for(auto temp = Set7.cbegin(); temp != Set7.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nSet8 = Set7.intersect({30, 80, 100, 20, 50})\n";
    auto Set8 = Set7.intersect({30, 80, 100, 20, 50});
    for(auto temp = Set8.cbegin(); temp != Set8.cend(); ++temp)
        std::cout << *temp << ", ";

    cout << "\n\nstd::find 50 in Set8: " 
         << *(std::find(Set8.begin(), Set8.end(), 50));
    
    cout << "\n\nprint the data std::for_each element in Set8:\n";
    std::for_each(Set8.begin(), Set8.end(), print<int>);

    // cout << "\n\nstd::swap Set8 and Set7:\nSet7:\n";
    // std::swap(Set7, Set8);
    // std::for_each(Set8.begin(), Set8.end(), print<int>);

    cout << endl;
}