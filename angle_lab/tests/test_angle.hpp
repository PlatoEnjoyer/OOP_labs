#include "angle.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>

void test_creation_and_conversion_methods() {
    std::cout << "=== Testing Creation and Conversion Methods ===" << std::endl;
    
    // Test fromDegrees
    std::cout << "Testing fromDegrees(180.0f) - expected: radians = " << Angle::Pi << std::endl;
    Angle a1 = Angle::fromDegrees(180.0f);
    assert(std::fabs(a1.getAsRadians() - Angle::Pi) < Angle::epsilon);
    std::cout << " Result: radians = " << a1.getAsRadians() << " - OK" << std::endl;
    
    // Test fromRadians
    std::cout << "Testing fromRadians(" << Angle::Pi << ") - expected: degrees = 180.0" << std::endl;
    Angle a2 = Angle::fromRadians(Angle::Pi);
    assert(std::fabs(a2.getAsDegrees() - 180.0f) < Angle::epsilon);
    std::cout << " Result: degrees = " << a2.getAsDegrees() << " - OK" << std::endl;
    
    // Test copy constructor
    std::cout << "Testing copy constructor - expected: identical angle values" << std::endl;
    Angle a3 = a1;
    assert(a3 == a1);
    std::cout << " Result: original = " << a1.getAsRadians() << ", copy = " << a3.getAsRadians() << " - OK" << std::endl;
}

void test_normalization_functionality() {
    std::cout << "\n=== Testing Normalization Functionality ===" << std::endl;
    
    // Test angles > 2pi (normalization function only, not automatic)
    std::cout << "Testing normalize_angle(3*Pi) - expected: normalized to = Pi" << std::endl;
    float normalized = Angle::normalize_angle(3 * Angle::Pi);
    assert(std::fabs(normalized - Angle::Pi) < Angle::epsilon);
    std::cout << " Result: 3*Pi normalized to " << normalized << " - OK" << std::endl;
    
    // Test negative angles
    std::cout << "Testing normalize_angle(-Pi/2) - expected: normalized to = 3*Pi/2" << std::endl;
    normalized = Angle::normalize_angle(-Angle::Pi / 2);
    assert(std::fabs(normalized - (3 * Angle::Pi / 2)) < Angle::epsilon);
    std::cout << " Result: -Pi/2 normalized to " << normalized << " - OK" << std::endl;
    
    // Test multiples of 2pi
    std::cout << "Testing normalize_angle(4*Pi) - expected: normalized to = 0" << std::endl;
    normalized = Angle::normalize_angle(4 * Angle::Pi);
    assert(std::fabs(normalized - 0.0f) < Angle::epsilon);
    std::cout << " Result: 4*Pi normalized to " << normalized << " - OK" << std::endl;
}

void test_comparison_operators() {
    std::cout << "\n=== Testing Comparison Operators ===" << std::endl;
    
    Angle a1 = Angle::fromDegrees(45.0f);
    Angle a2 = Angle::fromDegrees(45.0f);
    Angle a3 = Angle::fromDegrees(90.0f);
    Angle a4 = Angle::fromDegrees(30.0f);
    
    // Test operator==
    std::cout << "Testing operator== with 45 and 45 - expected: true" << std::endl;
    assert(a1 == a2);
    std::cout << " Result: " << (a1 == a2) << " - OK" << std::endl;
    
    std::cout << "Testing operator== with 45 and 90 - expected: false" << std::endl;
    assert(!(a1 == a3));
    std::cout << " Result: " << (a1 == a3) << " - OK" << std::endl;
    
    // Test operator!=
    std::cout << "Testing operator!= with 45 and 90 - expected: true" << std::endl;
    assert(a1 != a3);
    std::cout << " Result: " << (a1 != a3) << " - OK" << std::endl;
    
    // Test operator<
    std::cout << "Testing operator< with 30 and 45 - expected: true" << std::endl;
    assert(a4 < a1);
    std::cout << " Result: " << (a4 < a1) << " - OK" << std::endl;
    
    // Test operator>
    std::cout << "Testing operator> with 90 and 45 - expected: true" << std::endl;
    assert(a3 > a1);
    std::cout << " Result: " << (a3 > a1) << " - OK" << std::endl;
    
    // Test that 3pi ≠ pi (no automatic normalization)
    std::cout << "Testing 3pi vs pi - expected: not equal (no auto-normalization)" << std::endl;
    Angle pi = Angle::fromRadians(Angle::Pi);
    Angle three_pi = Angle::fromRadians(3 * Angle::Pi);
    assert(pi != three_pi);
    std::cout << " Result: pi = " << pi.getAsRadians() << ", 3pi = " << three_pi.getAsRadians() << ", equal = " << (pi == three_pi) << " - OK" << std::endl;
}

