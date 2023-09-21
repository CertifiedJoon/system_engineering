dict1 = dict({1: "A", 2: "B", 3: "C"})

dict2 = dict({5: "E"})

print(dict1.get(1, "Z"))
print(dict1.get(0, "Z"))
print(dict1.pop(0, "Z"))
print(dict1.popitem())
dict1.update(dict({4: "D"}))
print(dict1 | dict2)
