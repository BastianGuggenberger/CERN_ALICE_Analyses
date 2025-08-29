import matplotlib
matplotlib.use("Agg")
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
xyvec = []
zvec = []
avec = []
bvec = []
cvec = []
tvec= []

a_0 = 0.0
b_0 = 0.0
c_0 = 0.0
t_0 = 0.0

def getdata(folder):
    global a_0,b_0,c_0,t_0,alphavec,xyvec,zvec,avec,bvec,cvec,tvec

    alphavec = []
    xyvec = []
    zvec = []
    avec = []
    bvec = []
    cvec = []
    tvec= []

    with open("resources/original_a_b_c_t.txt") as f:
        for line in f:
            rawline = line.strip().split(",")
            a_0 = float(rawline[0])
            b_0 = float(rawline[1])
            c_0 = float(rawline[2])
            t_0 = float(rawline[3])


    sourcepath="resources/"+folder+"alpha_xy_z_a_b_c_t.txt"
    with open(sourcepath) as f:
        for line in f:
            rawline = line.strip().split(",")
            alphavec.append(float(rawline[0]))
            xyvec.append(float(rawline[1]))
            zvec.append(float(rawline[2]))
            avec.append(float(rawline[3]))
            bvec.append(float(rawline[4]))
            cvec.append(float(rawline[5]))
            tvec.append(float(rawline[6]))





#------------------------------------------------
#PLOTS FOR ALPHA: 

def plotalphavsgausmetrics():
    getdata("varyalpha/")

    #1.Plot: alphas as x axis

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
        ax1.vlines(xvec[i],0,0.2,color=colors_tab20[i],label=alpha, linewidth=0.7)
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
    plt.close()


    #4. Plot: nonkaon event ratio as x axis

    fig,ax1 = plt.subplots(figsize=(10,7))
    xvec = [b_1/b_0 for b_1 in bvec]
    ax1.set_xlabel("remaining non-kaon events B_1 / original non-kaon events B_0")

    ax1.plot(xvec, [a_1/a_0 for a_1 in avec], color="blue")
    ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0",color="blue",fontweight="bold")

    ax2=ax1.twinx()
    ax2.plot(xvec, [a_1/cvec[i] for i,a_1 in enumerate(avec)], color="red")
    ax2.set_ylabel("remaining kaon events A_1 / underground events C_1",color= "red",fontweight="bold")

    for i, alpha in enumerate(alphavec):
        ax1.vlines(xvec[i],0,0.2,color=colors_tab20[i],label=alpha, linewidth=0.7)
    #ax1.set_xticks(eventvec)
    #ax1.set_xticklabels(cutoffvec)


    ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="tolerance parameter alpha")
    fig.subplots_adjust(bottom=0.3)

    plt.yscale("log")
    plt.gca().invert_xaxis()
    plt.title("metrics vs remaining events", fontweight="bold")
    plt.savefig("results/nonkaonasx_alpha.png", dpi=300)
    plt.close()




#------------------------------------------------
#PLOTS FOR XY_Z: 

def plotxyzvsgausmetrics():
    getdata("varyxyz/")
    xytoz = [xy/zvec[i] for i,xy in enumerate(xyvec)]


    #1.Plot: xytoz as x axis

    fig,ax1 = plt.subplots()

    color = "blue"
    ax1.set_xlabel("acc_xy / acc_z  ratio")
    ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0", color=color)
    ax1.plot(xyvec, [a_1/a_0 for a_1 in avec], color=color)

    ax2 = ax1.twinx()
    color = "red"
    ax2.set_ylabel("remaining events T_1 / original events T_0", color=color)
    ax2.plot(xyvec, [t_1/t_0 for t_1 in tvec], color=color)

    plt.title("metrics vs accuracy ratio")
    plt.savefig("results/accuracyratiovsgausmetrics.png", dpi=300)
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

    for i, ratio in enumerate(xytoz):
        ax1.scatter(xvec[i],avec[i]/a_0,color=colors_tab20[i], label=ratio)
        ax2.scatter(xvec[i],avec[i]/cvec[i],color=colors_tab20[i])
    #ax1.set_xticks(eventvec)
    #ax1.set_xticklabels(cutoffvec)


    ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="xy-tolerance / z-tolerance ratio")
    fig.subplots_adjust(bottom=0.3)

    plt.yscale("log")
    plt.gca().invert_xaxis()
    plt.title("metrics vs remaining events", fontweight="bold")
    plt.savefig("results/gausmetricvsevents_xyz.png", dpi=300)
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

    for i, ratio in enumerate(xytoz):
        ax1.scatter(xvec[i],avec[i]/a_0,color=colors_tab20[i], label=ratio)
        ax2.scatter(xvec[i],bvec[i]/b_0,color=colors_tab20[i])
    #ax1.set_xticks(eventvec)
    #ax1.set_xticklabels(cutoffvec)


    ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="xy-tolerance / z-tolerance ratio")
    fig.subplots_adjust(bottom=0.3)

    plt.yscale("log")
    plt.gca().invert_xaxis()
    plt.title("metrics vs remaining events", fontweight="bold")
    plt.savefig("results/nokaonvsevents_xyz.png", dpi=300)
    plt.close()

    #4. Plot: nonkaon event ratio as x axis

    fig,ax1 = plt.subplots(figsize=(10,7))
    xvec = [b_1/b_0 for b_1 in bvec]
    ax1.set_xlabel("remaining non-kaon events B_1 / original non-kaon events B_0")

    ax1.plot(xvec, [a_1/a_0 for a_1 in avec], color="blue")
    ax1.set_ylabel("remaining kaon events A_1 / original kaon events A_0",color="blue",fontweight="bold")

    ax2=ax1.twinx()
    ax2.plot(xvec, [a_1/cvec[i] for i,a_1 in enumerate(avec)], color="red")
    ax2.set_ylabel("remaining kaon events A_1 / underground events C_1",color= "red",fontweight="bold")

    for i, ratio in enumerate(xytoz):
        ax1.scatter(xvec[i],avec[i]/a_0,color=colors_tab20[i], label=ratio)
        ax2.scatter(xvec[i],avec[i]/cvec[i],color=colors_tab20[i])
    #ax1.set_xticks(eventvec)
    #ax1.set_xticklabels(cutoffvec)


    ax1.legend( loc = "upper center", bbox_to_anchor=(0.5, -0.2),ncol=4,title="xy-tolerance / z-tolerance ratio")
    fig.subplots_adjust(bottom=0.3)

    plt.yscale("log")
    plt.gca().invert_xaxis()
    plt.title("metrics vs remaining events", fontweight="bold")
    plt.savefig("results/nonkaonasx_xyz.png", dpi=300)
    plt.close()    



#-------------------------------------------------
#MAIN:

plotalphavsgausmetrics()
plotxyzvsgausmetrics()