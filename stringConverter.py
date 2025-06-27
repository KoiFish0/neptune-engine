import sys

if len(sys.argv) < 2:
    print("Usage: python stringConverter.py <input_file>")
    sys.exit(1)

input_file = sys.argv[1]

with open(input_file, 'r', encoding='utf-8') as f:
    for line in f:
        line = line.rstrip('\n')
        print(f'"{line}\\n"')
