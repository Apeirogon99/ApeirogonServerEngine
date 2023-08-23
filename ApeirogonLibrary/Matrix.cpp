#include "pch.h"
#include "Matrix.h"

Matrix::Matrix()
{
    float mat[4][4] =
    {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    std::copy(&mat[0][0], &mat[0][0] + MAX_MATRIX * MAX_MATRIX, &mMatrix[0][0]);
}

Matrix::Matrix(const float inMatrix[MAX_MATRIX][MAX_MATRIX])
{
    std::copy(&inMatrix[0][0], &inMatrix[0][0] + MAX_MATRIX * MAX_MATRIX, &mMatrix[0][0]);
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

Matrix Matrix::operator+(const Matrix& inMatrix) const
{
    Matrix matrix;
    matrix.m00 = m00 + inMatrix.m00;    matrix.m01 = m01 + inMatrix.m01;   matrix.m02 = m02 + inMatrix.m02;  matrix.m03 = m03 + inMatrix.m03;
    matrix.m10 = m10 + inMatrix.m10;    matrix.m11 = m11 + inMatrix.m11;   matrix.m12 = m12 + inMatrix.m12;  matrix.m13 = m13 + inMatrix.m13;
    matrix.m20 = m20 + inMatrix.m20;    matrix.m21 = m21 + inMatrix.m21;   matrix.m22 = m22 + inMatrix.m22;  matrix.m23 = m23 + inMatrix.m23;
    matrix.m30 = m30 + inMatrix.m30;    matrix.m31 = m31 + inMatrix.m31;   matrix.m32 = m32 + inMatrix.m32;  matrix.m33 = m33 + inMatrix.m33;
    return matrix;
}

Matrix Matrix::operator-(const Matrix& inMatrix) const
{
    Matrix matrix;
    matrix.m00 = m00 - inMatrix.m00;    matrix.m01 = m01 - inMatrix.m01;   matrix.m02 = m02 - inMatrix.m02;  matrix.m03 = m03 - inMatrix.m03;
    matrix.m10 = m10 - inMatrix.m10;    matrix.m11 = m11 - inMatrix.m11;   matrix.m12 = m12 - inMatrix.m12;  matrix.m13 = m13 - inMatrix.m13;
    matrix.m20 = m20 - inMatrix.m20;    matrix.m21 = m21 - inMatrix.m21;   matrix.m22 = m22 - inMatrix.m22;  matrix.m23 = m23 - inMatrix.m23;
    matrix.m30 = m30 - inMatrix.m30;    matrix.m31 = m31 - inMatrix.m31;   matrix.m32 = m32 - inMatrix.m32;  matrix.m33 = m33 - inMatrix.m33;
    return matrix;
}

FVector Matrix::operator*(const FVector& inVector) const
{
    float resultX = mMatrix[0][0] * inVector.GetX() + mMatrix[0][1] * inVector.GetY() + mMatrix[0][2] * inVector.GetZ();
    float resultY = mMatrix[1][0] * inVector.GetX() + mMatrix[1][1] * inVector.GetY() + mMatrix[1][2] * inVector.GetZ();
    float resultZ = mMatrix[2][0] * inVector.GetX() + mMatrix[2][1] * inVector.GetY() + mMatrix[2][2] * inVector.GetZ();

    FVector vector(resultX, resultY, resultZ);
    return vector;
}

Matrix Matrix::operator*(const Matrix& inMatrix) const
{
    Matrix matrix;
    matrix.m00 = (this->m00 * inMatrix.m00) + (this->m01 * inMatrix.m10) + (this->m02 * inMatrix.m20) + (this->m03 * inMatrix.m30);
    matrix.m01 = (this->m00 * inMatrix.m01) + (this->m01 * inMatrix.m11) + (this->m02 * inMatrix.m21) + (this->m03 * inMatrix.m31);
    matrix.m02 = (this->m00 * inMatrix.m02) + (this->m01 * inMatrix.m12) + (this->m02 * inMatrix.m22) + (this->m03 * inMatrix.m32);
    matrix.m03 = (this->m00 * inMatrix.m03) + (this->m01 * inMatrix.m13) + (this->m02 * inMatrix.m23) + (this->m03 * inMatrix.m33);
                                          
    matrix.m10 = (this->m10 * inMatrix.m00) + (this->m11 * inMatrix.m10) + (this->m12 * inMatrix.m20) + (this->m13 * inMatrix.m30);
    matrix.m11 = (this->m10 * inMatrix.m01) + (this->m11 * inMatrix.m11) + (this->m12 * inMatrix.m21) + (this->m13 * inMatrix.m31);
    matrix.m12 = (this->m10 * inMatrix.m02) + (this->m11 * inMatrix.m12) + (this->m12 * inMatrix.m22) + (this->m13 * inMatrix.m32);
    matrix.m13 = (this->m10 * inMatrix.m03) + (this->m11 * inMatrix.m13) + (this->m12 * inMatrix.m23) + (this->m13 * inMatrix.m33);
                                          
    matrix.m20 = (this->m20 * inMatrix.m00) + (this->m21 * inMatrix.m10) + (this->m22 * inMatrix.m20) + (this->m23 * inMatrix.m30);
    matrix.m21 = (this->m20 * inMatrix.m01) + (this->m21 * inMatrix.m11) + (this->m22 * inMatrix.m21) + (this->m23 * inMatrix.m31);
    matrix.m22 = (this->m20 * inMatrix.m02) + (this->m21 * inMatrix.m12) + (this->m22 * inMatrix.m22) + (this->m23 * inMatrix.m32);
    matrix.m23 = (this->m20 * inMatrix.m03) + (this->m21 * inMatrix.m13) + (this->m22 * inMatrix.m23) + (this->m23 * inMatrix.m33);
                                          
    matrix.m30 = (this->m30 * inMatrix.m00) + (this->m31 * inMatrix.m10) + (this->m32 * inMatrix.m20) + (this->m33 * inMatrix.m30);
    matrix.m31 = (this->m30 * inMatrix.m01) + (this->m31 * inMatrix.m11) + (this->m32 * inMatrix.m21) + (this->m33 * inMatrix.m31);
    matrix.m32 = (this->m30 * inMatrix.m02) + (this->m31 * inMatrix.m12) + (this->m32 * inMatrix.m22) + (this->m33 * inMatrix.m32);
    matrix.m33 = (this->m30 * inMatrix.m03) + (this->m31 * inMatrix.m13) + (this->m32 * inMatrix.m23) + (this->m33 * inMatrix.m33);
    return matrix;
}

Matrix Matrix::operator/(const Matrix& inMatrix) const
{
    return Matrix();
}

Matrix Matrix::operator+(float inBias) const
{
    return Matrix();
}

Matrix Matrix::operator-(float inBias) const
{
    return Matrix();
}

Matrix Matrix::operator*(float inScale) const
{
    Matrix matrix;
    matrix.m00 = m00 * inScale;    matrix.m01 = m01 * inScale;   matrix.m02 = m02 * inScale;  matrix.m03 = m03 - inScale;
    matrix.m10 = m10 * inScale;    matrix.m11 = m11 * inScale;   matrix.m12 = m12 * inScale;  matrix.m13 = m13 - inScale;
    matrix.m20 = m20 * inScale;    matrix.m21 = m21 * inScale;   matrix.m22 = m22 * inScale;  matrix.m23 = m23 - inScale;
    matrix.m30 = m30 * inScale;    matrix.m31 = m31 * inScale;   matrix.m32 = m32 * inScale;  matrix.m33 = m33 - inScale;
    return matrix;
}

Matrix Matrix::operator/(float inScale) const
{
    return Matrix();
}

bool Matrix::operator==(const Matrix& inMatrix)
{
    return true;
}

void Matrix::ToString()
{
    printf("{\n");
    for (int32 i = 0; i < MAX_MATRIX; ++i)
    {
        printf("   ");
        for (int32 j = 0; j < MAX_MATRIX; ++j)
        {
            printf("%+.3f   ", mMatrix[i][j]);
        }
        printf("\n");
    }
    printf("}\n");
}

void Matrix::SetMatrix(const Matrix& inMatrix)
{
    this->m00 = inMatrix.m00;      this->m01 = inMatrix.m01;        this->m02 = inMatrix.m02;       this->m03 = inMatrix.m03;
    this->m10 = inMatrix.m10;      this->m11 = inMatrix.m11;        this->m12 = inMatrix.m12;       this->m13 = inMatrix.m13;
    this->m20 = inMatrix.m20;      this->m21 = inMatrix.m21;        this->m22 = inMatrix.m22;       this->m23 = inMatrix.m23;
    this->m30 = inMatrix.m30;      this->m31 = inMatrix.m31;        this->m32 = inMatrix.m32;       this->m33 = inMatrix.m33;
}

Matrix Matrix::Identity()
{
    Matrix matrix;
    matrix.m00 = 1.0f;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;              matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = 1.0f;              matrix.m12 = 0.0f;              matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = 1.0f;              matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::Scale(const float& inSX, const float& inSY, const float& inSZ)
{
    Matrix matrix;
    matrix.m00 = inSX;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;              matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = inSY;              matrix.m12 = 0.0f;              matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = inSZ;              matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::Translate(const FVector& inVector)
{
    Matrix matrix;
    matrix.m00 = 1.0f;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;              matrix.m03 = inVector.GetX();
    matrix.m10 = 0.0f;              matrix.m11 = 1.0f;              matrix.m12 = 0.0f;              matrix.m13 = inVector.GetY();
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = 1.0f;              matrix.m23 = inVector.GetZ();
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::RotateX(const float& inRX)
{
    Matrix matrix;
    matrix.m00 = 1.0f;              matrix.m01 = 0.0f;              matrix.m02 = 0.0f;              matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = std::cosf(inRX);   matrix.m12 = -std::sinf(inRX);  matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = std::sinf(inRX);   matrix.m22 = std::cosf(inRX);   matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::RotateY(const float& inRY)
{
    Matrix matrix;
    matrix.m00 = std::cosf(inRY);   matrix.m01 = 0.0f;              matrix.m02 = std::sinf(inRY);   matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;              matrix.m11 = 1.0f;              matrix.m12 = 0.0f;              matrix.m13 = 0.0f;
    matrix.m20 = -std::sinf(inRY);  matrix.m21 = 0.0f;              matrix.m22 = std::cosf(inRY);   matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::RotateZ(const float& inRZ)
{
    Matrix matrix;
    matrix.m00 = std::cosf(inRZ);   matrix.m01 = -std::sinf(inRZ);  matrix.m02 = 0.0f;              matrix.m03 = 0.0f;
    matrix.m10 = std::sinf(inRZ);   matrix.m11 = std::cosf(inRZ);   matrix.m12 = 0.0f;              matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;              matrix.m21 = 0.0f;              matrix.m22 = 1.0f;              matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;              matrix.m31 = 0.0f;              matrix.m32 = 0.0f;              matrix.m33 = 1.0f;
    return matrix;
}

Matrix Matrix::RotateMatrix(const FVector& inLocation, const FRotator& inRotator)
{
    Matrix matrix;

    matrix = Matrix::Scale(1.0f, 1.0f, 1.0f);

    Matrix Yawmatrix    = Matrix::RotateZ(inRotator.GetYaw());
    Matrix Pitchmatrix  = Matrix::RotateY(inRotator.GetPitch());
    Matrix Rollmatrix   = Matrix::RotateX(inRotator.GetRoll());
    matrix = Yawmatrix * Pitchmatrix * Rollmatrix;

    Matrix translate = Matrix::Translate(inLocation);
    matrix = matrix * translate;

    //printf("\nRotate + Translate MATRIX\n"); matrix.ToString();

    return matrix;
}

Matrix Matrix::RotateMatrix(const FRotator& inRoator)
{
    float pitch = inRoator.GetPitch() * PI / 180.0f;
    float yaw = inRoator.GetYaw() * PI / 180.0f;
    float roll = inRoator.GetRoll() * PI / 180.0f;

    // Calculate sine and cosine values
    float SP = std::sinf(pitch);
    float CP = std::cosf(pitch);
    float SY = std::sinf(yaw);
    float CY = std::cosf(yaw);
    float SR = std::sinf(roll);
    float CR = std::cosf(roll);

    Matrix matrix;
    matrix.mMatrix[0][0] = CP * CY;
    matrix.mMatrix[0][1] = CP * SY;
    matrix.mMatrix[0][2] = SP;
    matrix.mMatrix[0][3] = 0.f;

    matrix.mMatrix[1][0] = SR * SP * CY - CR * SY;
    matrix.mMatrix[1][1] = SR * SP * SY + CR * CY;
    matrix.mMatrix[1][2] = -SR * CP;
    matrix.mMatrix[1][3] = 0.f;

    matrix.mMatrix[2][0] = -(CR * SP * CY + SR * SY);
    matrix.mMatrix[2][1] = CY * SR - CR * SP * SY;
    matrix.mMatrix[2][2] = CR * CP;
    matrix.mMatrix[2][3] = 0.f;

    matrix.mMatrix[3][0] = 0.0f;
    matrix.mMatrix[3][1] = 0.0f;
    matrix.mMatrix[3][2] = 0.0f;
    matrix.mMatrix[3][3] = 1.f;
    return matrix;
}

Matrix Matrix::Inverse(const Matrix& inMatrix)
{
    float det = 0.0f;
    det =   inMatrix.m00 * inMatrix.m11 * inMatrix.m22 +
            inMatrix.m01 * inMatrix.m12 * inMatrix.m20 +
            inMatrix.m02 * inMatrix.m10 * inMatrix.m21 -
            inMatrix.m00 * inMatrix.m12 * inMatrix.m21 -
            inMatrix.m01 * inMatrix.m10 * inMatrix.m22 -
            inMatrix.m02 * inMatrix.m11 * inMatrix.m20;


    if (det == 0.0f)
    {
        return inMatrix;
    }

    Matrix invMatrix;
    det = 1.0f / det;
    invMatrix.mMatrix[0][0] = (inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][2] - inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][1]) * det;
    invMatrix.mMatrix[0][1] = (inMatrix.mMatrix[0][2] * inMatrix.mMatrix[2][1] - inMatrix.mMatrix[0][1] * inMatrix.mMatrix[2][2]) * det;
    invMatrix.mMatrix[0][2] = (inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][2] - inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][1]) * det;
    invMatrix.mMatrix[0][3] =   (
                                    inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][2] +
                                    inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][3] +
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][1] -
                                    inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][3] -
                                    inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][1] -
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][2]
                                ) * det;

    invMatrix.mMatrix[1][0] = (inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][0] - inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][2]) * det;
    invMatrix.mMatrix[1][1] = (inMatrix.mMatrix[0][0] * inMatrix.mMatrix[2][2] - inMatrix.mMatrix[0][2] * inMatrix.mMatrix[2][0]) * det;
    invMatrix.mMatrix[1][2] = (inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][0] - inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][2]) * det;
    invMatrix.mMatrix[1][3] =   (
                                    inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][3] +
                                    inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][0] +
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][2] -
                                    inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][2] -
                                    inMatrix.mMatrix[0][2] * inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][3] -
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][2] * inMatrix.mMatrix[2][0]
                                ) * det;

    invMatrix.mMatrix[2][0] = (inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][1] - inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][0]) * det;
    invMatrix.mMatrix[2][1] = (inMatrix.mMatrix[0][1] * inMatrix.mMatrix[2][0] - inMatrix.mMatrix[0][0] * inMatrix.mMatrix[2][1]) * det;
    invMatrix.mMatrix[2][2] = (inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][1] - inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][0]) * det;
    invMatrix.mMatrix[2][3] =   (
                                    inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][1] +
                                    inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][3] +
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][0] -
                                    inMatrix.mMatrix[0][0] * inMatrix.mMatrix[1][1] * inMatrix.mMatrix[2][3] -
                                    inMatrix.mMatrix[0][1] * inMatrix.mMatrix[1][3] * inMatrix.mMatrix[2][0] -
                                    inMatrix.mMatrix[0][3] * inMatrix.mMatrix[1][0] * inMatrix.mMatrix[2][1]
                                ) * det;

    invMatrix.mMatrix[3][0] = 0.0f;
    invMatrix.mMatrix[3][1] = 0.0f;
    invMatrix.mMatrix[3][2] = 0.0f;
    invMatrix.mMatrix[3][3] = 1.0f;
    return invMatrix;
}

