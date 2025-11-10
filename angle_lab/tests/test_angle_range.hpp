#include "angle_range.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void test_creation_methods() {
    std::cout << "=== Testing Creation Methods ===" << std::endl;
    
    // Test FromFloat
    std::cout << "Testing FromFloat(0.0f, 1.57f, true, false) - expected: begin=0.0f, end=1.57f, begin_inclusive=true, end_inclusive=false" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(0.0f, 1.57f, true, false);
    assert(range1.getBegin().getAsRadians() == 0.0f);
    assert(range1.getEnd().getAsRadians() == 1.57f);
    assert(range1.isBeginInclusive() == true);
    assert(range1.isEndInclusive() == false);
    std::cout << " Result: begin=" << range1.getBegin().getAsRadians() << ", end=" << range1.getEnd().getAsRadians() 
              << ", begin_inclusive=" << range1.isBeginInclusive() << ", end_inclusive=" << range1.isEndInclusive() << " - OK" << std::endl;
    
    // Test FromInt
    std::cout << "Testing FromInt(0, 3, false, true) - expected: begin=0.0f, end=3.0f, begin_inclusive=false, end_inclusive=true" << std::endl;
    AngleRange range2 = AngleRange::FromInt(0, 3, false, true);
    assert(range2.getBegin().getAsRadians() == 0.0f);
    assert(range2.getEnd().getAsRadians() == 3.0f);
    assert(range2.isBeginInclusive() == false);
    assert(range2.isEndInclusive() == true);
    std::cout << " Result: begin=" << range2.getBegin().getAsRadians() << ", end=" << range2.getEnd().getAsRadians() 
              << ", begin_inclusive=" << range2.isBeginInclusive() << ", end_inclusive=" << range2.isEndInclusive() << " - OK" << std::endl;
    
    // Test auto-swap when begin > end
    std::cout << "Testing FromFloat(3.14f, 1.57f, true, false) - expected auto-swap: begin=1.57f, end=3.14f, begin_inclusive=false, end_inclusive=true" << std::endl;
    AngleRange range3 = AngleRange::FromFloat(3.14f, 1.57f, true, false);
    assert(range3.getBegin().getAsRadians() == 1.57f);
    assert(range3.getEnd().getAsRadians() == 3.14f);
    assert(range3.isBeginInclusive() == false);
    assert(range3.isEndInclusive() == true);
    std::cout << " Result: begin=" << range3.getBegin().getAsRadians() << ", end=" << range3.getEnd().getAsRadians() 
              << ", begin_inclusive=" << range3.isBeginInclusive() << ", end_inclusive=" << range3.isEndInclusive() << " - OK" << std::endl;
}

void angle_range_test_comparison_operators() {
    std::cout << "\n=== Testing Comparison Operators ===" << std::endl;
    
    std::cout << "Testing operator== with identical ranges [1.0, 2.0] - expected: true" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    AngleRange range2 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    assert(range1 == range2);
    std::cout << " Result: " << (range1 == range2) << " - OK" << std::endl;
    
    std::cout << "Testing operator== with different inclusivity (1.0, 2.0] vs [1.0, 2.0] - expected: false" << std::endl;
    AngleRange range3 = AngleRange::FromFloat(1.0f, 2.0f, false, true);
    AngleRange range4 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    assert(!(range3 == range4));
    std::cout << " Result: " << (range3 == range4) << " - OK" << std::endl;
    
    std::cout << "Testing operator> with [0.0, 3.0] > [1.0, 2.0] - expected: true" << std::endl;
    AngleRange range5 = AngleRange::FromFloat(0.0f, 3.0f, true, true);
    AngleRange range6 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    assert(range5 > range6);
    std::cout << " Result: " << (range5 > range6) << " - OK" << std::endl;
}

void test_in_method() {
    std::cout << "\n=== Testing in() Method ===" << std::endl;
    
    std::cout << "Testing in() with angle 1.5 in range [1.0, 2.0] - expected: true" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    Angle angle1 = Angle::fromRadians(1.5f);
    assert(range1.in(angle1) == true);
    std::cout << " Result: " << range1.in(angle1) << " - OK" << std::endl;
    
    std::cout << "Testing in() with angle 1.0 in range (1.0, 2.0] - expected: false" << std::endl;
    AngleRange range2 = AngleRange::FromFloat(1.0f, 2.0f, false, true);
    Angle angle2 = Angle::fromRadians(1.0f);
    assert(range2.in(angle2) == false);
    std::cout << " Result: " << range2.in(angle2) << " - OK" << std::endl;
    
    std::cout << "Testing in() with angle 2.0 in range [1.0, 2.0) - expected: false" << std::endl;
    AngleRange range3 = AngleRange::FromFloat(1.0f, 2.0f, true, false);
    Angle angle3 = Angle::fromRadians(2.0f);
    assert(range3.in(angle3) == false);
    std::cout << " Result: " << range3.in(angle3) << " - OK" << std::endl;
    
    std::cout << "Testing range containment [0.0, 3.0] contains [1.0, 2.0] - expected: true" << std::endl;
    AngleRange container = AngleRange::FromFloat(0.0f, 3.0f, true, true);
    AngleRange contained = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    assert(container.in(contained) == true);
    std::cout << " Result: " << container.in(contained) << " - OK" << std::endl;
}

