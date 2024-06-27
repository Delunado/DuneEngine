#include "MatrixMN.h"

#include <stdexcept>

MatrixMN::MatrixMN(): _rowsNumber(0), _columnsNumber(0), _rows(nullptr)
{
}

MatrixMN::MatrixMN(int rowsNumber, int columnsNumber): _rowsNumber(rowsNumber), _columnsNumber(columnsNumber)
{
    _rows = new VecN[_rowsNumber];

    for (int i = 0; i < _rowsNumber; i++)
    {
        _rows[i] = VecN(_columnsNumber);
    }
}

MatrixMN::MatrixMN(const MatrixMN& other)
{
    if (this == &other) return;

    *this = other;
}

MatrixMN::~MatrixMN()
{
    delete[] _rows;
}

void MatrixMN::Zero() const
{
    for (int i = 0; i < _rowsNumber; i++)
    {
        _rows[i].Zero();
    }
}

MatrixMN MatrixMN::Transpose() const
{
    MatrixMN result(_columnsNumber, _rowsNumber);

    for (int i = 0; i < _rowsNumber; i++)
    {
        for (int j = 0; j < _columnsNumber; j++)
        {
            result._rows[j][i] = _rows[i][j];
        }
    }

    return result;
}

VecN MatrixMN::SolveGaussSeidel(const MatrixMN& leftHandSide, const VecN& rightHandSide, int iterations)
{
    const int rows = leftHandSide._rowsNumber;
    VecN x(rows);
    x.Zero();

    for (int k = 0; k < iterations; k++)
    {
        for (int i = 0; i < rows; i++)
        {
            float deltaX = (rightHandSide[i] / leftHandSide[i][i]) - (leftHandSide[i].Dot(x) / leftHandSide[i][i]);
            if (deltaX == deltaX) // This is a check for NaN, preventing them
            {
                x[i] += deltaX;
            }
        }
    }

    return x;
}

const MatrixMN& MatrixMN::operator=(const MatrixMN& other)
{
    delete[] _rows;

    _rowsNumber = other._rowsNumber;
    _columnsNumber = other._columnsNumber;

    _rows = new VecN[_rowsNumber];
    for (int i = 0; i < _rowsNumber; i++)
    {
        _rows[i] = other._rows[i];
    }

    return *this;
}

VecN MatrixMN::operator*(const VecN& other) const
{
    if (other.Count() != _columnsNumber)
    {
        throw std::invalid_argument("Vector dimension is not compatible for multiplication");
    }

    VecN result(_rowsNumber);

    for (int i = 0; i < _rowsNumber; i++)
    {
        result[i] = other.Dot(_rows[i]);
    }

    return result;
}

MatrixMN MatrixMN::operator*(const MatrixMN& other) const
{
    if (_columnsNumber != other._rowsNumber && _rowsNumber != other._columnsNumber)
    {
        throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
    }

    MatrixMN transposed = other.Transpose();
    MatrixMN result(_rowsNumber, other._columnsNumber);

    for (int i = 0; i < _rowsNumber; i++)
    {
        for (int j = 0; j < other._columnsNumber; j++)
        {
            result._rows[i][j] = _rows[i].Dot(transposed._rows[j]);
        }
    }

    return result;
}

VecN& MatrixMN::operator[](const int index) const
{
    return _rows[index];
}
