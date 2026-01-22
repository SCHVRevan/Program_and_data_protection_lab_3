import z3

# bv = 32 (if 8 -> overflow)
pwd = [
    z3.BitVec(f'pwd[{i}]', 32) for i in range(32)
]

all_printable = z3.And([
    z3.And([p > 32, p < 127]) for p in pwd
])

check_1 = z3.And([
    pwd[0] == ord('E'),
    pwd[1] == ord('1'),
    pwd[2] == ord('i'),
    pwd[3] == ord('t')
])

# check_2 по сути можно обходить, если считать,
# что мы поняли все приколы с флагами

check_3 = z3.And([
    pwd[19] == pwd[4] ^ pwd[9],
    pwd[21] == pwd[16] ^ pwd[13],
    pwd[4] == pwd[16],
    pwd[16] == pwd[27],
    pwd[9] == pwd[13],
    pwd[13] == pwd[24],
    pwd[4] - pwd[9] > 0x3a,
    pwd[4] - pwd[9] < 61,
    (pwd[4] + pwd[9]) % 127 == 4
])

s = z3.Solver()
s.add(all_printable)
s.add(check_1)
s.add(check_3)

for i in range(10):
    assert s.check() == z3.sat
    model = s.model()

    res = ''
    for p in pwd:
        res += chr(model[p].as_long())
    print(res)

    not_this_pwd = z3.Not(z3.And([
        p == model[p].as_long() for p in pwd
    ]))
    s.add(not_this_pwd)