# m-DFA accepting Regular Expression Language

## Environment
- gcc 4.7.1

## Files
- lex.yy.c : customized after genereted by flex
- y.tab.c : customized after generated by bison
- y.tab.h : generated by bison
- main.c : every proccess after parsing

## Compile
```bash
$ gcc lex.yy.c y.tab.c main.c
```

## Regular Expression Input Rule

- Parenthesis : `(E)`
- Closure : `E*`
- Concatenation : `EE`
- Union : `E+E`
- Epsilon : `$`
- Symbol : A-Za-z0-9

## Input Example
- Regular Expression : `(0a+0)*`
- Test : `0aa`
