arr = [i for i in range(10)]

del arr[1:2]

del arr[1:3:2]

del arr

arr2 = arr

arr2.insert(1, 2)
arr2.remove(7)
print(reversed(arr))
