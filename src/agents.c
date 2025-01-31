#include "server.h"


// Function to generate random agent data
void generate_agents(Agent agents[]) {
    for (int i = 0; i < AGENT_COUNT; i++) {
        agents[i].x = (float)(rand() % 100);
        agents[i].y = (float)(rand() % 100);
        agents[i].food = rand() % 50;
        for (int j = 0; j < 10; j++) {
            agents[i].genes[j] = rand() % 10;
        }
    }
}

void update_agents(Agent agents[]) {
    for (int i = 0; i < AGENT_COUNT; i++) {
        agents[i].x += (float)(rand() % 100) / 100.0f - 0.5f;
        agents[i].y += (float)(rand() % 100) / 100.0f - 0.5f;

        if (agents[i].x > 100.0f) {
            agents[i].x = 100.0f;
        } else if (agents[i].x < 0.0f) {
            agents[i].x = 0.0f;
        }

        if (agents[i].y > 100.0f) {
            agents[i].y = 100.0f;
        } else if (agents[i].y < 0.0f) {
            agents[i].y = 0.0f;
        }
    }
}