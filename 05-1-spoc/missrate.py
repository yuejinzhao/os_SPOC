import numpy

ACCESS_COUNT = 20
WINDOW_SIZE = 2
PAGE_NUM = 6
MEM_SIZE = 4

if __name__ == '__main__':
    aclist = numpy.random.randint(0, PAGE_NUM, ACCESS_COUNT)

    D = dict()
    last = -1
    for c in xrange(ACCESS_COUNT):
        idx = aclist[c]
        if idx in D:
            D[idx] = c
            print "access", idx, "hit"
        else:
            if c - last <= WINDOW_SIZE:
                D[idx] = c
                print "access", idx, "miss", "load", D.keys()
                last = c
                if(len(D) > MEM_SIZE):
                    temp = D.items()
                    temp.sort(lambda x, y:x[1]-y[1])
                    D = dict[temp[1:]]

            else:
                temp = filter(lambda x:x[1] >= last, D.items())
                D = dict(map(lambda x: (x[0], 0), temp))
                D[idx] = c
                print "access", idx, "miss", "filter & load", D.keys()
                last = c
        assert(len(D) <= MEM_SIZE)

