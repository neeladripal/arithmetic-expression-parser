# Arithmetic Expression Parser
Parser for a set of arithmetic expressions using yacc and lex

To run, execute the following commands:
```
byacc -d parse.y
flex tokenizer.l
gcc lex.yy.c y.tab.c
./a.out
```

### Input
Set of statements containing arithmetic expression of any length involving operators {+, -, *, /} and constants as operands assigned to a variable. '\\' character is used to mark end of input.

example -
```
v_1 = 1 + 67 * (4 / 8) * 10;
v_2 = -5 + 7 * 3;
\
```