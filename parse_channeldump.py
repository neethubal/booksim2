import networkx as nx

channels = []
channels_base = []

legal = list(map(str, range(10))) + ["i", "o"]

with open("base_netdump.txt", "r") as f:
    for line in f:
        if line[0] in legal:
            ll = line.split(",")
            channels_base.append(ll)

with open("netdump.txt", "r") as f:
    for line in f:
        if line[0] in legal:
            ll = line.split(",")
            channels.append(ll)

G = nx.DiGraph()

for source_router,source_port,dest_router,dest_port,count in channels_base:
    if dest_router[0] == "o" and ("i"+dest_router[1:], source_router) in G.edges():
        G["i"+dest_router[1:]][source_router]["dir"] = 'both'
        G = nx.relabel_nodes(G, {"i"+dest_router[1:]:"io"+dest_router[1:]})
        continue
    
    L = count.replace("\n","")
    L = int(L) if len(L)>0 else -1
    L = L if L >= 0 else ""


    if (dest_router, source_router) in G.edges():
        G[dest_router][source_router]["dir"] = 'both'
        continue
    
    if not isinstance(L, int) or L > 0:
        G.add_edge(source_router, dest_router, label="")

for source_router,source_port,dest_router,dest_port,count in channels:
    if dest_router[0] in ["i", "o"]:
        continue
    
    L = count.replace("\n","")
    L = int(L) if len(L)>0 else -1
    L = L if L >= 0 else ""

    if (dest_router, source_router) in G.edges():
        G[dest_router][source_router]["label"] += f"{source_router};{L} " if L!="" else ""
    
    if (source_router, dest_router) in G.edges():
        G[source_router][dest_router]["label"] += f"{source_router};{L} " if L!="" else ""

nx.nx_pydot.write_dot(G, "netdump.dot")