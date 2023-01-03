#include "PipelineTimer.h"

using std::endl;

PipelineTimings::PipelineTimer::PipelineTimer(milliseconds &count)
    : count(count)
{
    time = high_resolution_clock::now();
}

PipelineTimings::PipelineTimer::PipelineTimer(PipelineTimer &&other)
    : count(other.count)
    , time(other.time)
    , moved(other.moved)
{
    other.moved = true;
}

PipelineTimings::PipelineTimer::~PipelineTimer()
{
    if (moved) return;
    auto ms = duration_cast<milliseconds>(high_resolution_clock::now() - time);
    count += ms;
}

PipelineTimings::PipelineTimer PipelineTimings::time_read_image() { return PipelineTimer {read_image}; }
PipelineTimings::PipelineTimer PipelineTimings::time_write_image() { return PipelineTimer {write_image}; }
PipelineTimings::PipelineTimer PipelineTimings::time_gaussian_blur() { return PipelineTimer {gaussian_blur}; }
PipelineTimings::PipelineTimer PipelineTimings::time_gradient_magnitude() { return PipelineTimer {gradient_magnitude}; }
PipelineTimings::PipelineTimer PipelineTimings::time_gradient_nonmaximum_suppresion() { return PipelineTimer {gradient_nonmaximum_suppresion}; }
PipelineTimings::PipelineTimer PipelineTimings::time_double_threshold() { return PipelineTimer {double_threshold}; }
PipelineTimings::PipelineTimer PipelineTimings::time_hysteresis() { return PipelineTimer {hysteresis}; }

void PipelineTimings::combine_timings(PipelineTimings const &other)
{
    read_image += other.read_image;
    write_image += other.write_image;
    gaussian_blur += other.gaussian_blur;
    gradient_magnitude += other.gradient_magnitude;
    gradient_nonmaximum_suppresion += other.gradient_nonmaximum_suppresion;
    double_threshold += other.double_threshold;
    hysteresis += other.hysteresis;
}

ostream &operator<<(ostream &out, PipelineTimings const &timings)
{
    out << "read_image: " << timings.read_image.count() << "ms" << endl;
    out << "write_image: " << timings.write_image.count() << "ms" << endl;
    out << "gaussian_blur: " << timings.gaussian_blur.count() << "ms" << endl;
    out << "gradient_magnitude: " << timings.gradient_magnitude.count() << "ms" << endl;
    out << "gradient_nonmaximum_suppresion: " << timings.gradient_nonmaximum_suppresion.count() << "ms" << endl;
    out << "double_threshold: " << timings.double_threshold.count() << "ms" << endl;
    out << "hysteresis: " << timings.hysteresis.count() << "ms";

    return out;
}
