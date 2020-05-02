#pragma once

namespace ARWorkspace {

class ScreenRegion
{
public:
	ScreenRegion& operator = (const ScreenRegion& arg) 
	{
		this->x = arg.x;
		this->y = arg.y;
		this->w = arg.w;
		this->h = arg.h;
		return (*this);
	}

public:
	double GetX() const
	{
		return this->x;
	}
	double GetY() const
	{
		return this->y;
	}
	double GetWidth() const
	{
		return this->w;
	}
	double GetHeight() const
	{
		return this->h;
	}

	bool SetX(double arg_value)
	{
		this->x = arg_value;
		return true;
	}
	bool SetY(double arg_value)
	{
		this->y = arg_value;
		return true;
	}
	bool SetWidth(double arg_value)
	{
		if( arg_value < 1 || arg_value > 40000)
		{
			return false;
		}
		this->w = arg_value;
		return true;
	}
	bool SetHeight(double arg_value)
	{
		if (arg_value < 1 || arg_value > 40000)
		{
			return false;
		}
		this->h = arg_value;
		return true;
	}

private:
	double x = 0.0;
	double y = 0.0;
	double w = 200.0;
	double h = 200.0;

};

}
