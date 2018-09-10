/*********************************************************************************
*FileName:        d3dApp.h
*Author:          张尊庆
*Version:         1.0
*Date:            2018/8/1
*Description:     d3d 应用基类

D3D应用初始化流程：
1、使用 D3D12CreateDevice 函数创建 ID3D12Device 
2、创建 ID3D12Fence object 并确认 descriptor 大小
3、检查 4X MSAA quality level 支持
4、创建 command queue, command list allocator, and main command list
5、定义并创建 swap chain
6、创建应用需要的 descriptor heaps
7、重置 back buffer 大小，并为 back buffer 创建 render target view
8、创建 depth/stencil buffer 和 associated depth/stencil view
9、设置 viewport 和 裁剪框

**********************************************************************************/

#pragma once

// Debug模式下，启用内存检测工具
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "D3DUtil.h"
#include "GameTimer.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


class D3DApp
{
protected:

	D3DApp(HINSTANCE hInstance);

	// 禁止使用如下方式构造
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;

	virtual ~D3DApp();

public:

	static D3DApp* GetApp();							// 获取当前实例化的 D3DApp 的指针
	HINSTANCE AppInst()const;							// 获取当前传入的 HINSTANCE 引用
	HWND MainWnd()const;								// 获取窗口句柄
	float AspectRatio()const;							// 获取窗口宽高比

	// 设置/获取 多重纹理映射状态
	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);

	int Run();											// 进入主逻辑循环

	virtual bool Initialize();							// 初始化函数
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);				// 用户输入消息响应

protected:

	virtual void CreateRtvAndDsvDescriptorHeaps();		// 创建 descriptor heaps
	virtual void OnResize();							// 窗口大小改变时响应消息
	virtual void Update(const GameTimer& gt) = 0;		// 窗口更新
	virtual void Draw(const GameTimer& gt) = 0;			// 窗口绘制

	// 鼠标输入响应
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:

	bool InitMainWindow();								// 初始化游戏窗口
	bool InitDirect3D();								// 初始化D3D
	void CreateCommandObjects();						// 创建 command queue 和 command list
	void CreateSwapChain();								// 定义和创建 swap chain

	void FlushCommandQueue();							// 用以 CPU/GPU 同步

	ID3D12Resource* CurrentBackBuffer()const;						// 获取 back buffer
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;		// 获取当前 back buffer view
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;			// 获取当前 DSV

	void CalculateFrameStats();							// 计算帧速率和每帧消耗时间，结果会显示在窗口标题栏

	void LogAdapters();									// 打印所有 显卡/软件模拟 适配器
	void LogAdapterOutputs(IDXGIAdapter* adapter);		// 打印适配器输出
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);		// 打印显示模式

// 成员变量																					
protected:

	static D3DApp* mApp;								// 当前实例化的 D3DApp 引用（单例？）
	HINSTANCE mhAppInst = nullptr;						// Application 句柄
	HWND mhMainWnd = nullptr;							// 主窗口句柄

	bool mAppPaused = false;							// 当前应用是否被暂停
	bool mMinimized = false;							// 当前应用是否被最小化
	bool mMaximized = false;							// 当前应用是否被最大化
	bool mResizing = false;								// 当前应用是否在被重置大小（拖动边界）
	bool mFullscreenState = false;						// 是否可以全屏

	// 纹理映射相关参数
	bool m4xMsaaState = false;							// 4X MSAA enabled
	UINT m4xMsaaQuality = 0;							// quality level of 4X MSAA

	GameTimer mTimer;									// 用来计算时间

	// DXGI - DirectX Graphics Infrastructure: 多显卡时，一些通用的方法
	//用来创建 swap chain 和 用来遍历显示适配器
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;	

	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;	// 用以双 BUFFER 缓冲
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;	// D3D硬件设备

	// Fence：用以强制 CPU/GPU 同步
	// CPU一直等待到GPU执行到 fence, 才开始继续执行，用以防止数据冲突
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;			
	UINT64 mCurrentFence = 0;

	// command queue and command list
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;			// 使用双 buffer 缓冲
	int mCurrBackBuffer = 0;							// 当前 back buffer index

	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];		// back buffers
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;							// DSV buffer

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;		// RTV descriptor heap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;		// DSV descriptor heap

	// 窗口大小和裁切大小
	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	// Descriptor 大小（Descriptor大小在不同GPU之间不同，所以需要在初始化时确认）
	UINT mRtvDescriptorSize = 0;						// RTV：render target resources
	UINT mDsvDescriptorSize = 0;						// DSV: depth/stencil resources
	UINT mCbvSrvUavDescriptorSize = 0;					// DBV/SRV/UAV：constant buffers, shader resources, unloadered access view resources

	std::wstring mMainWndCaption = L"d3d App";			// 主窗口标题文字

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 游戏窗口初始宽高
	int mClientWidth = 800;
	int mClientHeight = 600;
};