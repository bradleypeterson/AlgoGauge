import random
from array import array


def full_random_array(length: int, max_value) -> array:
    return array('I', [random.randint(0, max_value) for _ in range(length)])

def repeating_array(length: int, max_value) -> array:
    random_num = random.randint(0, max_value)
    return array('I', [random_num for _ in range(length)])

def random_chunk_array(length: int, max_value) -> array:
    chunk_size = 1 if length < 10 else 5
    chunk = bool(random.getrandbits(1))
    
    arr = array("i", [])
    for i in range(length):
        random_num = random.randint(0, max_value)
        chunk = not chunk
        for _ in range(chunk_size):
            if i >= length:
                break
            if not chunk:
                random_num = random.randint(0, max_value) # selects a new random number for 5
            arr.append(random_num)
    return arr

def ordered_array(length) -> array:
    return array('I', [i+1 for i in range(length)])

def ordered_reversed_array(length) -> array:
    return array('I', [length-i for i in range(length)])
            
                    
# print(random_chunk_array(12, 20))
# print(ordered_reversed_array(12))
# print(repeating_array(12, 20))
# print(ordered_array(12))
# print(full_random_array(12, 20))


