import os
from pathlib import Path
from sys import argv

gameid = argv[1]
dolphin_dir = Path(os.path.expanduser('~/Documents/Dolphin Emulator/Maps'))

print('map file folder:', dolphin_dir)

map_file = dolphin_dir.joinpath(f'{gameid}.map')
map_file_contents = '.text section layout\n'

partitioned_map_files = list(dolphin_dir.glob(f'*_{gameid}.map'))

for partitioned_map in partitioned_map_files:

    print('    combining', partitioned_map.name, '->', map_file.name)

    lines = partitioned_map.read_text('utf-8').splitlines()
    lines = [l for l in lines if not l.startswith('.') and len(l) > 17]
    map_file_contents += '\n'.join(lines) + '\n'

print('combined', len(partitioned_map_files), 'map files into', map_file.name)
map_file.write_text(map_file_contents, 'utf-8')