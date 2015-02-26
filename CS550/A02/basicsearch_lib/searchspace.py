"""Search (Chapters 3-4)

Representation of nodes in a search space and an explored set to prevent
reexploration of duplicate states

Modified from text book code"""

#from utils import *

def print_nodes(nodes):
    "print_nodes - print node representations on same line"
    
    if len(nodes) > 0:
        nodereps = []  # string representation of each node
        linecounts = []  # lines in string representation
        widthstr = []  # format string for each node to align display
        # Split representation into lines and add to list
        for n in nodes:
            lines = str(n).split("\n")
            # Make format string k characters wider than longest node line
            # Produces format for fixed length string field, e.g. %12s
            widthstr.append("%%%ds"%(max([len(l) for l in lines])+2))
            linecounts.append(len(lines))  # track # lines needed for node
            nodereps.append(lines)
        
        for lineidx in xrange(max(linecounts)):
            for nodeidx in xrange(len(nodes)):
                # Print out row of node representation and stay on same line
                try:
                    print widthstr[nodeidx]%(nodereps[nodeidx][lineidx]),
                except IndexError:
                    # This node has fewer lines than the longest one
                    print widthstr[nodeidx]%" ", 
            print # move to next line


#______________________________________________________________________________

class Node:
    """A node in a search tree. Contains a pointer to the parent (the node
    that this is a successor of) and to the actual state for this node. Note
    that if a state is arrived at by two paths, then there are two nodes with
    the same state.  Also includes the action that got us to this state, and
    the total path_cost (also known as g) to reach the node.  Other functions
    may add an f and h value; see best_first_graph_search and astar_search for
    an explanation of how the f and h values are handled. You will not need to
    subclass this class."""

    def __init__(self, problem, state, parent=None, action=None):
        "Create a search tree Node, derived from a parent by an action."
        self.problem = problem # Save problem representation
        self.state = state
        self.parent = parent
        self.action = action
        # find new node's depth and parent and cost from start
        if parent:
            self.depth = parent.depth + 1
            if not action:
                raise ValueError("New search nodes can only be derived " +
                                 "via an action")
            self.g = problem.g(parent, action, self)
        else:
            self.depth = 0  # root of search tree
            self.g = 0  # cost of initial nodes
            
        self.h = problem.h(self.state)
        self.f = self.g + self.h
           
    def expand(self, problem):
        "List the nodes reachable in one step from this node."
        return [self.child_node(action)
                for action in problem.actions(self.state)]

    def child_node(self, action):
        """"child_node - Derive child node by applying an action to problem 
        problem contains the current state representation
        action indicates how the new state will be derived from the current
        Similar to Fig. 3.10 of your text."""

        # derive new state
        nstate = self.problem.result(self.state, action)
        # Create child
        return Node(self.problem, nstate, parent=self, action=action)

    def solution(self):
        "Return the sequence of actions to go from the root to this node."
        return [node.action for node in self.path()[1:]]

    def path(self):
        "Return a list of nodes forming the path from the root to this node."
        node, path = self, []
        # Chase parent pointers, appending each node as it is found
        while node:
            path.append(node)
            node = node.parent
        # List is from goal to initial state,
        # reverse to provide initial state to goal
        path.reverse()
        return path
    
    def get_f(self):
        "get_f estimate of cost from initial node to goal node"
        return self.f
    
    def get_g(self):
        "get_g estimate of cost form initial node to this node"
        return self.g
    
    def get_h(self):
        "get_h estimate of cost from this node to closest goal"
        return self.h    
    # We want for a queue of nodes in breadth_first_search or
    # astar_search to have no duplicated states, so we treat nodes
    # with the same state as equal. [Problem: this may not be what you
    # want in other contexts.]

    def __eq__(self, other):
        return isinstance(other, Node) and self.state == other.state

    def __hash__(self):
        return hash(self.state)
    
    def __repr__(self):
        return "f=%.1f (g=%.1f + h=%.1f)\n%s"%(
                self.f, self.g, self.h, self.state)


class Explored(object):
    "Maintain an explored set.  Assumes that states are hashable"

    def __init__(self):
        self.hashtable = dict()
        self.tuple = ()
        
    def exists(self, state):
        "exists - Has this state already been explored?"
        
        # hash the state for quick checking
        if self.tuple == state.state_tuple():
            print "matched"
            
        hashval = hash(state)
        try:
            # have we seen this hashcode before?
            hashlist = self.hashtable[hashval]
            # yes, check if in list of states associated with hash bucket
            inhash = False  # assume we didn't find it 
            for item in hashlist:
                if item == state:
                    inhash = True  # got it
                    break # no need to look further
        except KeyError:
            inhash = False # never saw it before
            
        return inhash
    
    def add(self, state):
        "add a state to the explored set.  Assumes not already in set"
        hashval = hash(state)
        try:
            # have we seen this hashcode before?
            hashlist = self.hashtable[hashval]
            # yes, add to a list
            hashlist.append(state)
        except KeyError:
            self.hashtable[hashval] = [state] # new hashcode
