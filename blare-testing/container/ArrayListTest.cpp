#include "container/ArrayList.h"

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class Entity
{
public:
	Entity() : m_UUID("uuid"), m_Hash(rand()) {};
	Entity& operator=(Entity&& other) 
	{
		m_Hash = std::move(other.m_Hash);
		m_UUID = std::move(other.m_UUID);
		return *this; 
	}
	Entity& operator=(const Entity& other) 
	{ 
		this->m_Hash = other.m_Hash;
		this->m_UUID = other.m_UUID;
		return *this; 
	}
	Entity(const Entity& other) : m_Hash(other.m_Hash), m_UUID(other.m_UUID) {}
	Entity(Entity&& other) : m_Hash(other.m_Hash), m_UUID(std::move(other.m_UUID)) {}
	~Entity() = default;

	friend std::ostream& operator<<(std::ostream& stream, const Entity& entity);
	bool operator==(const Entity& other) const { return this->m_Hash == other.m_Hash; }
	bool operator!=(const Entity& other) const {return !(*this == other); }

	std::string m_UUID;
	size_t m_Hash;
};


std::ostream& operator<<(std::ostream& stream, const Entity& entity)
{
	stream << entity.m_Hash;
	return stream;
}

template<typename T> int testContainer()
{
	blare::ArrayList<T> arraylist;
	std::vector<T> vector;

	for (size_t i = 0; i < 1'000; i++)
	{
		size_t randomPushBack = rand() % 10'000 + 10'000;
		for (size_t j = 0; j < randomPushBack; j++)
		{
			// size_t randomNum = rand() % 20'000;
			Entity entity;
			arraylist.pushBack(entity);
			vector.push_back(entity);
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
		T& arraylistFront = arraylist.front();
		T& arraylistBack = arraylist.back();
		size_t arraylistSize = arraylist.size();
		size_t arraylistCapacity = arraylist.capacity();

		T& vectorlistFront = vector.front();
		T& vectorlistBack = vector.back();
		size_t vectorlistSize = vector.size();
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

	blare::ArrayList<Entity> otherlist = arraylist;
	if (!std::equal(arraylist.begin(), arraylist.end(), otherlist.begin(), otherlist.end()))
		return 1;

	std::cout << "End of test\n";
	return 0;
}

int main()
{
	return testContainer<Entity>();
}