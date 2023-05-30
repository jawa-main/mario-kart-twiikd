

from pathlib import Path
import sys, cw_demangle

def demangle(sym_name):
    demangled =cw_demangle.demangle(sym_name).replace(" ", "_").replace("::", "_").replace("~", "destructor_").split("_(")[0]

    if "_(" in demangled:
        demangled = demangled.split("_(")[0]

    return demangled

file = Path(sys.argv[1])
ext = file.name.split(".")[-1]
lines = file.read_text().splitlines()
if ext == "txt":
    for i, l in enumerate(lines):
        sym_name, addr = l.split("=")
        if "__" not in sym_name:
            continue
        try:
            demangled = demangle(sym_name)
            lines[i] = f'{demangled}={addr}'
        except AssertionError:
            pass
    file.write_text("\n".join(lines))
elif ext == "map":
    for i, l in enumerate(lines):
        # 802c04b4 00000020 802c04b4 0 DVDReadPrio_hook
        addr, sz, _, zero, name = l.split(" ")
        if "__" not in name:
            continue
        try:
            demangled = demangle(name)
            lines[i] = f'{addr} {sz} {addr} 0 {demangled}'
        except AssertionError:
            pass
    file.write_text("\n".join(lines))
