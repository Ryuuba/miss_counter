#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <chrono>
#include <array>

enum MissType {
    CACHE = 0,
    MAINMEM,
    VIRTUALMEM
};

int main(int argc, char const *argv[])
{
    using std::cout, std::cerr, std::endl, std::fstream, 
          std::ifstream, std::chrono::high_resolution_clock, 
          std::chrono::nanoseconds, std::array;
    char const* path = argv[1];
    cout << "Starting to reading file " << path << endl;
    ifstream file(path, fstream::in);
    array<uint64_t, 3> counter {0, 0, 0};
    if (file.is_open())
    {
        while (!file.eof())
        {
            auto start = high_resolution_clock::now();
            file.get();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            if (duration.count() < 25)
                counter[CACHE]++;
            else if(duration.count() < 10000)
                counter[MAINMEM]++;
            else
                counter[VIRTUALMEM]++;
        }
        file.close();
    }
    else
    {
        cerr << "File " << path << " is not found" << endl;
        exit(1);
    }
    uint64_t total = counter[CACHE] + counter[MAINMEM] + counter[VIRTUALMEM];
    cout << "Results " << '\n'
         << "Read characters: " << total << '\n'
         << "Cache access number = " << counter[CACHE] << '\n'
         << "Main Memory access number = " << counter[MAINMEM] << '\n'
         << "Virtual Memory access number = " << counter[VIRTUALMEM] << endl;
    return 0;
}
