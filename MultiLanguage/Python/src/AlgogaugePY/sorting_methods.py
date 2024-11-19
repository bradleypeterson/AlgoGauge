from array import array
from math import floor


def built_in_sort(arr: array) -> array:
    return array(arr.typecode, sorted(arr))
    


def merge_sort(arr: array, first_index = 0, last_index = None):
    if(last_index is None):
        last_index = len(arr)
        
    if (last_index - first_index < 2):
        return
    
    middle_index = floor((last_index-first_index)/2 + first_index)
    
    merge_sort(arr=arr, first_index=first_index, last_index=middle_index)
    merge_sort(arr=arr, first_index=middle_index, last_index=last_index)


    left_half_size = middle_index - first_index;
    right_half_size = last_index - middle_index;
    
    left_array = array('I', arr[first_index:first_index+left_half_size])
    right_array = array('I', arr[middle_index: middle_index + right_half_size])

    left_index = 0
    right_index = 0
    arr_index = first_index
    
    while(left_index < left_half_size and right_index < right_half_size):
        if(left_array[left_index] <= right_array[right_index]):
            arr[arr_index] = left_array[left_index]
            left_index+=1;
        else:
            arr[arr_index] = right_array[right_index]
            right_index+=1
        arr_index+=1
    

    while(left_index < left_half_size):
        arr[arr_index] = left_array[left_index]
        left_index+=1;
        arr_index+=1;
        
    while(right_index < right_half_size):
        arr[arr_index] = right_array[right_index]
        right_index+=1;
        arr_index+=1;
        
    return arr


def quick_sort_partition(arr: array, first = 0, last: int = None) -> int:
    if(last is None):
        last = len(arr)
    pivot = arr[first]
    small_index = first
    for index in range(first+1, last):
        if arr[index] < pivot:
            small_index+=1
            arr[small_index], arr[index] = arr[index], arr[small_index]
    
    arr[first], arr[small_index] = arr[small_index], arr[first]
    
    return small_index;

def quick_sort(arr: array, first_index = 0, length: int = None) -> array:
    if(length is None):
        length = len(arr)
        
    if len(arr) <= 1:
        return arr
    
    if first_index < length:
        pivotLocation = quick_sort_partition(arr=arr, first=first_index, last=length)
        quick_sort(arr=arr, first_index=first_index, length=pivotLocation)
        quick_sort(arr=arr, first_index=pivotLocation+1, length=length)

    return arr


def bubble_sort(arr: array) -> array:
    
    swapped = True
    n = len(arr)
    
    while(swapped and n > 1):
        swapped = False;
        last_unsorted = 0
        for i in range(n-1):
            if arr[i] > arr[i+1]:
                arr[i], arr[i+1] = arr[i+1], arr[i]
                swapped = True
                last_unsorted = i+1
                
        n = last_unsorted if last_unsorted > 0 else n
    
    return arr

def insertion_sort(arr:array) -> array:
    
    for i in range(len(arr)):
        j = i
        while(0 < j and arr[j] < arr[j-1]):
            arr[j], arr[j-1] = arr[j-1], arr[j]
    
    return arr
    
def selection_sort(arr:array) -> array:
    for i in range(len(arr)):
        for j in range(i+1, len(arr)):
            if arr[j] < arr[i]:
                arr[i], arr[j] = arr[j], arr[i]
                
    return arr



def verify_sort(arr: array) -> bool:
    if(arr is None):
        return False

    for index, i in enumerate(arr[:-1]):
        if (i > arr[index+1]):
            return False
    
    return True


def heapify(arr: array, index, last_unsorted_index) -> array:
    largest = index
    left = index*2+1
    right = index*2+2
    
    if left < last_unsorted_index and arr[left] > arr[largest]:
        largest = left
        
    if right < last_unsorted_index and arr[right] > arr[largest]:
        largest = right
        
    if largest != index:
        arr[index], arr[largest] = arr[largest], arr[index]
        
        heapify(arr=arr, index=largest, last_unsorted_index=last_unsorted_index)
    return arr

def heap_sort(arr:array) -> array:
    last_unsorted_index = len(arr)

    i = floor(len(arr)/2)
    
    while(i > 0):
        i -=1
        heapify(arr=arr, index=i, last_unsorted_index=last_unsorted_index)
    
    while(last_unsorted_index > 0):
        last_unsorted_index -=1
        arr[0], arr[last_unsorted_index] = arr[last_unsorted_index], arr[0]
        heapify(arr=arr, index=0, last_unsorted_index=last_unsorted_index)
    
    return arr

# arrr = ac.full_random_array(120, 13245678)
# arrr.append(1)
# x = bubble_sort(arr=arrr)
# print(verify_sort(arr=x))
# x = insertion_sort(arr=arrr)
# print(verify_sort(arr=x))
# x = merge_sort(arr=arrr)
# print(verify_sort(arr=x))

# x = selection_sort(arr=arrr)
# print(verify_sort(arr=x))
# x = heap_sort(arr=arrr)
# print(verify_sort(arr=x))
