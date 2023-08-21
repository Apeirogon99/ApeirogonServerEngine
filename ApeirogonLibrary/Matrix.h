#pragma once

#define MAX_MATRIX 3

class APEIROGON_API Matrix
{
public:
	Matrix();
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

	bool operator==(const FVector& inMatrix);

	friend std::ostream& operator<<(std::ostream& inOstream, const Matrix& inMatrix);

public:
	void SetMatrix(const Matrix& inMatrix);

	static Matrix Identity();
	static Matrix Scale(const float& inSX, const float& inSY, const float& inSZ);
	static Matrix RotatePitch(const float& inRX);
	static Matrix RotateYaw(const float& inRY);
	static Matrix RotateRoll(const float& inRZ);
	static Matrix Inverse(const Matrix& inMatrix);

public:
	

private:
	union
	{
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};

		float mMatrix[MAX_MATRIX][MAX_MATRIX];
	};
};

