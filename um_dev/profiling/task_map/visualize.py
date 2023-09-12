'''
Visualize apollo task graph
Yuting
2022.4.24
'''

from analyze import load_map, build_graph
import networkx as nx
import matplotlib.pyplot as plot
import numpy as np

def build_nx_graph(adj_list):
  G = nx.DiGraph()
  edges = []
  for node in adj_list:
    for nxt in node["out"]:
      edges.append((node["name"].replace("Component", " "),
                    nxt.replace("Component", " ")))
  G.add_edges_from(edges)

  fig = plot.gcf()
  fig.set_size_inches(8.5, 8.5)
  colors = ['r'] * len(G.nodes)
  nx.draw_networkx(G, pos=nx.spring_layout(G), node_color=colors)
  plot.tick_params(labelbottom='off', labelleft='off')
  plot.axis('off')
  plot.subplots_adjust(top=1, bottom=0, left=0, right=1, hspace=0, wspace=0)
  plot.savefig('component_graph.png', dpi=100)
  plot.show()


if __name__ == "__main__":
  nodes, channels = load_map("component_map.txt")
  adj_list = build_graph(nodes)
  build_nx_graph(adj_list)
