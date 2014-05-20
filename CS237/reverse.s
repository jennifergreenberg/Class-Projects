* Programmer: Jessica To
* Class Account: masc1437
* Assignment or Title: Program #4
* Filename: reverse.s

*void reverse(char &in, char &out, int count)

                    ORG $7000
                   
buffer:   EQU      8
out:      EQU      12
count:    EQU      16
         
reverse:  link      A6,#0
          movem.l   A0-A1/A3/D1,-(SP)
          movea.l   buffer(A6),A0
          movea.l   out(A6),A1
          move.w    count(A6),D1
          tst.w     D1
          BEQ       done
          move.l    A0,A3
          adda.l    #1,A0          *in+1
          sub.w     #1,D1          *--count
          move.w    D1,-(SP)       *recursive call
          pea       (A1)
          pea       (A0)
          jsr       reverse
          adda.l    #10,SP
          ext.l     D1
          adda.l    D1,A1          *out+count
          move.b    (A3),(A1)
         
done:     movem.l  (SP)+,A0-A1/A3/D1
          unlk      A6
          rts
          end
