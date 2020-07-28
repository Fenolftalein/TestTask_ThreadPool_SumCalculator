
#include "tests.h"

int main()
{
    test::concurrentQueueSimple();
    test::concurrentQueueProducerConsumer();

    test::singleThreadAverage();
    test::multiThreadsAverage();
    test::threadPoolAverage();
//    test::threadPoolAverage2();
    return 0;
}
