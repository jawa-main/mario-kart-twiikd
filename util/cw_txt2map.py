import sys


lines = open(sys.argv[1], 'r').read().splitlines()

for i, l in enumerate(lines):
    lines[i] = f''