// Includes
#include "../Utility/Utility.h"



static void Exercise1(std::vector<int>& inNumbers)
{
    int num_increases = 0;
    for (int i = 1; i < inNumbers.size(); i++)
        num_increases += inNumbers[i] > inNumbers[i - 1];

    printf("Num increases: %d\n", num_increases);
}

static void Exercise2(std::vector<int>& inNumbers)
{
    int num_increases = 0;
    for (int i = 4; i < inNumbers.size(); i++)
    {
        int sum_a = inNumbers[i - 1] + inNumbers[i - 2] + inNumbers[i - 3];
        int sum_b = inNumbers[i - 0] + inNumbers[i - 1] + inNumbers[i - 2];
        num_increases += sum_b > sum_a;
    }

    printf("Num increases: %d\n", num_increases);
}

int main()
{
    // Collect numbers from the puzzle input
    std::vector<int> numbers;
    if (!AOCUtility::sParseInputFile(numbers))
        return 1;

    // Execute exercises
    Exercise1(numbers);
    Exercise2(numbers);

    return 0;
}
