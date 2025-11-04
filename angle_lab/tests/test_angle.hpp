#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include <stdexcept>
#include "angle.hpp"

using namespace std;

// Тестовые функции
void testCreationAndConversion() {
    cout << "=== Creation and Transformation test ===" << endl;
    
    // Тест создания из градусов
    Angle a1 = Angle::fromDegrees(180.0f);
    assert(fabs(a1.getAsDegrees() - 180.0f) < 0.001f);
    assert(fabs(a1.getAsRadians() - Angle::Pi) < 0.001f);
    cout << " 180 degrees = " << a1.getAsRadians() << " radians" << endl;
    
    // Тест создания из радиан
    Angle a2 = Angle::fromRadians(Angle::Pi / 2);
    assert(fabs(a2.getAsDegrees() - 90.0f) < 0.001f);
    cout << " pi/2 radians = " << a2.getAsDegrees() << " degrees" << endl;
    
    // Тест копирования
    Angle a3 = a1;
    assert(a3 == a1);
    cout << "Copying is working correctly" << endl;
    
    // Тест методов вывода
    cout << " printAsDegrees(): ";
    a1.printAsDegrees();
    
    cout << " printAsRadians(): ";
    a1.printAsRadians();
}

void testNormalization() {
    cout << "\n=== Normalization test ===" << endl;
    
    // Углы больше 2pi
    Angle a1 = Angle::fromRadians(3 * Angle::Pi);
    Angle a2 = Angle::fromRadians(Angle::Pi);
    assert(a1 == a2);
    cout << " 3pi is normalized to pi" << endl;
    
    // Отрицательные углы
    Angle a3 = Angle::fromRadians(-Angle::Pi / 2);
    Angle a4 = Angle::fromRadians(3 * Angle::Pi / 2);
    assert(a3 == a4);
    cout << " -pi/2 is normalized to 3pi/2 " << endl;
    
    // Углы кратные 2pi
    Angle a5 = Angle::fromRadians(4 * Angle::Pi);
    Angle a6 = Angle::fromRadians(0.0f);
    assert(a5 == a6);
    cout << "4pi is normalized to 0" << endl;
    
    // Граничные значения
    Angle a7 = Angle::fromRadians(2 * Angle::Pi);
    Angle a8 = Angle::fromRadians(0.0f);
    assert(a7 == a8);
    cout << " 2pi is normalized to 0 " << endl;
}

void testComparison() {
    cout << "\n=== Comparison test ===" << endl;
    
    Angle a1 = Angle::fromDegrees(45.0f);
    Angle a2 = Angle::fromDegrees(45.0f);
    Angle a3 = Angle::fromDegrees(90.0f);
    Angle a4 = Angle::fromDegrees(30.0f);
    
    // Тест равенства
    assert(a1 == a2);
    assert(!(a1 == a3));
    cout << " The == operator is working correctly " << endl;
    
    // Тест неравенства
    assert(a1 != a3);
    assert(!(a1 != a2));
    cout << " The operator!= works correctly " << endl;
    
    // Тест меньше
    assert(a4 < a1);
    assert(a1 < a3);
    assert(!(a3 < a1));
    cout << " The < operator is working correctly " << endl;
    
    // Тест больше
    assert(a3 > a1);
    assert(a1 > a4);
    assert(!(a4 > a1));
    cout << " Operator > is working correctly " << endl;
    
    // Тест меньше или равно
    assert(a1 <= a2);
    assert(a1 <= a3);
    assert(!(a3 <= a1));
    cout << " The <= operator is working correctly " << endl;
    
    // Тест больше или равно
    assert(a1 >= a2);
    assert(a3 >= a1);
    assert(!(a4 >= a1));
    cout << " The operator >= is working correctly " << endl;
}

