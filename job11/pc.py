#!/usr/bin/python3
def produce(te):
    print("PRODUCE")

    i=0
    while(i<te):
        print("produce "+str(i))
        yield i
        i=i+1

def consume(te):
    print("\tCONSUME")
    i=0
    while(i<te):
        x = next(coro_p)
        print("\tconsume %d" % x)
        i=i+1
coro_p = produce(8)
consume(8)
