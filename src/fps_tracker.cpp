#include "fps_tracker.h"

float FPSTracker::GetFPS() {
    auto current_time = std::chrono::steady_clock::now();
    int ms_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         current_time - past_time_m)
                         .count();
    if (ms_elapsed > report_interval_ms_m) {
        float fps = frames_displayed_m / (ms_elapsed / 1000.0f);
        past_fps_m = fps;
        frames_displayed_m = 0;
        past_time_m = std::chrono::steady_clock::now();
    }
    return past_fps_m;
}
