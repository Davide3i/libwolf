#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "libwebsockets.h"

#define PROTOCOL_NAME "oneproperty-protocol"

enum enum_type {Int, Float, String};

typedef union {
    int Int;
    float Float;
    char* String;
    char byte[4];
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
int register_property(property_t p);
int register_properties(property_t p[]);
int wolf_start();
void wolf_close();

property_list_node_t *search_by_id(uint32_t id);
int get_property(uint32_t id, property_t *p);
value_t get_value(property_list_node_t *node);

int server_initialize(int port);
static int callback_http(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len); //manage the http protocol
static int callback_oneproperty(struct libwebsocket_context *context, struct libwebsocket *wsi,	enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);//manage the oneproperty protocol
void server(struct libwebsocket_context *context);
int sendMessage(struct libwebsocket *wsi, unsigned char *message, size_t lenght);

void get_Query_Decoder(struct libwebsocket *wsi, char *in, size_t len);
void message_get_update_encoder(property_t p, struct libwebsocket *wsi);

void get_dispatcher(uint32_t id, struct libwebsocket *wsi);

int insert(property_list_node_t **ptail, property_t p);
property_list_node_t* search_id(uint32_t id, property_list_node_t *ptail);
void free_property_list(property_list_node_t *tail);




