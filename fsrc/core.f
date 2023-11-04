
: HERE HP @ ;
: LAST LP @ ;


: OVER 1 PICK ;
: ROT R< SWAP R> SWAP ;
: -ROT SWAP R< SWAP R> ;
: R@ 1 RICK ;
: NIP SWAP DROP ;
: TUCK SWAP OVER ;



: CELL+ CELL + ;
: CELLS CELL * ;

: BYTE+ BYTE + ;
: BYTES BYTE * ;


: +! TUCK @ + SWAP ! ;
: ALLOT HP +! ;
: , HERE ! CELL ALLOT ;
: C, HERE C! BYTE ALLOT ;



: 1+ 1 + ;
: 1- 1 - ;


: INVERT -1 XOR ;
: 0= 0 = ;
: 0< 0 < ;
: 0> 0 > ;
: 0<> 0 <> ;

: >= < 0= ;
: <= > 0= ;

(
1 6 << CONSTANT IMM
1 7 << CONSTANT VIS
31     CONSTANT LEN

: IMM? C@ IMM AND ;
: VIS? C@ VIS AND ;
: LEN? C@ LEN AND ;

: LINK ;
: FLAGS CELL+ ;
: NAME CELL+ BYTE+ ;
: CODE DUP FLAGS LEN? SWAP NAME + ;
: PREV @ ;

: MARK OVER C@ OR SWAP C! ;
: IMMEDIATE LAST FLAGS IMM MARK ;
: VISIBLE LAST FLAGS VIS MARK ; IMMEDIATE
)

: ++ +1 SWAP +! ;
: -- -1 SWAP +! ;

: OFF FALSE SWAP ! ;
: ON TRUE SWAP ! ;
: OFF? @ 0= ;
: ON? @ 0<> ;

: 0, 0 , ;



: [ STATE OFF ; IMMEDIATE
: ] STATE ON ;

: LITERAL ['] LIT , , ; IMMEDIATE
: COMPILE, , ;




(
: [SELF] BRANCH ;
: SELF
  POSTPONE [SELF]
  LAST CODE ,
; IMMEDIATE


: [RECURSE] ;
: RECURSE
  POSTPONE [RECURSE]
  LAST CODE ,
; IMMEDIATE
)
\ Doesnt work literal the address cell-
: [IF] BRANCH0 ;
: IF
  POSTPONE [IF]
  HERE CELL ALLOT
; IMMEDIATE

: [THEN] ;
: THEN
  POSTPONE [THEN]
  HERE SWAP !
; IMMEDIATE

: [ELSE] BRANCH ;
: ELSE
  POSTPONE [ELSE]
  HERE SWAP
  CELL ALLOT
  HERE SWAP !
; IMMEDIATE





















