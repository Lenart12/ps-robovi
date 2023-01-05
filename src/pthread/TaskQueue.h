#ifndef SRC_PTHREAD_TASK_QUEUE_H
#define SRC_PTHREAD_TASK_QUEUE_H

#include <variant>
#include <filesystem>
#include <memory>
#include <queue>
#include <optional>

using std::variant;
using std::filesystem::path;
using std::unique_ptr;
using std::shared_ptr;
using std::queue;
using std::optional;

#include <pthread.h>

#include <opencv2/opencv.hpp>
using cv::Mat;

#include "../ScopeTimer.h"
#include "../Canny.h"
using AngleMagnitude = Canny::AngleMagniutude;

namespace CannyTasks {
    struct ReadImage {};

    struct WriteImage {
        unique_ptr<Mat> image;
    };

    struct GaussianBlur {
        unique_ptr<Mat> image;       
    };

    struct GradientMagnitude {
        unique_ptr<Mat> blurred;
    };

    struct GradientNonmaximumSuppresion {
        unique_ptr<AngleMagnitude> angle_magnitude;
    };

    struct DoubleThreshold {
        unique_ptr<Mat> nonmax;
    };
    
    struct Hysteresis {
        unique_ptr<Mat> threshold;
    };

    using Task = variant<
        ReadImage,
        WriteImage,
        GaussianBlur,
        GradientMagnitude,
        GradientNonmaximumSuppresion,
        DoubleThreshold,
        Hysteresis
    >;
};

struct CannyTask {
    using Task = CannyTasks::Task;

    struct Properties {
        path input_path;
        path output_path;
        double threshold_low;
        double threshold_high;
        int image_order;
        ScopeTimer processing_time;
    };

    shared_ptr<Properties> properties;
    Task task;

    CannyTask create_from(Task&& new_task);
};


class CannyTaskQueue {
public:
    explicit CannyTaskQueue()
        : queue_lock(PTHREAD_MUTEX_INITIALIZER)
        , empty_queue_cond(PTHREAD_COND_INITIALIZER)
        {}

    ~CannyTaskQueue();

    void add_task(CannyTask&& task);
    optional<CannyTask> get_task();
    void mark_task_finished();
    void finish_adding_tasks();

private:
    queue<CannyTask> tasks {};
    size_t processing_tasks = 1;
    pthread_mutex_t queue_lock;
    pthread_cond_t empty_queue_cond;
};

#endif /* SRC_PTHREAD_TASK_QUEUE_H */
