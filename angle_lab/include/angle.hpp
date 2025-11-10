#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>



class Angle {
private:
    float angle_radian_val;

    explicit Angle(float radians) : angle_radian_val(radians) {};
public:
    static constexpr float Pi = 3.1415f;
    static constexpr float epsilon = 0.0001f;
    
    static Angle fromDegrees(float degrees) {
        return Angle(degrees * Pi/180);
    }

    static Angle fromRadians(float radians) {
        return Angle(radians);
    }

    Angle(const Angle& other) : angle_radian_val(other.angle_radian_val) {};

    float getAsDegrees() const {
        return angle_radian_val * 180.0 / Pi;
    }

    float getAsRadians() const {
        return angle_radian_val;
    }

    void printAsDegrees() const {
        std::cout << angle_radian_val * 180.0 / Pi << " degrees" << std::endl;
    }

    void printAsRadians() const {
        std::cout << angle_radian_val / Pi << " pi" << std::endl;
    }

    static float normalize_angle(float radians) {
        float result = std::fmod(radians, 2.0f * Pi);
        if (result < 0.0f) {
            result += 2.0f * Pi;
        }
        
        if (std::fabs(result - 2.0f * Pi) < epsilon) {
            result = 0.0f;
        }
        
        return result;
    }

    bool is_eq_with_2pi_mod(const Angle& other) const {
        return std::fabs(normalize_angle(this->angle_radian_val) - normalize_angle(other.angle_radian_val)) < epsilon; 
    }

    bool operator==(const Angle& other) const {
        return std::fabs((this->angle_radian_val) - (other.angle_radian_val)) < epsilon; 
    }

    bool operator<(const Angle& other) const {
        return !(*this == other) && this->angle_radian_val < other.angle_radian_val;
    }

    bool operator!=(const Angle& other) const {
        return !(*this == other);
    }

    bool operator>(const Angle& other) const {
        return !(*this == other) && this->angle_radian_val > other.angle_radian_val;
    }

    bool operator>=(const Angle& other) const {
        return (*this == other) || (*this > other);
    }

    bool operator<=(const Angle& other) const {
        return (*this == other) || (*this < other);
    }

    std::string to_string() const {
        return std::to_string(this->angle_radian_val);
    }

    int to_int() const {
        return (int)this->angle_radian_val;
    }

    float to_float() const {
        return this->angle_radian_val;
    }

    Angle operator+(const Angle& other) const {
        return Angle(this->angle_radian_val + other.angle_radian_val); 
    }

    Angle& operator+=(const Angle& other) {
        this->angle_radian_val += other.angle_radian_val;
        return *this;
    }
    Angle operator-(const Angle& other) const {
        return Angle(this->angle_radian_val - other.angle_radian_val); 
    }

    Angle& operator-=(const Angle& other) {
        this->angle_radian_val -= other.angle_radian_val;
        return *this;
    }

    Angle operator*(const float scalar) const {
        return Angle(this->angle_radian_val * scalar);
    }

    Angle& operator*=(const float scalar) {
        this->angle_radian_val *= scalar;
        return *this;
    }

    Angle operator/(const float devider) const {
        if (devider == 0.0f) {
            throw std::invalid_argument("Division by zero");
        }
        return Angle(this->angle_radian_val / devider);
    }

    Angle& operator/=(const float devider) {
        if (devider == 0.0f) {
            throw std::invalid_argument("Division by zero");
        }
        this->angle_radian_val /= devider;
        return *this;
    }

    Angle operator+(float radians) {
        return Angle(this->angle_radian_val + radians);
    }

    Angle& operator+=(float radians) {
        this->angle_radian_val += radians;
        return *this;
    }

    Angle operator-(float radians) {
        return Angle(this->angle_radian_val - radians);
    }

    Angle& operator-=(float radians) {
        this->angle_radian_val -= radians;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Angle& angle) {
        os << angle.angle_radian_val / Pi << " pi";
        return os;
    }
};