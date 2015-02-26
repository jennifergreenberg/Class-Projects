from A02 import *

# A* search on an n-puzzle
n=8
pmanhattan = NPuzzle(n, g=manhattan.g, h=manhattan.h)
print "Manhattan distance A* search"
graph_search(pmanhattan)

# Rerun on the same puzzle with BFS
print "Breadth first search"
pbreadth = NPuzzle(n, g=breadth_first.g, h=breadth_first.h, force_state = pmanhattan.initial.state_tuple())
graph_search(pbreadth)

# Rerun on the same puzzle with DFS
print "Depth first search"
pdepthfirst = NPuzzle(n, g=depth_first.g, h=depth_first.h, force_state = pmanhattan.initial.state_tuple())
graph_search(pdepthfirst)

# Simple puzzle that can be solved in 2 moves for easy testing purposes
# temp = [None,1,3,4,2,5,6,7,8]
# print "Depth First Search"
# puzzle = NPuzzle(8, g=depth_first.g, h=depth_first.h, force_state = temp)
# graph_search(puzzle)
