import matplotlib.pyplot as plt

names = ["vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"]

rundict = {0: [], 1: [], 2:[], 3:[], 4:[], 5:[]}
meandict = {0: [], 1: [], 2:[], 3:[], 4:[], 5:[]}
widthdict = {0: [], 1: [], 2:[], 3:[], 4:[], 5:[]}

with open("runnumberplots/meanvswidth.csv") as f:
    for line in f:
        rawline = line.strip().split(",")
        rundict[int(rawline[0])].append(int(rawline[1]))
        meandict[int(rawline[0])].append(float(rawline[2]))
        widthdict[int(rawline[0])].append(float(rawline[3]))

for i in range(6):
    fig, ax1 = plt.subplots()

    label = "mean " + names[i]
    ax1.plot(rundict[i],meandict[i],label=label,color = "r")
    ax1.set_ylabel(label)
    ax1.set_xlabel("RunNumber")

    ax2 = ax1.twinx()
    label = "width " + names[i]
    ax2.plot(rundict[i],widthdict[i],label=label,color = "b")
    ax2.set_ylabel(label)

    fig.autofmt_xdate()
    plt.tight_layout()


    plt.savefig("runnumberplots/"+names[i])