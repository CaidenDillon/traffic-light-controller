#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

enum class Phase { NS_GREEN, NS_YELLOW, EW_GREEN, EW_YELLOW };

struct TrafficLight {
    Phase phase{Phase::NS_GREEN};
    std::chrono::seconds elapsed{0s}; // time spent in current phase
};

constexpr auto GREEN_DURATION  = 5s;
constexpr auto YELLOW_DURATION = 2s;

constexpr std::chrono::seconds duration_for(Phase p) {
    switch (p) {
        case Phase::NS_GREEN:  return GREEN_DURATION;
        case Phase::NS_YELLOW: return YELLOW_DURATION;
        case Phase::EW_GREEN:  return GREEN_DURATION;
        case Phase::EW_YELLOW: return YELLOW_DURATION;
    }
    return 0s; // unreachable in practice
}

constexpr Phase next_phase(Phase p) {
    switch (p) {
        case Phase::NS_GREEN:  return Phase::NS_YELLOW;
        case Phase::NS_YELLOW: return Phase::EW_GREEN;
        case Phase::EW_GREEN:  return Phase::EW_YELLOW;
        case Phase::EW_YELLOW: return Phase::NS_GREEN;
    }
    return Phase::NS_GREEN;
}

void render(const TrafficLight& tl) {
    switch (tl.phase) {
        case Phase::NS_GREEN:  std::cout << "NS: GREEN, EW: RED\n";   break;
        case Phase::NS_YELLOW: std::cout << "NS: YELLOW, EW: RED\n";  break;
        case Phase::EW_GREEN:  std::cout << "NS: RED, EW: GREEN\n";   break;
        case Phase::EW_YELLOW: std::cout << "NS: RED, EW: YELLOW\n";  break;
    }
}

void advance(TrafficLight& tl, std::chrono::seconds dt) {
    tl.elapsed += dt;
    if (tl.elapsed >= duration_for(tl.phase)) {
        tl.phase = next_phase(tl.phase);
        tl.elapsed = 0s;
    }
}

int main() {
    TrafficLight tl;

    while (true) {
        render(tl);
        advance(tl, 1s);
        std::this_thread::sleep_for(1s);
    }
}
