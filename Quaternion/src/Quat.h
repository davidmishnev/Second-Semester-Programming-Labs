#pragma once
#include <cmath>

class Quat
{
    double w_m;
    double xyz_m[3];

public:
    Quat();
    Quat(double a, double b, double c, double d) noexcept;
    Quat(double angle, bool is_radian, double v[3]) noexcept;

    Quat operator+(const Quat& other) noexcept;
    Quat operator-(const Quat& other) noexcept;
    Quat& operator+=(const Quat& other) noexcept;
    Quat& operator-=(const Quat& other) noexcept;
    Quat operator*(const Quat& other) noexcept;
    Quat& operator*=(const Quat& other) noexcept;

    Quat operator~() noexcept;

    bool operator==(const Quat& other) const noexcept;
    bool operator!=(const Quat& other) const noexcept;

    explicit operator double() const noexcept;

    const double* data() const noexcept;
    double** rotation_matrix() const noexcept;
    double** matrix() const noexcept;
};
