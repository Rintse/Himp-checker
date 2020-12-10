# h2smt
This program can parse, and verify hoare proofs within an [imp-like](https://en.wikipedia.org/wiki/IMP_(programming_language)) language.

## Grammar
The language accepted is as follows: 

>*Aexp* &rarr; *var* | *int* | *Aexp* + *Aexp* | \
>  *Aexp* - *Aexp* | *Aexp* * *Aexp* | *Aexp* / *Aexp*
  
>Bexp &rarr; true | false | Bexp = Bexp | \
>  Bexp &ne Bexp | Bexp > Bexp | Bexp < Bexp | \
>  Bexp &ge; Bexp | Bexp &le; Bexp | \
>  &not; Bexp | Bexp &#8743; Bexp | \
>  Bexp &#8744; Bexp | Bexp &rArr; Bexp
  
>Command &rarr; **skip** | var := Aexp | \
>  **if** Bexp **then** Block **else** Block | \
>  **while** Bexp **do** WhileBlock
  
>Block &rarr; ( { Bexp } Command; )* { Bexp } \
>WhileBlock &rarr; ( Command { Bexp } )* Command

## Overview
test
