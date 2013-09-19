from math import sqrt

def quadSum(nums):
    tot = 0
    for n in nums:
        tot += n*n
    return sqrt(tot)
