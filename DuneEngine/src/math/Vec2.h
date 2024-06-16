#pragma once

struct Vec2
{
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);

    ~Vec2() = default;

    void Add(const Vec2& v);
    void Sub(const Vec2& v);
    void Scale(float n);
    Vec2 Rotate(float angle) const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    void Normalize();
    Vec2 Normal() const;
    Vec2 NormalPerpendicular() const;

    float Dot(const Vec2& v) const;
    float Cross(const Vec2& v) const;

    // Operators
    Vec2& operator = (const Vec2& v);
    bool operator == (const Vec2& v) const;
    bool operator != (const Vec2& v) const;

    Vec2 operator + (const Vec2& v) const;
    Vec2 operator - (const Vec2& v) const;
    Vec2 operator * (float n) const;
    Vec2 operator / (float n) const;
    Vec2 operator - () const; // Invert

    Vec2& operator += (const Vec2& v);
    Vec2& operator -= (const Vec2& v);
    Vec2& operator *= (float n);
    Vec2& operator /= (float n);
};
