
import sys
from argparse import ArgumentParser
from run_algorithms import run_algorithm

MAX_NUMBER = 2**32 - 1

USAGE = f"Usage: python {sys.argv[0]} [--help] | [-s <sep>] [first [incr]] last"
VERSION = f"{sys.argv[0]} version 1.0.0"

parser = ArgumentParser(
                    prog='AlgoGauge Python',
                    description='This is a simplified command line version of the AlgoGauge Program written in Python.',
                    epilog='', 
                    usage='python {sys.argv[0]} <options>')

parser.add_argument('--version', action='version', version='%(prog)s 1.0')


parser.add_argument('-v', '--verbose', action='store_true', help='Verbose mode')
parser.add_argument('-o', '--output', action='store_true', help='Will output the before and after of the sorted array length should be set to under 100.')
parser.add_argument('-j', '--json', action='store_true', help='Will output the before and after of the sorted array.')
parser.add_argument('-m', '--max', action='store', help= 'the largest number the array could have optional')

parser.add_argument('-a', '--algorithm', action='append', help="Select sort algorithm")
parser.add_argument('-l', '--length', action='append', help="the number of elements in the array")
parser.add_argument('-s', '--strategy', action='append', help= "the array creation method")

parser.add_argument('-f', '--file', action='append', help='The save location for json, json needs to be true')
parser.add_argument('-n', '--name', action='append', help='optional the Canonical Name')



def main():
    args = parser.parse_args()
    if args is None:
        parser.print_help()
        exit(1)

    if(args.length is None or args.strategy is None or args.algorithm is None):
        parser.print_help()
        exit(1)
    if(len(args.length) != len(args.strategy) or len(args.length) != len(args.algorithm)):
        print("Number of provided algorithm(s), length(s), language(s), and strategy(s) arguments do not match!")
        exit(1);
    
    json_results = "";
    for index, item in enumerate(args.length):
        json_results += run_algorithm(
            algorithm=args.algorithm[index],
            strategy=args.strategy[index],
            length=int(args.length[index]),
            verbose=args.verbose if args.verbose else False,
            output=args.output if args.output else False,
            max_value= MAX_NUMBER,
            name=args.name[index] if args.name and args.name[index]else ""
        )
        
    if (args.json): print(json_results)
    return 0

if __name__ == "__main__":
    main()