#include "pch.h"
#include "Matrix.h"

Matrix::Matrix()
{
    this->SetMatrix(Matrix::Identity());
}

Matrix::Matrix(const Matrix& inMatrix)
{
    this->SetMatrix(inMatrix);
}

Matrix& Matrix::operator=(const Matrix& inMatrix)
{
    this->SetMatrix(inMatrix);
    return *this;
}

FVector Matrix::operator*(const FVector& inVector) const
{
    FVector vector;
    vector.SetX(mMatrix[0][0] * inVector.GetX() + mMatrix[0][1] * inVector.GetX() + mMatrix[0][2] * inVector.GetX());
    vector.SetY(mMatrix[1][0] * inVector.GetY() + mMatrix[1][1] * inVector.GetY() + mMatrix[1][2] * inVector.GetY());
    vector.SetZ(mMatrix[2][0] * inVector.GetZ() + mMatrix[2][1] * inVector.GetZ() + mMatrix[2][2] * inVector.GetZ());
    return vector;
}

Matrix Matrix::operator*(const Matrix& inVector) const
{
    Matrix matrix;
    for (int32 indexY = 0; indexY < MAX_MATRIX; ++indexY)
    {
        for (int32 indexX = 0; indexX < MAX_MATRIX; ++indexX)
        {
            matrix.mMatrix[indexY][indexX] += this->mMatrix[indexY][indexX] * inVector.mMatrix[indexX][indexY];
        }
    }
    return matrix;
}

void Matrix::SetMatrix(const Matrix& inMatrix)
{
    this->m00 = inMatrix.m00;      this->m01 = inMatrix.m01;        this->m02 = inMatrix.m02;
    this->m10 = inMatrix.m10;      this->m11 = inMatrix.m11;        this->m12 = inMatrix.m12;
    this->m20 = inMatrix.m20;      this->m21 = inMatrix.m21;        this->m22 = inMatrix.m22;
}

Matrix Matrix::Identity()
{
    Matrix matrix;
    matrix.m00 = 1.0f;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = 1.0f;              matrix.m12 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = 1.0f;
    return matrix;
}

Matrix Matrix::Scale(const float& inSX, const float& inSY, const float& inSZ)
{
    Matrix matrix;
    matrix.m00 = inSX;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = inSY;              matrix.m12 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = inSZ;
    return matrix;
}

Matrix Matrix::RotatePitch(const float& inRX)
{
    Matrix matrix;
    matrix.m00 = 1.0f;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = std::cosf(inRX);   matrix.m12 = -std::sinf(inRX);
    matrix.m20 = 0.0f;              matrix.m21 = std::sinf(inRX);   matrix.m22 = std::cosf(inRX);
    return matrix;
}

Matrix Matrix::RotateYaw(const float& inRY)
{
    Matrix matrix;
    matrix.m00 = std::cosf(inRY);   matrix.m01 = 0.0f;              matrix.m02 = std::sinf(inRY);
    matrix.m10 = 0.0f;              matrix.m11 = 1.0f;              matrix.m12 = 0.0f;
    matrix.m20 = -std::sinf(inRY);  matrix.m21 = 0.0f;              matrix.m22 = std::cosf(inRY);
    return matrix;
}

Matrix Matrix::RotateRoll(const float& inRZ)
{
    Matrix matrix;
    matrix.m00 = std::cosf(inRZ);   matrix.m01 = -std::sinf(inRZ);  matrix.m02 = 0.0f;
    matrix.m10 = std::sinf(inRZ);   matrix.m11 = std::cosf(inRZ);   matrix.m12 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = 1.0f;
    return matrix;
}

Matrix Matrix::Inverse(const Matrix& inMatrix)
{
    float det = 0.0f;
    det = 
        inMatrix.mMatrix[0][0] * (inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][2] - inMatrix.mMatrix[2][1] * inMatrix.mMatrix[1][2]) - inMatrix.mMatrix[0][1] * (inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][2] - inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][0]) + inMatrix.mMatrix[0][2] * (inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][1] - inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][0]);

    if (det == 0.0f)
    {
        return inMatrix;
    }

    Matrix invMatrix;
    det = 1.0f / det;
    invMatrix.mMatrix[0][0] = (invMatrix.mMatrix[1][1] * invMatrix.mMatrix[2][2] - invMatrix.mMatrix[2][1] * invMatrix.mMatrix[1][2]) * det;
    invMatrix.mMatrix[0][1] = (invMatrix.mMatrix[0][2] * invMatrix.mMatrix[2][1] - invMatrix.mMatrix[0][1] * invMatrix.mMatrix[2][2]) * det;
    invMatrix.mMatrix[0][2] = (invMatrix.mMatrix[0][1] * invMatrix.mMatrix[1][2] - invMatrix.mMatrix[0][2] * invMatrix.mMatrix[1][1]) * det;

    invMatrix.mMatrix[1][0] = (invMatrix.mMatrix[1][2] * invMatrix.mMatrix[2][0] - invMatrix.mMatrix[1][0] * invMatrix.mMatrix[2][2]) * det;
    invMatrix.mMatrix[1][1] = (invMatrix.mMatrix[0][0] * invMatrix.mMatrix[2][2] - invMatrix.mMatrix[0][2] * invMatrix.mMatrix[2][0]) * det;
    invMatrix.mMatrix[1][2] = (invMatrix.mMatrix[1][0] * invMatrix.mMatrix[0][2] - invMatrix.mMatrix[0][0] * invMatrix.mMatrix[1][2]) * det;

    invMatrix.mMatrix[2][0] = (invMatrix.mMatrix[1][0] * invMatrix.mMatrix[2][1] - invMatrix.mMatrix[2][0] * invMatrix.mMatrix[1][1]) * det;
    invMatrix.mMatrix[2][1] = (invMatrix.mMatrix[2][0] * invMatrix.mMatrix[0][1] - invMatrix.mMatrix[0][0] * invMatrix.mMatrix[2][1]) * det;
    invMatrix.mMatrix[2][2] = (invMatrix.mMatrix[0][0] * invMatrix.mMatrix[1][1] - invMatrix.mMatrix[1][0] * invMatrix.mMatrix[0][1]) * det;
    return invMatrix;
}
