#include <chrono>
#include <iostream>
#include <thread>

enum class Phase {
    NS_GREEN,
    NS_YELLOW,
    EW_GREEN,
    EW_YELLOW
};

struct TrafficLight {
    Phase phase{Phase::NS_GREEN};
    int timer{0}; // seconds elapsed in current phase
};

constexpr int GREEN_DURATION  = 5;
constexpr int YELLOW_DURATION = 2;

void tick(TrafficLight& tl) {
    using std::cout;

    switch (tl.phase) {
        case Phase::NS_GREEN:
            cout << "NS: GREEN, EW: RED\n";
            ++tl.timer;
            if (tl.timer >= GREEN_DURATION) {
                tl.phase = Phase::NS_YELLOW;
                tl.timer = 0;
            }
            break;

        case Phase::NS_YELLOW:
            cout << "NS: YELLOW, EW: RED\n";
            ++tl.timer;
            if (tl.timer >= YELLOW_DURATION) {
                tl.phase = Phase::EW_GREEN;
                tl.timer = 0;
            }
            break;

        case Phase::EW_GREEN:
            cout << "NS: RED, EW: GREEN\n";
            ++tl.timer;
            if (tl.timer >= GREEN_DURATION) {
                tl.phase = Phase::EW_YELLOW;
                tl.timer = 0;
            }
            break;

        case Phase::EW_YELLOW:
            cout << "NS: RED, EW: YELLOW\n";
            ++tl.timer;
            if (tl.timer >= YELLOW_DURATION) {
                tl.phase = Phase::NS_GREEN;
                tl.timer = 0;
            }
            break;
    }
}

int main() {
    TrafficLight tl;

    while (true) {
        tick(tl);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
