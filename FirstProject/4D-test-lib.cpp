// 4D-test-lib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Line.cpp"
#include "Hyperplane.cpp"
#include "Slice.cpp"

int fourD_test_main()
{
    /*Line line(Vec4(0.0f, 1.0f, 0.0f, 0.0f), Vec4(0.0f, -1.0f, 0.0f, 0.0f));
    Tetrahedron tetra(
        Vec4(0.0f, 0.0f, 0.0f, 1.0f),
        Vec4(0.0f, -1.0f, 0.0f, -1.0f),
        Vec4(0.0f, 1.0f, 0.0f, -1.0f),
        Vec4(0.0f, 0.0f, 1.0f, -1.0f)
    );
    Hyperplane hp(Vec4(1.0f, 0.0f, 0.0f, 0.0f), 0.0f);

    std::optional<Vec4> ip = intersect(line, hp);
    std::vector<Triangle> tris = intersect(tetra, hp);

    std::cout << "Hello World!\n";*/

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
