#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Analyse apollo task graph
Yuting
2022.4.24
'''

import re

def load_map(map_file):
  nodes = []
  channels = set()
  with open(map_file) as f:

    for line in f.readlines():
      node = line.split()[0]
      nodes.append({
          "name": node,
          "in_channels": set(),
          "out_channels": set(),
        })
      pattern = re.compile("({.*?})")
      in_channels, out_channels = pattern.findall(line)
      in_channels, out_channels = in_channels.strip('{}'), out_channels.strip('{}')
      in_channels, out_channels = in_channels.split(',') , out_channels.split(',')
      for channel in in_channels:
        channel = channel.strip(" \"")
        nodes[-1]["in_channels"].add(channel.strip(" \"")) # also strip space!
        channels.add(channel)
      for channel in out_channels:
        channel = channel.strip(" \"")
        nodes[-1]["out_channels"].add(channel.strip(" \""))
        channels.add(channel)

  return nodes, channels

def build_graph(nodes):
  adj_list = []
  for cur in nodes:
      adj_list.append({
        "name": cur["name"],
        "out": [],
      })
      for nxt in nodes:
        if nxt["name"] == cur["name"]:
          continue
        if len(cur["out_channels"]&nxt["in_channels"]) > 0:
          adj_list[-1]["out"].append(nxt["name"])
  return adj_list



if __name__ == "__main__":
  nodes, channels = load_map("component_map.txt")
  adj_list = build_graph(nodes)
  for each in adj_list:
    print(f'{each["name"]} -> {each["out"]}')


