************************************************************************
*     -----------------
      SUBROUTINE NECARDATM(FCARD)
*     -----------------
*
*     (Purpose)
*       Read NEUT card and setup common blocks.
*
*     (Input)
*       card file
*
*     (Output)
*
*     (Creation Date and Author)
*       1995.1.30 ; Koshio ( SGCARD )
*       1995.11.17 ; K.Kaneyuki
*       1999.10.11 ; D.Casper - allow user to specify #years in card file
*       2003.03.30 ; add neutparams.h(PFSURF,PFMAX,VNUINI,VNUFIN)
*       2007.04.24 ; G.Mitsuka necard->necardap(for ATMPD)
*                    add NECOHEPI
*       2007.12.05 ; G.Mitsuka added NEWATERVOL (default is 0)
************************************************************************

C#include "dsbnkf.h"
#include "necardatm.h"
#include "nesolact.h"      

      CHARACTER*40 TMPFIL, COMMAND
      CHARACTER*(*) FCARD
      INTEGER*4 GEOMVER
C --  Initialize

      SOLACT = 0.
      NEYEARS = 1.0
      IMODE = 0.
      ITAU=0.
      IDETECTOR=0
      POT=0.
      FLUXTYPE=0
      
      WRITE(6,*) '======= CONFIGURATION FOR ATMPD ======='
      WRITE(6,*) 'NEUT card file you read is ',FCARD

C -- extract NEUT card

      CALL GETUNI(TMPFIL)


      OPEN(UNIT=90, FILE=FCARD, STATUS='OLD')
      OPEN(UNIT=91, FILE=TMPFIL,STATUS='UNKNOWN')
      CALL EXCARD("ATM", 90, 91, IRET)
      CLOSE(UNIT=91)
      CLOSE(UNIT=90)

      OPEN(UNIT=91, FILE=TMPFIL,STATUS='OLD')
      CALL FFINIT(0)
      CALL FFSET('LINP', 91)
      CALL FFSET('SIZE', 25)

     
      CALL FFKEY('SOL' ,SOLACT  , 1,'REAL')
      CALL FFKEY('YEAR',NEYEARS,1,'REAL')
      CALL FFKEY('MODE',IMODE,1,'INTEGER')
      CALL FFKEY('IDETECTOR',IDETECTOR,1,INTEGER);
      CALL FFKEY('FLUXTYPE',FLUXTYPE,1,INTEGER);
      CALL FFKEY('POT',POT,1,REAL);
      CALL FFKEY('TAU',ITAU,1,'INTEGER');
      CALL FFGO
      CLOSE(UNIT=91)
      COMMAND = '/bin/rm '//TMPFIL
      CALL SYSTEM(COMMAND)

      if(solact<0) then
         solact=0.5
      endif
      WRITE(6,*) 'NECARDATM : SOL  = ',SOLACT
      if(FLUXTYPE.lt.2) then
         WRITE(6,*) 'NECARDATM : Generating ',NEYEARS,' years of data'
      else
         WRITE(6,*) 'NECARDATM : Generating ',POT,' x 10^20 POT of data'
      endif
      WRITE(6,*) 'NECARDATM : RANDOM= ',NEYEARS,' years of data'
      WRITE(6,*) 'NECARDATM : MODE = ',IMODE
      WRITE(6,*) 'NECARDATM : DETECTOR = ',IDETECTOR
      WRITE(6,*) 'NECARDATM : FLUXTYPE = ',FLUXTYPE

      RETURN
      END


