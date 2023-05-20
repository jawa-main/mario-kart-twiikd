import json


data = open('data.json', 'r').read()

info = json.loads(data)
code_numbers = [int(code.replace("<code>", "").replace("</code>", ""), 16) for code in info['Code Number']]
code_numbers = list(dict.fromkeys(code_numbers))

print(code_numbers)

lines = open('dolphin.log', 'r').readlines()
lines = [l[64:][:-1] for l in lines if "CHETOF" in l]
for line in lines:
    id_, cb = line.split(',')
    id_ = int(id_.replace('id: ', ''), 16)
    cb = int(cb.replace('cb: ', ''), 16)

    if id_ not in code_numbers:
        continue
    idx = code_numbers.index(id_)
    desc = info['Description'][idx]

    print(hex(cb), desc)