void test_isEquivalent_method() {
    std::cout << "\n=== Testing isEquivalent Method ===" << std::endl;
    
    // Test equivalent angles (modulo 2pi)
    std::cout << "Testing isEquivalent with 45 and 405 - expected: true" << std::endl;
    Angle a1 = Angle::fromDegrees(45.0f);
    Angle a2 = Angle::fromDegrees(405.0f); // 405 = 45 after normalization
    assert(a1.is_eq_with_2pi_mod(a2));
    std::cout << " Result: " << a1.is_eq_with_2pi_mod(a2) << " - OK" << std::endl;
    
    // Test non-equivalent angles
    std::cout << "Testing isEquivalent with 45 and 90 - expected: false" << std::endl;
    Angle a3 = Angle::fromDegrees(90.0f);
    assert(!a1.is_eq_with_2pi_mod(a3));
    std::cout << " Result: " << a1.is_eq_with_2pi_mod(a3) << " - OK" << std::endl;
    
    // Test that isEquivalent differs from operator==
    std::cout << "Testing that isEquivalent differs from operator== for 3pi and pi - expected: isEquivalent=true, operator==false" << std::endl;
    Angle pi = Angle::fromRadians(Angle::Pi);
    Angle three_pi = Angle::fromRadians(3 * Angle::Pi);
    assert(three_pi.is_eq_with_2pi_mod(pi));
    assert(!(three_pi == pi));
    std::cout << " Result: isEquivalent=" << three_pi.is_eq_with_2pi_mod(pi) << ", operator==" << (three_pi == pi) << " - OK" << std::endl;
}

void test_arithmetic_operations() {
    std::cout << "\n=== Testing Arithmetic Operations ===" << std::endl;
    
    Angle a1 = Angle::fromDegrees(30.0f);
    Angle a2 = Angle::fromDegrees(60.0f);
    
    // Test addition
    std::cout << "Testing operator+ with 30 + 60 - expected: 90" << std::endl;
    Angle sum = a1 + a2;
    assert(std::fabs(sum.getAsDegrees() - 90.0f) < Angle::epsilon);
    std::cout << " Result: " << sum.getAsDegrees() << " - OK" << std::endl;
    
    // Test subtraction
    std::cout << "Testing operator- with 60 - 30 - expected: 30" << std::endl;
    Angle diff = a2 - a1;
    assert(std::fabs(diff.getAsDegrees() - 30.0f) < Angle::epsilon);
    std::cout << " Result: " << diff.getAsDegrees() << " - OK" << std::endl;
    
    // Test multiplication by scalar
    std::cout << "Testing operator* with 30 * 2 - expected: 60" << std::endl;
    Angle multiplied = a1 * 2.0f;
    assert(std::fabs(multiplied.getAsDegrees() - 60.0f) < Angle::epsilon);
    std::cout << " Result: " << multiplied.getAsDegrees() << " - OK" << std::endl;
    
    // Test division by scalar
    std::cout << "Testing operator/ with 60 / 2 - expected: 30" << std::endl;
    Angle divided = a2 / 2.0f;
    assert(std::fabs(divided.getAsDegrees() - 30.0f) < Angle::epsilon);
    std::cout << " Result: " << divided.getAsDegrees() << " - OK" << std::endl;
}

void test_compound_assignment_operators() {
    std::cout << "\n=== Testing Compound Assignment Operators ===" << std::endl;
    
    // Test +=
    std::cout << "Testing operator+= with 10 += 30 - expected: 40" << std::endl;
    Angle a1 = Angle::fromDegrees(10.0f);
    a1 += Angle::fromDegrees(30.0f);
    assert(std::fabs(a1.getAsDegrees() - 40.0f) < Angle::epsilon);
    std::cout << " Result: " << a1.getAsDegrees() << " - OK" << std::endl;
    
    // Test -=
    std::cout << "Testing operator-= with 50 -= 30 - expected: 20" << std::endl;
    Angle a2 = Angle::fromDegrees(50.0f);
    a2 -= Angle::fromDegrees(30.0f);
    assert(std::fabs(a2.getAsDegrees() - 20.0f) < Angle::epsilon);
    std::cout << " Result: " << a2.getAsDegrees() << " - OK" << std::endl;
    
    // Test *=
    std::cout << "Testing operator*= with 15 *= 3 - expected: 45" << std::endl;
    Angle a3 = Angle::fromDegrees(15.0f);
    a3 *= 3.0f;
    assert(std::fabs(a3.getAsDegrees() - 45.0f) < Angle::epsilon);
    std::cout << " Result: " << a3.getAsDegrees() << " - OK" << std::endl;
    
    // Test /=
    std::cout << "Testing operator/= with 90 /= 3 - expected: 30" << std::endl;
    Angle a4 = Angle::fromDegrees(90.0f);
    a4 /= 3.0f;
    assert(std::fabs(a4.getAsDegrees() - 30.0f) < Angle::epsilon);
    std::cout << " Result: " << a4.getAsDegrees() << " - OK" << std::endl;
}

