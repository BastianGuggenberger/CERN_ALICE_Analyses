from matplotlib import pyplot as plt


cutoffvec = []
metricvec = []
eventvec = []

with open("cutsvsmetrics.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        cutoffvec.append(float(rawline[0]))
        metricvec.append(float(rawline[1]))
        eventvec.append(float(rawline[2]))

fig,ax1 = plt.subplots()

color = "blue"
ax1.set_xlabel("tolerance parameter")
ax1.set_ylabel("metric", color=color)
ax1.plot(cutoffvec, metricvec, color=color, label="metric")

ax2 = ax1.twinx()
color = "red"
ax2.set_ylabel("remaining events", color=color)
ax2.plot(cutoffvec, eventvec, color=color, label="remaining events")

plt.title("metric and events vs tolerance parameter")
plt.savefig("tolerancevsmetric.png", dpi=300)
