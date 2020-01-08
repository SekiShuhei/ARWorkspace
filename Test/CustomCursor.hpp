#pragma once



//#include <siv3d.hpp>


class CustomCursor
{

public:
	CustomCursor();
	~CustomCursor();


	void	DrawAt(int pos_x, int pos_y);

private:

	int		pos_x;
	int		pos_y;

	int		size = 30;

};

