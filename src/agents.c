#include "server.h"


void initialize_food_grid(int food_grid[][FOOD_TILES_ACROSS]) {
    for (int x=0; x<FOOD_TILES_ACROSS;x++) {
        for (int y=0;y<FOOD_TILES_ACROSS;y++) {
            food_grid[x][y] = 100;
        }
    }
}


void generate_agents(Agent agents[]) {
    for (int i = 0; i < AGENT_COUNT; i++) {
        agents[i].x = (float)(rand() % 100);
        agents[i].y = (float)(rand() % 100);
        agents[i].food = rand() % 50;
        for (int j = 0; j < 10; j++) {
            agents[i].genes[j] = (float)(rand() % 100) / 100.0f;
        }
    }
}

void update_agents(Agent agents[]) {
    for (int i = 0; i < AGENT_COUNT; i++) {

        int agent_index_x = (int)(agents[i].x / (100.0f / FOOD_TILES_ACROSS));
        int agent_index_y = (int)(agents[i].y / (100.0f / FOOD_TILES_ACROSS));

        
        if (food_grid[agent_index_x][agent_index_y] > 0) {
            food_grid[agent_index_x][agent_index_y]--;
            if (agents[i].food < 49) {
                agents[i].food++;
            }
            
        }

        if ((float)(rand() % 100) / 100.0f < agents[i].genes[0]) {
            agents[i].x += (float)(rand() % 100) / 100.0f - 0.5f;
        }

        if ((float)(rand() % 100) / 100.0f < agents[i].genes[1]) {
            agents[i].y += (float)(rand() % 100) / 100.0f - 0.5f;
        }

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

        if (rand() % 10 == 0 && agents[i].food > 0) {
            agents[i].food--;
        }
        if (rand() % 2 == 0) {
            int rand_x = rand() % FOOD_TILES_ACROSS;
            int rand_y = rand() % FOOD_TILES_ACROSS;
            if (food_grid[rand_x][rand_y] < 100) {
                food_grid[rand_x][rand_y]++;
            }
        }
    }
}