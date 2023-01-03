#ifndef SRC_PTHREAD_PIPELINE_TIMER_H
#define SRC_PTHREAD_PIPELINE_TIMER_H

#include <chrono>
#include <ostream>

using std::ostream;
using namespace std::chrono;




struct PipelineTimings {
private:
    class PipelineTimer {
    public:
        PipelineTimer(milliseconds& count);
        PipelineTimer(PipelineTimer&& other);
        ~PipelineTimer();
    private:
        milliseconds& count;
        time_point<high_resolution_clock> time;
        bool moved = false;
    };
public:
    milliseconds read_image {0};
    milliseconds write_image {0};
    milliseconds gaussian_blur {0};
    milliseconds gradient_magnitude {0};
    milliseconds gradient_nonmaximum_suppresion {0};
    milliseconds double_threshold {0};
    milliseconds hysteresis {0};

    PipelineTimer time_read_image();
    PipelineTimer time_write_image();
    PipelineTimer time_gaussian_blur();
    PipelineTimer time_gradient_magnitude();
    PipelineTimer time_gradient_nonmaximum_suppresion();
    PipelineTimer time_double_threshold();
    PipelineTimer time_hysteresis();
    milliseconds total_time();

    void combine_timings(PipelineTimings const& other);

    friend ostream& operator<< (ostream& out, PipelineTimings const& timings);
};


#endif /* SRC_PTHREAD_PIPELINE_TIMER_H */
