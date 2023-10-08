from decimal import *
import math

getcontext().prec = 6
getcontext().rounding = ROUND_HALF_UP

dec = Decimal(2.15)
print(math.ceil(dec))
print(math.floor(dec))

print(math.comb(4, 2))
print(math.perm(4, 2))
