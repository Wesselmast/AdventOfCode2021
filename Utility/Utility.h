#pragma once

// Includes
#include <fstream>
#include <iterator>
#include <vector>



/**
@brief Win32 only timer

This was made windows-only because I dislike std::chrono
and I am too lazy to make it actually platform-independent.

Because this code is excluded for non-windows users, the code will still compile and run.
See, I'm not 'that' evil!
**/
#ifdef _WIN32
// Includes
#include <windows.h>



/**
@brief Utility class for calculating time taken between StartTimer and StopTimer calls
Please the global instance 'gAOCTimer' instead of instancing this class.
**/
class AOCTimer
{
public:
    /**
    @brief Start the timer
    **/
    void StartTimer(const char* inName)
    {
        // Stop any running timer
        if (IsTiming())
            StopTimer();

        mName   = inName;
        mTiming = true;

        // Query performance frequency
        LARGE_INTEGER frequency;
        if (!QueryPerformanceFrequency(&frequency))
        {
            printf("Timing failed, unable to query performance frequency");
            return;
        }

        // Calculate the frequency to use for the final calculation.
        // In this case we divide by a thousand to get milliseconds.
        mFrequency = double(frequency.QuadPart) / 1000.0;

        // Query the start frequency
        QueryPerformanceCounter(&frequency);
        mStart = frequency.QuadPart;
    }

    /**
    @brief Stop the timer and print the duration since starting the timer
    **/
    void StopTimer()
    {
        // Query the end frequency
        LARGE_INTEGER frequency;
        QueryPerformanceCounter(&frequency);

        // Print the difference
        printf("%s took %fms\n", mName, double(frequency.QuadPart - mStart) / mFrequency);

        // We have stopped timing
        mTiming = false;
    }


    /**
    @brief Check if any timer is currently running
    **/
    bool IsTiming() const
    {
        return mTiming;
    }

private:
    ///@name Properties
    const char* mName;
    __int64     mStart      = 0;
    bool        mTiming     = false;
    double      mFrequency  = 0.0;
};
#else // Non-windows
/**
@brief Non-windows dummy timer class
**/
class AOCTimer
{
public:
    void StartTimer(const char*) {}
    void StopTimer() {}
    bool IsTiming() const { return false; }
};
#endi



/**
@brief Global instance of AOCTimer that is meant to be used in code
It can be used a follows:

    gAOCTimer.StartTimer("Your Defined Name");

    // Your custom logic
    ...

    gAOCTimer.StopTimer();
    // 'Your Defined Name took Xms'

**/
AOCTimer gAOCTimer;



// gTrace is a printf wrapper that disables printf during timing for proper performance capturing
#define gTrace(...) if (!gAOCTimer.IsTiming()) printf(__VA_ARGS__)



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
