//#ifdef __GNUC__
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
//#endif

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

void
DuffsDevice(int* to, int* from, int count)
{
    int x = (count + 7) / 8;
    switch (count % 8)
    {
    case 0: do
    {
        *to++ = *from++ + 1;
        [[fallthrough]];
    case 7:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 6:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 5:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 4:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 3:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 2:      *to++ = *from++ + 1;
        [[fallthrough]];
    case 1:      *to++ = *from++ + 1;
    } while (--x > 0);
    }
}

void
NormalCopy(int* to, int* from, int count)
{
    std::transform(from, from + count, to, [](int value) { return value + 1; });
}

void
MemcpyCopy(int* to, int* from, int count)
{
    int* temp = new int[count];
    std::memcpy(temp, from, count * sizeof(int));
    for (int i = 0; i < count; ++i)
    {
        to[i] = temp[i] + 1;
    }

    delete[] temp;
}

int main()
{
    const int size = 1000000;
    const int repeats = 100;
    std::vector<int> source(size, 1);
    std::vector<int> destination(size);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        DuffsDevice(destination.data(), source.data(), size);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> difference = end - start;
    std::cout << "Duff's device time: " << difference.count() << "s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        NormalCopy(destination.data(), source.data(), size);
    }

    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    std::cout << "std::transform time: " << difference.count() << "s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        MemcpyCopy(destination.data(), source.data(), size);
    }

    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    std::cout << "memcpy time: " << difference.count() << "s" << std::endl;

    return 0;
}
