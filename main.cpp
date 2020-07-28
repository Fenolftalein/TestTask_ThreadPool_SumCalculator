
#include "tests.h"

int main()
{
    test::concurrentQueueSimple();
    test::concurrentQueueProducerConsumer();

    test::singleThreadAverage();
    test::multiThreadsAverage();
    test::threadPoolAverage();
    return 0;
}
