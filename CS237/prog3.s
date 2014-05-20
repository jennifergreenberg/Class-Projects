*----------------------------------------------------------------------
* Programmer: Jessica To
* Class Account: masc 1437
* Assignment or Title: Program #3
* Filename: prog3.s
* Date completed: 11/16/2012
*----------------------------------------------------------------------
* Problem statement: Write an assembly language program to determine the optimum
* dimensions of a closed cylindrical can.
* Input: The cost of the end material per square cm, the cost of the side
* material per square cm, and the volume of the can in milliliters.
* Output: The calculus and brute force answer giving the optimized height,
* diamter, and cost of the can.
* Error conditions tested: Syntax errors.
* Included files: none
* Method and/or pseudocode: Set the radius equal to .01 and calculate the cost
* of the can. Create a do loop that will increment the radius by .01 and
* calculate the new cost of the can. The stopping condition should be when the
* new cost is greater than the old cost. At this point, there should be a branch
* to print the old cost which would be the optimum price of the can. Print the
* optimum dimensions of the can as well.
* References: none
*----------------------------------------------------------------------
*
        ORG    $0
        DC.L   $3000          * Stack pointer value after a reset
        DC.L   start          * Program counter value after a reset
        ORG    $3000          * Start at location 3000 Hex
*
*----------------------------------------------------------------------
*
#minclude /home/ma/cs237/bsvc/iomacs.s
#minclude /home/ma/cs237/bsvc/evtmacs.s

********** CONSTANTS **********
TWO:          EQU    $40000000       
PI:           EQU    $40490FDA
ONE_THIRD:    EQU    $3EAAAAAb
*******************************

start:  initIO                  * Initialize (required for I/O)
        initF
        setEVT
        lineout title
        lineout skipln
        lineout  p1
        floatin  buffer
        cvtaf    buffer,D5  * END cost
        lineout  p2
        floatin  buffer
        cvtaf    buffer,D6  * SIDE cost
        lineout  p3
        floatin  buffer
        cvtaf    buffer,D7  * VOLUME
        lineout skipln
        lineout calc
        lineout skipln

**********************************************************************
** Calculus Answer
** Formula for the radius of the optimum can:
** radius = (((volume*side_cost)/(2*PI*end_cost))^(1/3)     

** numerator, volume*side_cost:
        move.l      D7,D1      * VOLUME
        fmul        D6,D1      * VOLUME*SIDE_COST
       
** denominator, 2*PI*end_cost       
        move.l      D5,D2      * END_COST
        fmul        #TWO,D2    * END_COST * 2.0
        fmul        #PI,D2     * END_COST * 2.0 * PI

** now take result to 1/3 power
        fdiv        D2,D1        * numerator/denominator
        move.l      #ONE_THIRD,D0             
        fpow        D1,D0 *(numerator/denominator) ^ (1/3)
       
** Calulus answer done, now calculate diameter, height, cost
        move.l      D0,D1      * D1 has radius
        fmul        #TWO,D0    * D0 has diameter       
        cvtfa      diameter,#2
       
** calculate height = (volume / PI*r^2)
        move.l      D1,D2      * radius
        fmul        D2,D2      * radius^2
        fmul        #PI,D2     * radius^2*PI
        move.l      D7,D3      * copy of volume
        fdiv        D2,D3      * vol / PI*radius^2  HEIGHT --> D3
        move.l      D3,D0     
        cvtfa      height,#2
       
** calculate cost = SIDE_COST*SIDE_SURFACE + 2*END_COST*END_SURFACE
        *** side cost:
        move.l      #PI,D2
        fmul        #TWO,D2    * 2*PI
        fmul        D1,D2      * 2*PI*radius
        fmul        D3,D2      * 2*PI*radius*height  = side surface area
        fmul        D6,D2      * side surface area * SIDE_COST
       
        *** end cost:
        move.l      #PI,D0
        fmul        #TWO,D0    * 2*PI
        fmul        D1,D0      * 2*PI*radius
        fmul        D1,D0      * 2*PI*radius*radius
        fmul        D5,D0      * 2*PI*radius*radius*END_COST
        fadd        D2,D0
        cvtfa      cost,#2
       
** DONE, print the  calculus answer
       
        lineout    ans1
        lineout    ans2
        lineout    ans3
        lineout    skipln
        lineout    brute
        lineout    skipln
**********************************************************************
*** ADD YOUR CODE HERE FOR THE BRUTE FORCE ANSWER
*** Register usage:
*** D5 ->  END_COST
*** D6 ->  SIDE_COST
*** D7 ->  VOLUME
        move.l      #$3C23D70A,D1  * radius set .01
        ** height(volume / PI*r^2)
        move.l      D1,D2
        fmul        D2,D2
        fmul        #PI,D2
        move.l      D7,D3
        fdiv        D2,D3          * height --> D3
        move.l      D3,D0
        cvtfa       height,#2
        ** side cost: 2*PI*r*h*sidecost
        move.l      D1,D2
        fmul        #PI,D2
        fmul        D3,D2
        fmul        #TWO,D2
        fmul        D6,D2          * sidecost --> D2
        ** end cost: 2*PI*r^2*endcost
        move.l      D1,D4
        fmul        D4,D4
        fmul        #PI,D4
        fmul        #TWO,D4
        fmul        D5,D4          * endcost --> D4
        ** calculate cost
        fadd        D2,D4          * oldcost --> D4
do:     fadd        #$3C23D70A,D1
        ** height(volume / PI*r^2)
        move.l      D1,D2
        fmul        D2,D2
        fmul        #PI,D2
        move.l      D7,D3
        fdiv        D2,D3          * height --> D3
        move.l      D3,D0
         height,#2
        ** side cost: 2*PI*r*h*sidecost
        move.l      D1,D2
        fmul        #PI,D2
        fmul        D3,D2
        fmul        #TWO,D2
        fmul        D6,D2          * sidecost --> D2
        ** end cost: 2*PI*r^2*endcost
        move.l      D1,D3
        fmul        D3,D3
        fmul        #PI,D3
        fmul        #TWO,D3
        fmul        D5,D3          * endcost --> D3
        ** calculate cost
        fadd        D2,D3          * newcost --> D3
        ** compare new cost to old cost
        fcmp        D4,D3
        BGT         print
        move.l      D3,D4      * set old cost=new cost
        BRA         do
print:  move.l      D4,D0
        cvtfa       cost,#2
        ** diameter: 2*r
        move.l      D1,D0   
        fmul        #TWO,D0           
        cvtfa       diameter,#2
        lineout ans1
        lineout ans2
        lineout ans3
       
        break
       
buffer:  ds.b    80     
p1:      dc.b    'Please enter the cost of the end material in dollars/cm^2:',0
p2:      dc.b    'Please enter the cost of the side material in dollars/cm^2:',0
p3:      dc.b    'Please enter the volume of the can in milliliters:',0
ans1:    dc.b    'Can cost: '
cost:    ds.b    40
ans2:    dc.b    'Diameter: '
diameter: ds.b    40
ans3:    dc.b    'Height: '
height:  ds.b    40
title:    dc.b    'Program #3, masc1437, Jessica To',0
skipln:  dc.b    0
calc:    dc.b    'Calculus Answer:',0
brute:    dc.b    'Brute Force Answer:',0
          end
