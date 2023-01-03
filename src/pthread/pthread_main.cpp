#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <type_traits>
#include <memory>
#include <cstdio>
#include <sstream>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::make_shared;
using std::filesystem::path;
using std::filesystem::directory_iterator;
using std::decay_t;
using std::is_same_v;
using std::stringstream;

#include <unistd.h>
#include <pthread.h>

#include "TaskQueue.h"
#include "../ImageOps.h"
#include "../ScopeTimer.h"
#include "../PipelineTimer.h"

template<class> inline constexpr bool always_false_v = false;

extern "C" void* process_queue_task(void* _arg) {
    FunctionTimer();
    auto& queue = *static_cast<CannyTaskQueue*>(_arg);

    PipelineTimings& timings = *new PipelineTimings;
    
    while (!queue.all_complete()) {
        auto maybe_task = queue.get_task();

        if (maybe_task.has_value()) {
            auto& task = maybe_task.value();

            std::visit([&](auto&& arg) {
                using T = decay_t<decltype(arg)>;
                using namespace CannyTasks;

                if constexpr(is_same_v<T, ReadImage>) {
                    auto t = timings.time_read_image();

                    auto image = ImageOps::read_image(task.properties->input_path);

                    auto next_task = task.create_from(GaussianBlur { std::move(image) });
                    queue.add_task(std::move(next_task));
                }
                else if constexpr (is_same_v<T, WriteImage>) {
                    auto t = timings.time_write_image();

                    ImageOps::write_image(task.properties->output_path, *arg.image);

                    // TODO: Write in order
                }
                else if constexpr (is_same_v<T, GaussianBlur>) {
                    auto t = timings.time_gaussian_blur();

                    auto blurred = Canny::gaussian_blur(*arg.image);

                    auto next_task = task.create_from(GradientMagnitude { std::move(blurred) });
                    queue.add_task(std::move(next_task));
                }
                else if constexpr (is_same_v<T, GradientMagnitude>) {
                    auto t = timings.time_gradient_magnitude();
                    auto angle_magnitude = Canny::gradient_magnitude(*arg.blurred);

                    auto next_task = task.create_from(GradientNonmaximumSuppresion { std::move(angle_magnitude) });
                    queue.add_task(std::move(next_task));
                }
                else if constexpr (is_same_v<T, GradientNonmaximumSuppresion>) {
                    auto t = timings.time_gradient_nonmaximum_suppresion();
                    auto nonmax = Canny::gradient_nonmaximum_suppresion(*arg.angle_magnitude);

                    auto next_task = task.create_from(DoubleThreshold { std::move(nonmax) });
                    queue.add_task(std::move(next_task));
                }
                else if constexpr (is_same_v<T, DoubleThreshold>) {
                    auto t = timings.time_double_threshold();
                    auto threshold = Canny::double_threshold(
                        *arg.nonmax,
                        task.properties->threshold_low,
                        task.properties->threshold_high
                    );

                    auto next_task = task.create_from(Hysteresis { std::move(threshold) });
                    queue.add_task(std::move(next_task));
                }
                else if constexpr (is_same_v<T, Hysteresis>) {
                    auto t = timings.time_hysteresis();
                    auto image = Canny::hysteresis(*arg.threshold);

                    auto next_task = task.create_from(WriteImage { std::move(image) });
                    queue.add_task(std::move(next_task));
                }
                else
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, task.task);

            queue.mark_task_finished();
        }
    }
    
    return &timings;
}


void pthread_main(size_t thread_count, path input_directory, path output_directory, size_t images_count) {
    FunctionTimer();

    CannyTaskQueue queue;

    vector<pthread_t> threads;

    threads.resize(thread_count);

    for (auto& thread : threads) {
        pthread_create(&thread, nullptr, process_queue_task, &queue);
    }

    int i = 0;
    for (auto const& entry : directory_iterator(input_directory)) {
        stringstream timer_name;
        timer_name << entry.path().filename().string() << " [" << i << ']';

        queue.add_task(CannyTask {
            make_shared<CannyTask::Properties>(
                entry.path(),
                output_directory / entry.path().filename(),
                50,
                150,
                i,
                ScopeTimer {timer_name.str(), true}
            ),
            CannyTasks::ReadImage {}
        });
        i++;
        if (images_count != -1 && i == images_count) break;
    }

    queue.finish_adding_tasks();    

    PipelineTimings total;

    i = 0;
    for (auto& thread : threads) {
        void* ret;
        pthread_join(thread, &ret);

        auto& timings = *static_cast<PipelineTimings*>(ret);
        cout << "Timings for thread " << i << endl;
        cout << timings << endl;

        total.combine_timings(timings);
        delete &timings;
    }

    cout << "Total timings" << endl;
    cout << total << endl;
}


