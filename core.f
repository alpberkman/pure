

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


: ++ +1 SWAP +! ;
: -- -1 SWAP +! ;

: OFF FALSE SWAP ! ;
: ON TRUE SWAP ! ;

: 0, 0 , ;
: SET     ( n mask -- n ) OR ;
: CLEAR   ( n mask -- n ) INVERT AND ;
: TOGGLE  ( n mask -- n ) XOR ;
: TEST    ( n mask -- f ) AND ;


: [ STATE OFF ; IMMEDIATE
: ] STATE ON ;

: LITERAL ['] LIT , , ; IMMEDIATE
: COMPILE, , ;
: [POSTPONEI] ;
: [POSTPONE] R> DUP @ , CELL+ R< ;

: BRANCH R> DROP R> @ JMP ;
: BRANCH0 R> DROP 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;


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


: [BEGIN] ;
: BEGIN
  ['] [BEGIN] ,
  HERE
; IMMEDIATE

: [AGAIN] BRANCH ;
: AGAIN
  ['] [AGAIN] ,
  ,
; IMMEDIATE

: [UNTIL] BRANCH0 ;
: UNTIL
  ['] [UNTIL] ,
  ,
; IMMEDIATE

: [WHILE] BRANCH0 ;
: WHILE
  ['] [WHILE] ,
  HERE CELL ALLOT
; IMMEDIATE

: [REPEAT] BRANCH ;
: REPEAT
  ['] [REPEAT] ,
  SWAP ,
  HERE SWAP !
; IMMEDIATE


: BURY R> SWAP R> SWAP R< R< R< ;
: UNCOVER R> R> R> SWAP R< SWAP R< ;

: I 1 RICK ;
: I-LIMIT 2 RICK ;
: I-JMP 3 RICK ;
: J 4 RICK ;
: J-LIMIT 5 RICK ;
: J-JMP 6 RICK ;

: UNLOOP UNCOVER DROP UNCOVER DROP UNCOVER DROP ;

: BRANCH0-UNLOOP
  R> DROP 6 CELLS IP@ + JZ
  R> CELL+ UNLOOP JMP
  R> @ JMP
;


: [DO]
  R> DUP @ R< CELL+ R<
  SWAP BURY BURY
;
: DO
  ['] [DO] ,
  HERE CELL ALLOT
  HERE
; IMMEDIATE

: [LOOP]
  UNCOVER 1+ BURY
  1 RICK 2 RICK = BRANCH0-UNLOOP
;
: LOOP
  ['] [LOOP] ,
  ,
  HERE SWAP !
; IMMEDIATE

: [+LOOP]
  1 RICK SWAP
  UNCOVER + BURY
  2 RICK <
  1 RICK 2 RICK >= AND BRANCH0-UNLOOP
;
: +LOOP
  ['] [+LOOP] ,
  ,
  HERE SWAP !
; IMMEDIATE


: [LEAVE] R> DROP R> DROP R> DROP ;
: LEAVE
  ['] [LEAVE] ,
; IMMEDIATE


: MAKE-HEADER
  HERE
  LAST ,
  LP !
  0 C,
;

: [:NONAME] ;
: :NONAME
  MAKE-HEADER
  HERE
  ['] [:NONAME] ,
  STATE ON
;


: WITHIN ( n1 n2 n3 -- flag )
  OVER OVER < IF
    R< OVER > 0= SWAP R> < AND
  ELSE
     SWAP R< OVER > 0= SWAP R> < AND 0=
  THEN
;
: BETWEEN  ( n min max -- flag ) 1+ WITHIN ;


32 CONSTANT BL
 9 CONSTANT HT
10 CONSTANT LF
11 CONSTANT VT
12 CONSTANT FF
13 CONSTANT CR


: ISSPACE ( n -- )
  0
  OVER BL = OR
  OVER HT = OR
  OVER LF = OR
  OVER VT = OR
  OVER FF = OR
  OVER CR = OR
  NIP
;


: CHAR BEGIN KEY DUP ISSPACE WHILE DROP REPEAT ;
: [CHAR] CHAR POSTPONE LITERAL ; IMMEDIATE
: CR LF EMIT ;


: \ BEGIN KEY LF       = UNTIL ; IMMEDIATE
: ( BEGIN KEY [CHAR] ) = UNTIL ; IMMEDIATE
\ : ?( IF POSTPONE ( THEN ; IMMEDIATE
\ : ) ; IMMEDIATE
\ : !( KEY DUP [CHAR] ) <> IF EMIT SELF THEN DROP ; IMMEDIATE


: COUNT ( addr1 -- addr2 n ) DUP CELL+ SWAP @ ;

: TYPE ( addr n -- )
  DUP 0 <= IF DROP DROP EXIT THEN
  0 DO DUP I + C@ EMIT LOOP DROP
;

\ Broken crashes after "
: ["] ;
: [S"]
  R@ COUNT
  R@ @ CELL+ R> + R<
;
: [."]
  R@ COUNT TYPE
  R@ @ CELL+ R> + R<
;
: S"
  ['] [S"] ,
  HERE 0 ,
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, DUP ++
  REPEAT DROP DROP
  ['] ["] ,
; IMMEDIATE
: ."
  ['] [."] ,
  HERE 0 ,
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, DUP ++
  REPEAT DROP DROP
  ['] ["] ,
; IMMEDIATE


: MOVE ( addr1 addr2 n -- )
  DUP 0 <= IF DROP DROP DROP EXIT THEN
  0 DO OVER I + C@ OVER I + C! LOOP
  DROP DROP
;



: STRNCMP ( addr1 addr2 n -- n )
  DUP 0 <= IF DROP DROP DROP 0 EXIT THEN
  0 DO
    OVER I + C@ OVER I + C@ - DUP 0<> IF
      NIP NIP UNLOOP EXIT
    THEN DROP
  LOOP DROP DROP 0
;

: STREQ ( addr1 addr2 n -- )
  STRNCMP 0=
;


VARIABLE BASE
: BIN     ( -- )  2 BASE ! ;
: DECIMAL ( -- ) 10 BASE ! ;
: HEX     ( -- ) 16 BASE ! ;


























