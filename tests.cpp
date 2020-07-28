//
// Created by vladislav.saenko on 27.07.2020.
//

#include "tests.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <numeric>

#include "utils.h"

#include "calculator_pair.h"
#include "calculator_range.h"


namespace
{
    using Int = std::int64_t;
    using Vector = std::vector<Int>;

    constexpr const Int N{100'000};

    template <typename Iterator>
    void sumBlockWithInit (Iterator first, Iterator last, Int& result)
    {
        result = std::accumulate(first, last, result);
    }

    template <typename Iterator>
    Int sumBlock(Iterator first, Iterator last)
    {
        return std::accumulate(first, last, Int{});
    }

    template <class Futures>
    Int sumFutures(Futures&& futures)
    {
        Int res{};
        for (auto& f : futures)
        {
            res += f.get();
        }
        return res;
    }

    struct DoubleEqual
    {
        bool operator()(double a, double b) const
        {
            return std::abs(a - b) < std::numeric_limits<double>::epsilon();
        }
    };


    template <typename T, typename Compare = std::equal_to<T>>
    void printResult(const T& exp, const T& given, Compare comp = std::equal_to<T>())
    {
        if (comp(exp, given))
        {
            std::cout << "SUCCESS!\n";
        }
        else
        {
            std::cout << "FAILED!\n";
            std::cout << "Expected: " << exp << ", but given: " << given << "\n";
        }
        std::cout << "===============\n";
    }
}

void test::concurrentQueueSimple()
{
    Vector v(100);
    std::generate(v.begin(), v.end(), RandomGenerator());

    ConcurrentQueue<Int> q;
    for (auto e : v)
    {
        q.push(e);
    }

    Vector out;
    while (!q.empty())
    {
        if (Int e{}; q.try_pop(e))
        {
            out.push_back(e);
        }
    }

    if (v != out)
    {
        std::cout << "FAILED\n";
    }
    else
    {
        std::cout << "SUCCESS\n";
    }

    std::cout << std::boolalpha;
    std::cout << "Queue empty: " << q.empty() << "\n";
    std::cout << "===============\n";
}

void test::concurrentQueueProducerConsumer()
{
    const Int N{ 100};
    const Int expected = N * (N - 1) / 2;

    using PCType = ProducerConsumerTester<Int>;
    PCType pc;
    PCType::QueueType q;
    {
        TimeLogger t("Producer-consumer with concurrent queue");

        std::thread tWriter(
            &PCType::producer,
            &pc,
            N,
            std::ref(q));

        std::thread tReader{
            &PCType::consumer,
            &pc,
            std::ref(q)};

        tWriter.join();
        tReader.join();
    }

    printResult(expected, pc.res_);
}


void test::singleThreadAverage()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    {
        TimeLogger t("Single thread average");
        given = sumBlock(v.begin(), v.end()) * 1.0 / v.size();
    }
    printResult(expected, given, DoubleEqual());
}


void test::multiThreadsAverage()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    {
        const std::size_t numThreads{
            std::max(
                std::thread::hardware_concurrency(),
                4u)};

        const std::size_t blockSize = N / numThreads;

        std::vector<Int> results(numThreads);

        std::vector<std::thread> threads(numThreads - 1);

        TimeLogger t("Multi threading average");
        // sum by equal ranges
        auto blockStart = v.begin();
        for(std::size_t i{}; i < (numThreads - 1); ++i)
        {
            auto blockEnd = blockStart;
            std::advance(blockEnd, blockSize);

            threads[i] = std::thread(
                sumBlockWithInit<Vector::iterator>,
                blockStart,
                blockEnd,
                std::ref(results[i]));

            blockStart = blockEnd;
        }

        // sum the rest elements
        sumBlockWithInit(blockStart, v.end(), results[numThreads - 1]);

        for(auto& t: threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }

        given = sumBlock(results.begin(), results.end()) * 1.0 / N;
    }
    printResult(expected, given, DoubleEqual());
}

//=====================================================

void test::threadPoolAverageRanges()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    ThreadPool tp;
    CalculatorRange calculator{v};

    {
        TimeLogger time("Thread pool: ranges");
        calculator.run(tp);

        given = calculator.result().get();
    }
    printResult(expected, given, DoubleEqual());
}


void test::threadPoolAveragePairs()
{
    Vector v(N);
    std::iota(v.begin(), v.end(), Int{1});
    const double expected{0.5 * (N + 1)};
    double given{};
    ThreadPool tp;
    CalculatorPair calculator{v};

    {
        TimeLogger time("Thread pool: pairs");
        calculator.run(tp);

        given = calculator.result().get();
    }
    printResult(expected, given, DoubleEqual());
}
