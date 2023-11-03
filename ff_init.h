
#include "ff.h"


#define STR(X) #X
#define S(X) STR(X)

#define HEADER(name, flag) m_header(vm, STR(name), (flag));
#define OPCODES(ops) m_opcodes(vm, (char *) ops, sizeof(ops));
#define ALLOC(num) m_alloc(vm, num);
#define WORD(name) m_word(vm, STR(name));
#define NUMBER(num) m_number(vm, num);
#define CONST(name, num) m_const(vm, STR(name), num);
#define VAR(name) m_var(vm, STR(name));
#define VAR2(name, num) m_var2(vm, STR(name), num);
#define SET(ADDR, NUM) *((cell *) &(vm->ram[ADDR])) = NUM;
#define H(name, flag) HEADER(name, flag); WORD([:]);
#define T() WORD([;]);


void m_header(VM *vm, char *name, unsigned char flag);
void m_opcodes(VM *vm, char *opcodes, int size);
void m_alloc(VM *vm, cell number);
void m_word(VM *vm, char *word);
void m_number(VM *vm, cell num) ;
void m_const(VM *vm, char *name, cell num);
void m_var(VM *vm, char *name);
void m_var2(VM *vm, char *name, cell num);

void ff_base_words(VM *vm);
void ff_high_words(VM *vm);
void ff_init_words(VM *vm);


