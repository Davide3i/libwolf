#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#include "libwebsockets.h"

#define PROTOCOL_NAME "property-protocol"
#define MAX_NAME_LENGTH 100
#define MAX_STRING_LENGTH 100
#define MAX_DESC_LENGTH 100

enum enum_type {Int, Float, String, Double};

typedef union {
    int Int;
    float Float;
    char* String;
    double Double;
    char byte[8];
} value_t;

typedef struct property {
    char* name;
    char* description;
    uint32_t id;
    enum enum_type type;
    value_t value;
    struct timeval timestamp;
    value_t (*callback)();
} property_t;

typedef struct property_list_node {
    property_t pr;
    struct property_list_node *p_prev;
    struct property_list_node *p_next;
} property_list_node_t;

typedef struct server_state
{

    uint32_t first_free_id;
    property_list_node_t *property_list_tail;
    struct libwebsocket_context *context;

} server_state_t;


//################ Function prototypes ################

//public API
int wolf_init(int port, char *web_path);
int wolf_start();
int wolf_run();
void wolf_close();
int register_property(property_t p);
int register_properties(property_t p[],int lenght);




