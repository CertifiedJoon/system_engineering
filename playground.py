from time import sleep


def factor(n):
    for i in range(1, 1 + n):
        if n % i == 0:
            yield i


for i in factor(4):
    sleep(1)
    print(i)

arr = [i for i in factor(4)]
print(arr)

while i:
    i = factor(4)
    print(i)
