#include <stdio.h>
#include <windows.h>

typedef enum
{
    NS_GREEN,
    NS_YELLOW,
    EW_GREEN,
    EW_YELLOW
} TrafficLightState;

typedef struct
{
    TrafficLightState state;
    int timer;
} TrafficLight;

#define GREEN_DURATION 5
#define YELLOW_DURATION 2
#define TICK_MS 1000

void simulation(TrafficLight *trafficlight)
{
    while (1)
    {
        switch (trafficlight->state)
        {
        case NS_GREEN:
            printf("NS: GREEN, EW: RED\n");
            trafficlight->timer++;
            if (trafficlight->timer >= GREEN_DURATION)
            {
                trafficlight->state = NS_YELLOW;
                trafficlight->timer = 0;
            }
            break;

        case NS_YELLOW:
            printf("NS: YELLOW, EW: RED\n");
            trafficlight->timer++;
            if (trafficlight->timer >= YELLOW_DURATION)
            {
                trafficlight->state = EW_GREEN;
                trafficlight->timer = 0;
            }
            break;

        case EW_GREEN:
            printf("NS: RED, EW: GREEN\n");
            trafficlight->timer++;
            if (trafficlight->timer >= GREEN_DURATION)
            {
                trafficlight->state = EW_YELLOW;
                trafficlight->timer = 0;
            }
            break;

        case EW_YELLOW:
            printf("NS: RED, EW: YELLOW\n");
            trafficlight->timer++;
            if (trafficlight->timer >= YELLOW_DURATION)
            {
                trafficlight->state = NS_GREEN;
                trafficlight->timer = 0;
            }
            break;

        default:
            break;
        }

        Sleep(TICK_MS);
    }
}

int main(void)
{
    TrafficLight trafficlight = {NS_GREEN, 0};
    simulation(&trafficlight);
    return 0;
}
