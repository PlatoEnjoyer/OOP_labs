#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include "angle_range.hpp"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void AngleRangeTestCreationAndBasicProperties() {
    cout << "=== Testing Creation and Basic Properties ===" << endl;
    
    // Test creation from radians with inclusive bounds
    AngleRange range1 = AngleRange::FromFloat(0.0f, M_PI/2, true, true);
    assert(range1.len() > 0);
    cout << "✓ Created inclusive range [0, π/2] radians" << endl;
    
    // Test creation from radians with exclusive bounds
    AngleRange range2 = AngleRange::FromFloat(0.0f, M_PI/2, false, false);
    cout << "✓ Created exclusive range (0, π/2) radians" << endl;
    
    // Test creation from angles (in radians)
    Angle begin = Angle::fromRadians(M_PI/6);  // 30°
    Angle end = Angle::fromRadians(M_PI/3);    // 60°
    AngleRange range3 = AngleRange::FromAngle(begin, end, true, true);
    assert(range3.len() > 0);
    cout << "✓ Created range from Angle objects" << endl;
    
    // Test creation from integers (radians)
    AngleRange range4 = AngleRange::FromInt(0, 3, true, false);
    cout << "✓ Created range from integers (radians)" << endl;
}

void AngleRangeTestBoundaryTypes() {
    cout << "\n=== Testing Boundary Types ===" << endl;
    
    AngleRange inclusive = AngleRange::FromFloat(0.0f, M_PI/2, true, true);      // [0, π/2]
    AngleRange exclusive = AngleRange::FromFloat(0.0f, M_PI/2, false, false);    // (0, π/2)
    AngleRange mixed_start = AngleRange::FromFloat(0.0f, M_PI/2, true, false);   // [0, π/2)
    AngleRange mixed_end = AngleRange::FromFloat(0.0f, M_PI/2, false, true);     // (0, π/2]

    Angle zero = Angle::fromRadians(0.0f);
    Angle ninety = Angle::fromRadians(M_PI/2);      // 90° in radians
    Angle forty_five = Angle::fromRadians(M_PI/4);  // 45° in radians
    Angle small_positive = Angle::fromRadians(0.001f);
    Angle almost_ninety = Angle::fromRadians(M_PI/2 - 0.001f);
    
    // Test inclusive range [0, π/2]
    assert(inclusive.in(zero) == true);
    assert(inclusive.in(ninety) == true);
    assert(inclusive.in(forty_five) == true);
    cout << "✓ Inclusive range includes boundary angles" << endl;
    
    // Test exclusive range (0, π/2)
    assert(exclusive.in(zero) == false);
    assert(exclusive.in(ninety) == false);
    assert(exclusive.in(forty_five) == true);
    assert(exclusive.in(small_positive) == true);
    assert(exclusive.in(almost_ninety) == true);
    cout << "✓ Exclusive range excludes boundary angles" << endl;
    
    // Test mixed range [0, π/2)
    assert(mixed_start.in(zero) == true);
    assert(mixed_start.in(ninety) == false);
    assert(mixed_start.in(forty_five) == true);
    cout << "✓ Mixed range [0, π/2) handles boundaries correctly" << endl;
    
    // Test mixed range (0, π/2]
    assert(mixed_end.in(zero) == false);
    assert(mixed_end.in(ninety) == true);
    assert(mixed_end.in(forty_five) == true);
    cout << "✓ Mixed range (0, π/2] handles boundaries correctly" << endl;
}

