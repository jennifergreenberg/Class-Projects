# Bryant To, Jessica To
# CS550 - AI A02

from basicsearch_lib.Representation import Problem
from basicsearch_lib.TileBoard import TileBoard
from basicsearch_lib.utils import PriorityQueue
from basicsearch_lib.searchspace import *

class NPuzzle(Problem):

	def __init__(self, n, force_state=None, **kwargs):
		"""Override the constructor from the Problem class. n is the puzzle size, the
		optional force_state argument initializes the puzzle to a specific state, and
		the remaining kwargs will be passed on to the parent constructor"""
		if force_state!=None:
			board = TileBoard(n, force_state=force_state)
		else:
			board = TileBoard(n)
		goal = [[1,2,3], [4,None,5], [6,7,8]]
		super(NPuzzle, self).__init__(initial=board, goals=goal, **kwargs)

	# actions method provided in Representation source file

	def result(self, state, action):
		"""Override the result method from the Problem class to return a new state
		based upon the specified action"""
		if action not in self.actions(state):
			raise ValueError("%d is an illegal move" %action)
		return state.move(action)

class breadth_first(object):
	@classmethod
	def g(cls, parentnode, action, childnode):
		"""Computes the cost to move from initial state to the child via the action"""
		return childnode.depth

	@classmethod
	def h(cls, state):
		"""Estimates the cost of moving from current state to nearest goal node
		Using the sum of Manhattan distance as heuristic"""
		return 0

class depth_first(object):
	@classmethod
	def g(cls, parentnode, action, childnode):
		"""Computes the cost to move from initial state to the child via the action"""
		cost = -childnode.depth
		return cost

	@classmethod
	def h(cls, state):
		"""Estimates the cost of moving from current state to nearest goal node
		Using the sum of Manhattan distance as heuristic"""
		return 0

class manhattan(object):
	@classmethod
	def g(cls, parentnode, action, childnode):
		"""Computes the cost to move from initial state to the child via the action"""
		return parentnode.depth + 2

	@classmethod
	def h(cls, state):
		"""Estimates the cost of moving from current state to nearest goal node
		Using the sum of Manhattan distance as heuristic"""
		distance = 0
		goal = [[0,0],[0,1],[0,2],[1,0],[1,1],[1,2],[2,0],[2,1],[2,2]]
		for row in range(state.boardsize):
			for column in range(state.boardsize):
				value = state.board[row][column]
				if value is None:
					tile = goal[4]
					distance += abs(row-tile[0]) + abs(column-tile[1])
				elif value in range(1,5,1):
					tile = goal[value-1]
					distance += abs(row-tile[0]) + abs(column-tile[1])
				else:
					tile = goal[value]
					distance += abs(row-tile[0]) + abs(column-tile[1])
		return distance

def graph_search(problem, debug=False):
	"""Given a problem description and optional search debug argument, provides
	a solution"""
	frontier = PriorityQueue(f=Node.get_f)
	explored = Explored()
	done = False

	root = Node(problem=problem, state=problem.initial, parent=None, action=None)
	frontier.append(root)
	explored.add(root.state)

	while not done:
		parent = frontier.pop()

		# Print nodes popped from queue for debugging purposes
		# print "dequeued node======================="
		# print parent.__repr__()

		# Check if the popped node is the solution
		if parent.state.solved():
			print parent.solution()
			done = True
		
		# If the solution is not found, expand the children and add them to the
		# explored set and queue while taking care of duplicates
		else:
			children = parent.expand(problem=problem)

			# Print children for debugging purposes
			# print "children=========================="
			# print_nodes(children)
			
			# Put children into frontier and explored
			for i in range(children.__len__()):
				if not explored.exists(children[i].state):
					explored.add(children[i].state)
					frontier.append(children[i])
