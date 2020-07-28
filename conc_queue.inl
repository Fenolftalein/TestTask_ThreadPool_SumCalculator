//
// Created by vladislav.saenko on 27.07.2020.
//


template <typename T>
void ConcurrentQueue<T>::push(T value)
{
    std::lock_guard lg{mutex_};
    queue_.push(std::move(value));
    cv_.notify_one();
}

template <typename T>
void ConcurrentQueue<T>::wait_pop(T &value)
{
    std::unique_lock ul{mutex_};
    cv_.wait(ul, [this]{ return !queue_.empty(); });
    value = queue_.front();
    queue_.pop();
}


template <typename T>
bool ConcurrentQueue<T>::try_pop(T& value)
{
    std::lock_guard lg{mutex_};
    if (queue_.empty())
    {
        return false;
    }
    value = std::move(queue_.front());
    queue_.pop();
    return true;
}


template <typename T>
bool ConcurrentQueue<T>::empty() const
{
    std::lock_guard lg{mutex_};
    return queue_.empty();
}
