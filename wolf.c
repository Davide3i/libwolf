#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "libwebsockets.h"

#define PROTOCOL_NAME "property-protocol"
#define MAX_NAME_LENGTH 100
#define MAX_STRING_LENGTH 100
#define MAX_DESC_LENGTH 100

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

property_list_node_t *search_by_id(uint32_t id);
int get_property(uint32_t id, property_t *p);
value_t get_value(property_list_node_t *node);

int server_initialize(int port);
static int callback_http(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len); //manage the http protocol
static int callback_property_protocol(struct libwebsocket_context *context, struct libwebsocket *wsi,	enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);//manage the oneproperty protocol
void server(struct libwebsocket_context *context);
int sendMessage(struct libwebsocket *wsi, unsigned char *message, size_t lenght);

void get_Query_Decoder(struct libwebsocket *wsi, char *in, size_t len);
void message_get_update_encoder(property_t p, struct libwebsocket *wsi);

void get_dispatcher(uint32_t id, struct libwebsocket *wsi);

//public API
int wolf_init(int port, char *web_path);
int register_property(property_t p);
int register_properties(property_t p[]);
int wolf_start();
void wolf_close();


void init();

//Aux function for property list management
int insert(property_list_node_t **ptail, property_t p);
property_list_node_t* search_id(uint32_t id, property_list_node_t *ptail);
void free_property_list(property_list_node_t *tail);





//################ Global variables ################

server_state_t state;

int wolf_server_stop;

void init(){
    state.first_free_id = 1;
    state.property_list_tail = NULL;
}



static struct libwebsocket_protocols protocols[] =   //list of allowed protocols
{

    // First protocol must always be HTTP handler.
    {
        "http-only",		// name
        callback_http,		// callback
        0,       // per_session_data_size
        0,			// max frame size / rx buffer
    },

    {
        PROTOCOL_NAME,
        callback_property_protocol,
        0,
        512,
    },

    { NULL, NULL, 0, 0 }//terminator

};




//################ Function definition ################
int register_property(property_t p)
{
    p.id = state.first_free_id;
    state.first_free_id++;
    return insert(&(state.property_list_tail), p);
}

int register_properties(property_t p[])
{

    int i;
    for (i = 0; p[i].name != NULL; i++)   //TODO qualcosa di più safe del nome a null!
    {
        register_property(p[i]);
    }

}

property_list_node_t* search_by_id(uint32_t id)
{
    property_list_node_t *found;
    found = search_id(id, state.property_list_tail);
    return found;
}

value_t get_value(property_list_node_t *found)
{

    value_t (*callback)();

    callback = found->pr.callback;

    return callback();

}

int get_property(uint32_t id, property_t* p)
{

    property_list_node_t *found;
    found = search_by_id(id);

    if(found != NULL)
    {

        p->id = found->pr.id;
        p->name = found->pr.name;
        p->description = found->pr.description;
        p->type = found->pr.type;
        p->value = get_value(found);
        gettimeofday(&(p->timestamp), NULL);

        return 1;

    }
    else
    {
        return -1;
    }

}




int wolf_init(int port, char *web_path)
{
    wolf_server_stop = 0;
    state.first_free_id = 1;
    state.property_list_tail = NULL;

    if (server_initialize(port))
    {
        return -1;
    }

    return 0;

}

int wolf_start()
{

    server(state.context);

    return 0;

}

void wolf_close()
{

    wolf_server_stop = 1;
    libwebsocket_context_destroy(state.context);
    free_property_list(state.property_list_tail);

}




int server_initialize(int port)
{

    struct lws_context_creation_info info;

    memset(&info, 0, sizeof(info));
    info.port = port;
    info.iface = NULL;
    info.protocols = protocols;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    state.context = libwebsocket_create_context(&info);
    if (state.context == NULL)
    {
        return -1;
    }

    return 0;

}

void server(struct libwebsocket_context *context)
{
    int server_error=0;
    while ((wolf_server_stop == 0 )&&(server_error ==0))
    {
        server_error = libwebsocket_service(context, 200);
    }

}

int sendMessage(struct libwebsocket *wsi, unsigned char *message,size_t lenght)   // NOTICE: MESSAGE PAYLOAD 512 BYTE
{

    int byte_sended;
    unsigned char buffer[LWS_SEND_BUFFER_PRE_PADDING + 512 + LWS_SEND_BUFFER_POST_PADDING]; // NOTICE: MESSAGE PAYLOAD 512 BYTE
    unsigned char *start_message_pointer = &buffer[LWS_SEND_BUFFER_PRE_PADDING];

    memcpy(start_message_pointer, message, lenght);

    byte_sended = libwebsocket_write(wsi, start_message_pointer, lenght,LWS_WRITE_BINARY);

    if (byte_sended < lenght)
    {
        return -1;
    }

    return 0;

}

