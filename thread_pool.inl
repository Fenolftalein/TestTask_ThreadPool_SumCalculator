//
// Created by vladislav.saenko on 27.07.2020.
//

template <typename Func>
std::shared_future<std::result_of_t<Func()>> ThreadPool::addTask(Func f)
{
    using ResultType = std::result_of_t<Func()>;
    std::packaged_task<ResultType()> task(std::move(f));
    auto res {task.get_future()};
    queue_.push(std::move(task));
    return res;
}


// todo for func with 2 numbers
template <typename Func>
void ThreadPool::addTaskNoReturn(Func&& f)
{
    queue_.push(TaskWrapper(f));    //std::function<void()>
}
