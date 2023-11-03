
#include "ff.h"

#include <stdio.h>


void _nop(VM *vm) {
    (void) vm;
}
void _lit(VM *vm) {
    cell val = *((cell *) &(vm->ram[vm->spu.ip]));
    vm->spu.ps[vm->spu.psp++] = val;
    vm->spu.ip += CELL_SIZE;
}
void _halt(VM *vm) {
    vm->spu.p = OFF;
}
void _dup(VM *vm) {
    cell val = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = val;
    vm->spu.ps[vm->spu.psp++] = val;
}
void _drop(VM *vm) {
    --vm->spu.psp;
}
void _swap(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = a;
    vm->spu.ps[vm->spu.psp++] = b;
}
void _push(VM *vm) {
    vm->spu.rs[vm->spu.rsp++] = vm->spu.ps[--vm->spu.psp];
}
void _pop(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = vm->spu.rs[--vm->spu.rsp];
}
void _pick(VM *vm) {
    byte n = vm->spu.ps[--vm->spu.psp] + 1;
    n = vm->spu.psp - n;
    vm->spu.ps[vm->spu.psp++] = vm->spu.ps[n];
}
void _rick(VM *vm) {
    byte n = vm->spu.ps[--vm->spu.psp] + 1;
    n = vm->spu.rsp - n;
    vm->spu.ps[vm->spu.psp++] = vm->spu.rs[n];
}
void _jmp(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    vm->spu.ip = addr;
}
void _jz(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    cell flag = vm->spu.ps[--vm->spu.psp];
    if(flag == ((cell) FFALSE))
        vm->spu.ip = addr;
}
void _ret(VM *vm) {
    cell addr = vm->spu.rs[--vm->spu.rsp];
    vm->spu.ip = addr;
}
void _eq(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b == a ? TTRUE : FFALSE;
}
void _neq(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b != a ? TTRUE : FFALSE;
}
void _gt(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b > a ? TTRUE : FFALSE;
}
void _lt(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b < a ? TTRUE : FFALSE;
}
void _and(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b & a;
}
void _or(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b | a;
}
void _xor(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b ^ a;
}
void _shr(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b >> a;
}
void _shl(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b << a;
}
void _tru(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = TTRUE;
}
void _fls(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = FFALSE;
}
void _add(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b + a;
}
void _sub(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b - a;
}
void _mul(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b * a;
}
void _div(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b / a;
}
void _mod(VM *vm) {
    cell a = vm->spu.ps[--vm->spu.psp];
    cell b = vm->spu.ps[--vm->spu.psp];
    vm->spu.ps[vm->spu.psp++] = b % a;
}
void _ldc(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    cell val = *((cell *) &(vm->ram[addr]));
    vm->spu.ps[vm->spu.psp++] = val;
}
void _strc(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    cell val = vm->spu.ps[--vm->spu.psp];
    *((cell *) &(vm->ram[addr])) = val;
}
void _ldb(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    byte val = vm->ram[addr];
    vm->spu.ps[vm->spu.psp++] = val;
}
void _strb(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    byte val = vm->spu.ps[--vm->spu.psp];
    vm->ram[addr] = val;
}
void _cell(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = CELL_SIZE;
}
void _byte(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = BYTE_SIZE;
}
void _mem(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = MEM_SIZE;
}
void _ldp(VM *vm) {
    byte val = vm->spu.psp;
    vm->spu.ps[vm->spu.psp++] = val;
}
void _strp(VM *vm) {
    byte val = vm->spu.ps[--vm->spu.psp];
    vm->spu.psp = val;
}
void _ldr(VM *vm) {
    byte val = vm->spu.rsp;
    vm->spu.ps[vm->spu.psp++] = val;
}
void _strr(VM *vm) {
    byte val = vm->spu.ps[--vm->spu.psp];
    vm->spu.rsp = val;
}
void _ldi(VM *vm) {
    cell val = vm->spu.ip;
    vm->spu.ps[vm->spu.psp++] = val;
}
void _stri(VM *vm) {
    cell val = vm->spu.ps[--vm->spu.psp];
    vm->spu.ip = val;
}
void _key(VM *vm) {
    vm->spu.ps[vm->spu.psp++] = getchar();
}
void _emit(VM *vm) {
    putchar(vm->spu.ps[--vm->spu.psp]);
}
void _call(VM *vm) {
    cell addr = vm->spu.ps[--vm->spu.psp];
    vm->spu.ip = addr;
}

