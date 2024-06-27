#pragma once

#include "VecN.h"

struct MatrixMN
{
public:
    MatrixMN();
    MatrixMN(int rowsNumber, int columnsNumber);
    MatrixMN(const MatrixMN& other);
    ~MatrixMN();

    void Zero() const;
    MatrixMN Transpose() const;

    static VecN SolveGaussSeidel(const MatrixMN& leftHandSide, const VecN& rightHandSide, int iterations);

    const MatrixMN& operator =(const MatrixMN& other);
    VecN operator *(const VecN& other) const;
    MatrixMN operator *(const MatrixMN& other) const;
    VecN& operator [](const int index) const;

private:
    int _rowsNumber;
    int _columnsNumber;

    VecN* _rows;
};
