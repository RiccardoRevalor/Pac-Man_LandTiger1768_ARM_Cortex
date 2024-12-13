import os

#generate an empty max
xmax = 48
ymax = 64
mazestart = 20

if os.path.exists("map.txt"):
    os.remove("map.txt")

f = open("map.txt", "w")
#put walls on the edges
for y in range(mazestart, ymax - mazestart):
    for x in range(xmax):
        if x == 0 or x == 1 or x == xmax - 1 or y == mazestart or y == ymax - mazestart:
            f.write("1")
        else:
            f.write("0")
    f.write("\n")

f.close()