
# 0.4 - 100 0.1 - 25
Period = 100
t1 = 2000
t2 = 2000
t3 = 2000

v = 0.25
data = []
output = []
for t in range(int(t1 / Period)):
    data.append(int(1023 * (1 + t * Period * (v - 1) / t1) ))
for t in range(int(t2 / Period)):
    data.append(int(1023 * v ))
for t in range(int(t3 / Period)):
    data.append(int(1023 * (((39 * v + 1)/40) + t * Period * (1 - v) / (t1)) ))
for _ in range(len(data)):
    output.append(data[_] << 10 | data[_])
with open('res.txt', 'w') as f:
    cnt = 0
    for _ in range(len(output)):
        cnt += 1
        f.write(str(output[_]) + ',')
        if cnt % 10 == 0:
            f.write('\n')
