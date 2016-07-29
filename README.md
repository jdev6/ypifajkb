#ypifajkb
Esoteric stack-based programming language

#Operations

": Toggle string mode (when on string mode, push every character found)

+: Pop a,b and push a+b

-: Pop a,b and push b-a

\*: Pop a,b and push a\*b

/: Pop a,b and push b/a rounded down

%: Pop a,b and push the remainder of the integer division of b/a

~: Pop a,b, push a and push b (swap values)

,: Pop a and print a as a character

.: Pop a and print a as a number

;: Push one character from input

=: Pop a,b and push 1 if a==b, 0 otherwise

!: Pop a, and push 1 if a==0, 0 otherwise (logical not)

$: Pop a

^: Pop a and put a in the tmp storage

v: Push the value from the tmp storage

[: If last value at the stack is 0 jump to the matching ]

]: If last value at the stack is not 0 jump to the matching [

[] are for loops and `if`s like in brainfuck
