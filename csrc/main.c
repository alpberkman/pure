#include <stdio.h>
#include <stdlib.h>


#include "ff.h"
cell hp;
cell lp;
cell hp_addr;
cell lp_addr;
cell st_addr;
#include "ff_init.h"
#include "ff_debug.h"
#include "ff_interp.h"

#define streq(X, Y) (strcmp((X), (Y)) == 0)
#define pint(X) printf("%i\n", X)


int main(/*int argc, char *argv[]*/) {
    VM X;
    X.ram = malloc(MEM_SIZE);
    ff_init_words(&X);
    list1(&X);
    list2(&X);
    stacks(&X);
    //exec(&X, ADD);
    //stacks(&X);
    dump(&X, "ff.dump");
    carr(&X, "rom.h");

    for(;;) {
        //printf("LP: %i, HP: %i >>> ", (*((cell *) &(X.mem[lp_addr]))), (*((cell *) &(X.mem[hp_addr]))));
        //printf(">>> ");
        stacks(&X);
        printf(">>> ");
        fflush(stdout);
        read();
        puts("");
        eval(&X);
        run(&X);
//        stacks2(&X);
        lp = (*((cell *) &(X.ram[lp_addr])));
        hp = (*((cell *) &(X.ram[hp_addr])));
    }

    return 0;
}
