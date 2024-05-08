#pragma once

class IKartListener
{
public:
	virtual void OnKartPositionChanged(float x, float y, float z) = 0;

protected:
	~IKartListener() = default;
};