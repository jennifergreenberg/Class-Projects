
"""Search (Chapters 3-4)

The way to use this code is to subclass Problem to create a class of problems,
then create problem instances and solve them with calls to the various search
functions.

Modified from text book code"""

#______________________________________________________________________________
class Problem(object):
    """The abstract class for a formal problem.  You should subclass
    this and implement the methods actions and result, and possibly
    __init__, goal_test, and path_cost. Then you will create instances
    of your subclass and solve them with the various search functions."""

    def __init__(self, initial, goals=None, 
                 g = lambda oldnode, action, newnode : oldnode.depth+1, 
                 h = lambda newnode : 0):
        """The constructor specifies the initial state, and one or
        more goal states if they are countable states (override goal_test to
        provide a suitable goal predicate if this is not the case).
        
        Callers should provide functions to estimate g (cost from initial
        node to current node in search tree) as an argument of the 
        of the new edge of the search tree being added:
            oldnode, action that caused transition, newnode
        and h, the heuristic value for the newnode.
        
        By default, breadth-first search behavior is provided.
        
        Your subclass's constructor can add other arguments.
        """
        
        self.initial = initial  # store initial state
        # store goal(s) as a list (make it a list if it is not)
        if goals != None:
            self.goals = goals if isinstance(goals, list) else list(goals)
        else:
            self.goals = []
        
        # store function handles
        self.g = g
        self.h = h 

    def actions(self, state):
        """Return the actions that can be executed in the given
        state. The result would typically be a list, but if there are
        many actions, consider yielding them one at a time in an
        iterator, rather than building them all at once."""
        
        return state.get_actions()

    def result(self, state, action):
        """Return the state that results from executing the given
        action in the given state. The action must be one of
        self.actions(state)."""
        raise NotImplementedError

    def goal_test(self, state):
        """Return True if the state is a goal. The default method checks if
        state is one of the constructor specified goals. Override this
        method if checking against a list of goals is not sufficent."""
        return state in self.goals

    def value(self, state):
        """For optimization problems, each state has a value.  Hill-climbing
        and related algorithms try to maximize this value."""
        raise NotImplementedError
