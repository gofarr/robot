#LinkedList

class node():
    val = None
    next = None
def creat_linklist(num):
    pHead = node()
    tmp = pHead
    for i in num:
        pHead.next = node()
        pHead = pHead.next
        pHead.val = i
    return tmp.next
def print_linklist(pHead):
    res = []
    while pHead:
        res.append(pHead.val)
        pHead = pHead.next
    print(res)
    return
def reverse_linklist(pHead, m, n):
    head = node()
    head.next = pHead
    prenodeM = head
    nodeN = head

    for i in range(m - 1):
        prenodeM = prenodeM.next
    nodeM = prenodeM.next

    for i in range(n):
        nodeN = nodeN.next
    nexnodeN = nodeN.next
    
    #prenodeM.next = nodeN
    #nodeM.next = nexnodeN
    last = None
    node1 = nodeM
    while not (node1 is nexnodeN):
        tmp = node1.next
        node1.next = last
        last = node1
        node1 = tmp
    nodeM.next = nexnodeN
    prenodeM.next = nodeN
    return head.next


#bag&question

def bag():
    n = int(input('输入物品个数：'))
    c = int(input('输入背包容量：'))
    w = [0] + list(map(int, input('输入各物品体积').split(' ')))
    v = [0] + list(map(int, input('输入各物品价值').split(' ')))
    f = [[0 for i in range(c + 1)] for j in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(c, -1, -1):
            if j >= w[i]:
                f[i][j] = max(f[i - 1][j - w[i]] + v[i], f[i - 1][j])
            else:
                f[i][j] = f[i - 1][j]
    return f[n][c]

def bag_improve():
    n = int(input('输入物品个数：'))
    c = int(input('输入背包容量：'))
    w = [0] + list(map(int, input('输入各物品体积').split(' ')))
    v = [0] + list(map(int, input('输入各物品价值').split(' ')))
    f = [0 for i in range(c + 1)]
    for i in range(1, n + 1):
        for j in range(c, -1, -1):
            if j >= w[i]:
                f[j] = max(f[j - w[i]] + v[i], f[j])
    return f[c]


#quick_sort

def quick_sort(L,start,end):
    if start < end:
        i , j , pivot = start , end , L[start]
        while i < j:
#从右开始向左寻找第一个小于pivot的值
            while (i < j) and (L[j] >= pivot):
                j = j-1
#将小于pivot的值移到左边
            if (i < j):
                L[i] = L[j]
                i = i+1 
#从左开始向右寻找第一个大于pivot的值
            print(L)
            while (i < j) and (L[i] < pivot):
                i = i+1
#将大于pivot的值移到右边
            if (i < j):
                L[j] = L[i]
                j = j-1
            print(L)
#循环结束后，说明 i=j，此时左边的值全都小于pivot,右边的值全都大于pivot
#pivot的位置移动正确，那么此时只需对左右两侧的序列调用此函数进一步排序即可
#递归调用函数：依次对左侧序列：从0 ~ i-1//右侧序列：从i+1 ~ end
        L[i] = pivot
        print(L)
#左侧序列继续排序
        quick_sort(L,start,i-1)
#右侧序列继续排序
        quick_sort(L,i+1,end)

def wgw_sort(nums, start, end):
    if start >= end:
        return
    pivot = nums[start]
    left = start
    right = end
    while left < right:
        while left < right and nums[right] >= pivot:
            right -= 1
        if left < right:
            nums[left] = nums[right]
            left += 1
        while left < right and nums[left] <= pivot:
            left += 1
        if left < right:
            nums[right] = nums[left]
            right -= 1
    nums[left] = pivot
    wgw_sort(nums, start, left - 1)
    wgw_sort(nums, left + 1, end)
    return
