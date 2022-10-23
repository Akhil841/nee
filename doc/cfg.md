# NEE's Context Free Grammar
NEE uses the following context-free grammar to verify and evaulate expressions:
```
S -> SOS|N|PoSPc
O -> +|-|/|*
N -> 1|2|3|4|5|6|7|8|9|0|NN
Po -> (
Pc -> )
```

In practice, due to implementation, NEE only supports 2<sup>31</sup>-1 nested parentheses, and numbers smaller than 2<sup>64</sup>-1. This should be small enough for almost any use case you throw at it, but exceeding these limits results in undefined behavior.

You may notice that negative numbers cannot be used as numbers. This is because I have not implemented any unary operators yet.
In the meantime, please replace any negative numbers with 0 minus the positive version of that number.
For example, -5+1 would become (0-5)+1.

This also means that inputs such as 1+2+3 are not valid, because + is a binary operator and this would generate an ambiguous parse tree. You'd instead have to either write it as (1+2)+3 or 1+(2+3).