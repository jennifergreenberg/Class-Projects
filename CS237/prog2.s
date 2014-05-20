*----------------------------------------------------------------------
* Programmer: Jessica To
* Class Account: masc1437
* Assignment or Title: Program #2
* Filename: prog2.s
* Date completed: 10/22/2012
*----------------------------------------------------------------------
* Problem statement: Read a date from the keyboard, then calculate and print the
* day of the week for the given date.
* Input: A date in the form (mm/dd/yyyy).
* Output: "The day of the week is " followed by a day.
* Error conditions tested: none
* Included files: none
* Method and/or pseudocode: Print title and prompt the user for a date. Store
* the month, date, and year in different data registers and go through the
* algorithm to calculate the day of the week. Print the answer and correct day.
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
*
*----------------------------------------------------------------------
*
* Register use
* D0 macro usage
* D1 holds offset calculation
* D2 through D7 holds algorithm data
* A0 holds address of days
*
*----------------------------------------------------------------------
*
start:  initIO                   * Initialize (required for I/O)
     setEVT             * Error handling routines
*    initF              * For floating point macros only   

    lineout title
    lineout skipln
    lineout prompt
    linein buffer
    cvta2 buffer,#2
    move.w D0,D1             *D1 holds month
    cvta2 buffer+3,#2
    move.w D0,D2             *D2 holds day
    cvta2 buffer+6,#4
    move.l D0,D3             *D3 holds year
    move.w #14,D4
    sub.w D1,D4
    ext.l D4
    divs #12,D4              *a=(14-month)/12 -> D4
    sub.w D4,D3              *y=year-a -> D3
    muls #12,D4
    add.w D1,D4
    sub.w #2,D4              *m=month+12a-2 -> D4
    move.l D3,D5
    ext.l D5
    divs #4,D5               *y/4 -> D5
    move.l D3,D6
    ext.l D6
    divs #100,D6             *y/100 -> D6
    move.l D3,D7
    ext.l D7
    divs #400,D7             *y/400 -> D7
    move.w #31,D1
    muls D4,D1
    divs #12,D1              *31m/12 -> D1
    add.l D2,D3
    add.l D3,D5
    sub.l D6,D5
    add.l D5,D7
    add.l D7,D1
    ext.l D1
    divs #7,D1               *d=(day+y+y/4-y/100+y/400+31m/12)mod7 -> D1
    swap D1
    muls #12,D1              *offset calculation
    lea days,A0
    adda.l D1,A0
    move.l (A0)+,day
    move.l (A0)+,day+4
    move.l (A0)+,day+8
    lineout skipln
    lineout answer

        break                * Terminate execution
*
*----------------------------------------------------------------------
*      Storage declarations

skipln:   dc.b 0
title:    dc.b 'CS237 Program #2, Jessica To, masc1437',0
prompt:   dc.b 'Please enter a date (mm/dd/yy):',0
buffer:   ds.b 81
answer:   dc.b 'The day of the week is '
day:      ds.b 12
days:     dc.b 'Sunday.    ',0
          dc.b 'Monday.    ',0
          dc.b 'Tuesday.   ',0
          dc.b 'Wednesday. ',0
          dc.b 'Thursday.  ',0
          dc.b 'Friday.    ',0
          dc.b 'Saturday.  ',0
        end
