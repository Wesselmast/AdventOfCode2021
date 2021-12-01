#pragma once

// Includes
#include <vector>
#include <fstream>
#include <iterator>



/**
@brief Class with utility functions for this years' AOC!
**/
class AOCUtility
{
public:
    /**
    @brief Parse the puzzle input into an array of T
    **/
    template<typename T>
    static bool sParseInputFile(std::vector<T>& outNumbers)
    {
        // Try to open the puzzle input file
        const char* filename = "input.txt";
        std::ifstream stream(filename, std::ifstream::in);
        if (!stream)
        {
            printf("File %s was not found\n", filename);
            return false;
        }

        // Copy elements from the file that can be implicitly casted to type T
        std::copy(std::istream_iterator<T>(stream),
                  std::istream_iterator<T>(),
                  std::back_inserter(outNumbers));
        return true;
    }
};
