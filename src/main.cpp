#include "Controller.h"


__attribute__((constructor))
static void init(void)
{
    Controller::Run();
    printf("controller started\n");
}
