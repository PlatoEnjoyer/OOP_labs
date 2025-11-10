#include <iostream>
#include <cmath>
#include <string>
#include "angle.hpp"
#include "test_angle.hpp"
#include "test_angle_range.hpp"


int main() {
    std::cout << "Starting Angle Class Tests...\n" << std::endl;
    
    test_creation_and_conversion_methods();
    test_normalization_functionality();
    test_comparison_operators();
    test_isEquivalent_method();
    test_arithmetic_operations();
    test_compound_assignment_operators();
    test_float_operators();
    test_edge_cases();
    test_conversion_methods();
    test_division_by_zero();
    test_output_operations();
    
    std::cout << "\n=== All Angle Tests Completed Successfully! ===" << std::endl << std::endl;

    std::cout << "Starting tests for AngleRange class..." << std::endl;
    std::cout << "=======================================" << std::endl;
    
    std::cout << "Starting AngleRange Tests...\n" << std::endl;
    test_creation_methods();
    angle_range_test_comparison_operators();
    test_in_method();
    test_addition_operator();
    test_subtraction_operator();
    test_vector_operations();
    angle_range_test_edge_cases();
    
    std::cout << "\n=== All Tests Passed! ===" << std::endl;
}