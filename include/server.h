#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  // For va_list in log_callback
#include <microhttpd.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define PORT 5000
#define AGENT_COUNT 30
#define FOOD_TILES_ACROSS 50

// Struct definition for Agent
typedef struct {
    float x;
    float y;
    int food;
    float genes[10];
} Agent;

// Declare the global arrays
extern Agent agents[AGENT_COUNT];

extern int food_grid[FOOD_TILES_ACROSS][FOOD_TILES_ACROSS];





// Function prototypes
void generate_agents(Agent agents[]);
void initialize_food_grid(int food_grid[][FOOD_TILES_ACROSS]);
void update_agents(Agent agents[]);
void *agent_updater(void *arg);
void generate_json(char *buffer, size_t size, Agent agents[], int count);
enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method, const char *version,
                                const char *upload_data, size_t *upload_data_size, void **con_cls);
void log_callback(void *cls, const char *fmt, va_list ap);
void panic_callback(void *cls, const char *file, unsigned int line, const char *reason);


#endif // SERVER_H
