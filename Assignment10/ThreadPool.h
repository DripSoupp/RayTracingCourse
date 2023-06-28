#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include "Constants.h"

// This header file guard ensures that the code is included only once
// to prevent duplicate definitions when multiple files include this header.

class ThreadPool {
public:
    explicit ThreadPool(const unsigned tCount) : workers(tCount), threadsCount(tCount) {}
    // The constructor initializes the ThreadPool with a specified number of threads.

    ThreadPool() = delete;
    // Deleting the default constructor to prevent its usage.

    ThreadPool(const ThreadPool&) = delete;
    // Deleting the copy constructor to prevent its usage.

    ThreadPool& operator=(const ThreadPool&) = delete;
    // Deleting the assignment operator to prevent its usage.
    
    void start() {
        Assert(!running && "Can't start ThreadPool if it's already running");
        running = true;
        for (size_t i = 0; i < workers.size(); ++i) {
            workers[i] = std::thread(&ThreadPool::workerBase, this);
        }
    }
    // The `start` function initializes and starts the ThreadPool by creating and starting worker threads.
    // It assigns each worker thread to execute the `workerBase` function.

    void stop() {
        Assert(running && "Can't stop ThreadPool if it's not running");
        running = false;
        workersCv.notify_all();
        std::for_each(workers.begin(), workers.end(), std::mem_fn(&std::thread::join));
        workers.clear();
    }
    // The `stop` function stops the ThreadPool by setting the `running` flag to false,
    // notifying all worker threads to wake up and exit, joining all worker threads,
    // and clearing the workers vector.

    void completeTasks() {
        for (;;) {
            if (numTasks == 0) {
                break;
            }
            std::this_thread::yield();
        }
    }
    // The `completeTasks` function waits until all tasks in the ThreadPool are completed.
    // It repeatedly checks if the number of tasks is zero and yields the current thread
    // to allow other threads to execute.

    template <typename F, typename... Args>
    void scheduleTask(F&& task, Args&&... args) {
        {
            std::lock_guard<std::mutex> lock(tasksMutex);
            tasksQueue.emplace(std::bind(std::forward<F>(task), std::forward<Args>(args)...));
        }
        ++numTasks;
        workersCv.notify_one();
    }
    // The `scheduleTask` function schedules a task to be executed by the ThreadPool.
    // It enqueues the task into the tasksQueue using perfect forwarding,
    // increments the number of tasks, and notifies one worker thread to wake up and execute the task.

private:
    void workerBase() {
        for (;;) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(tasksMutex);
                workersCv.wait(lock, [this] { return !tasksQueue.empty() || !running; });
                if (!running)
                    return;
                task = std::move(tasksQueue.front());
                tasksQueue.pop();
            }
            task();
            --numTasks;
        }
    }
    // The `workerBase` function represents the main loop of each worker thread.
    // It waits until there is a task in the tasksQueue or the ThreadPool is stopped.
    // It retrieves a task from the tasksQueue, executes it, and decrements the number of tasks.

private:
    std::vector<std::thread> workers{};
    // The vector to store worker threads.

    mutable std::mutex tasksMutex{};
    // The mutex to protect access to the tasksQueue.

    std::queue<std::function<void()>> tasksQueue{};
    // The queue to store tasks to be executed by the worker threads.

    std::condition_variable workersCv{};
    // The condition variable to synchronize worker threads.

    std::atomic_bool running = false;
    // The flag indicating if the ThreadPool is running.

    std::atomic_size_t numTasks{};
    // The number of tasks in the ThreadPool.

    const unsigned threadsCount{};
    // The number of worker threads in the ThreadPool.

};

#endif

