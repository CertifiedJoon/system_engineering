import heapq

arr = [1, 2, 4, 5, 3, 6, 2, 3, 1]

heapq.heapify(arr)

print(arr)

heapq.heappush(arr, 1)

print(arr)

a = heapq.heappop(arr)

print(a)

print(heapq.nlargest(3, arr))

print(heapq.nsmallest(4, arr))
