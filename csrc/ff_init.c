
#include "ff.h"
#include "ff_init.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>


extern cell hp;
extern cell lp;

extern cell hp_addr;
extern cell lp_addr;
extern cell st_addr;


void m_header(VM *vm, char *name, unsigned char flag) {
    cell i;

    *((cell *) &(vm->ram[hp])) = *((cell *) &(lp));
    lp = hp;
    hp += CELL_SIZE;

    vm->ram[hp++] = strlen(name) | flag;

    for(i = 0; i < (cell) strlen(name); ++i)
        vm->ram[hp + i] = toupper(name[i]);
    hp += strlen(name);
}
void m_opcodes(VM *vm, char *opcodes, int size) {
    for(int i = 0; i < size; ++i)
        vm->ram[hp+i] = opcodes[i];
    hp += size;
}
void m_alloc(VM *vm, cell number) {
    hp += number;
    (void) vm;
}
void m_word(VM *vm, char *word) {
    cell addr;
    byte flags;
    cell len = strlen(word);
    char buf[32] = {0};

    for(unsigned int i = 0; i < strlen(word); ++i)
        buf[i] = toupper(word[i]);

    for(addr = lp; addr != 0; addr = *((cell *) &(vm->ram[addr]))) {
        flags = vm->ram[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->ram[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0) {
        printf("Error in m_word: word not found %s\n", buf);
        return;
    }

    *((cell *) &(vm->ram[hp])) =  addr + CELL_SIZE + 1 + len;
    hp += CELL_SIZE;
}
void m_number(VM *vm, cell num) {
    m_word(vm, "LIT");
    *((cell *) &(vm->ram[hp])) =  num;
    hp += CELL_SIZE;
}
/*
void m_const(VM *vm, char *name, cell num) {
    m_header(vm, name, MASK_VIS);
    m_word(vm, "[CONSTANT]");
    *((cell *) &(vm->ram[hp])) = num;
    hp += CELL_SIZE;
}
*/
void m_var(VM *vm, char *name) {
    m_header(vm, name, MASK_VIS);
    m_number(vm, hp+CELL_SIZE*3);
    m_word(vm, "EXIT");
    *((cell *) &(vm->ram[hp])) = 0;
    hp += CELL_SIZE;
}


void ff_base_words(VM *vm) {

    vm->spu.ip = 0;
    vm->spu.psp = 0;
    vm->spu.rsp = 0;
    for(int i = 0; i < MEM_SIZE; ++i)
        vm->ram[i] = 0;

    ALLOC(FF+256);


    // Special
    cell op_nop[] = {
        RET,
    };
    cell op_lit[] = {
        POP, DUP, CELL, ADD, PUSH, LDC, RET,
    };
    cell op_halt[] = {
        HALT,
    };

    HEADER(nop, MASK_VIS)
    OPCODES(op_nop);

    HEADER(lit, MASK_VIS);
    OPCODES(op_lit);

    HEADER(halt, MASK_VIS);
    OPCODES(op_halt);


    // Stack
    cell op_dup[] = {
        DUP, RET,
    };
    cell op_drop[] = {
        DROP, RET,
    };
    cell op_swap[] = {
        SWAP, RET,
    };
    cell op_push[] = {
        POP, SWAP, PUSH, PUSH, RET,
    };
    cell op_pop[] = {
        POP, POP, SWAP, PUSH, RET,
    };
    cell op_pick[] = {
        PICK, RET,
    };
    cell op_rick[] = {
        LIT, 1, ADD, RICK, RET,
    };

    HEADER(dup, MASK_VIS);
    OPCODES(op_dup);

    HEADER(drop, MASK_VIS);
    OPCODES(op_drop);

    HEADER(swap, MASK_VIS);
    OPCODES(op_swap);

    HEADER(R<, MASK_VIS);
    OPCODES(op_push);

    HEADER(R>, MASK_VIS);
    OPCODES(op_pop);

    HEADER(pick, MASK_VIS);
    OPCODES(op_pick);

    HEADER(rick, MASK_VIS);
    OPCODES(op_rick);


    // Controlflow
    cell op_jmp[] = {
        POP, DROP, JMP,
    };
    cell op_jz[] = {
        PUSH,
        CELL, LIT, 6, MUL, LDI, ADD, JZ,
        POP, DROP, POP, JMP,
        POP, POP, DROP, JMP,
    };
    cell op_ret[] = {
        POP, DROP, RET,
    };

    HEADER(jmp, MASK_VIS);
    OPCODES(op_jmp);

    HEADER(jz, MASK_VIS);
    OPCODES(op_jz);

    HEADER(exit, MASK_VIS);
    OPCODES(op_ret);


    // Logic
    cell op_eq[] = {
        EQ, RET,
    };
    cell op_neq[] = {
        NEQ, RET,
    };
    cell op_gt[] = {
        GT, RET,
    };
    cell op_lt[] = {
        LT, RET,
    };
    cell op_and[] = {
        AND, RET,
    };
    cell op_or[] = {
        OR, RET,
    };
    cell op_xor[] = {
        XOR, RET,
    };
    cell op_shr[] = {
        SHR, RET,
    };
    cell op_shl[] = {
        SHL, RET,
    };
    cell op_tru[] = {
        TRU, RET,
    };
    cell op_fls[] = {
        FLS, RET,
    };

    HEADER(=, MASK_VIS);
    OPCODES(op_eq);

    HEADER(<>, MASK_VIS);
    OPCODES(op_neq);

    HEADER(>, MASK_VIS);
    OPCODES(op_gt);

    HEADER(<, MASK_VIS);
    OPCODES(op_lt);

    HEADER(and, MASK_VIS);
    OPCODES(op_and);

    HEADER(or, MASK_VIS);
    OPCODES(op_or);

    HEADER(xor, MASK_VIS);
    OPCODES(op_xor);

    HEADER(>>, MASK_VIS);
    OPCODES(op_shr);

    HEADER(<<, MASK_VIS);
    OPCODES(op_shl);

    HEADER(true, MASK_VIS);
    OPCODES(op_tru);

    HEADER(false, MASK_VIS);
    OPCODES(op_fls);


    // Arithmetic
    cell op_add[] = {
        ADD, RET,
    };
    cell op_sub[] = {
        SUB, RET,
    };
    cell op_mul[] = {
        MUL, RET,
    };
    cell op_div[] = {
        DIV, RET,
    };
    cell op_mod[] = {
        MOD, RET,
    };

    HEADER(+, MASK_VIS);
    OPCODES(op_add);

    HEADER(-, MASK_VIS);
    OPCODES(op_sub);

    HEADER(*, MASK_VIS);
    OPCODES(op_mul);

    HEADER(/, MASK_VIS);
    OPCODES(op_div);

    HEADER(%, MASK_VIS);
    OPCODES(op_mod);


    // Memory
    cell op_ldc[] = {
        LDC, RET,
    };
    cell op_strc[] = {
        STRC, RET,
    };
    cell op_ldb[] = {
        LDB, RET,
    };
    cell op_strb[] = {
        STRB, RET,
    };
    cell op_cell[] = {
        CELL, RET,
    };
    cell op_byte[] = {
        BYTE, RET,
    };
    cell op_mem[] = {
        MEM, RET,
    };

    HEADER(@, MASK_VIS);
    OPCODES(op_ldc);

    HEADER(!, MASK_VIS);
    OPCODES(op_strc);

    HEADER(C@, MASK_VIS);
    OPCODES(op_ldb);

    HEADER(C!, MASK_VIS);
    OPCODES(op_strb);

    HEADER(CELL, MASK_VIS);
    OPCODES(op_cell);

    HEADER(BYTE, MASK_VIS);
    OPCODES(op_byte);

    HEADER(MEM, MASK_VIS);
    OPCODES(op_mem);


    // Special
    cell op_ldp[] = {
        LDP, RET,
    };
    cell op_strp[] = {
        STRP, RET,
    };
    cell op_ldr[] = {
        LDR, RET,
    };
    cell op_strr[] = {
        STRR, RET,
    };
    cell op_ldi[] = {
        POP, DUP, PUSH, RET,
    };
    cell op_stri[] = {
        STRI, RET,
    };

    HEADER(PSP@, MASK_VIS);
    OPCODES(op_ldp);

    HEADER(PSP!, MASK_VIS);
    OPCODES(op_strp);

    HEADER(RSP@, MASK_VIS);
    OPCODES(op_ldr);

    HEADER(RSP!, MASK_VIS);
    OPCODES(op_strr);

    HEADER(IP@, MASK_VIS);
    OPCODES(op_ldi);

    HEADER(IP!, MASK_VIS);
    OPCODES(op_stri);


    // IO
    cell op_key[] = {
        KEY, RET,
    };
    cell op_emit[] = {
        EMIT, RET,
    };

    HEADER(key, MASK_VIS);
    OPCODES(op_key);

    HEADER(emit, MASK_VIS);
    OPCODES(op_emit);


    // Call
    cell op_call[] = {
        POP, DROP, PUSH, RET,
    };

    HEADER(EXECUTE, MASK_VIS);
    OPCODES(op_call);
}

void ff_high_words(VM *vm) {
    VAR(HP);
    hp_addr = hp - CELL_SIZE;
    VAR(LP);
    lp_addr = hp - CELL_SIZE;
    VAR(STATE);
    st_addr = hp - CELL_SIZE;

    /* If needed other words in C

    */

    SET(hp_addr, hp);
    SET(lp_addr, lp);
    SET(st_addr, FFALSE);
}

void ff_init_words(VM *vm) {
    ff_base_words(vm);
    ff_high_words(vm);
}








