from array import array
from time import perf_counter
import sys
import sorting_methods as sm
import array_creation as ac

def get_unsorted_array(strategy: str, length: int, max_value: int) -> array:
    strategy_map = {
        "random": lambda: ac.full_random_array(length=length, max_value=max_value),
        "chunk": lambda: ac.random_chunk_array(length=length, max_value=max_value),
        "repeating": lambda: ac.repeating_array(length=length, max_value=max_value),
        "ordered": lambda: ac.ordered_array(length=length),
        "reversed": lambda: ac.ordered_reversed_array(length=length)
    }
    unsorted_array = strategy_map.get(strategy)
    
    if unsorted_array:
        return unsorted_array()  
    else:
        print(f"error: option '-s, --strategy <string>' argument '{strategy}' is invalid. "
            "Allowed choices are random, chunk, repeating, ordered, reversed.", file=sys.stderr)
        exit(2)
    return -1


def run_algorithm(algorithm: str, strategy: str, length: int, verbose: bool, output: bool, max_value: int = sys.maxsize, name = "") -> str:
    algorithm = algorithm.lower()
    strategy = strategy.lower()
    
    unsorted_array = get_unsorted_array(strategy, length, max_value)
    
    sorted_array = None
    algorithm_map = {
        "default": lambda: sm.built_in_sort(arr=unsorted_array),
        "merge": lambda: sm.merge_sort(arr=unsorted_array),
        "bubble": lambda: sm.bubble_sort(arr=unsorted_array),
        "selection": lambda: sm.selection_sort(arr=unsorted_array),
        "insertion": lambda: sm.insertion_sort(arr=unsorted_array),
        "quick": lambda: sm.quick_sort(arr=unsorted_array),
        "heap": lambda: sm.heap_sort(arr=unsorted_array),
    }
    
    selected_algorithm = algorithm_map.get(algorithm)
    if selected_algorithm is None:  
        print(f"error: option '-a --algo --algorithm <string>' argument '{algorithm}' is invalid. Allowed choices are built-in, default, bubble, selection, insertion, quick, merge, heap.", file=sys.stderr)
        exit(3)
    
    
    
    if verbose:
        print(f"Python3 Starting sort:{algorithm.capitalize()}")
        
    if output and verbose:
        print(f"Python3 Original Array: \"{unsorted_array.tolist()}\"")
        
    start = perf_counter()
    
    sorted_array = selected_algorithm()
    
    time_taken = perf_counter() - start
    
    if output and verbose:
        print(f"Python3 Sorted Array: \"{sorted_array.tolist()}\"")
    
    if verbose: print(f"Python3 Verifying sort: \"{algorithm.capitalize()}\"")
    
    correct = sm.verify_sort(arr=sorted_array)
            
    if not correct:
        print(f"{algorithm.capitalize()} there was an error when sorting", file=sys.stderr)
    else:
        if verbose: print(f"Python3 Sort: \"{algorithm.capitalize()}\" Verified!")

    if verbose:
        print(f"Python3 Sort: \"{algorithm.capitalize()}\" with Algorithm Option \"{strategy.capitalize()}\" of length {length}, completed in {time_taken} milliseconds")
        
        
    result = (f'{{"algorithmName": "{algorithm.capitalize()}",'
          f'"algorithmOption": "{strategy.capitalize()}",'
          f'"algorithmLength": {length},'
          f'"language": "Python",'
          f'"verified": {int(correct)},'
          f'"algorithmCanonicalName": "{name if name else ""}",'
          f'"algorithmRunTime_ms": {time_taken:6f},'
          f'"perfData": {{}}}}')
    return result
        