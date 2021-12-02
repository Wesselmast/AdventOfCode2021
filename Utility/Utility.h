#pragma once

// Includes
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <cassert>



// gAssert
#define gAssert(condition, ...)                 \
    if (!condition)                             \
    {                                           \
        printf("\n[ERROR] ");                   \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
        assert(!"[ASSERT HIT] Check log");      \
    }



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
@brief Utility class for performance capturing

Please refer to the global instance 'gAOCTimer' instead of instancing this class.
**/
class AOCTimer
{
public:
    /**
    @brief Constructor
    **/
    AOCTimer()
    {
        // Query performance frequency
        LARGE_INTEGER frequency;
        if (!QueryPerformanceFrequency(&frequency))
        {
            printf("Timing failed, unable to query performance frequency");
            return;
        }

        // Calculate the unit to use for timing.
        // In this case we divide by 1000 to get milliseconds.
        mUnit = double(frequency.QuadPart) / 1000.0;
    }



    /**
    @brief Start the timer
    **/
    void StartTimer()
    {
        // Stop any running timer
        if (IsTiming())
            StopTimer();

        mTiming = true;

        // Query the start frequency
        LARGE_INTEGER frequency;
        QueryPerformanceCounter(&frequency);
        mStart = frequency.QuadPart;
    }



    /**
    @brief Stop the timer and print the duration since starting the timer
    **/
    void StopTimer(const char* inName = "<None>")
    {
        // Query the end frequency
        LARGE_INTEGER frequency;
        QueryPerformanceCounter(&frequency);

        // Print the difference
        printf("%s took %fms\n", inName, double(frequency.QuadPart - mStart) / mUnit);

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
    __int64     mStart      = 0;
    bool        mTiming     = false;
    double      mUnit       = 0.0;
};
#else // Non-windows
/**
@brief Non-windows dummy timer class
**/
class AOCTimer
{
public:
    void StartTimer() {}
    void StopTimer(const char*) {}
    bool IsTiming() const { return false; }
};
#endif



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



/**
@brief AOCResult
**/
struct AOCResult
{
    int         mAnswer;    // Answer to an AOC Exercise (will always be an int)
    const char* mName;      // Name of the Exercise
};
#define RESULT(num) AOCResult{num, __func__};



/**
@brief AOCUseTiming
**/
enum class AOCUseTiming
{
    Yes,                    // Use timing (and disable printing the answer)
    No                      // Do not use timing
};



/**
@brief Class with utility functions for this years' AOC!
**/
class AOCUtility
{
public:
    /**
    @brief Parse the file at @a inFilepath into an array of T
    **/
    template<typename T>
    static bool sParseInputFile(const char* inFilepath, std::vector<T>& outElements)
    {
        std::ifstream stream(inFilepath, std::ifstream::in);
        if (!stream)
        {
            printf("File %s was not found\n", inFilepath);
            return false;
        }

        // Copy elements from the file that can be implicitly casted to type T
        std::copy(std::istream_iterator<T>(stream),
                  std::istream_iterator<T>(),
                  std::back_inserter(outElements));
        return true;
    }



    /**
    @brief Execute an AOC Exercise

    If @a inUseTiming is enabled, an answer file will be expected.
    If the answer has not yet been generated, please run once without using timing.

    The goal of this behaviour is to use disable timing initially to find the correct answer,
    and enable timing once the answer has been found and focus shifts towards optimizing the code.
    **/
    template<typename F>
    static void sDoExercise(AOCUseTiming inUseTiming, F inExercise)
    {
        if (inUseTiming == AOCUseTiming::Yes)
        {
            // Execute the exercise with timing
            gAOCTimer.StartTimer();
            AOCResult result = inExercise();
            gAOCTimer.StopTimer(result.mName);

            // Open answer file
            std::string filename = std::string(result.mName) + ".answer";
            std::ifstream stream(filename, std::ifstream::in);
            gAssert(stream, "\"%s\" must exist", filename.c_str());

            // Deserialize the answer
            int answer;
            stream >> answer;

            // Assert that we still have the right answer
            gAssert(bool(result.mAnswer == answer), "Results of \"%s\" do not match:\n\tCURRENT: %d\n\tCORRECT: %d\n", result.mName, result.mAnswer, answer);
        }
        else
        {
            // Execute the exercise and print the result
            AOCResult result = inExercise();
            printf("%s:\t%d\n", result.mName, result.mAnswer);

            // Serialize answer to an answer file
            std::string filename = std::string(result.mName) + ".answer";
            std::ofstream stream(filename, std::ifstream::out);
            stream << result.mAnswer;
        }
    }
 };
