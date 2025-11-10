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

    // Проверка пересечения диапозонов
    bool intersects(const AngleRange& other) const {
        return !(this->end < other.begin || other.end < this->begin) ||
               (this->end == other.begin && this->end_inclusive && other.begin_inclusive) ||
               (other.end == this->begin && other.end_inclusive && this->begin_inclusive);
    }

    // Проверка соприкосновения диапозонов
    bool touches(const AngleRange& other) const {
        return (this->end == other.begin && (this->end_inclusive || other.begin_inclusive)) ||
               (other.end == this->begin && (other.end_inclusive || this->begin_inclusive));
    }

public:
    explicit AngleRange(Angle begin, Angle end, bool begin_inclusive, bool end_inclusive) : begin(begin), end(end), 
    begin_inclusive(begin_inclusive), end_inclusive(end_inclusive) {
        if (this->begin > this->end) {
            std::swap(this->begin, this->end);
            std::swap(this->begin_inclusive, this->end_inclusive);
        }
    };
    
    static AngleRange FromFloat(float begin_radians, float end_radians, bool begin_inclusive, bool end_inclusive) {
        Angle begin = Angle::fromRadians(begin_radians);
        Angle end = Angle::fromRadians(end_radians);
        return AngleRange(begin, end, begin_inclusive, end_inclusive);
    }

    static AngleRange FromInt(int begin_radians, int end_radians, bool begin_inclusive, bool end_inclusive) {
        Angle begin = Angle::fromRadians(begin_radians);
        Angle end = Angle::fromRadians(end_radians);
        return AngleRange(begin, end, begin_inclusive, end_inclusive);
    }

    static AngleRange FromAngle(Angle begin, Angle end, bool begin_inclusive, bool end_inclusive) {
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

    Angle getBegin() const { return begin; }
    Angle getEnd() const { return end; }
    bool isBeginInclusive() const { return begin_inclusive; }
    bool isEndInclusive() const { return end_inclusive; }

    bool in(const Angle& angle) const {
        // Учитываем тип границ
        bool after_begin = begin_inclusive ? (angle >= begin) : (angle > begin);
        bool before_end = end_inclusive ? (angle <= end) : (angle < end);
        
        return after_begin && before_end;
    }

    bool in(const AngleRange& other) const {
        // Проверяем, что другой диапазон полностью содержится в этом
        bool begin_contained = this->in(other.begin);
        bool end_contained = this->in(other.end);
        
        return begin_contained && end_contained;
    }

    std::vector<AngleRange> operator+(const AngleRange& other) const {
        std::vector<AngleRange> result;
        
        // Если диапазоны пересекаются или соприкасаются - объединяем
        if (this->intersects(other) || this->touches(other)) {
            Angle new_begin = std::min(this->begin, other.begin);
            Angle new_end = std::max(this->end, other.end);
            
            // Определяем включения для границ
            bool new_begin_inclusive = (new_begin == this->begin && this->begin_inclusive) || 
                                    (new_begin == other.begin && other.begin_inclusive);
            bool new_end_inclusive = (new_end == this->end && this->end_inclusive) || 
                                    (new_end == other.end && other.end_inclusive);
            
            result.push_back(AngleRange(new_begin, new_end, new_begin_inclusive, new_end_inclusive));
        } else {
            // Если не пересекаются - возвращаем оба
            result.push_back(*this);
            result.push_back(other);
        }
        
        return result;
    }

    void operator+(std::vector<AngleRange>& vector_of_angle_range) const {
        vector_of_angle_range.push_back(*this);
    }

    // Оператор += для добавления к вектору диапазонов
    friend void operator+=(std::vector<AngleRange>& vector, const AngleRange& range) {
        vector.push_back(range);
    }

    std::vector<AngleRange> operator-(const AngleRange& other) const {
        std::vector<AngleRange> result;
        
        if (!this->intersects(other)) {
            result.push_back(*this);
            return result;
        }
        
        // Левая часть
        if (other.begin > this->begin) {
            AngleRange left_part(this->begin, other.begin, 
                            this->begin_inclusive, !other.begin_inclusive);
            // Проверяем, что диапазон не пустой
            if (left_part.begin < left_part.end || 
                (left_part.begin == left_part.end && left_part.begin_inclusive && left_part.end_inclusive)) {
                result.push_back(left_part);
            }
        }
        
        // Правая часть
        if (other.end < this->end) {
            AngleRange right_part(other.end, this->end, 
                                !other.end_inclusive, this->end_inclusive);
            // Проверяем, что диапазон не пустой
            if (right_part.begin < right_part.end || 
                (right_part.begin == right_part.end && right_part.begin_inclusive && right_part.end_inclusive)) {
                result.push_back(right_part);
            }
        }
        
        return result;
    }
};
