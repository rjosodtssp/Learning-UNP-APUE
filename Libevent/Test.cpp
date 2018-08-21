#include <iostream>
#include <event2/event.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main()
{
    struct event_base* base;
    base = event_base_new();
    int i;
    const char **methods = event_get_supported_methods();
    printf("starting %s\nmethods are:\n",event_get_version());
    for(i=0;methods[i]!=NULL;i++)
    {
        printf("   %s\n",methods[i]);
    }
    event_base_dispatch(base);
    event_base_free(base);
    return 0;
}