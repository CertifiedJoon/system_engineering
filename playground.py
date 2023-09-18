class C:
    @classmethod
    def increment(cls):
        cls.counter += 1


def greater(a, b):
    return a > b


nums1 = [1, 2, 3, 4]
nums2 = [5, 6, 7, 8]

print(list(map(greater, nums1, nums2)))
