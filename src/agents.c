#include "server.h"


// Function to generate random agent data
void generate_agents(Agent agents[], int count) {
    for (int i = 0; i < count; i++) {
        agents[i].x = (float)(rand() % 100);
        agents[i].y = (float)(rand() % 100);
        agents[i].food = rand() % 50;
        for (int j = 0; j < 10; j++) {
            agents[i].genes[j] = rand() % 10;
        }
    }
}