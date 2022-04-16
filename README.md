# Arithmetic Expression Parser
Parser for a set of arithmetic expressions using yacc and lex

To run, execute the following commands:
```
bison -d parse.y
flex tokenizer.l
gcc lex.yy.c parse.tab.c
./a.out
```

### Input
Statement containing arithmetic expression of any length involving operators {+, -, *, /} and constants as operands assigned to a variable.

example -
```
v_1 = 1 + 67 * (4 / 8) * 10;
```