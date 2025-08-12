from matplotlib import pyplot as plt

colors_tab20 = [
    "#1f77b4", "#aec7e8", "#ff7f0e", "#ffbb78",
    "#2ca02c", "#98df8a", "#d62728", "#ff9896",
    "#9467bd", "#c5b0d5", "#8c564b", "#c49c94",
    "#e377c2", "#f7b6d2", "#7f7f7f", "#c7c7c7",
    "#bcbd22", "#dbdb8d", "#17becf", "#9edae5"
]

#LOAD DATA:

alphavec = []
avec = []
bvec = []
cvec = []
tvec= []

with open("ressources/original_a_b_c_t.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        a_0 = float(rawline[0])
        b_0 = float(rawline[1])
        c_0 = float(rawline[2])
        t_0 = float(rawline[3])

with open("ressources/alpha_a_b_c_t.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        alphavec.append(float(rawline[0]))
        avec.append(float(rawline[1]))
        bvec.append(float(rawline[2]))
        cvec.append(float(rawline[3]))
        tvec.append(float(rawline[4]))





#------------------------------------------------
#PLOTS: 


#1.Plot: cutoffs as x axis

fig,ax1 = plt.subplots()

color = "blue"
ax1.set_xlabel("tolerance parameter alpha")
ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0", color=color)
ax1.plot(alphavec, [a_1/a_0 for a_1 in avec], color=color)

ax2 = ax1.twinx()
color = "red"
ax2.set_ylabel("remaining events T_1 / original events T_0", color=color)
ax2.plot(alphavec, [t_1/t_0 for t_1 in tvec], color=color)

plt.title("metrics vs tolerance parameter alpha")
plt.savefig("results/tolerancevsgausmetrics.png", dpi=300)
plt.close()



#2. Plot: event ratio as x axis

fig,ax1 = plt.subplots(figsize=(10,7))
xvec = [t_1/t_0 for t_1 in tvec]

ax1.set_xlabel("remaining events T_1 / original events T_0")
ax1.plot(xvec, [a_1/a_0 for a_1 in avec], color="blue")
ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0",color="blue",fontweight="bold")

ax2=ax1.twinx()
ax2.plot(xvec, [a_1/cvec[i] for i,a_1 in enumerate(avec)], color="red")
ax2.set_ylabel("remaining kaon events A_1 / underground events C_1",color= "red",fontweight="bold")

for i, alpha in enumerate(alphavec):
    ax1.vlines(xvec[i],0,0.2,color=colors_tab20[i],linestyles="dashed", label=alpha, linewidth=0.7)
#ax1.set_xticks(eventvec)
#ax1.set_xticklabels(cutoffvec)


ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="tolerance parameter alpha")
fig.subplots_adjust(bottom=0.3)

plt.yscale("log")
plt.gca().invert_xaxis()
plt.title("metrics vs remaining events", fontweight="bold")
plt.savefig("results/gausmetricvsevents.png", dpi=300)
plt.close()


#3. Plot: event ratio as x axis, plotting remaining non-kaon events
originalnokaon = t_0 - a_0

fig,ax1 = plt.subplots(figsize=(10,7))
xvec = [t_1/t_0 for t_1 in tvec]

ax1.set_xlabel("remaining events T_1 / original events T_0")
ax1.plot(xvec, [a_1/a_0 for a_1 in avec], color="blue")
ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0",color="blue",fontweight="bold")

ax2=ax1.twinx()
ax2.plot(xvec, [b_1/b_0 for b_1 in bvec], color="green")
ax2.set_ylabel("remaining non-kaon events B_1 / original non-kaon events B_0",color= "green",fontweight="bold")

for i, alpha in enumerate(alphavec):
    ax1.vlines(xvec[i],0,0.2,color=colors_tab20[i],linestyles="dashed", label=alpha, linewidth=0.7)
#ax1.set_xticks(eventvec)
#ax1.set_xticklabels(cutoffvec)


ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="tolerance parameter alpha")
fig.subplots_adjust(bottom=0.3)

plt.yscale("log")
plt.gca().invert_xaxis()
plt.title("metrics vs remaining events", fontweight="bold")
plt.savefig("results/nokaonvsevents.png", dpi=300)
