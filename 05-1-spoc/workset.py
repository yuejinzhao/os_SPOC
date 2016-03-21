mem = ['e', 'd', 'a']
a = ['e', 'd', 'a']
addr = ['c', 'c', 'd', 'b', 'c', 'e', 'a','d']
time = 4
for ad in addr:
    if not ad in addr:
        print "miss\n"
        print ad
        mem.append(ad)
    else:
        print "hit\n"
        print ad
        if(len(a) == time):
            a.append(ad)
            a = a[1:]
        else:
            a.append(ad)
    for me in mem:
        if not me in a:
            print "filter\n"
            print me
            mem.remove(me)

