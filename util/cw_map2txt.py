import sys


file = sys.argv[1]
lines = open(file, 'r').read().splitlines()

for i, l in enumerate(lines):
    sym_name = l[29:]
    addr = l[:8]

    lines[i] = f'{sym_name}=0x{addr}'

open(file, 'w').write("\n".join(lines))