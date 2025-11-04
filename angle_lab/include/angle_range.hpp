#pragma once
#include "angle.hpp"
#include <vector>




class AngleRange
{
private:
    Angle begin;
    Angle end;
    bool begin_inclusive;
    bool end_inclusive;
    explicit AngleRange(Angle begin, Angle end, bool begin_inclusive, bool end_inclusive) : begin(begin), end(end), 
    begin_inclusive(begin_inclusive), end_inclusive(end_inclusive) {
        if (this->begin > this->end) {
            std::swap(this->begin, this->end);
            std::swap(this->begin_inclusive, this->end_inclusive);
        }
        // if (!begin_inclusive) this->begin += Angle::epsilon;
        // if (!end_inclusive) this->end -= Angle::epsilon;
    };
public:
    static AngleRange FromFloat(float begin_radians, float end_radians, bool begin_inclusive, bool end_inclusive) {
        Angle begin = Angle::fromRadians(Angle::normalize_angle(begin_radians));
        Angle end = Angle::fromRadians(Angle::normalize_angle(end_radians));
        return AngleRange(begin, end, begin_inclusive, end_inclusive);
    }

    static AngleRange FromInt(int begin_radians, int end_radians, bool begin_inclusive, bool end_inclusive) {
        Angle begin = Angle::fromRadians(Angle::normalize_angle((float)begin_radians));
        Angle end = Angle::fromRadians(Angle::normalize_angle((float)end_radians));
        return AngleRange(begin, end, begin_inclusive, end_inclusive);
    }

    static AngleRange FromAngle(Angle begin, Angle end, bool begin_inclusive, bool end_inclusive) {
        begin = Angle::fromRadians(Angle::normalize_angle(begin.getAsRadians()));
        end = Angle::fromRadians(Angle::normalize_angle(end.getAsRadians()));
        return AngleRange(begin, end, begin_inclusive, end_inclusive);
    }

    bool operator==(const AngleRange& other) const {
        if (!(this->begin_inclusive == other.begin_inclusive) || !(this->end_inclusive == other.end_inclusive)) return false;
        return (this->begin == other.begin && this->end == other.end);
    }

    bool operator!=(const AngleRange& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const AngleRange& angle_range) {
        if (angle_range.begin_inclusive) os << "[";
        else os << "(";
        os << angle_range.begin << ";" << angle_range.end;
        if (angle_range.end_inclusive) os << "]";
        else os << ")";
        return os;
    }

    float len() const {
        return this->end.getAsRadians() - this->begin.getAsRadians();
    }

    bool operator>(const AngleRange& other) const {
        return this->len() > other.len();
    }

    bool operator<(const AngleRange& other) const {
        return this->len() < other.len();
    }

    bool operator>=(const AngleRange& other) const {
        return (*this == other) || (*this > other);
    }

    bool operator<=(const AngleRange& other) const {
        return (*this == other) || (*this < other);
    }

    bool in(const Angle& angle) const {
        Angle normalized_angle = Angle::fromRadians(Angle::normalize_angle(angle.getAsRadians()));
        
        // Учитываем тип границ
        bool after_begin = begin_inclusive ? (normalized_angle >= begin) : (normalized_angle > begin);
        bool before_end = end_inclusive ? (normalized_angle <= end) : (normalized_angle < end);
        
        return after_begin && before_end;
    }

    bool in(const AngleRange& other) const {
        // Проверяем, что другой диапазон полностью содержится в этом
        bool begin_contained = this->in(other.begin);
        bool end_contained = this->in(other.end);
        
        return begin_contained && end_contained;
    }

    std::vector<AngleRange> operator+(const AngleRange& other) {
        std::vector<AngleRange> result = {*this, other};
        return result;
    }

    void operator+(std::vector<AngleRange> vector_of_angle_range) {
        vector_of_angle_range.push_back(*this);
    }
};
