#include "quat.h"

Quat::Quat() : w_m(0), xyz_m{ 0, 0, 0 } {}

Quat::Quat(double a, double b, double c, double d) noexcept : w_m(a), xyz_m{ b, c, d } {}

Quat::Quat(double angle, bool is_radian, double v[3]) noexcept
{
    if (!is_radian)
    {
        angle = angle * M_PI / 180.0;
    }

    double length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    v[0] /= length;
    v[1] /= length;
    v[2] /= length;

    angle /= 2.0;
    w_m = cos(angle);
    double c = sin(angle);
    xyz_m[0] = c * v[0];
    xyz_m[1] = c * v[1];
    xyz_m[2] = c * v[2];
}

Quat Quat::operator+(const Quat &other) noexcept
{
    return { this->w_m + other.w_m,
             this->xyz_m[0] + other.xyz_m[0],
             this->xyz_m[1] + other.xyz_m[1],
             this->xyz_m[2] + other.xyz_m[2] };
}

Quat Quat::operator-(const Quat &other) noexcept
{
    return { this->w_m - other.w_m,
             this->xyz_m[0] - other.xyz_m[0],
             this->xyz_m[1] - other.xyz_m[1],
             this->xyz_m[2] - other.xyz_m[2] };
}

Quat &Quat::operator+=(const Quat &other) noexcept
{
    *this = *this + other;
    return *this;
}

Quat &Quat::operator-=(const Quat &other) noexcept
{
    *this = *this - other;
    return *this;
}

Quat Quat::operator*(const Quat &other) noexcept
{
    return {
            this->w_m * other.w_m - this->xyz_m[0] * other.xyz_m[0] - this->xyz_m[1] * other.xyz_m[1] -
            this->xyz_m[2] * other.xyz_m[2],
            this->w_m * other.xyz_m[0] + other.w_m * this->xyz_m[0] + this->xyz_m[1] * other.xyz_m[2] -
            this->xyz_m[2] * other.xyz_m[1],
            this->w_m * other.xyz_m[1] + other.w_m * this->xyz_m[1] + this->xyz_m[2] * other.xyz_m[0] -
            this->xyz_m[0] * other.xyz_m[2],
            this->w_m * other.xyz_m[2] + other.w_m * this->xyz_m[2] + this->xyz_m[0] * other.xyz_m[1] -
            this->xyz_m[1] * other.xyz_m[0]
    };
}

Quat &Quat::operator*=(const Quat &other) noexcept
{
    *this = *this * other;
    return *this;
}

Quat Quat::operator~() noexcept
{
    this->xyz_m[0] = -xyz_m[0];
    this->xyz_m[1] = -xyz_m[1];
    this->xyz_m[2] = -xyz_m[2];
    return *this;
}

bool Quat::operator==(const Quat &other) const noexcept
{
    return (this->w_m == other.w_m && this->xyz_m[0] == other.xyz_m[0] && this->xyz_m[1] == other.xyz_m[1] &&
            this->xyz_m[2] == other.xyz_m[2]);
}

bool Quat::operator!=(const Quat &other) const noexcept
{
    return !(this->w_m == other.w_m && this->xyz_m[0] == other.xyz_m[0] && this->xyz_m[1] == other.xyz_m[1] &&
             this->xyz_m[2] == other.xyz_m[2]);
}

Quat::operator double() const noexcept
{
    return sqrt(this->w_m * this->w_m + this->xyz_m[0] * this->xyz_m[0] + this->xyz_m[1] * this->xyz_m[1] +
                this->xyz_m[2] * this->xyz_m[2]);
}

const double *Quat::data() const noexcept
{
    auto *returnData = new double[4];
    returnData[0] = this->xyz_m[0];
    returnData[1] = this->xyz_m[1];
    returnData[2] = this->xyz_m[2];
    returnData[3] = this->w_m;
    return returnData;
}

double **Quat::rotation_matrix() const noexcept
{
    // matrix init
    auto **rotationMatrix = new double *[3];
    rotationMatrix[0] = new double[3 * 3];
    rotationMatrix[1] = rotationMatrix[0] + 3;
    rotationMatrix[2] = rotationMatrix[1] + 3;

    // rotation formula
    rotationMatrix[0][0] = 1.0 - 2.0 * (pow(this->xyz_m[1], 2.0) + pow(this->xyz_m[2], 2.0));
    rotationMatrix[0][1] = 2.0 * (this->xyz_m[0] * this->xyz_m[1] + this->w_m * this->xyz_m[2]);
    rotationMatrix[0][2] = 2.0 * (this->xyz_m[0] * this->xyz_m[2] - this->w_m * this->xyz_m[1]);

    rotationMatrix[1][0] = 2.0 * (this->xyz_m[0] * this->xyz_m[1] - this->w_m * this->xyz_m[2]);
    rotationMatrix[1][1] = 1 - 2 * (pow(this->xyz_m[0], 2.0) + pow(this->xyz_m[2], 2.0));
    rotationMatrix[1][2] = 2.0 * (this->xyz_m[1] * this->xyz_m[2] + this->w_m * this->xyz_m[0]);

    rotationMatrix[2][0] = 2.0 * (this->xyz_m[0] * this->xyz_m[2] + this->w_m * this->xyz_m[1]);
    rotationMatrix[2][1] = 2.0 * (this->xyz_m[1] * this->xyz_m[2] - this->w_m * this->xyz_m[0]);
    rotationMatrix[2][2] = 1 - 2 * (pow(this->xyz_m[0], 2.0) + pow(this->xyz_m[1], 2.0));

    return rotationMatrix;
}

double **Quat::matrix() const noexcept
{
    auto **matrix = new double *[4];
    matrix[0] = new double[4 * 4];
    matrix[1] = matrix[0] + 4;
    matrix[2] = matrix[1] + 4;
    matrix[3] = matrix[2] + 4;

    matrix[0][0] = this->w_m;
    matrix[0][1] = this->xyz_m[0] * -1.0;
    matrix[0][2] = this->xyz_m[1] * -1.0;
    matrix[0][3] = this->xyz_m[2] * -1.0;

    matrix[1][0] = this->xyz_m[0];
    matrix[1][1] = this->w_m;
    matrix[1][2] = this->xyz_m[2] * -1.0;
    matrix[1][3] = this->xyz_m[1];

    matrix[2][0] = this->xyz_m[1];
    matrix[2][1] = this->xyz_m[2];
    matrix[2][2] = this->w_m;
    matrix[2][3] = this->xyz_m[0] * -1.0;

    matrix[3][0] = this->xyz_m[2];
    matrix[3][1] = this->xyz_m[1] * -1.0;
    matrix[3][2] = this->xyz_m[0];
    matrix[3][3] = this->w_m;

    return matrix;
}
