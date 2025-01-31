#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 5001
#define AGENT_COUNT 5

typedef struct {
    float x;
    float y;
    int food;
    int genes[10];
} Agent;

Agent agents[AGENT_COUNT];

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

// Function to convert agent data to JSON
void generate_json(char *buffer, size_t size, Agent agents[], int count) {
    snprintf(buffer, size, "{ \"agents\": [");
    size_t offset = strlen(buffer);

    for (int i = 0; i < count; i++) {
        char agent_json[512];
        snprintf(agent_json, sizeof(agent_json),
                 "{ \"x\": %.2f, \"y\": %.2f, \"food\": %d, \"genes\": [",
                 agents[i].x, agents[i].y, agents[i].food);

        strcat(buffer + offset, agent_json);
        offset += strlen(agent_json);

        for (int j = 0; j < 10; j++) {
            char gene[16];
            snprintf(gene, sizeof(gene), "%d%s", agents[i].genes[j], (j < 9) ? ", " : "");
            strcat(buffer + offset, gene);
            offset += strlen(gene);
        }

        strcat(buffer + offset, (i < count - 1) ? "] }, " : "] }");
        offset += strlen(buffer + offset);
    }

    strcat(buffer + offset, "] }");
}

// HTTP response handler
static int request_handler(void *cls, struct MHD_Connection *connection,
                           const char *url, const char *method, const char *version,
                           const char *upload_data, size_t *upload_data_size, void **con_cls) {
    (void)cls; (void)url; (void)method; (void)version;
    (void)upload_data; (void)upload_data_size; (void)con_cls;

    generate_agents(agents, AGENT_COUNT);

    char json_response[4096];
    generate_json(json_response, sizeof(json_response), agents, AGENT_COUNT);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json_response), (void *)json_response, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
                              PORT, NULL, NULL, &request_handler, NULL, MHD_OPTION_END);
    
    if (NULL == daemon) {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }

    printf("Server running on http://0.0.0.0:%d (Externally accessible)\n", PORT);
    printf("Press Enter to stop...\n");

    getchar();  // Wait for user input to terminate
    MHD_stop_daemon(daemon);

    return 0;
}