void test_addition_operator() {
    std::cout << "\n=== Testing Addition Operator ===" << std::endl;
    
    std::cout << "Testing operator+ with overlapping ranges [1.0, 3.0] + [2.0, 4.0] - expected: single merged range [1.0, 4.0]" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(1.0f, 3.0f, true, true);
    AngleRange range2 = AngleRange::FromFloat(2.0f, 4.0f, true, true);
    std::vector<AngleRange> result1 = range1 + range2;
    assert(result1.size() == 1);
    assert(result1[0].getBegin().getAsRadians() == 1.0f);
    assert(result1[0].getEnd().getAsRadians() == 4.0f);
    std::cout << " Result: " << result1.size() << " range(s), first range: [" << result1[0].getBegin().getAsRadians() 
              << ", " << result1[0].getEnd().getAsRadians() << "] - OK" << std::endl;
    
    std::cout << "Testing operator+ with non-overlapping ranges [1.0, 2.0] + [3.0, 4.0] - expected: two separate ranges" << std::endl;
    AngleRange range3 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    AngleRange range4 = AngleRange::FromFloat(3.0f, 4.0f, true, true);
    std::vector<AngleRange> result2 = range3 + range4;
    assert(result2.size() == 2);
    std::cout << " Result: " << result2.size() << " ranges - OK" << std::endl;
}

void test_subtraction_operator() {
    std::cout << "\n=== Testing Subtraction Operator ===" << std::endl;
    
    std::cout << "Testing operator- with [1.0, 4.0] - [2.0, 3.0] - expected: two ranges [1.0, 2.0) and (3.0, 4.0]" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(1.0f, 4.0f, true, true);
    AngleRange range2 = AngleRange::FromFloat(2.0f, 3.0f, true, true);
    std::vector<AngleRange> result1 = range1 - range2;
    assert(result1.size() == 2);
    assert(result1[0].getBegin().getAsRadians() == 1.0f);
    assert(result1[0].getEnd().getAsRadians() == 2.0f);
    assert(result1[0].isEndInclusive() == false);
    assert(result1[1].getBegin().getAsRadians() == 3.0f);
    assert(result1[1].getEnd().getAsRadians() == 4.0f);
    assert(result1[1].isBeginInclusive() == false);
    std::cout << " Result: " << result1.size() << " ranges - OK" << std::endl;
    
    std::cout << "Testing operator- with non-overlapping ranges [1.0, 2.0] - [3.0, 4.0] - expected: original range [1.0, 2.0]" << std::endl;
    AngleRange range3 = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    AngleRange range4 = AngleRange::FromFloat(3.0f, 4.0f, true, true);
    std::vector<AngleRange> result2 = range3 - range4;
    assert(result2.size() == 1);
    assert(result2[0].getBegin().getAsRadians() == 1.0f);
    assert(result2[0].getEnd().getAsRadians() == 2.0f);
    std::cout << " Result: " << result2.size() << " range(s) - OK" << std::endl;
}

void test_vector_operations() {
    std::cout << "\n=== Testing Vector Operations ===" << std::endl;
    
    std::cout << "Testing operator+ with vector - adding range [1.0, 2.0] to empty vector - expected: vector size = 1" << std::endl;
    std::vector<AngleRange> vec;
    AngleRange range = AngleRange::FromFloat(1.0f, 2.0f, true, true);
    range + vec;
    assert(vec.size() == 1);
    std::cout << " Result: vector size = " << vec.size() << " - OK" << std::endl;
    
    std::cout << "Testing operator+= with vector - adding range [3.0, 4.0] to existing vector - expected: vector size = 2" << std::endl;
    AngleRange range2 = AngleRange::FromFloat(3.0f, 4.0f, true, true);
    vec += range2;
    assert(vec.size() == 2);
    std::cout << " Result: vector size = " << vec.size() << " - OK" << std::endl;
}

void angle_range_test_edge_cases() {
    std::cout << "\n=== Testing Edge Cases ===" << std::endl;
    
    std::cout << "Testing empty range subtraction [2.0, 3.0] - [1.0, 4.0] - expected: empty result" << std::endl;
    AngleRange range1 = AngleRange::FromFloat(2.0f, 3.0f, true, true);
    AngleRange range2 = AngleRange::FromFloat(1.0f, 4.0f, true, true);
    std::vector<AngleRange> result = range1 - range2;
    assert(result.size() == 0);
    std::cout << " Result: " << result.size() << " ranges - OK" << std::endl;
    
    std::cout << "Testing single point range [1.0, 1.0] - expected: length = 0" << std::endl;
    AngleRange point_range = AngleRange::FromFloat(1.0f, 1.0f, true, true);
    assert(point_range.len() == 0.0f);
    std::cout << " Result: length = " << point_range.len() << " - OK" << std::endl;
}