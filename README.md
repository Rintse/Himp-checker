# h2smt
This program can parse, and verify hoare proofs within an [imp-like](https://en.wikipedia.org/wiki/IMP_(programming_language)) language.

## Overview
Imp-like programs, annotated with hoare predicates, are parsed into an AST using flex and bison.

The AST is then traversed in accordance with the common hoare rules to verify the correctness of the annotated program.

Hoare triples are translated into SMT formulas, as described [here](https://fmt.ewi.utwente.nl/media/71.pdf), which are then verified using the [Z3 SMT prover](https://github.com/Z3Prover/z3). Using this translation scheme, counterexamples to the hoare proof can be extracted from the SMT model if the hoare proof turns out to be invalid.

## Grammar
The accepted language is as follows: 

>*Aexp* &rarr; *var* | *int* \
>&nbsp;&nbsp;  | *Aexp* + *Aexp* \
>&nbsp;&nbsp;  | *Aexp* - *Aexp* \
>&nbsp;&nbsp;  | *Aexp* * *Aexp* \
>&nbsp;&nbsp;  | *Aexp* / *Aexp*
  
>*Bexp* &rarr; ***true*** | ***false*** \
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

Where 
- Aexp represents an arithmetic expression
- Bexp represents a boolean expression
- Command represents an imp-like command
- { Bexp } represents a hoare predicate

