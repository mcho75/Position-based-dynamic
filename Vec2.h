#ifndef POSITION_BASED_DYNAMIC_VEC2_H
#define POSITION_BASED_DYNAMIC_VEC2_H


class Vec2 {

public:
    Vec2(double x, double y) {
        _components[0] = x;
        _components[1] = y;
    }
    double operator[](int i) const {
        return _components[i];
    }

private:
    double _components[2] = {0.0, 0.0};
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) {
    return {a[0] + b[0], a[1] + b[1]};
}

inline Vec2 operator-(const Vec2& a, const Vec2& b) {
    return {a[0] - b[0], a[1] - b[1]};
}

inline Vec2 operator*(const Vec2& a, const Vec2& b) {
    return {a[0] * b[0], a[1] * b[1]};
}

inline Vec2 operator*(const Vec2& a, double b) {
    return {a[0] * b, a[1] * b};
}

inline Vec2 operator*(double b, const Vec2& a) {
    return {a[0] * b, a[1] * b};
}


#endif //POSITION_BASED_DYNAMIC_VEC2_H