void test_float_operators() {
    std::cout << "\n=== Testing Float Operators ===" << std::endl;
    
    Angle a1 = Angle::fromRadians(1.0f);
    
    // Test operator+ with float
    std::cout << "Testing operator+ with angle + 1.0 rad - expected: 2.0 rad" << std::endl;
    Angle result1 = a1 + 1.0f;
    assert(std::fabs(result1.getAsRadians() - 2.0f) < Angle::epsilon);
    std::cout << " Result: " << result1.getAsRadians() << " rad - OK" << std::endl;
    
    // Test operator+= with float
    std::cout << "Testing operator+= with angle += 0.5 rad - expected: 1.5 rad" << std::endl;
    Angle a2 = Angle::fromRadians(1.0f);
    a2 += 0.5f;
    assert(std::fabs(a2.getAsRadians() - 1.5f) < Angle::epsilon);
    std::cout << " Result: " << a2.getAsRadians() << " rad - OK" << std::endl;
    
    // Test that operations don't auto-normalize
    std::cout << "Testing that 2pi + pi doesn't auto-normalize - expected: 3pi" << std::endl;
    Angle two_pi = Angle::fromRadians(2 * Angle::Pi);
    Angle pi = Angle::fromRadians(Angle::Pi);
    Angle result = two_pi + pi;
    assert(std::fabs(result.getAsRadians() - 3 * Angle::Pi) < Angle::epsilon);
    std::cout << " Result: " << result.getAsRadians() / Angle::Pi << "pi - OK" << std::endl;
}

void test_edge_cases() {
    std::cout << "\n=== Testing Edge Cases ===" << std::endl;
    
    // Test zero angle
    std::cout << "Testing zero angle creation - expected: 0 radians" << std::endl;
    Angle zero1 = Angle::fromDegrees(0.0f);
    Angle zero2 = Angle::fromRadians(0.0f);
    assert(zero1 == zero2);
    std::cout << " Result: both zero angles are equal - OK" << std::endl;
    
    // Test that 360 ≠ 0 (no auto-normalization)
    std::cout << "Testing 360 angle - expected: not equal to 0 (no auto-normalization)" << std::endl;
    Angle full = Angle::fromDegrees(360.0f);
    assert(full != zero1);
    std::cout << " Result: 360 = " << full.getAsRadians() << ", 0 = " << zero1.getAsRadians() << ", equal = " << (full == zero1) << " - OK" << std::endl;
    
    // Test large angles (no normalization)
    std::cout << "Testing 720 angle - expected: not equal to 0" << std::endl;
    Angle big = Angle::fromDegrees(720.0f);
    assert(big != zero1);
    std::cout << " Result: 720 = " << big.getAsRadians() << ", equal to 0 = " << (big == zero1) << " - OK" << std::endl;
}

void test_conversion_methods() {
    std::cout << "\n=== Testing Conversion Methods ===" << std::endl;
    
    Angle a1 = Angle::fromRadians(2.0f);
    
    // Test to_string
    std::cout << "Testing to_string() - expected: non-empty string" << std::endl;
    std::string str = a1.to_string();
    assert(!str.empty());
    std::cout << " Result: '" << str << "' - OK" << std::endl;
    
    // Test to_int
    std::cout << "Testing to_int() with 2.7 rad - expected: 2" << std::endl;
    Angle a2 = Angle::fromRadians(2.7f);
    int int_val = a2.to_int();
    assert(int_val == 2);
    std::cout << " Result: " << int_val << " - OK" << std::endl;
    
    // Test to_float
    std::cout << "Testing to_float() - expected: original radian value" << std::endl;
    float float_val = a1.to_float();
    assert(std::fabs(float_val - 2.0f) < Angle::epsilon);
    std::cout << " Result: " << float_val << " - OK" << std::endl;
}

void test_division_by_zero() {
    std::cout << "\n=== Testing Division by Zero ===" << std::endl;
    
    Angle a1 = Angle::fromDegrees(90.0f);
    bool caught_exception = false;
    
    // Test operator/ with zero
    std::cout << "Testing operator/ with divisor 0 - expected: exception" << std::endl;
    try {
        Angle result = a1 / 0.0f;
    } catch (const std::invalid_argument& e) {
        caught_exception = true;
        std::cout << " Result: exception caught - '" << e.what() << "' - OK" << std::endl;
    }
    assert(caught_exception);
    
    // Test operator/= with zero
    caught_exception = false;
    std::cout << "Testing operator/= with divisor 0 - expected: exception" << std::endl;
    try {
        a1 /= 0.0f;
    } catch (const std::invalid_argument& e) {
        caught_exception = true;
        std::cout << " Result: exception caught - '" << e.what() << "' - OK" << std::endl;
    }
    assert(caught_exception);
}

void test_output_operations() {
    std::cout << "\n=== Testing Output Operations ===" << std::endl;
    
    Angle a1 = Angle::fromRadians(Angle::Pi);
    
    // Test operator<<
    std::cout << "Testing operator<< with pi radians - expected: formatted output" << std::endl;
    std::cout << " Result: " << a1 << " - OK" << std::endl;
    
    // Test printAsDegrees
    std::cout << "Testing printAsDegrees() - output: ";
    a1.printAsDegrees();
    
    // Test printAsRadians
    std::cout << "Testing printAsRadians() - output: ";
    a1.printAsRadians();
}