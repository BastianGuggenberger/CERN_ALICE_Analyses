import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.lines as mlines

databaseids = [0,1,2,3,4,5,6,7,8]
databasenames = ["LHC22_pass4_highIR_Thin",#0
                 "LHC22_pass4_highIR_Thin_sampling",#1
                 "LHC22_pass4_lowIR",#2
                 "LHC22_pass7_skimmed",#3
                 "LHC23_pass4_skimmed",#4
                 "LHC23_pass4_Thin",#5
                 "LHC23_pass4_Thin_small",#6
                 "LHC24_pass1_MinBias",#7
                 "LHC24_pass1_skimmed"]#8


#Load total runmap:

totalrunmap = {}
for id in databaseids:
    path = "../runmaps/runmapdb" + str(id) + ".txt"
    runmapfile = open(path, "r")
    for line in runmapfile:
        run = int(line.split(",")[0])
        amount = (line.split(",")[1])
        if run in totalrunmap:
            totalrunmap[run][id] = amount
        else:
            totalrunmap[run] = [0 for i in databaseids]
            totalrunmap[run][id] = amount


#Start plotting:
plt.figure(figsize=(17,5))
colorset = ["red","blue","green","orange","purple","cyan","magenta","gold","brown"]
for id in databaseids:
    x = totalrunmap.keys()
    y = [float(id)] * len(x)
    plt.ylim(-1,9)
    colors = [colorset[id] if totalrunmap[run][id]!=0 else "grey" for run in x]
    label = "DB " + str(id)
    plt.vlines(x,ymin=id-0.4, ymax=id+0.4 ,colors=colors,label=label)

plt.xlabel("RunNumber",fontweight="bold")
plt.ylabel("Dataset",fontweight="bold")
plt.yticks(databaseids,databasenames)
legend_handles = []
for id in databaseids:
    legend_handles.append(mlines.Line2D([], [], color=colorset[id], marker='o', linestyle='None', markersize=4, label="DB " + str(id)))
plt.legend(handles=legend_handles, loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=len(databaseids))
plt.title("Overlap in RunNumbers for ALICE Datasets",fontweight="bold")
plt.subplots_adjust(bottom=0.23)
plt.subplots_adjust(left=0.23)
plt.savefig("runoverlap.png",dpi=400)



#convert totalrunmap to csv
csvfile = open("runoverlap.csv","w")
csvfile.write("DB")
for run in totalrunmap.keys():
    csvfile.write(",")
    csvfile.write(str(run))
csvfile.write("\n")

for id in databaseids:
    csvfile.write(str(id))
    for run in totalrunmap.keys():
        csvfile.write(",")
        csvfile.write(str(totalrunmap[run][id]).strip())
    csvfile.write("\n")


runmapfile.close()
csvfile.close()