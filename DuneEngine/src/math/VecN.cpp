#include "VecN.h"

#include <iostream>

VecN::VecN(): _N(0), _data(nullptr)
{
}

VecN::VecN(const int N): _N(N)
{
    _data = new float[_N];
}

VecN::VecN(const VecN& other)
{
    _N = other._N;
    _data = new float[_N];

    for (int i = 0; i < _N; i++)
    {
        _data[i] = other._data[i];
    }
}

VecN::~VecN()
{
    delete[] _data;
}

void VecN::Zero() const
{
    for (int i = 0; i < _N; i++)
        _data[i] = 0.0f;
}

float VecN::Dot(const VecN& other) const
{
    if (_N != other._N)
    {
        std::cout << "Error: VecN::Dot() - Vectors must be the same size" << '\n';
        return 0.0f;
    }

    float result = 0.0f;

    for (int i = 0; i < _N; i++)
        result += _data[i] * other._data[i];

    return result;
}

VecN& VecN::operator=(const VecN& other)
{
    if (this == &other) return *this;

    delete[] _data;

    _N = other._N;
    _data = new float[_N];

    for (int i = 0; i < _N; i++)
    {
        _data[i] = other._data[i];
    }

    return *this;
}

VecN VecN::operator+(const VecN& other) const
{
    VecN result = *this;
    result += other;
    return result;
}

VecN VecN::operator-(const VecN& other) const
{
    VecN result = *this;
    result -= other;
    return result;
}

VecN VecN::operator*(const float scalar) const
{
    VecN result = *this;
    result *= scalar;
    return result;
}

const VecN& VecN::operator+=(const VecN& other)
{
    for (int i = 0; i < _N; i++)
    {
        _data[i] += other._data[i];
    }

    return *this;
}

const VecN& VecN::operator-=(const VecN& other)
{
    for (int i = 0; i < _N; i++)
    {
        _data[i] -= other._data[i];
    }

    return *this;
}

const VecN& VecN::operator*=(const float scalar)
{
    for (int i = 0; i < _N; i++)
    {
        _data[i] *= scalar;
    }

    return *this;
}

float VecN::operator[](int index) const
{
    if (index < 0 || index >= _N)
    {
        std::cout << "Error: VecN::operator[] - Index out of bounds" << '\n';
        return 0.0f;
    }

    return _data[index];
}

float& VecN::operator[](const int index)
{
    if (index < 0 || index >= _N)
    {
        std::cout << "Error: VecN::operator[] - Index out of bounds" << '\n';
        return _data[0];
    }

    return _data[index];
}
