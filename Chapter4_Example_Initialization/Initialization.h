/*********************************************************************************
*FileName:        Initialization.h
*Author:           
*Version:         1.0
*Date:            2018/8/1
*Description:     Initialization ”¶”√¿‡
**********************************************************************************/

#pragma once

#include "../Common/D3DApp.h"

class Initialization : public D3DApp
{
public:
	Initialization(HINSTANCE hInstance);
	Initialization(const Initialization& rhs) = delete;
	Initialization& operator=(const Initialization& rhs) = delete;
	~Initialization();

	virtual bool Initialize()override;

protected:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;
};


