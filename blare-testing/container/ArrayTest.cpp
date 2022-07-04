#include "container/Array.hxx"

#include <iostream>
#include <array>
#include <algorithm>

template<typename T> int arrayTest()
{
    std::array<T, 2048> first;
    blare::container::Array<T, 2048> second;

    if (first.size() != second.size())
        return 1;

    std::array<T, 2048> additionalFirst;
    blare::container::Array<T, 2048> additionalSecond;
    for (std::size_t i = 0; i < additionalFirst.size(); i++)
    {
        T t;
        additionalFirst.at(i) = t;
        additionalSecond.at(i) = t;
    }
    if (!std::equal(additionalFirst.begin(), additionalFirst.end(), additionalSecond.begin()))
        return 1;

    first.swap(additionalFirst);
    second.swap(additionalSecond);
    if (!std::equal(first.begin(), first.end(), second.begin()))
        return 1;

    additionalSecond = second;
    if (additionalSecond != second)
        return 1;

    std::cout << "Reached the end of ArrayTest. Success!\n";
    return 0;
}

struct Animal
{
    Animal() : age(rand()) {}
    Animal(const std::string& name) : name(name), age(rand()) {}

    constexpr inline bool operator==(const Animal& other) const { return age == other.age; }

    std::string name = "unknown";
    uint32_t age;
};

std::ostream& operator<<(std::ostream& stream, const Animal& animal)
{
    stream << animal.name << ", " << animal.age;
    return stream;
}

int main()
{
    std::cout << "Starting ArrayTest...\n";
    std::cout << "Starting ArrayTest... - Done\n";
    return arrayTest<Animal>();
}