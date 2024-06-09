#!/usr/bin/python3
def produce(te):
    print("PRODUCE")

    i=0
    while(i<8):
        print(f"produce {i}")
        yield i
        i=i+1

def filter(te):
    print("\tFILTER")

    i=0
    while(i<8):
        x=next(coro_p)
        print(f"\tfilter {x}->{x*10}")
        yield x*10
        i=i+1

def consume(te):
    print("\t\tCONSUME")

    i=0
    while(i<8):
        x = next(coro_f)
        print(f"\t\tconsume {x}")
        i=i+1

    print("END")

coro_p = produce(8)
coro_f =filter(8)
consume(8)
