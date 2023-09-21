dict1 = dict({1: "A", 2: "B", 3: "C"})

dict2 = dict({5: "E"})

print(dict1.get(1, "Z"))
print(dict1.get(0, "Z"))
print(dict1.pop(0, "Z"))
print(dict1.popitem())
dict1.update(dict({4: "D"}))
print(dict1 | dict2)

"""
OS 
loads code
allocate stack/heap
execute main

Hardware
Runs program

Process
executes code
calls a system call

Hardware
Trap

OS
execute system call
returnfromtrap

Hardware
return execution




Os boots
create trap tables
strat interrupt timer

Hardware boots
remember the addresses of interrupt handlers

process
executes

hardware
timer interrupt and trap

OS
perform context switch()
return from trap

hardware
execute new process

process
another process runs

"""
