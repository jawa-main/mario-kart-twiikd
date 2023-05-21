

from pathlib import Path
import sys, cw_demangle


file = Path(sys.argv[1])
ext = file.name.split(".")[-1]
lines = file.read_text().splitlines()

if ext == "txt":
    for i, l in enumerate(lines):
        sym_name, addr = l.split("=")
        if "__" not in sym_name:
            continue
        try:
            demangled = cw_demangle.demangle(sym_name).replace(" ", "_").replace("::", "_")
            lines[i] = f'{demangled}={addr}'
        except AssertionError:
            pass
    file.write_text("\n".join(lines))