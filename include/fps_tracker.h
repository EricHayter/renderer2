#pragma once

#include <chrono>

class FPSTracker {
    public:
    FPSTracker() = default;
    float GetFPS();
    void RecordFrame() { frames_displayed_m++; }

    private:
    int frames_displayed_m = 0;
    std::chrono::time_point<std::chrono::steady_clock> past_time_m = std::chrono::steady_clock::now();
    float past_fps_m = 0.0f;
    int report_interval_ms_m = 3000;
};
