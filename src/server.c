#include "server.h"

Agent agents[AGENT_COUNT];

int food_grid[FOOD_TILES_ACROSS][FOOD_TILES_ACROSS];

// Function to convert agent data to JSON
void generate_json(char *buffer, size_t size, Agent agents[], int count) {
    snprintf(buffer, size, "{ \"agents\": [");
    size_t offset = strlen(buffer);

    for (int i = 0; i < count; i++) {
        char agent_json[1024];
        snprintf(agent_json, sizeof(agent_json),
                 "{ \"x\": %.2f, \"y\": %.2f, \"food\": %d, \"genes\": [",
                 agents[i].x, agents[i].y, agents[i].food);

        strcat(buffer + offset, agent_json);
        offset += strlen(agent_json);

        for (int j = 0; j < 10; j++) {
            char gene[16];
            snprintf(gene, sizeof(gene), "%.2f%s", agents[i].genes[j], (j < 9) ? ", " : "");
            strcat(buffer + offset, gene);
            offset += strlen(gene);
        }

        strcat(buffer + offset, "] }");
        if (i < count - 1) {
            strcat(buffer + offset, ", ");
        }
        offset += strlen(buffer + offset);
    }

    strcat(buffer + offset, "], \"food_grid\": [");
    offset += strlen(buffer);

    // Add food grid data
    for (int i = 0; i < FOOD_TILES_ACROSS; i++) {
        strcat(buffer + offset, "[");
        offset += strlen(buffer);

        for (int j = 0; j < FOOD_TILES_ACROSS; j++) {
            char food_value[12];
            snprintf(food_value, sizeof(food_value), "%d%s", food_grid[i][j], (j < FOOD_TILES_ACROSS - 1) ? ", " : "");
            strcat(buffer + offset, food_value);
            offset += strlen(food_value);
        }

        strcat(buffer + offset, (i < FOOD_TILES_ACROSS - 1) ? "], " : "]");
        offset += strlen(buffer + offset);
    }

    strcat(buffer + offset, "] }");
}


// Function to handle HTTP requests
enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                const char *url, const char *method, const char *version,
                                const char *upload_data, size_t *upload_data_size, void **con_cls) {
    // Ensure non-null values to prevent crashes
    if (!method) method = "UNKNOWN";
    if (!url) url = "/";
    if (!version) version = "HTTP/1.1";

    char json_response[4096];
    generate_json(json_response, sizeof(json_response), agents, AGENT_COUNT);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json_response),
                                                                    (void *)json_response,
                                                                    MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return (ret == MHD_YES) ? MHD_YES : MHD_NO;
}


// Function to handle logging (fixing old-style function declaration)
void log_callback(void *cls, const char *fmt, va_list ap) {
    (void)cls;  // Avoid unused parameter warning
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}


void panic_callback(void *cls, const char *file, unsigned int line, const char *reason) {
    (void)cls; // Avoid unused parameter warning
    fprintf(stderr, "[libmicrohttpd] Panic in %s:%u: %s\n", file, line, reason);
}

void *agent_updater(void *arg) {
    (void)arg;  // Unused parameter
    while (1) {
        update_agents(agents);
        usleep(100000);  // Sleep for 100ms (100,000 microseconds)
    }
    return NULL;
}

int main() {

    srand(time(NULL));
    initialize_food_grid(food_grid);
    generate_agents(agents);

    // Create a thread for updating agents
    pthread_t updater_thread;
    if (pthread_create(&updater_thread, NULL, agent_updater, NULL) != 0) {
        fprintf(stderr, "Error creating updater thread\n");
        return 1;
    }

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
                              PORT, NULL, NULL, &request_handler, NULL, MHD_OPTION_END);

    if (NULL == daemon) {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }

    // Attach log function correctly
    MHD_set_panic_func(panic_callback, NULL);  // Fix: Corrected function name

    printf("Server running on http://0.0.0.0:%d (Externally accessible)\n", PORT);
    printf("Press Enter to stop...\n");

    getchar();  // Wait for user input to terminate
    MHD_stop_daemon(daemon);

    return 0;
}
