from pathlib import Path
import sys
from elftools.elf.elffile import ELFFile, SymbolTableSection
from elftools.construct.lib.container import Container

elf_file = ELFFile(open(sys.argv[1], 'rb'))
output_map = Path(sys.argv[2])
dwi = elf_file.get_dwarf_info()

symtab: SymbolTableSection = elf_file.get_section_by_name(".symtab")

txt = ''

for symbol in symtab.iter_symbols():
    # print(symbol.__dict__)

    sym_type = symbol.entry['st_info']['type']

    if sym_type != "STT_FUNC":
        continue

    sym_off = symbol.entry['st_value']
    sym_sz = symbol.entry['st_size']

    sym_name: str = symbol.name.split("__F")[0]
    addr = 0x802c_0000 + sym_off
    txt += f'{addr:08x} {sym_sz:08x} {addr:08x} 0 {sym_name}\n'

output_map.write_text(txt)