/*********************************************************************************
*FileName:        ColorTint.h
*Author:
*Version:         1.0
*Date:            2018/8/6
*Description:     ColorTint 应用类
**********************************************************************************/

#pragma once

#include "../Common/D3DApp.h"
#include "../Common/UploadBuffer.h"
#include "../Common/MathHelper.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

// 盒子的顶点数据结构
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	//XMCOLOR Color;
};

// Constant buffer 结构
struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	float GTime = 0.f;
};

class ColorTint : public D3DApp
{
public:
	ColorTint(HINSTANCE hInstance);
	ColorTint(const ColorTint& rhs) = delete;
	ColorTint& operator=(const ColorTint& rhs) = delete;
	~ColorTint();

	virtual bool Initialize() override;

protected:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

	void BuildDescriptorHeaps();						// 创建 Descriptor Heap
	void BuildConstantBuffers();						// 创建 Constant buffer
	void BuildRootSignature();							// 创建 root signature：用以绑定 shader 和要使用的资源
	void BuildShadersAndInputLayout();					// 创建 shader 相关
	void BuildBoxGeometry();							// 创建盒子物体
	void BuildPSO();									// 定义最终渲染管线

private:

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;		// 相当于 Shader 的函数签名
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;			// Constant buffer view Descriptor heap

	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;			// Constant buffer

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;			// 物体 Mesh Geometry 结构

	ComPtr<ID3DBlob> mvsByteCode = nullptr;						// vertex shader 代码
	ComPtr<ID3DBlob> mpsByteCode = nullptr;						// 

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;			// shader layout

	ComPtr<ID3D12PipelineState> mPSO = nullptr;					// pipeline

	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f*XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	POINT mLastMousePos;
};
