#include <signal.h>
#include "wolf.h"

int run_stupid_loop=1;

value_t getCookiesProduction(){
    value_t val;
    val.Int = 2001;
    return val;
}

value_t getOvenTemperature(){
    value_t val;
    val.Float =(float)123.45678f;
    return val;
}

value_t getCookiesSize(){
    value_t val;
    val.Double=(double)123.45678;
    return val;
}

value_t getCookiesName(){
	value_t val;
	char cookiesName[20]="Super Cookies";
	val.String=cookiesName;
	return val;
}

value_t getAnswer(){
    value_t val;
    val.Int = 42;
    return val;
}


void sighandler(int sig){
	wolf_close();
	run_stupid_loop=0;
}


int main(int argc, char** argv){

	property_t prop1, p2, p3, p4,p5;

	if (wolf_init(7681,""))
        return -1;


    prop1.name="CookiesProduction";
    prop1.description="number of cookies produced in the last minute";
    prop1.type=Int;
    prop1.callback=getCookiesProduction;

    if (register_property(prop1))
        return -1;

    p2.name="OvenTemp";
    p2.description="cookies oven temperature";
    p2.type=Float;
    p2.callback=getOvenTemperature;

    if (register_property(p2))
        return -1;

    p3.name="Answer";
    p3.description="the answer for all";
    p3.type=Int;
    p3.callback=getAnswer;

    if (register_property(p3))
        return -1;


    p4.name="cookiesName";
    p4.description="name of the cookies";
    p4.type=String;
    p4.callback=getCookiesName;

    if (register_property(p4))
        return -1;


    p5.name="cookiesSize";
    p5.description="diameter of cookies";
    p5.type=Double;
    p5.callback=getCookiesSize;

    if (register_property(p5))
        return -1;

    signal(SIGINT, sighandler); //after this point Control+C make soft termination

    //bloccking function
    if (wolf_run())
        return -1;

    while(run_stupid_loop){
        sleep(1);
    }

	return 0;
}
