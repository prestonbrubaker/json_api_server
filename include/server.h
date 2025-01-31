#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 5000
#define AGENT_COUNT 5

// Struct definition for Agent
typedef struct {
    float x;
    float y;
    int food;
    int genes[10];
} Agent;

// Extern declaration of global agents array (DO NOT REDEFINE in server.c)
extern Agent agents[AGENT_COUNT];

// Function prototypes
void generate_agents(Agent agents[], int count);
void generate_json(char *buffer, size_t size, Agent agents[], int count);
void log_callback(void *cls, const char *fmt, va_list ap)

// Marking request_handler as `MHD_AccessHandlerCallback` for compatibility
enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method, const char *version,
                                const char *upload_data, size_t *upload_data_size, void **con_cls);

#endif // SERVER_H
