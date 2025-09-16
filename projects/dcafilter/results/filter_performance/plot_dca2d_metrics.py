import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import math

#-----------------------------------
#VARIABLES (custom)
alpha = 0.6

#-----------------------------------
#FUNCTIONS:
def get_i_j(acc_xy, acc_z):
    i = 0
    for possible_i in range(1,12):
        if(0.00125+0.0025*possible_i>acc_xy):
            i=possible_i
            break
    j = 0
    for possible_j in range(1,12):
        if(0.00125+0.0025*possible_j>acc_z):
            j=possible_j
            break

    return i,j

#-----------------------------------
#LOAD DATA:

alphavec = []
xyvec = []
zvec = []
avec = []
bvec = []
cvec = []
tvec= []
Ngvec = []
Nbvec = []

a_0 = 0.0
b_0 = 0.0
c_0 = 0.0
t_0 = 0.0

Ng_0 = 0.0
Nb_0 = 0.0

with open("resources/original_a_b_c_t.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        a_0 = float(rawline[0])
        b_0 = float(rawline[1])
        c_0 = float(rawline[2])
        t_0 = float(rawline[3])

with open("resources/original_statistics.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        Ng_0 = float(rawline[0])
        Nb_0 = float(rawline[1])


with open("resources/2d/alpha_xy_z_a_b_c_t.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        alphavec.append(float(rawline[0]))
        xyvec.append(float(rawline[1]))
        zvec.append(float(rawline[2]))
        avec.append(float(rawline[3]))
        bvec.append(float(rawline[4]))
        cvec.append(float(rawline[5]))
        tvec.append(float(rawline[6]))

with open("resources/2d/alpha_xy_z_statistics.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        Ngvec.append(float(rawline[3]))
        Nbvec.append(float(rawline[4]))
#---------------------------------------------------
#MAIN:


dcas = [0.00 + 0.0025*i for i in range(1,12)]


#PLOT A-ratios:

aratios = np.zeros((12,12))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    aratios[i,j] = avec[n]/a_0

plt.figure(figsize=(8,6))
plt.imshow(aratios,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter performance for varying dca_xy and dca_z tolerances",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("remaining kaon events A_1 / original kaon events A_0", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_Aratios.png", dpi=300)


#PLOT B-ratios:

bratios = np.zeros((12,12))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    bratios[i,j] = bvec[n]/b_0

plt.figure(figsize=(8,6))
plt.imshow(bratios,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter performance for varying dca_xy and dca_z tolerances",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("remaining non-kaon events B_1 / original non-kaon events B_0", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_Bratios.png", dpi=300)


#PLOT efficiency:
#efficiency = (Ng after cut)/(Ng before cut)

efficiencies = np.zeros((11,11))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    i-=1
    j-=1
    efficiencies[i,j] = Ngvec[n]/Ng_0

plt.figure(figsize=(8,6))
plt.imshow(efficiencies,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter efficiency ( (Ng after cut)/(Ng before cut) )",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("efficiency", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_efficiency.png", dpi=300)


#PLOT rejection:
#rejection = 1 - (Nb after cut)/(Nb before cut)

rejections = np.zeros((11,11))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    i-=1
    j-=1
    rejections[i,j] = 1.0 -Nbvec[n]/Nb_0

plt.figure(figsize=(8,6))
plt.imshow(rejections,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter rejection ( 1 - (Nb after cut)/(Nb before cut))",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("rejection", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_rejection.png", dpi=300)


#PLOT purity:
#purity = (Ng after cut)/(Ng+Nb after cut)

purities = np.zeros((11,11))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    i-=1
    j-=1
    purities[i,j] = (Ngvec[n]/(Nbvec[n]+Ngvec[n]))

plt.figure(figsize=(8,6))
plt.imshow(purities,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter purity ( (Ng after cut)/(Ng+Nb after cut) )",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("purity", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_purity.png", dpi=300)


#PLOT significance:
#significance = (Ng after cut)/sqrt(Ng+Nb after cut)

significances = np.zeros((11,11))
for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    i-=1
    j-=1
    significances[i,j] = Ngvec[n]/math.sqrt(Nbvec[n]+Ngvec[n])

plt.figure(figsize=(8,6))
plt.imshow(significances,cmap ="viridis", extent=[dcas[0],dcas[-1],dcas[0],dcas[-1]], origin="lower")
plt.xlabel("dca_xy tolerance in mm",labelpad=15)
plt.ylabel("dca_z tolerance in mm", labelpad=15)
plt.title("kaon filter significance ( (Ng after cut)/sqrt(Ng+Nb after cut) )",fontweight="bold", pad = 20)
colorbar = plt.colorbar()
colorbar.set_label("significances", fontweight="bold",labelpad=15)
plt.subplots_adjust(top =0.85,bottom=0.15)
plt.savefig("results/2dcolorplot_significances.png", dpi=300)