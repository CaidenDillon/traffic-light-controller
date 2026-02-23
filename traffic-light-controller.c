#include <chrono>
#include <iostream>
#include <thread>

#if defined(_WIN32)
#include <conio.h>
#endif

using namespace std::chrono_literals;

enum class Phase {
    NS_GREEN,
    NS_PED_WALK,
    NS_YELLOW,
    EW_GREEN,
    EW_PED_WALK,
    EW_YELLOW
};

struct PedRequests {
    bool ns{false};
    bool ew{false};
};

struct TrafficLight {
    Phase phase{Phase::NS_GREEN};
    std::chrono::seconds elapsed{0s};
    PedRequests ped{};
};

constexpr auto GREEN_DURATION  = 5s;
constexpr auto YELLOW_DURATION = 2s;
constexpr auto WALK_DURATION   = 3s;

constexpr std::chrono::seconds duration_for(Phase p) {
    switch (p) {
        case Phase::NS_GREEN:     return GREEN_DURATION;
        case Phase::NS_PED_WALK:  return WALK_DURATION;
        case Phase::NS_YELLOW:    return YELLOW_DURATION;
        case Phase::EW_GREEN:     return GREEN_DURATION;
        case Phase::EW_PED_WALK:  return WALK_DURATION;
        case Phase::EW_YELLOW:    return YELLOW_DURATION;
    }
    return 0s;
}

constexpr Phase normal_next_phase(Phase p) {
    switch (p) {
        case Phase::NS_GREEN:     return Phase::NS_YELLOW;
        case Phase::NS_PED_WALK:  return Phase::NS_YELLOW;
        case Phase::NS_YELLOW:    return Phase::EW_GREEN;
        case Phase::EW_GREEN:     return Phase::EW_YELLOW;
        case Phase::EW_PED_WALK:  return Phase::EW_YELLOW;
        case Phase::EW_YELLOW:    return Phase::NS_GREEN;
    }
    return Phase::NS_GREEN;
}

void request_ped_crossing(TrafficLight& tl, Phase served_by) {
    if (served_by == Phase::NS_GREEN) tl.ped.ns = true;
    if (served_by == Phase::EW_GREEN) tl.ped.ew = true;
}

void render(const TrafficLight& tl) {
    switch (tl.phase) {
        case Phase::NS_GREEN:     std::cout << "NS: GREEN,  EW: RED   | PED(NS): DON'T\n"; break;
        case Phase::NS_PED_WALK:  std::cout << "NS: GREEN,  EW: RED   | PED(NS): WALK\n";  break;
        case Phase::NS_YELLOW:    std::cout << "NS: YELLOW, EW: RED   | PED(NS): DON'T\n"; break;
        case Phase::EW_GREEN:     std::cout << "NS: RED,    EW: GREEN | PED(EW): DON'T\n"; break;
        case Phase::EW_PED_WALK:  std::cout << "NS: RED,    EW: GREEN | PED(EW): WALK\n";  break;
        case Phase::EW_YELLOW:    std::cout << "NS: RED,    EW: YELLOW| PED(EW): DON'T\n"; break;
    }
}

void advance(TrafficLight& tl, std::chrono::seconds dt) {
    tl.elapsed += dt;

    if (tl.elapsed < duration_for(tl.phase))
        return;

    tl.elapsed = 0s;

    if (tl.phase == Phase::NS_GREEN && tl.ped.ns) {
        tl.ped.ns = false;
        tl.phase = Phase::NS_PED_WALK;
        return;
    }

    if (tl.phase == Phase::EW_GREEN && tl.ped.ew) {
        tl.ped.ew = false;
        tl.phase = Phase::EW_PED_WALK;
        return;
    }

    tl.phase = normal_next_phase(tl.phase);
}

#if defined(_WIN32)
bool poll_input(TrafficLight& tl) {
    while (_kbhit()) {
        const int ch = _getch();
        if (ch == 'q' || ch == 'Q') return false;
        if (ch == 'n' || ch == 'N') request_ped_crossing(tl, Phase::NS_GREEN);
        if (ch == 'e' || ch == 'E') request_ped_crossing(tl, Phase::EW_GREEN);
    }
    return true;
}
#else
bool poll_input(TrafficLight&) { return true; }
#endif

int main() {
    TrafficLight tl;

    while (true) {
        if (!poll_input(tl)) break;

        render(tl);
        advance(tl, 1s);

        std::this_thread::sleep_for(1s);
    }
}
