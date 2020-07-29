//
// Created by vladislav.saenko on 27.07.2020.
//

#include "tests.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

#include "utils.h"

#include "calculator.h"


namespace
{
    using Int = std::int64_t;
    using Vector = std::vector<Int>;

    constexpr const Int N{10'000};

    struct DoubleEqual
    {
        bool operator()(double a, double b) const
        {
            return std::abs(a - b) < std::numeric_limits<double>::epsilon();
        }
    };


    template <typename T, typename Compare = std::equal_to<T>>
    int checkResult(const T& exp, const T& given, Compare comp = std::equal_to<T>())
    {
        int error{};
        if (comp(exp, given))
        {
            std::cout << "SUCCESS!\n";
        }
        else
        {
            std::cerr << "FAILED!\n";
            std::cerr << "Expected: " << exp << ", but given: " << given << std::endl;
            error = 1;
        }
        std::cout << "===============\n";
        return error;
    }
}


int test::threadPoolAverageRanges()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    ThreadPool tp;
    Calculator calculator{v};
    {
        TimeLogger time("Thread pool: ranges");
        calculator.run(tp, Calculator::TYPE::RANGES);
        given = calculator.get();
    }
    return checkResult(expected, given, DoubleEqual());
}


int test::threadPoolAveragePairs()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    ThreadPool tp;
    Calculator calculator{v};
    {
        TimeLogger time("Thread pool: pairs");
        calculator.run(tp, Calculator::TYPE::PAIRS);
        given = calculator.get();
    }
    return checkResult(expected, given, DoubleEqual());
}
