#pragma once
class Display
{
public:
	Display() {};
	virtual void swap() {};
	virtual void prepareForNewFrame() {};
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual ~Display() {};
};

