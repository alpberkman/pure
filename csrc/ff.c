

#include "ff.h"


cell next(VM *vm) {
    cell opcode = CELL_VAL(vm, vm->spu.ip);
    INCIP(vm);
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
        case EXE: _exe(vm); break;
        default:
            RPUSH(vm) = vm->spu.ip;
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

void _nop(VM *vm) {
    (void) vm;
}
void _lit(VM *vm) {
    cell val = CELL_VAL(vm, vm->spu.ip);
    PPUSH(vm) = val;
    INCIP(vm);
}
void _halt(VM *vm) {
    vm->spu.p = OFF;
}
void _dup(VM *vm) {
    cell val = PPOP(vm);
    PPUSH(vm) = val;
    PPUSH(vm) = val;
}
void _drop(VM *vm) {
    --(vm)->spu.psp;
}
void _swap(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = a;
    PPUSH(vm) = b;
}
void _push(VM *vm) {
    RPUSH(vm) = PPOP(vm);
}
void _pop(VM *vm) {
    PPUSH(vm) = RPOP(vm);
}
void _pick(VM *vm) {
    byte n = PPOP(vm) + 1;
    n = vm->spu.psp - n;
    PPUSH(vm) = vm->spu.ps[n];
}
void _rick(VM *vm) {
    byte n = PPOP(vm) + 1;
    n = vm->spu.rsp - n;
    PPUSH(vm) = vm->spu.rs[n];
}
void _jmp(VM *vm) {
    cell addr = PPOP(vm);
    vm->spu.ip = addr;
}
void _jz(VM *vm) {
    cell addr = PPOP(vm);
    cell flag = PPOP(vm);
    if(flag == ((cell) FFALSE))
        vm->spu.ip = addr;
}
void _ret(VM *vm) {
    cell addr = RPOP(vm);
    vm->spu.ip = addr;
}
void _eq(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b == a ? TTRUE : FFALSE;
}
void _neq(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b != a ? TTRUE : FFALSE;
}
void _gt(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b > a ? TTRUE : FFALSE;
}
void _lt(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b < a ? TTRUE : FFALSE;
}
void _and(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b & a;
}
void _or(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b | a;
}
void _xor(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b ^ a;
}
void _shr(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b >> a;
}
void _shl(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b << a;
}
void _tru(VM *vm) {
    PPUSH(vm) = TTRUE;
}
void _fls(VM *vm) {
    PPUSH(vm) = FFALSE;
}
void _add(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b + a;
}
void _sub(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b - a;
}
void _mul(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b * a;
}
void _div(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b / a;
}
void _mod(VM *vm) {
    cell a = PPOP(vm);
    cell b = PPOP(vm);
    PPUSH(vm) = b % a;
}
void _ldc(VM *vm) {
    cell addr = PPOP(vm);
    cell val = CELL_VAL(vm, addr);
    PPUSH(vm) = val;
}
void _strc(VM *vm) {
    cell addr = PPOP(vm);
    cell val = PPOP(vm);
    CELL_VAL(vm, addr) = val;
}
void _ldb(VM *vm) {
    cell addr = PPOP(vm);
    byte val = BYTE_VAL(vm, addr);
    PPUSH(vm) = val;
}
void _strb(VM *vm) {
    cell addr = PPOP(vm);
    byte val = PPOP(vm);
    BYTE_VAL(vm, addr) = val;
}
void _cell(VM *vm) {
    PPUSH(vm) = CELL_SIZE;
}
void _byte(VM *vm) {
    PPUSH(vm) = BYTE_SIZE;
}
void _mem(VM *vm) {
    PPUSH(vm) = MEM_SIZE;
}
void _ldp(VM *vm) {
    byte val = vm->spu.psp;
    PPUSH(vm) = val;
}
void _strp(VM *vm) {
    byte val = PPOP(vm);
    vm->spu.psp = val;
}
void _ldr(VM *vm) {
    byte val = vm->spu.rsp;
    PPUSH(vm) = val;
}
void _strr(VM *vm) {
    byte val = PPOP(vm);
    vm->spu.rsp = val;
}
void _ldi(VM *vm) {
    cell val = vm->spu.ip;
    PPUSH(vm) = val;
}
void _stri(VM *vm) {
    cell val = PPOP(vm);
    vm->spu.ip = val;
}
void _key(VM *vm) {
    PPUSH(vm) = IO_KEY;
}
void _emit(VM *vm) {
    IO_EMIT(PPOP(vm));
}
void _exe(VM *vm) {
    RPUSH(vm) = vm->spu.ip;
    cell addr = PPOP(vm);
    vm->spu.ip = addr;
}














