
: SET     ( n mask -- n ) OR ;
: CLEAR   ( n mask -- n ) INVERT AND ;
: TOGGLE  ( n mask -- n ) XOR ;
: TEST    ( n mask -- f ) AND ;
