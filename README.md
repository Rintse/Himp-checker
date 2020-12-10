# h2smt
This program can parse, and verify hoare proofs within an [imp-like](https://en.wikipedia.org/wiki/IMP_(programming_language)) language.

## Grammar
The language accepted is as follows: 

>*Aexp* &rarr; *var* | *int* \
>&nbsp;&nbsp;  | *Aexp* + *Aexp* \
>&nbsp;&nbsp;  | *Aexp* - *Aexp* \
>&nbsp;&nbsp;  | *Aexp* * *Aexp* \
>&nbsp;&nbsp;  | *Aexp* / *Aexp*
  
>*Bexp &rarr; ***true*** | ***false*** \
>&nbsp;&nbsp;  | *Bexp* = *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &ne; *Bexp* \
>&nbsp;&nbsp;  | *Bexp* > Bexp \
>&nbsp;&nbsp;  | *Bexp* < *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &ge; *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &le; *Bexp* \
>&nbsp;&nbsp;  | &not; *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &#8743; *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &#8744; *Bexp* \
>&nbsp;&nbsp;  | *Bexp* &rArr; *Bexp*
  
>Command &rarr; ***skip*** \
>&nbsp;&nbsp;  | *var* := *Aexp* \
>&nbsp;&nbsp;  | ***if*** *Bexp* ***then*** *Block* ***else*** *Block* \
>&nbsp;&nbsp;  | ***while*** *Bexp* ***do*** *WhileBlock*
  
>*Block* &rarr; ( { *Bexp* } *Command*; )* { *Bexp* } \
>*WhileBlock* &rarr; ( *Command* { *Bexp* } )* *Command*

## Overview
test
