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


fig, ax1 = plt.subplots()
ax1.set_xlabel("dcaxy")
ax1.set_ylabel("mean vertex position")
for i in range(4):
    ax1.plot(dcadict[names[i]],meandict[names[i]],label=names[i])
plt.legend()

plt.savefig("dcaplots/meanvertexvsdca")

fig2, ax2 = plt.subplots()
ax2.set_xlabel("dcaxy")
ax2.set_ylabel("vertex position width")
for i in range(4):
    ax2.plot(dcadict[names[i]],widthdict[names[i]],label=names[i])
plt.legend()

plt.savefig("dcaplots/widthvertexvsdca")