Matrix Matrix::Transpose(const Matrix& inMatrix)
{
    Matrix matrix;
    matrix.m00 = inMatrix.m00;      matrix.m01 = inMatrix.m01;      matrix.m02 = inMatrix.m02;      matrix.m03 = inMatrix.m03;
    matrix.m10 = inMatrix.m10;      matrix.m11 = inMatrix.m11;      matrix.m12 = inMatrix.m12;      matrix.m13 = inMatrix.m13;
    matrix.m20 = inMatrix.m20;      matrix.m21 = inMatrix.m21;      matrix.m22 = inMatrix.m22;      matrix.m23 = inMatrix.m23;
    matrix.m30 = inMatrix.m30;      matrix.m31 = inMatrix.m31;      matrix.m32 = inMatrix.m32;      matrix.m33 = inMatrix.m33;
    return matrix;
}

void Matrix::GetAxis(const FRotator& inRotation, FVector& outX, FVector& outY, FVector& outZ)
{
    Matrix matrix = Matrix::RotateMatrix(inRotation);

    outX.SetX(matrix.mMatrix[0][0]); outX.SetY(matrix.mMatrix[0][1]); outX.SetZ(matrix.mMatrix[0][2]);
    outY.SetX(matrix.mMatrix[1][0]); outY.SetY(matrix.mMatrix[1][1]); outY.SetZ(matrix.mMatrix[1][2]);
    outZ.SetX(matrix.mMatrix[2][0]); outZ.SetY(matrix.mMatrix[2][1]); outZ.SetZ(matrix.mMatrix[2][2]);

}