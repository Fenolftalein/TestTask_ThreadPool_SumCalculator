//
// Created by vladislav.saenko on 27.07.2020.
//

#ifndef DISNEY_PLUS_FUNC_WRAPPER_H
#define DISNEY_PLUS_FUNC_WRAPPER_H

#include <memory>


class TaskWrapper
{
    struct ImplBase
    {
        virtual void call() = 0;
        virtual ~ImplBase() = default;
    };
    std::unique_ptr<ImplBase> impl_;

    template<typename F>
    struct Impl : ImplBase
    {
        Impl(F&& f)
        : f_{std::move(f)}
        {}

        void call()
        {
            f_();
        }

        F f_;
    };

public:
    TaskWrapper() = default;

    template <typename F>
    TaskWrapper(F&& f)
    : impl_{std::make_unique<Impl<F>>(std::move(f))}
    {}

    TaskWrapper(TaskWrapper&& other)
    : impl_ { std::move(other.impl_) }
    {}

    TaskWrapper& operator=(TaskWrapper&& other)
    {
        impl_ = std::move(other.impl_);
        return *this;
    }

    void operator()()
    {
        impl_->call();
    }
};


#endif //DISNEY_PLUS_FUNC_WRAPPER_H

