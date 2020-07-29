
#include "tests.h"

#include <iostream>

int main()
{
    int rangesErrors{};
    int pairsErrors{};
    for (int i{}; i < 100; ++i)
    {
        std::cout << "\nTEST_" << i + 1 << "\n";

        rangesErrors += test::threadPoolAverageRanges();
        pairsErrors += test::threadPoolAveragePairs();
    }

    std::cout << "------------------\n";
    if (!pairsErrors && !rangesErrors)
    {
        std::cout << "SUCCESSFULL TESTS!\n";
    }
    else
    {
        std::cerr << "\n- Errors in pairsum: " << pairsErrors << "\n";
        std::cerr << "- Errors in rangesum: " << rangesErrors <<  "\n";
    }
    return 0;
}