void testArithmetic() {
    cout << "\n=== Arithmetic operations test ===" << endl;
    
    Angle a1 = Angle::fromDegrees(30.0f);
    Angle a2 = Angle::fromDegrees(60.0f);
    
    // Сложение
    Angle sum = a1 + a2;
    assert(fabs(sum.getAsDegrees() - 90.0f) < 0.001f);
    cout << " 30 + 60 = " << sum.getAsDegrees() << "" << endl;
    
    // Вычитание
    Angle diff = a2 - a1;
    assert(fabs(diff.getAsDegrees() - 30.0f) < 0.001f);
    cout << " 60 - 30 = " << diff.getAsDegrees() << "" << endl;
    
    // Умножение на скаляр
    Angle multiplied = a1 * 2.0f;
    assert(fabs(multiplied.getAsDegrees() - 60.0f) < 0.001f);
    cout << " 30 * 2 = " << multiplied.getAsDegrees() << "" << endl;
    
    // Деление на скаляр
    Angle divided = a2 / 2.0f;
    assert(fabs(divided.getAsDegrees() - 30.0f) < 0.001f);
    cout << " 60 / 2 = " << divided.getAsDegrees() << "" << endl;
    
    // Составные операторы
    Angle a3 = Angle::fromDegrees(10.0f);
    a3 += a1;
    assert(fabs(a3.getAsDegrees() - 40.0f) < 0.001f);
    cout << " The += operator is working correctly " << endl;
    
    Angle a4 = Angle::fromDegrees(50.0f);
    a4 -= a1;
    assert(fabs(a4.getAsDegrees() - 20.0f) < 0.001f);
    cout << " The -= operator is working correctly " << endl;
    
    Angle a5 = Angle::fromDegrees(15.0f);
    a5 *= 3.0f;
    assert(fabs(a5.getAsDegrees() - 45.0f) < 0.001f);
    cout << " The *= operator is working correctly " << endl;
    
    Angle a6 = Angle::fromDegrees(90.0f);
    a6 /= 3.0f;
    assert(fabs(a6.getAsDegrees() - 30.0f) < 0.001f);
    cout << " The /= operator is working correctly " << endl;
}

void testEdgeCases() {
    cout << "\n=== Boundary case test ===" << endl;
    
    // Нулевой угол
    Angle zero = Angle::fromDegrees(0.0f);
    Angle zero2 = Angle::fromRadians(0.0f);
    assert(zero == zero2);
    cout << " The zero angle is working correctly " << endl;
    
    // Полный круг
    Angle full1 = Angle::fromDegrees(360.0f);
    Angle full2 = Angle::fromDegrees(0.0f);
    assert(full1 == full2);
    cout << " 360 is normalized to 0 " << endl;
    
    // Большие углы
    Angle big = Angle::fromDegrees(720.0f);
    assert(big == zero);
    cout << " 720 is normalized to 0 " << endl;
    
    // Очень большие углы
    Angle very_big = Angle::fromDegrees(1080.0f);
    assert(very_big == zero);
    cout << " 1080 is normalized to 0 " << endl;
    
    // Очень маленькие углы
    Angle small = Angle::fromDegrees(-360.0f);
    assert(small == zero);
    cout << " -360 is normalized to 0 " << endl;
}

void testConversionMethods() {
    cout << "\n=== Test of conversion methods ===" << endl;
    
    Angle a1 = Angle::fromDegrees(45.0f);
    
    // to_string
    string str = a1.to_string();
    cout << " to_string(): " << str << endl;
    
    // to_int
    int int_val = a1.to_int();
    cout << " to_int(): " << int_val << endl;
    
    // to_float
    float float_val = a1.to_float();
    cout << " to_float(): " << float_val << endl;
}

void testDivisionByZero() {
    cout << "\n=== The division by zero test ===" << endl;
    
    Angle a1 = Angle::fromDegrees(90.0f);
    bool caught_exception = false;
    
    try {
        Angle result = a1 / 0.0f;
    } catch (const invalid_argument& e) {
        caught_exception = true;
        cout << " Exception caught: " << e.what() << endl;
    }
    
    assert(caught_exception);
    
    caught_exception = false;
    try {
        a1 /= 0.0f;
    } catch (const invalid_argument& e) {
        caught_exception = true;
        cout << " Exception caught (operator /=): " << e.what() << endl;
    }
    
    assert(caught_exception);
}

void testOutputStream() {
    cout << "\n=== The Output operator test ===" << endl;
    
    Angle a1 = Angle::fromRadians(Angle::Pi);
    cout << " Operator <<: " << a1 << endl;
}