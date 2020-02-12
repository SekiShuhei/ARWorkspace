#pragma once

namespace ARWorkspace {

class SimpleCounter
{
public:
	SimpleCounter(int counter_max) : _counter_max(counter_max)
	{
		
	}

	bool Count()
	{
		_counter += 1;
		if (_counter > _counter_max)
		{
			_counter = 0;
			return true;
		}
		return false;
	}

	void Reset()
	{
		_counter = 0;
	}

private:
	int _counter = 0l;
	int _counter_max = 1;
};

}