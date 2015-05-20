import itertools, re
from csp_lib.csp import CSP, different_values_constraint, if_
import csp_lib.csp
from AC3 import AC3

def flatten(seqs): 
    return sum(seqs, [])

easy1   = '..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..'
harder1 = '4173698.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......'

class Sudoku(CSP):
    """A Sudoku problem.
    The box grid is a 3x3 array of boxes, each a 3x3 array of cells.
    Each cell holds a digit in 1..9. In each box, all digits are
    different; the same for each row and column as a 9x9 grid.
    >>> e = Sudoku(easy1)
    >>> e.display(e.infer_assignment())
    . . 3 | . 2 . | 6 . .
    9 . . | 3 . 5 | . . 1
    . . 1 | 8 . 6 | 4 . .
    ------+-------+------
    . . 8 | 1 . 2 | 9 . .
    7 . . | . . . | . . 8
    . . 6 | 7 . 8 | 2 . .
    ------+-------+------
    . . 2 | 6 . 9 | 5 . .
    8 . . | 2 . 3 | . . 9
    . . 5 | . 1 . | 3 . .
    >>> AC3(e); e.display(e.infer_assignment())
    True
    4 8 3 | 9 2 1 | 6 5 7
    9 6 7 | 3 4 5 | 8 2 1
    2 5 1 | 8 7 6 | 4 9 3
    ------+-------+------
    5 4 8 | 1 3 2 | 9 7 6
    7 2 9 | 5 6 4 | 1 3 8
    1 3 6 | 7 9 8 | 2 4 5
    ------+-------+------
    3 7 2 | 6 8 9 | 5 1 4
    8 1 4 | 2 5 3 | 7 6 9
    6 9 5 | 4 1 7 | 3 8 2
    >>> h = Sudoku(harder1)
    >>> None != backtracking_search(h, select_unassigned_variable=mrv, inference=forward_checking)
    True
    """
    
    # Class variables
    
    # Sudoku units are referenced with indices:
    #     0  1  2  9 10 11 18 19 20
    #     3  4  5 12 13 14 21 22 23
    #     6  7  8 15 16 17 24 25 26
    #    27 28 29 36 37 38 45 46 47
    #    30 31 32 39 40 41 48 49 50
    #    33 34 35 42 43 44 51 52 53
    #    54 55 56 63 64 65 71 73 74 
    #    57 58 59 66 67 68 75 76 77
    #    60 61 62 69 70 71 78 79 80

    R3 = range(3)   # 0, 1, ..., N-1
    # Create an iterator function that increments its value each time
    # it is called
    Cell = itertools.count().next
    # Create a grid of box units:
    # [ [ [0, 1, 2],   [3, 4, 5], [6, 7, 8] ],
    #   [ [9, 10, 11], [12, 13, 14], [15, 16, 17 ] ]
    #   ...
    # ]
    bgrid = [[[[Cell() for x in R3] for y in R3] for bx in R3] for by in R3]
    # Indices of boxes  boxes[0] has 0-8, boxes[1] 9-17, etc.
    boxes = flatten([map(flatten, brow)       for brow in bgrid])
    # Indices of rows; rows[0] :  0  1  2  9 10 11 18 19 20
    rows  = flatten([map(flatten, zip(*brow)) for brow in bgrid])
    # Indices of cols; cols[0]: 0, 3, 6, 27, 30, 33, 54, 57, 60
    cols  = zip(*rows)

    # Build a dictionary showing every cell index that is associated
    # with each square.  
    # Start with the keys and empty sets
    neighbors = dict([(v, set()) for v in flatten(rows)])
    # Find the union of related boxes, rows, and columns...
    # Create a list of sets where each set is initialized to one of the
    # three unit types:  boxes, rows, and columns.
    for unit in map(set, boxes + rows + cols):
        # For each position within a unit, update the neighbors map
        # to reflect the constraint relationship with all other positions 
        # of the unit.  As we are using a set representation, we don't
        # need to worry about things being added twice.
        for v in unit:
            neighbors[v].update(unit - set([v]))

    def __init__(self, grid):
        """Build a Sudoku problem from a string representing the grid.

        the digits 1-9 denote a filled cell, '.' or '0' an empty one;
        other characters are ignored.
        
        grid represents the puzzle in row-major (row by row) order"""
        
        # Get list of positions, filtering out anything we don't use
        squares = iter(re.findall(r'\d|\.', grid))
        # Build dictionary.  Each variable is referred to by a grid
        # position and its value is the list of possible values.
        domains = dict((var, if_(ch in '123456789', [ch], '123456789'))
                       for var, ch in zip(flatten(self.rows), squares))
        # If anything left in squares iterator, we didn't pair up all of
        # the indices with the valid string labels.
        for _ in squares:
            raise ValueError("Not a Sudoku grid", grid) # Too many squares
        
        # Construct a CSP.  Note that we don't pass in the variables explicitly,
        # these are derived from the keys of domains.
        # Constraint is that all neighbors must be different.
        CSP.__init__(self, None, domains, self.neighbors,
                     different_values_constraint)

    def display(self, assignment):
        def show_box(box): return [' '.join(map(show_cell, row)) for row in box]
        def show_cell(cell): return str(assignment.get(cell, '.'))
        def abut(lines1, lines2): return map(' | '.join, zip(lines1, lines2))
        print '\n------+-------+------\n'.join(
            '\n'.join(reduce(abut, map(show_box, brow))) for brow in self.bgrid)
        
        
# for puzzle in [easy1, harder1]:
#     s  = Sudoku(puzzle)  # construct a Sudoku problem
#     print "\nInitial problem:"
#     # Show current bindings; displays "." when a domain has multiple values
#     s.display(s.infer_assignment())
    
#     # Run AC3 constraint propagation
#     AC3(s)
    
#     print "\nAfter AC3 constraint propagation\n"
#     s.display(s.infer_assignment())
    
#     if not s.goal_test(s.curr_domains):
#         print "Running backtracking search..."
#         result = csp_lib.csp.backtracking_search(s, 
#                 select_unassigned_variable=csp_lib.csp.mrv)
#         if result:
#             print "Backtracking solution"
#             s.display(s.infer_assignment())
#         else:
#             print "Backtracking failed"

s  = Sudoku(easy1)
print "\nInitial problem:"
s.display(s.infer_assignment())

AC3(s)
    
print "\nAfter AC3 constraint propagation\n"
s.display(s.infer_assignment())