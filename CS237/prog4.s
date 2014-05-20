*----------------------------------------------------------------------
* Programmer: Jessica To
* Class Account: masc1437
* Assignment or Title: Program #4
* Filename: prog4.s
* Date completed: 12/7/12
*----------------------------------------------------------------------
* Problem statement:Write a recursive subroutine to reverse a string and print
* the results.
* Input: The user will be prompted to enter any string.
* Output: The program will print the reverse of the string the user inputed.
* Error conditions tested: Address errors.
* Included files: reverse.s
* Method and/or pseudocode: Prompt the user to input a string. Push the
* parameters onto the stack and call the recursive subroutine reverse. Pop the
* garbage off the stack and print the reversed string.
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
* D0: macro usage
* A1: store out
* A2: store count
*
*----------------------------------------------------------------------
*
reverse:  EQU  $7000

start:  initIO                  * Initialize (required for I/O)
    setEVT            * Error handling routines
*    initF            * For floating point macros only   

    lineout    title
    lineout    prompt
    linein     buffer
    move.l     D0,A2         * save count value A2
    move.w     D0,-(SP)      * push parameters right to left
    pea        out
    pea        buffer
    jsr        reverse
    adda.l     #10,SP
    lea        out,A1
    adda.l     A2,A1         * null terminate string
    clr.b      (A1)
    lineout    skpln
    lineout    msg
    lineout    out

        break                  * Terminate execution
*
*----------------------------------------------------------------------
*      Storage declarations
title:    dc.b 'Program #4, Jessica To, masc1437',0
prompt:   dc.b 'Enter a string:',0
buffer:   ds.b 82
msg:      dc.b 'The reversed string is:',0
out:      ds.b 82
skpln:    dc.b 0

        end
