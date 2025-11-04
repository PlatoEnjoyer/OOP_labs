#include <iostream>
#include <cmath>
#include <string>
#include "angle.hpp"
#include "test_angle.hpp"
#include "test_angle_range.hpp"


int main() {
    cout << "Running tests for the Angle class..." << endl;
    cout << "=====================================" << endl;
    
    try {
        testCreationAndConversion();
        testNormalization();
        testComparison();
        testArithmetic();
        testEdgeCases();
        testConversionMethods();
        testDivisionByZero();
        testOutputStream();
        
        cout << "\n=====================================" << endl;
        cout << "All tests have been passed successfully" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error" << endl;
        return 1;
    }

    cout << "Starting tests for AngleRange class..." << endl;
    cout << "=======================================" << endl;
    
    try {
        AngleRangeTestCreationAndBasicProperties();
        AngleRangeTestBoundaryTypes();
        AngleRangeTestComparisonOperators();
        AngleRangeTestLengthCalculation();
        AngleRangeTestContainsAngle();
        AngleRangeTestRangeContainment();
        AngleRangeTestNormalization();
        AngleRangeTestOperatorPlus();
        AngleRangeTestSpecialCases();
        AngleRangeTestOutputStream();
        AngleRangeTestBoundarySwapping();
        
        cout << "\n=======================================" << endl;
        cout << "All tests passed successfully!" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred" << endl;
        return 1;
    }
}