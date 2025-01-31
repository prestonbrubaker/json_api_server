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

        if (agent_index_x >= FOOD_TILES_ACROSS) agent_index_x = FOOD_TILES_ACROSS - 1;
        if (agent_index_y >= FOOD_TILES_ACROSS) agent_index_y = FOOD_TILES_ACROSS - 1;

        
        if (food_grid[agent_index_x][agent_index_y] > 0) {
            food_grid[agent_index_x][agent_index_y]--;
            if (agents[i].food < 10000) {
                agents[i].food++;
            }
            
        }

        if ((float)(rand() % 1000) / 1000.0f < agents[i].genes[0]) {
            agents[i].x += 0.3f * ((float)(rand() % 10000) / 10000.0f - 0.5f);
        }

        if ((float)(rand() % 1000) / 1000.0f < agents[i].genes[1]) {
            agents[i].y += 0.3f * ((float)(rand() % 10000) / 10000.0f - 0.5f);
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

        if (rand() % 100 < 25 && agents[i].food > 0) {
            agents[i].food = (int)( ((float)(agents[i].food)) * 0.9999f);
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

    if (rand() % 1000 == 0) {
        int lowest_food = 100000;
        int lowest_food_index = 0;
        for (int i = 0; i < AGENT_COUNT; i++) {
            if (agents[i].food < lowest_food) {
                lowest_food = agents[i].food;
                lowest_food_index = i;
            }
        }
        int k = rand() % 10;
        int j = rand() % AGENT_COUNT;
        for (int i=0;i<10;i++) {
            
            agents[lowest_food_index].genes[i] = agents[j].genes[i];
            if (i==k && rand() % 2) {
                agents[lowest_food_index].genes[i] += (0.01f * 2.0f * ((float)(rand() % 10000) / 10000.0f - 0.5f));
                if (agents[lowest_food_index].genes[i] > 1.0f) {
                    agents[lowest_food_index].genes[i] = 1.0f;
                } else if (agents[lowest_food_index].genes[i] < 0.0f) {
                    agents[lowest_food_index].genes[i] = 0.0f;
                }

                if (rand() % 2 == 0) {
                    agents[lowest_food_index].genes[i] = (float)(rand() % 1000) / 1000.0f;
                }
            }

            
        }

        agents[lowest_food_index].food = 100;
    }

}