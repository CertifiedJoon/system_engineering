class C:
    @classmethod
    def increment(cls):
        cls.counter += 1


def greater(a, b):
    return a > b


nums1 = [1, 2, 3, 4]
nums2 = [5, 6, 7, 8]

print(list(map(greater, nums1, nums2)))


"""
Kernel mode.
load code from persistant memory,
allocate stack with argc/argv
allocate heap
call main()

User mode.
starts program
ends

Kernel mode.
Free memory
remove from process list.
"""
