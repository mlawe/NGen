C---------------------------------------------------------
C
C     INCLUDE FILE FOR NEUTRINO INTERACTION ( nesolact.h )
C
C     SOLAR ACTIVITY
C  
C---------------------------------------------------------
C
C     COMMON /NESOLACT/
C
C     ISOL        : FLAG OF SOLAR ACTIVETY
C                   0    : SOLACT = 0.5
C                   ELSE : USE SOLACT VALUE
C     SOLACT      : SOLAR ACTIVETY 0~1
C                   0    : SOLAR MIN
C                   1    : SOLAR MAX
C
      INTEGER ISOL
      REAL    SOLACT
      COMMON /NESOLACT/ ISOL, SOLACT


