/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <deque>

namespace dit {

    //-----------------------------------//

    /**
     * Concurrent queue that is safe to use even with multiple producers
     * and consumers. This class is based from code located at website:
     * http://www.justsoftwaresolutions.co.uk/threading/implementing-a-
     * thread-safe-queue-using-condition-variables.html
     */

    template<typename T> class ConcurrentQueue
    {
    public:
        void push_front(const T& value)
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };
            
            queue.push_front(value);

            _cond.notify_one();
        }

        //-----------------------------------//

        void push_back(const T& value)
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };

            queue.push_back(value);

            _cond.notify_one();
        }

        //-----------------------------------//

        bool empty() const
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };
            bool empty = queue.empty();

            return empty;
        }

        //-----------------------------------//

        bool try_pop_front(T& popped_value)
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };

            if (queue.empty())
                return false;

            popped_value = queue.front();
            queue.pop_front();

            return true;
        }

        //-----------------------------------//

        void wait_and_pop_front(T& popped_value)
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };

            while (queue.empty())
                _cond.wait(lock);

            popped_value = queue.front();
            queue.pop_front();
        }

        //-----------------------------------//

        bool find(const T& value)
        {
            boost::unique_lock<boost::mutex> lock{ _mutex };

            auto it = std::find(queue.begin(), queue.end(), value);
            bool found = it != queue.end();

            return found;
        }

        //-----------------------------------//

    public:
        boost::mutex _mutex;
        boost::condition_variable _cond;

        std::deque<T> queue;
    };

    //-----------------------------------//

}