cell next(VM *vm) {
    cell opcode = *((cell *) &(vm->ram[vm->spu.ip]));
    vm->spu.ip += CELL_SIZE;
    return opcode;
}
void exec(VM *vm, cell opcode) {
    switch(opcode) {
        case NOP: _nop(vm); break;
        case LIT: _lit(vm); break;
        case HALT: _halt(vm); break;
        case DUP: _dup(vm); break;
        case DROP: _drop(vm); break;
        case SWAP: _swap(vm); break;
        case PUSH: _push(vm); break;
        case POP: _pop(vm); break;
        case PICK: _pick(vm); break;
        case RICK: _rick(vm); break;
        case JMP: _jmp(vm); break;
        case JZ: _jz(vm); break;
        case RET: _ret(vm); break;
        case EQ: _eq(vm); break;
        case NEQ: _neq(vm); break;
        case GT: _gt(vm); break;
        case LT: _lt(vm); break;
        case AND: _and(vm); break;
        case OR: _or(vm); break;
        case XOR: _xor(vm); break;
        case SHR: _shr(vm); break;
        case SHL: _shl(vm); break;
        case TRU: _tru(vm); break;
        case FLS: _fls(vm); break;
        case ADD: _add(vm); break;
        case SUB: _sub(vm); break;
        case MUL: _mul(vm); break;
        case DIV: _div(vm); break;
        case MOD: _mod(vm); break;
        case LDC: _ldc(vm); break;
        case STRC: _strc(vm); break;
        case LDB: _ldb(vm); break;
        case STRB: _strb(vm); break;
        case CELL: _cell(vm); break;
        case BYTE: _byte(vm); break;
        case MEM: _mem(vm); break;
        case LDP: _ldp(vm); break;
        case STRP: _strp(vm); break;
        case LDR: _ldr(vm); break;
        case STRR: _strr(vm); break;
        case LDI: _ldi(vm); break;
        case STRI: _stri(vm); break;
        case KEY: _key(vm); break;
        case EMIT: _emit(vm); break;
        case CALL: _call(vm); break;
        default:
            vm->spu.rs[vm->spu.rsp++] = vm->spu.ip;
            vm->spu.ip = opcode;
            break;
    }
}
void tick(VM *vm) {
    exec(vm, next(vm));
}
void run(VM *vm) {
    vm->spu.p = ON;
    while(vm->spu.p == ON)
        tick(vm);
}

void dump(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "w");
    if(fptr == NULL) {
        printf("[DUMP]: Can't open file\n");
        return;
    }

    for(int i = 0; i < MEM_SIZE; ++i)
        fputc(vm->ram[i], fptr);
}
void load(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "r");
    if(fptr == NULL) {
        printf("[LOAD]: Can't open file\n");
        return;
    }

    for(int i = 0; i < MEM_SIZE; ++i)
        vm->ram[i] = fgetc(fptr);
}
void carr(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "w");
    if(fptr == NULL) {
        printf("[CARR]: Can't open file\n");
        return;
    }

    fprintf(fptr, "\n\n{");
    for(int i = 0; i < MEM_SIZE; ++i) {
        if(i%8 == 0) fprintf(fptr, "\n\t");
        fprintf(fptr, "%03x, ", vm->ram[i]);
    }
    fprintf(fptr, "\n};\n\n");
}













