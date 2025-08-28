total = 0.0
counter = 0.0
with open("ressources/history.txt") as historyfile:
    for line in historyfile:
        sigma = float(line.split(",")[5])
        if(sigma>0.01): continue
        total += sigma
        counter += 1.0
        print(sigma,counter)
        print("\n")

meansigma = float(total)/float(counter)
outputfile = open("ressources/meansigma.txt","w")
outputfile.write(str(meansigma))