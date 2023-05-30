from pathlib import Path
import sys


all_files = Path("build/full").rglob('*.*')

# for i,f in enumerate(all_files):
    # print("    ", hex(i), f.as_posix())

indexes = [file.as_posix().split("/full/")[1] for file in all_files if file.is_file()]

if sys.argv[2] == "p2e":
    print(hex(indexes.index(sys.argv[1])))
else:
    print(indexes[int(sys.argv[1], 16)])