void AngleRangeTestComparisonOperators() {
    cout << "\n=== Testing Comparison Operators ===" << endl;
    
    AngleRange range1 = AngleRange::FromFloat(0.0f, M_PI/2, true, true);      // [0, π/2]
    AngleRange range2 = AngleRange::FromFloat(0.0f, M_PI/2, true, true);      // [0, π/2]
    AngleRange range3 = AngleRange::FromFloat(M_PI/6, M_PI/3, true, true);    // [π/6, π/3]
    AngleRange range4 = AngleRange::FromFloat(0.0f, M_PI, true, true);        // [0, π]
    
    // Test equality
    assert(range1 == range2);
    assert(!(range1 == range3));
    cout << "✓ Equality operator works correctly" << endl;
    
    // Test inequality
    assert(range1 != range3);
    assert(!(range1 != range2));
    cout << "✓ Inequality operator works correctly" << endl;
    
    // Test less than (by length)
    assert(range3 < range1);
    assert(range1 < range4);
    assert(!(range4 < range1));
    cout << "✓ Less than operator works correctly" << endl;
    
    // Test greater than (by length)
    assert(range1 > range3);
    assert(range4 > range1);
    assert(!(range3 > range1));
    cout << "✓ Greater than operator works correctly" << endl;
    
    // Test less than or equal
    assert(range1 <= range2);
    assert(range3 <= range1);
    assert(!(range4 <= range1));
    cout << "✓ Less than or equal operator works correctly" << endl;
    
    // Test greater than or equal
    assert(range1 >= range2);
    assert(range4 >= range1);
    assert(!(range3 >= range1));
    cout << "✓ Greater than or equal operator works correctly" << endl;
}

void AngleRangeTestLengthCalculation() {
    cout << "\n=== Testing Length Calculation ===" << endl;
    
    AngleRange range1 = AngleRange::FromFloat(0.0f, M_PI/2, true, true);  // [0, π/2]
    AngleRange range2 = AngleRange::FromFloat(0.0f, M_PI, true, true);    // [0, π]
    
    float len1 = range1.len();
    float len2 = range2.len();
    
    assert(len1 > 0);
    assert(len2 > 0);
    assert(len2 > len1);
    assert(abs(len1 - M_PI/2) < 0.001f);
    assert(abs(len2 - M_PI) < 0.001f);
    
    cout << "✓ Length calculation is correct" << endl;
    cout << "✓ Range [0, π/2] length: " << len1 << " radians" << endl;
    cout << "✓ Range [0, π] length: " << len2 << " radians" << endl;
}

void AngleRangeTestContainsAngle() {
    cout << "\n=== Testing Angle Containment ===" << endl;
    
    // Range [π/6, π/3] radians = [30°, 60°]
    AngleRange range = AngleRange::FromFloat(M_PI/6, M_PI/3, true, true);
    
    Angle inside = Angle::fromRadians(M_PI/4);          // 45°
    Angle boundary_low = Angle::fromRadians(M_PI/6);    // 30°
    Angle boundary_high = Angle::fromRadians(M_PI/3);   // 60°
    Angle outside_low = Angle::fromRadians(M_PI/12);    // 15°
    Angle outside_high = Angle::fromRadians(M_PI/2);    // 90°
    
    assert(range.in(inside) == true);
    assert(range.in(boundary_low) == true);
    assert(range.in(boundary_high) == true);
    assert(range.in(outside_low) == false);
    assert(range.in(outside_high) == false);
    
    cout << "✓ Angle containment works correctly" << endl;
}

void AngleRangeTestRangeContainment() {
    cout << "\n=== Testing Range Containment ===" << endl;
    
    AngleRange large = AngleRange::FromFloat(1.0f, 3.0f, true, true);      // [1, 3]
    AngleRange small = AngleRange::FromFloat(1.5f, 2.5f, true, true);      // [1.5, 2.5]
    AngleRange outside = AngleRange::FromFloat(4.0f, 5.0f, true, true);    // [4, 5]
    
    assert(large.in(small) == true);
    cout << "✓ Small range contained in large range" << endl;
    
    assert(small.in(large) == false);
    cout << "✓ Large range not contained in small range" << endl;
    
    assert(large.in(outside) == false);
    cout << "✓ Outside range not contained" << endl;
}

