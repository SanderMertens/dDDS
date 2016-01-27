/* _load.c
 *
 * This file is generated. Do not modify.
 */

#include "corto/corto.h"
#include "WaitSet.h"

int main(int argc, char* argv[]) {
corto_start();
    if (corto_load("/dDDS", 0, NULL)) return -1;
    int WaitSetMain(int argc, char* argv[]);
    if (WaitSetMain(argc, argv)) return -1;
    corto_stop();
    return 0;
}

