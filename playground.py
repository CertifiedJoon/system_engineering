from time import sleep


def factor(n):
    for i in range(1, 1 + n):
        if n % i == 0:
            yield i


for i in factor(4):
    sleep(1)
    print(i)


class LinkedList:
    def __init__(self):
        self._val = None
        self._next = None

    def iter(self):
        return self._next


def reverse(head):
    curr = head
    prev = None

    while curr:
        next = curr.next
        curr.next = prev
        prev = curr
        curr = next

    return prev
