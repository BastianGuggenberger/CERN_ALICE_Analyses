import matplotlib.pyplot as plt
import numpy as np

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

a_0 = 0.0
b_0 = 0.0
c_0 = 0.0
t_0 = 0.0

with open("ressources/original_a_b_c_t.txt") as f:
    for line in f:
        rawline = line.strip().split(",")
        a_0 = float(rawline[0])
        b_0 = float(rawline[1])
        c_0 = float(rawline[2])
        t_0 = float(rawline[3])


sourcepath="ressources/2d/alpha_xy_z_a_b_c_t.txt"
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