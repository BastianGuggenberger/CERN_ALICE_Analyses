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

aratios = np.zeros((12,12))
bratios = np.zeros((12,12))

for n in range(len(xyvec)):
    i, j = get_i_j(xyvec[n],zvec[n])
    print(i,j)
    aratios[i,j] = avec[n]/a_0


#PLOT:
plt.imshow(aratios,cmap ="viridis", origin="lower")
plt.colorbar(label="emaining kaon events A_1 / original kaon events A_0")
plt.show()

