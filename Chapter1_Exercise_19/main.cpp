#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

// Overload the "<<" operators so that we can use cout to
// output XMVECTOR objects.
ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

int main()
{
	cout.setf(ios_base::boolalpha);

	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	XMVECTOR p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
	XMVECTOR q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
	XMVECTOR w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);

	cout << "XMVectorAbs(v) = " << XMVectorAbs(v) << endl;			// 对每一个分量求绝对值
	cout << "XMVectorCos(w) = " << XMVectorCos(w) << endl;			// 对每一个分量求arccosine值
	cout << "XMVectorLog(u) = " << XMVectorLog(u) << endl;			// 对每一个分量求log2的值
	cout << "XMVectorExp(p) = " << XMVectorExp(p) << endl;			// 对每一个分量乘2的值
	cout << "XMVectorPow(u, p) = " << XMVectorPow(u, p) << endl;	// 对u的每一个分量做p对应分量的平方
	cout << "XMVectorSqrt(u) = " << XMVectorSqrt(u) << endl;		// 对每一个分量开平方
	cout << "XMVectorSwizzle(u, 2, 2, 1, 3) = " << XMVectorSwizzle(u, 2, 2, 1, 3) << endl;	// XMVectorSwizzle 没看懂 - -！
	cout << "XMVectorSwizzle(u, 2, 1, 0, 3) = " << XMVectorSwizzle(u, 2, 1, 0, 3) << endl;	// XMVectorSwizzle 没看懂 - -！
	cout << "XMVectorMultiply(u, v) = " << XMVectorMultiply(u, v) << endl;	// 对每一个对应分量相乘
	cout << "XMVectorSaturate(q) = " << XMVectorSaturate(q) << endl;	// 把每一个分量修改为0~1的值
	cout << "XMVectorMin(p, v = " << XMVectorMin(p, v) << endl;			// 对每一个分量求 最小/最大 的值
	cout << "XMVectorMax(p, v) = " << XMVectorMax(p, v) << endl;		// 对每一个分量求 最小/最大 的值

	system("pause");

	return 0;
}