static int callback_http(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{
    char *path;
    //TODO dinamico
    char fullpath[200]="";
    char folder[200]="webfiles";
    char pathcopy[200]="";
    char* extension;


    switch (reason)
    {

    case LWS_CALLBACK_HTTP:

        // For security reasons block all requests for ../filename //TODO TEST
        path = (char*)in;
        if ((char)(path[0]) == '.')
        {
            break;
        }

        strcpy(pathcopy,path);
        strtok(pathcopy,".");
        extension=strtok(NULL,".");

        strcat(fullpath,folder);
        strcat(fullpath,path);

        if (libwebsockets_serve_http_file(context, wsi,fullpath,extension_to_mime_type(extension)))// es "text/html"
        {
            return -1; // Through completion or error, close the socket.
        }


        // NOTICE: sending of the file completes asynchronously.
        // We'll get a LWS_CALLBACK_HTTP_FILE_COMPLETION callback when it's done.

        break;


    case LWS_CALLBACK_HTTP_FILE_COMPLETION:

        return -1; // Kill the connection after we sent one file.

    }

    return 0;

}

static int callback_property_protocol(struct libwebsocket_context *context, struct libwebsocket *wsi,	enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{

    char *message;

    switch (reason)
    {

    case LWS_CALLBACK_RECEIVE:
        message = (char*)in;
        //TODO remove
        lwsl_notice("Received query");
        //TODO sostitire con  get_Query_Decoder(wsi, message, len); quando funzionerà bene
        get_dispatcher(0, wsi);
        break;

    }

    return 0;

}




void get_Query_Decoder(struct libwebsocket *wsi, char *in, size_t len)
{

    unsigned int id;

    id = byte_to_int(in + 1);
    get_dispatcher(id, wsi);

}

void message_get_update_encoder(property_t p,struct libwebsocket *wsi)
{

    //TODO definire un massimo
    char buffer[400];

    int offset;
    int i;

    // Message Type, 1 byte
    buffer[0] = 2;

    // ID, 4 byte
    int_to_byte(buffer + 1, p.id);

    i = strlen(p.name)+1; // 1 more for terminator character
    //TODO if(i>MAXNAMELENGHT) return -
    buffer[5] = (char)i; // Name Lenght

    //TODO snprintf
    snprintf(buffer + 6,MAX_NAME_LENGTH, "%s", p.name); // Name
    offset = i + 6;

    switch(p.type)   // Value Type, 1 byte + Value
    {

    case Int:
        //Type
        buffer[offset++] = 1;
        //Value
        int_to_byte(buffer + offset, p.value.Int);
        offset += 4;
        break;

    case Float:
        //Type
        buffer[offset++] = 2;
        //Value //TODO test
        buffer[offset++] = p.value.byte[0];
        buffer[offset++] = p.value.byte[1];
        buffer[offset++] = p.value.byte[2];
        buffer[offset++] = p.value.byte[3];
        break;

    case String:
        //Type
        buffer[offset++] = 3;

        //String length
        i=strlen(p.value.String)+1; // 1 more for terminator character
        buffer[offset++] = i;

        //Value
        snprintf(buffer + offset,MAX_STRING_LENGTH, "%s", p.value.String);
        offset+=i;

        break;
    }

    i = strlen(p.description)+1; // 1 more for terminator character
    buffer[offset++] = i; // Description Lenght

    snprintf(buffer + offset,MAX_DESC_LENGTH, "%s", p.description); // Description //TODO snprintf
    offset += i;

    time_to_byte(p.timestamp, buffer + offset); // SecTimestamp, 4 byte + uSecTimestamp, 4 byte
    offset+=8;

    sendMessage(wsi,buffer,offset);
}




void get_dispatcher(uint32_t id,struct libwebsocket *wsi)
{
    int i;

    if(id==0){
        for(i=1;i<state.first_free_id;i++)
            get_dispatcher(i,wsi);
    }else{
        property_t p;
        get_property(id,&p);
        message_get_update_encoder(p,wsi);
    }
}



//--- Aux functions for property list management ---


int insert(property_list_node_t **ptail, property_t p){
    property_list_node_t *pnew;

    pnew = malloc(sizeof(property_list_node_t));
    if(pnew == NULL)
        return 1;

    pnew->p_prev = *ptail;
    pnew->p_next = NULL;
    pnew->pr = p;
    if(*ptail != NULL)
        (*ptail)->p_next = pnew;

    *ptail = pnew;
    return 0;
}

void free_property_list(property_list_node_t *tail){
    if(tail == NULL)
        return;
    free_property_list(tail->p_prev);
    free(tail);
}

property_list_node_t* search_id(uint32_t id, property_list_node_t *ptail)
{
    property_list_node_t *ptemp;

    for(ptemp = ptail; ptemp != NULL; ptemp = ptemp->p_prev)
        if(ptemp->pr.id == id)
            return ptemp;
    return NULL;
}
