import pygraphviz as pgv



def parseRes(res):
    res=res.split()[1:]
    for i in range(0,len(res)):
        res[i]=int(res[i])
    return res

def creation_graphe(nbNoeuds,nbFeatures,res,nom_fichier):

    print("creation "+nom_fichier)
    LNoeuds=[]
    G = pgv.AGraph(strict="true",directed="true",rankdir="LR")
    k=0
    print("etape" + str(k) +":")
    print(res)
    for i in range(0,nbNoeuds):
        for j in range(0,nbFeatures):
            if(res[i*nbFeatures+j]>0):
                LNoeuds.append("f{}".format(j+1))
    res=res[nbNoeuds*nbFeatures:]
    k+=1
    print(LNoeuds)
    print("etape" + str(k) +":")
    print(res)
    for i in range(1,nbNoeuds+1):
        for j in range(i+1,nbNoeuds+1):
            if(res[int(nbNoeuds*(i-1)-((i*i-i)/2)+j-i)-1]>0):
                print(LNoeuds[i-1],LNoeuds[j-1])
                G.add_edge(LNoeuds[i-1],LNoeuds[j-1],color="green")
    res=res[int((nbNoeuds*nbNoeuds-nbNoeuds)/2):]
    k+=1
    print("etape" + str(k) +":")
    print(res)
    for i in range(0,nbNoeuds):
        if (res[i]>0):
            G.add_edge(LNoeuds[i],"T",color="green")
    res=res[nbNoeuds:]
    k+=1
    print("etape" + str(k) +":")
    print(res)
    for i in range(0,nbNoeuds):
        if (res[i]>0):
            G.add_edge(LNoeuds[i],"_|_",color="green")
    res=res[nbNoeuds:]
    k+=1
    print("etape" + str(k) +":")
    print(res)
    for i in range(1,nbNoeuds+1):
        for j in range(i+1,nbNoeuds+1):
            if(res[int(nbNoeuds*(i-1)-(i*(i-1)/2)+j-i)-1]>0):
                G.add_edge(LNoeuds[i-1],LNoeuds[j-1],color="red")
    res=res[int(nbNoeuds*(nbNoeuds-1)/2):]
    k+=1
    print("etape" + str(k) +":")
    print(res)
    for i in range(0,nbNoeuds):
        if (res[i]>0):
            G.add_edge(LNoeuds[i],"T",color="red")
    res=res[nbNoeuds:]
    k+=1
    print("etape" + str(k) +":")
    print(res)
    for i in range(0,nbNoeuds):
        if (res[i]>0):
            G.add_edge(LNoeuds[i],"_|_",color="red")
            k+=1
    res=res[nbNoeuds:]
    print("etape" + str(k) +":")
    print(res)

    print(res[(nbNoeuds-1)*10:(nbNoeuds+1)*10])
    res=res[(nbNoeuds+1)*10:]
    print("------------Tjk---------------")
    print(res)
    G.write("oui.dot")
    G.layout("dot")
    G.draw("{}.png".format(nom_fichier))
    print("------FIN--------")
    print("------FIN--------")
    print("------FIN--------")
    print("------FIN--------")
    print("------FIN--------")



