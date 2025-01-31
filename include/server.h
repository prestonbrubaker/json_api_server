#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 5000
#define AGENT_COUNT 5

// Struct definition for agents
typedef struct {
    float x;
    float y;
    int food;
    int genes[10];
} Agent;

// Global array of agents
extern Agent agents[AGENT_COUNT];

// Function declarations
void generate_agents(Agent agents[], int count);
void generate_json(char *buffer, size_t size, Agent agents[], int count);
static int request_handler(void *cls, struct MHD_Connection *connection,
                           const char *url, const char *method, const char *version,
                           const char *upload_data, size_t *upload_data_size, void **con_cls);

#endif // SERVER_H
