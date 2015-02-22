
#include <iostream>
#include "Exceptions.h"
#include "ImageCommand.h"
#include "ImageTests.h"

using namespace std;
using namespace IManip;

/**
 * Runs tests on the library using ImageTester
 */
void runTests() {
    try {
        ImageTester tester;
        tester.run();
        tester.report();
    }
    catch (FileException ex)
    {
        cout << ex.getMessage() << ": " << ex.getFilename() << endl;
    }
    catch (Exception ex)
    {
        cout << ex.getMessage() << endl;
    }
}

int main(int argc, const char** argv) {
    try {
        if(argc > 1 && string(argv[1]) == "-test")
        {
            runTests();
        }
        else
        {
            parseAndRun(argc - 1, argv + 1);
        }
    }
    catch(FileException ex) {
        cout << ex.getMessage() << ": " << ex.getFilename() << endl;
    }
    catch(Exception ex) {
        cout << ex.getMessage() << endl;
    }
    
    return 0;
}
