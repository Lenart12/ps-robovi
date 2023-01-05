#include "TaskQueue.h"
#include <cassert>

CannyTaskQueue::~CannyTaskQueue()
{
    pthread_mutex_destroy(&queue_lock);
    pthread_cond_destroy(&empty_queue_cond);
}

void CannyTaskQueue::add_task(CannyTask &&task)
{
    pthread_mutex_lock(&queue_lock);
    tasks.emplace(std::forward<CannyTask>(task));
    pthread_cond_signal(&empty_queue_cond);
    processing_tasks++;
    pthread_mutex_unlock(&queue_lock);
}

optional<CannyTask> CannyTaskQueue::get_task(){
    pthread_mutex_lock(&queue_lock);
    while (tasks.empty() && processing_tasks > 0) {
        pthread_cond_wait(&empty_queue_cond, &queue_lock);
    }
    if (processing_tasks == 0) {
        pthread_mutex_unlock(&queue_lock);
        return std::nullopt;
    }
    auto task = std::move(tasks.front());
    tasks.pop();
    pthread_mutex_unlock(&queue_lock);
    return task;
}

void CannyTaskQueue::mark_task_finished()
{
    pthread_mutex_lock(&queue_lock);
    processing_tasks--;
    assert(processing_tasks >= 0);
    if (processing_tasks == 0) {
        pthread_cond_broadcast(&empty_queue_cond);
    }
    pthread_mutex_unlock(&queue_lock);
}

void CannyTaskQueue::finish_adding_tasks()
{
    mark_task_finished();
}

CannyTask CannyTask::create_from(Task &&new_task)
{
    return CannyTask { properties, std::move(new_task) };
}
