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

"""
Kernel Mode
load code from persistant memory
allocate stack from argc/argv
allocate heap
call main()

User Mode
starts executing main()
executes system call
trap into OS

Kernel Mode
return from trap,
execute and return

User Mode
finishes executing main()

Kernel Mode
free memory
removes from process memory.


OS @ boot
initialize trap table

Hardware
remember addresses of syscall and timer handler

OS @ boot
start interrupt time

Hardward
Start timer

process       hardware                      OS                           process
process a -> timer interrupt -> traps -> switch() -> return-from trap -> process b


"""
