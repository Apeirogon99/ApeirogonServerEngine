#pragma once

#define MAX_MATRIX 4

class APEIROGON_API Matrix
{
public:
	Matrix();
	Matrix(const float inMatrix[MAX_MATRIX][MAX_MATRIX]);
	Matrix(const Matrix& inMatrix);
	Matrix& operator=(const Matrix& inMatrix);

public:
	Matrix	operator+(const Matrix& inMatrix) const;
	Matrix	operator-(const Matrix& inMatrix) const;
	FVector operator*(const FVector& inVector) const;
	Matrix	operator*(const Matrix& inMatrix) const;
	Matrix	operator/(const Matrix& inMatrix) const;

	Matrix	operator+(float inBias) const;
	Matrix	operator-(float inBias) const;
	Matrix	operator*(float inScale) const;
	Matrix	operator/(float inScale) const;

	bool operator==(const Matrix& inMatrix);

	void ToString();

	//friend std::ostream& operator<<(std::ostream& inOstream, const Matrix& inMatrix);

public:
	void SetMatrix(const Matrix& inMatrix);

	static Matrix Identity();
	static Matrix Scale(const float& inSX, const float& inSY, const float& inSZ);
	static Matrix Translate(const FVector& inVector);
	static Matrix RotateX(const float& inRX);
	static Matrix RotateY(const float& inRY);
	static Matrix RotateZ(const float& inRZ);
	static Matrix RotateRX(const float& inRX);
	static Matrix RotateRY(const float& inRY);
	static Matrix RotateRZ(const float& inRZ);
	static Matrix RotateMatrix(const FVector& inLocation, const FRotator& inRotator);
	static Matrix RotateMatrix(const FRotator& inRoator);
	static Matrix Inverse(const Matrix& inMatrix);
	static Matrix Transpose(const Matrix& inMatrix);

	static void GetAxis(const FRotator& inRotation, FVector& outX, FVector& outY, FVector& outZ);
	static FVector GetXAxis(const FRotator& inRotation);
	static FVector GetYAxis(const FRotator& inRotation);
	static FVector GetZAxis(const FRotator& inRotation);

public:

public:
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		float mMatrix[MAX_MATRIX][MAX_MATRIX];
	};
};

