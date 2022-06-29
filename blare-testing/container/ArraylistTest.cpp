#include "container/Arraylist.h"

#include <vector>
#include <iostream>

int main()
{
	blare::ArrayList<int> arraylist;
	std::vector<int> vector;

	for (size_t i = 0; i < 1'000; i++)
	{
		size_t randomPushBack = rand() % 10'000 + 10'000;
		for (size_t j = 0; j < randomPushBack; j++)
		{
			size_t randomNum = rand() % 20'000;
			arraylist.pushBack(randomNum);
			vector.push_back(randomNum);
		}
		size_t randomPopBack = rand() % 10'000 + 10'000;
		for (size_t j = 0; j < randomPopBack; j++)
		{
			if (!arraylist.isEmpty()) arraylist.popBack();
			if (!vector.empty()) vector.pop_back();
		}
	}

	if (arraylist.size() != 0)
	{
		int& arraylistFront = arraylist.front();
		int& arraylistBack = arraylist.back();
		int arraylistSize = arraylist.size();
		int arraylistCapacity = arraylist.capacity();

		int& vectorlistFront = vector.front();
		int& vectorlistBack = vector.back();
		int vectorlistSize = vector.size();
		if (arraylist.front() != vector.front() or arraylist.back() != vector.back()
			or arraylist.size() != vector.size())
		{
			std::cout << arraylistFront << " and " << vectorlistFront << std::endl;
			std::cout << arraylistBack << " and " << vectorlistBack << std::endl;
			std::cout << arraylistSize << " and " << vectorlistSize << std::endl;
			return 1;
		}
	}

	vector.shrink_to_fit();
	arraylist.compress();
	if (arraylist.size() != vector.size())
		return 1;

	return 0;
}