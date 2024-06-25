#pragma once

struct VecN
{
public:
    VecN();
    VecN(int N);
    VecN(const VecN& other);
    ~VecN();

    void Zero() const;
    float Dot(const VecN& other) const;
    int Count() const;

    VecN& operator =(const VecN& other);
    VecN operator +(const VecN& other) const;
    VecN operator -(const VecN& other) const;
    VecN operator *(const float scalar) const;
    const VecN& operator +=(const VecN& other);
    const VecN& operator -=(const VecN& other);
    const VecN& operator *=(const float scalar);
    float operator [](const int index) const;
    float& operator[](const int index);

private:
    int _N;
    float* _data;
};
