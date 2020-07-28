//
// Created by vladislav.saenko on 27.07.2020.
//

template <typename Func>
std::future<std::result_of_t<Func()>> ThreadPool::addTask(Func f)
{
    using ResultType = std::result_of_t<Func()>;
    std::packaged_task<ResultType()> task(std::move(f));
    auto res {task.get_future()};
    queue_.push(std::move(task));
    return res;
}
