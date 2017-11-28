from matplotlib import pyplot as plt

def readTimes(fname):
    with open(fname, "rt") as f:
        return [list(map(int, line.split())) for line in f.readlines()]

def plotTimes(data, **kwargs):
    ns = [n for (n, _) in data]
    ts = [t for (_, t) in data]
    
    #ks = [n / t for (n, t) in data]
    #kMid = sum(ks) / len(ks)
    #errors = [abs(k - kMid) / abs((k + kMid) / 2) for k in ks]
    
    plt.plot(ns, ts, **kwargs)
    plt.scatter(ns, ts, **kwargs)

plotTimes(readTimes("times.txt"))
plt.savefig("plot.png")
plt.show()
