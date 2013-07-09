#include <signal.h>
#include "wolf.h"

value_t getCookiesProduction(){
    value_t val;
    val.Int = 2001;
    return val;
}

value_t getOvenTemperature(){
    value_t val;
    val.Float = 180.88;
    return val;
}

void sighandler(int sig){
	wolf_close();
}

 //TODO testare anche int register_properties(property p[]);
int main(int argc, char** argv){

	property_t p;
	property_list_node_t *temp;

	if (wolf_init(7681,""))
        return -1;

    p.name="CookiesProduction";
    p.description="number of cookies produced in the last minute";
    p.type=Int;
    p.callback=getCookiesProduction;

    if (register_property(p))
        return -1;


    p.name="OvenTemp";
    p.description="cookies oven temperature";
    p.type=Float;
    p.callback=getOvenTemperature;

    if (register_property(p))
        return -1;


    signal(SIGINT, sighandler); //after this point Control+C make soft termination

    //bloccking function
    if (wolf_start())
        return -1;

	return 0;
}
