import matplotlib.pyplot as plt

names = ["histo_vt_k0band", "histo_vt_nonk0band", "histo_vz_k0band", "histo_vz_nonk0band"]
meandict = {"histo_vt_k0band": [], "histo_vt_nonk0band": [], "histo_vz_k0band":[], "histo_vz_nonk0band":[]}
widthdict = {"histo_vt_k0band": [], "histo_vt_nonk0band": [], "histo_vz_k0band":[], "histo_vz_nonk0band":[]}
dcadict = {"histo_vt_k0band": [], "histo_vt_nonk0band": [], "histo_vz_k0band":[], "histo_vz_nonk0band":[]}

with open("dcaplots/meanvswidth.csv") as f:
    for line in f:
        rawline = line.strip().split(",")
        dcadict[rawline[0]].append(0.0125 + int(rawline[1])*0.025)
        meandict[rawline[0]].append(float(rawline[2]))
        widthdict[rawline[0]].append(float(rawline[3]))

for i in range(4):
    fig, ax1 = plt.subplots()

    label = "mean " + names[i]
    ax1.plot(dcadict[names[i]],meandict[names[i]],label=label,color = "r")
    ax1.set_ylabel(label)
    ax1.set_xlabel("dcaxy")
    ax1.yaxis.label.set_color("r")

    ax2 = ax1.twinx()
    label = "width " + names[i]
    ax2.plot(dcadict[names[i]],widthdict[names[i]],label=label,color = "b")
    ax2.set_ylabel(label)
    ax2.yaxis.label.set_color("b")

    fig.autofmt_xdate()
    plt.tight_layout()

    plt.savefig("dcaplots/"+names[i])