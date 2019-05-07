


> Written with [StackEdit](https://stackedit.io/).
> **

## Sum Sum Digits Digits

**
> by Paul H Alfille 2019 based on idea of Daniel Saddawi-Konefka
> 

## The problem:
Imagine the number:
9+
99+
999+
...
99...9 (321 nines)

What is the sum of the digits of this rather big number?

## Trials:

 1. 9 -> 9
 2. 9+99=108 ->9
 3. 9+99+999=1007 ->9 
 4. hmmm... but it breaks  
    down after 9 9's.

## Method:

 1. 9=10-1 
 2. 99=100-1 
 3. 999=1000-1 
 4. so summing N 9's
    11111...1110 - N*1 (there are N 1's there)

## Solution:

N=321
So using the fact that each 9999 is 10000-1
| 9|10-1|
|99|100-1|
|999|1000-1|
|9|10-1  |
|--|--|
|  |  |


we have
111..(321)..1110 - 321
Sum of the 9's is
**111..(321)..1110 - 321 =**
**111..(321-3)..1110000 + 1110-321**
**=111..(318)..1110789
So Sum of digits = 318+7+8+9 = 342**



<!--stackedit_data:
eyJoaXN0b3J5IjpbMjEyNzg4ODkzOSwxMTk2NDEwMDI1XX0=
-->