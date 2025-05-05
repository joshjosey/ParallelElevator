// queue.h
// Author: Jaden Hicks

#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <mutex>

// Adapated form CHatGPT
template <typename T>
class ThreadSafeDeque {
// Wrapper for a thread safe queue
    ThreadSafeQueue( )
    private:
        std::deque<T> queue;
        std::mutex mtx;
    
    public:
        void push(T val) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push_back(val);
        }
    
        bool pop(T& val) {
            std::lock_guard<std::mutex> lock(mtx);
            if (queue.empty()) return false;
            val = dq.front();
            dq.pop_front();
            return true;
        }
    };

#endif