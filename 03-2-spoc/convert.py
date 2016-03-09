
data = []
block = []

print eval("0xdf")
f = open("data", "r")
while(True):
    line = f.readline()
    if(line == ""):
        break;
    line = line.split()
    for i in line:
        i = "0x" + i
        i = eval(i)
        data.append(i)
    block.append(data)
    data = []
f.close()

def convert(str):
    print "Virtual Address " + str + ":"
    global block
    number = eval(str)
    offset = number & 0x1f
    pdeNo = number >> 10
    pteNo = number >> 5 & 0x1f
    pde = block[17][pdeNo]
    pdeValid = pde >> 7
    print " --> pde index:0x%x  pde contents:(valid %d pfn 0x%x)" % (pdeNo, pdeValid, pde & 0x7f)

    pte = block[pde & 0x7f][pteNo]
    print "    --> pte index:0x%x  pte contents:(valid %d pfn 0x%x)" % (pteNo, pte >> 7, pte & 0x7f)

    if (pdeValid != 1) or ((pte >> 7) != 1):
        print "      --> Fault (page directory entry not valid)"
        return

    Address = offset + (pte % 128)*32
    value = block[pte & 0x7f][offset]
    print "      --> Translates to Physical Address 0x%x --> Value: %x" % (Address, value)

convert("0x03df")
convert("0x69dc")