void AngleRangeTestNormalization() {
    cout << "\n=== Testing Angle Normalization in Ranges ===" << endl;
    
    // Test range that crosses 2π boundary [7π/4, π/2] = [315°, 90°]
    AngleRange range1 = AngleRange::FromFloat(7*M_PI/4, M_PI/2, true, true);
    assert(range1.len() > 0);
    cout << "✓ Range crossing 2π boundary handled correctly" << endl;
    
    // Test negative angles [-π/6, π/6]
    AngleRange range2 = AngleRange::FromFloat(-M_PI/6, M_PI/6, true, true);
    assert(range2.len() > 0);
    cout << "✓ Negative angles normalized correctly" << endl;
    
    // Test large angles [4π, 9π/2] = [720°, 810°]
    AngleRange range3 = AngleRange::FromFloat(4*M_PI, 9*M_PI/2, true, true);
    Angle test_angle = Angle::fromRadians(M_PI/4); // Should be outside this range
    assert(range3.in(test_angle) == true);
    cout << "✓ Large angles normalized correctly" << endl;
}

void AngleRangeTestOperatorPlus() {
    cout << "\n=== Testing Operator+ ===" << endl;
    
    AngleRange range1 = AngleRange::FromFloat(0.0f, M_PI/2, true, true);      // [0, π/2]
    AngleRange range2 = AngleRange::FromFloat(M_PI, 3*M_PI/2, true, true);    // [π, 3π/2]
    
    vector<AngleRange> result = range1 + range2;
    
    assert(result.size() == 2);
    assert(result[0] == range1);
    assert(result[1] == range2);
    
    cout << "✓ Operator+ returns vector of both ranges" << endl;
}

void AngleRangeTestSpecialCases() {
    cout << "\n=== Testing Special Cases ===" << endl;
    
    // Zero-length range (inclusive)
    AngleRange zero_range_inc = AngleRange::FromFloat(M_PI/4, M_PI/4, true, true);
    Angle angle45 = Angle::fromRadians(M_PI/4);
    assert(zero_range_inc.in(angle45) == true);
    cout << "✓ Zero-length inclusive range contains the angle" << endl;
    
    // Zero-length range (exclusive)
    AngleRange zero_range_exc = AngleRange::FromFloat(M_PI/4, M_PI/4, false, false);
    assert(zero_range_exc.in(angle45) == false);
    cout << "✓ Zero-length exclusive range excludes the angle" << endl;
    
    // Very small range
    AngleRange small_range = AngleRange::FromFloat(M_PI/4, M_PI/4 + 0.01f, true, true);
    assert(small_range.len() > 0);
    cout << "✓ Very small range has positive length" << endl;
}

void AngleRangeTestOutputStream() {
    cout << "\n=== Testing Output Stream ===" << endl;
    
    AngleRange inclusive = AngleRange::FromFloat(0.0f, M_PI/2, true, true);
    AngleRange exclusive = AngleRange::FromFloat(0.0f, M_PI/2, false, false);
    AngleRange mixed = AngleRange::FromFloat(0.0f, M_PI/2, true, false);
    
    cout << "✓ Inclusive range output: " << inclusive << endl;
    cout << "✓ Exclusive range output: " << exclusive << endl;
    cout << "✓ Mixed range output: " << mixed << endl;
}

void AngleRangeTestBoundarySwapping() {
    cout << "\n=== Testing Boundary Swapping ===" << endl;
    
    // Create range with begin > end, should automatically swap
    AngleRange swapped = AngleRange::FromFloat(M_PI/2, 0.0f, true, false);
    Angle zero = Angle::fromRadians(0.0f);
    Angle ninety = Angle::fromRadians(M_PI/2);
    
    // Check that boundaries were swapped
    assert(swapped.in(zero) == false);  // Now 0 is the end (exclusive)
    assert(swapped.in(ninety) == true); // Now π/2 is the begin (inclusive)
    cout << "✓ Boundaries automatically swapped when begin > end" << endl;
}