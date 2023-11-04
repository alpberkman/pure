
#include "ff.h"
#include "ff_interp.h"
#include "ff_init.h"
#include "ff_debug.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define streq(X, Y) (strcmp((char *)(X), (char *)(Y)) == 0)
#define update \
        (*((cell *) &(vm->ram[lp_addr]))) = lp; \
        (*((cell *) &(vm->ram[hp_addr]))) = hp;

extern cell hp;
extern cell lp;

extern cell hp_addr;
extern cell lp_addr;
extern cell st_addr;

byte buf[WORD_LEN+1];
cell len = 0;

void read() {
    int c;

    /*Remove white space*/
    while(isspace(c = getchar()));

    /*Get the string*/
    len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = toupper(c);
    } while(len < WORD_LEN && !isspace(c = getchar()));
    buf[len] = '\0';
}

cell find(VM *vm) {
    cell addr;
    byte flags;

    for(addr = lp; addr != 0; addr = *((cell *) &(vm->ram[addr]))) {
        flags = vm->ram[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp((char *) buf, (char *) &(vm->ram[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0)
        return 0;
    else
        return addr;
}


// postpone ' [']
void interp(VM *vm) {

    cell addr = find(vm);
    byte flags = vm->ram[addr + CELL_SIZE];
    if(addr != 0 && (flags & MASK_VIS)) {
        (*((cell *) &(vm->ram[0]))) = addr + CELL_SIZE + 1 + len;
        (*((cell *) &(vm->ram[CELL_SIZE]))) = HALT;
    } else if(streq(buf, "(")) {
        while(getchar() != ')');
    }  else if(streq(buf, "\\")) {
        while(getchar() != '\n');
    } else if(streq(buf, ":")) {
        (*((cell *) &(vm->ram[st_addr]))) = TTRUE;
        read();
        m_header(vm, (char *)buf, 0);
        /*
        m_word(vm, "[:]");
        *((cell *) &(vm->ram[hp])) =  hp - CELL_SIZE;
        hp += CELL_SIZE;
        */
        update;

    } else if(streq(buf, "IMMEDIATE")) {
        vm->ram[lp + CELL_SIZE] |= MASK_IMM;
        update;
    } else if(streq(buf, "\'")) {
        read();
        cell addr = find(vm);
        (*((cell *) &(vm->ram[0]))) = LIT;
        (*((cell *) &(vm->ram[CELL_SIZE]))) = addr ? addr + CELL_SIZE + 1 + len : 0;
        (*((cell *) &(vm->ram[CELL_SIZE*2]))) = HALT;
    } else if(streq(buf, "VARIABLE")) {
        read();
        m_var(vm, (char *)buf);
        update;
    } /* else if(streq(buf, "CONSTANT")) {
        read();
        m_const(vm, (char *)buf, vm->spu.ps[--vm->spu.psp]);
        update;
    } */ else {
        (*((cell *) &(vm->ram[0]))) = LIT;
        (*((cell *) &(vm->ram[CELL_SIZE]))) = atoi((char *)buf);
        (*((cell *) &(vm->ram[CELL_SIZE*2]))) = HALT;
    }
}

void compile(VM *vm) {

    cell addr = find(vm);
    byte flags = vm->ram[addr + CELL_SIZE];
    if(addr != 0 && (flags & MASK_VIS)) {
        if(flags & MASK_IMM) {
            (*((cell *) &(vm->ram[0]))) = addr + CELL_SIZE + 1 + len;
            (*((cell *) &(vm->ram[CELL_SIZE]))) = HALT;
        } else {
            m_word(vm, (char *)buf);
            update;
        }
    } else if(streq(buf, "(")) {
        while(getchar() != ')');
    }  else if(streq(buf, "\\")) {
        while(getchar() != '\n');
    } else if(streq(buf, ";")) {
        //m_word(vm, "[;]");
        vm->ram[lp + CELL_SIZE] |= MASK_VIS;
        m_word(vm, "EXIT");
        (*((cell *) &(vm->ram[st_addr]))) = FFALSE;
        update;
    } else if(streq(buf, "[\']")) {
        read();
        cell addr = find(vm);
        m_number(vm, addr ? addr + CELL_SIZE + 1 + len : 0);
        update;
    } else if(streq(buf, "::")) {
        // Should use find in the correct address
        read();
        //cell addr = find(vm, POP-FROM-STACK @ );
        //m_number(vm, addr ? addr + CELL_SIZE + 1 + len : 0);
        // if IMMEDIATE run else do what makes sense

        // basically ':' that doesn't start from last
        // but a given address
        // That way : : LAST :: ;
        update;
    } else if(streq(buf, "POSTPONE")) {
        read();
        cell addr = find(vm);
        if(addr != 0) {
            flags = vm->ram[addr + CELL_SIZE];
            if(flags & MASK_IMM) {
                m_word(vm, (char *)buf);
            } else {
                m_number(vm, addr);
                m_word(vm, "COMPILE,");
            }
            update;
        } else {
            printf("POSTPONE couldn't find the word: %s\n", buf);
        }
    } else {
        m_number(vm, atoi((char *)buf));
        update;
    }
}

void eval(VM *vm) {
    vm->spu.ip = 0;
    (*((cell *) &(vm->ram[0]))) = HALT;

    if(streq(buf, "DUMP")) {
        dump(vm, "ff.dump");
        carr(vm, "rom.h");
    } else if(streq(buf, "BYE")) {
        dump(vm, "dump.bin");
        carr(vm, "rom.h");
        exit(0);
    } else if(streq(buf, "L1")) {
        list1(vm);
    } else if(streq(buf, "L2")) {
        list2(vm);
    } else if((*((cell *) &(vm->ram[st_addr]))) == FFALSE) {
        interp(vm);
    } else {
        compile(vm);
    }
}


