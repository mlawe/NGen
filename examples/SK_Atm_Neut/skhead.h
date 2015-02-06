************************************************************************
*     ------------------
*     INCLUDE 'skhead.h'
*     ------------------
*
*       NRUNSK ; run #
*       NSUBSK ; subrun #
*       NEVSK  ; ev # 
*
*       NDAYSK(1) ; year
*             (2) ; month 
*             (3) ; day
*       NTIMSK(1) ; hour
*             (2) ; minute
*             (3) ; second
*             (4) ; 1/100 sec
*
*       NT48SK(1)-(3) ; 48bit clock
*
*       MDRNSK ; run mode
*                0  Monte Carlo
*                1  Normal run
*                2  Laser calib.
*                3  Pedestal data
*                4  Xe lamp calib.
*                5  Nickel calib.
*                6  random trigger run
*                7  Linac calib.
*       IDTGSK ; trigger ID
*         (SK-IV)
*                Value      Cable #    Trigger type
*            --------------------------------------------------
*          0  0x00000001     -----     LE  (by software trig.)
*          1  0x00000002     -----     HE  (by software trig.)
*          2  0x00000004     -----     SLE (by software trig.)
*          3  0x00000008     -----     OD  (by software trig.)
*          4  0x00000010     15001     Periodic  (same as SK-I,II,III)
*          5  0x00000020     15002     After/CAL (same as SK-I,II,III)
*          6  0x00000040     15003     (VETO START)
*          7  0x00000080     15004     (VETO STOP)
*
*          8  0x00000100     15005
*          9  0x00000200     15006
*         10  0x00000400     15007
*         11  0x00000800     15008
*         12  0x00001000     15009     Laser (ID)
*         13  0x00002000     15010     LED
*         14  0x00004000     15011     Ni
*         15  0x00008000     15012     Laser (OD)
*         16  0x00010000     15013     LE  (by hitsum signal)
*         17  0x00020000     15014     HE  (by hitsum signal)
*         18  0x00040000     15015     SLE (by hitsum signal)
*         19  0x00080000     15016     OD  (by hitsum signal)
*         20  0x00100000     15017
*         21  0x00200000     15018
*         22  0x00400000     15019     SN Burst
*         23  0x00800000     15020     mu->e decay
*         24  0x01000000     15021     LINAC
*         25  0x02000000     15022     LINAC microwave
*         26  0x04000000     15023
*         27  0x08000000     15024     Periodic (simple periodic trigger)
*         28  0x10000000     -----     SHE trigger    ( by software trig. )
*         29  0x20000000     -----     After trigger  ( by software trig. )
*         30  0x40000000     -----     pedestal event ( by software trig. )  
*         31  0x80000000     -----     T2K event      ( by software trig. )  
*         (SK-I,II,III)
*                Value         Trigger type
*            --------------------------------------------------
*                0x01         Low Energy trigger       
*                0x02         High Energy trigger
*                0x04         Super Low Energy trigger (In Normal Run)
*                0x08         Outer Detector trigger
*                             Fission trigger (In Nickel Run)
*                0x10         Periodical trigger firing one of 
*                               a. nothing (null trigger)
*                               b. TQ map laser
*                               c. water attenuation meas. laser
*                               d. Xe ball
*                0x20         After trigger (In Normal Run)
*                             Calibration trigger (In Calibration Run)
*                               Laser trigger
*                               Xe trigger
*                               Ni trigger
*                               Linac trigger
*                0x40         veto start
*                0x80         vato end
*       IFEVSK ; event status flag
*                value       SK-I,II,III              SK-IV
*              ---------------------------------------------------------
*           0  0x00000001  ATM                      QBEE TQ
*           1  0x00000002  TRG                      HARD TRG
*           2  0x00000004  SMP REGISTER             QBEE STAT
*           3  0x00000008  SCALER                   DB_STAT_BLOCK
*           4  0x00000010  PEDESTAL START           CORRUPTED_CHECKSUM
*           5  0x00000020  PEDESTAL DATA(ATM)       MISSING SPACER
*           6  0x00000040  PEDESTAL HISTOGRAM       PED_HIST_BLOCK
*           7  0x00000080  PEDESTAL END             
*       
*           8  0x00000100  END OF RUN               
*           9  0x00000200  PEDESTAL(ON)             PEDESTAL ON
*          10  0x00000400                           RAW_AMT_BLOCK
*          11  0x00000800  GPS DATA                 GPS DATA
*       
*          12  0x00001000  CAMAC ADC                PEDESTAL_CHECK
*          13  0x00002000  ANTI DATA                SEND_BLOCK
*          14  0x00004000  INNER SLOW DATA          INNER SLOW DATA
*          15  0x00008000  RUN INFORMATION          RUN INFORMATION
*  
*          16  0x00010000  ERROR (TKO-PS)           PREV T0 BLOCK
*          17  0x00020000  ERROR (HV-PS)       
*          18  0x00040000  ERROR (TEMPERARTURE)     FE_TRL_BLOCK
*          19  0x00080000                           SPACER_BLOCK
*  
*          20  0x00100000  UNCOMPLETED ATM DATA     INCOMPLETE TQ
*          21  0x00200000  INVALID     ATM DATA     CORRUPT TQ BLOCK
*          22  0x00400000                           TRG MISMATCH TQ
*          23  0x00800000                           QBEE ERROR
*  
*          24  0x01000000  ERROR (DATA)             SORT_BLOCK
*          25  0x02000000  UNREASONABLE DATA        CORRUPTED_BLOCK
*          26  0x04000000  LED BURST ON             LED BURST ON
*          27  0x08000000                           EVNT TRAILER
*  
*          28  0x10000000  INNER DETECTOR OFF       INNER DETECTOR OFF
*          29  0x20000000  ANTI  DETECTOR OFF       ANTI  DETECTOR OFF
*          30  0x40000000                           T2K GPS
*          31  0x80000000  TRG IS AVAILABLE         (EVNT HDR)&(SOFTWARE TRG)
*
*      ---------
*      Contents of /SKHEADA/:
*      --------- 
*      LTCGPS ; Local time clock at last GPS time
*      NSGPS  ; GPS time (sec)
*      NUSGPS ; GPS time (usec)
*      LTCTRG ; Local time clock at TRG
*      LTCBIP ; Local time clock at end of BIP
*      ITDCT0(I); TDC T0 (TRG) time for hut I (I = 1,4)
*      IFFSCC ; FSCC busy flags
*      ICALVA ; Calibration constant version.
*     
*      ---------
*      Contents of /SKHEADG/:
*      --------- 
*      SK_GEOMETRY ; Control geometry definition used
*      (INTEGER*4)         = SK_I  (=1) SUPER-KAMIOKANDE I
*                          = SK_II (=2) SUPER-KAMIOKANDE II
*                          = SK_III(=3) SUPER-KAMIOKANDE III 
*                          = 4          SUPER-KAMIOKANDE IV
*
*      ---------
*      Contents of /SKHEADF/:
*      --------- 
*      SK_FILE_FORMAT ; data file format for sk*read(), etc. 
*                          = 0   ZBS
*                          = 1   SKROOT
*      ROOT_ID ; the current LUN for the SKROOT files used in sk*read()
*
*      ---------
*      Contents of /SKHEADQB/:
*      --------- 
*      ONLIDSK ; online format block ID
*      NEVSWSK ; software trigger id
*      NEVHWSK ; TRG EVENT COUNTER (where T0 have)
*      HIDHSK  ; HOST ID (H)
*      HIDLSK  ; HOST ID (L)
*      IT0SK   ; Original T0 of the event 
*      IT0XSK  ; T0 of the event for ITISKZ,ITASKZ,ITABSK,TISK,IHCAB,TASK,IHACAB,...
*      GATEWSK ; Gate Width
*      TRGMASK ; software trg mask
*      CONTSK  ; data contents
*      NUMHWSK ; Number of HW triggers
*      HWSK(I) ; TRG EVENT COUNTER list (I=1,NUMHWSK)
*      NTRIGSK ; sub-trigger # (=(it0xsk-it0sk)/count_per_nsec/10)
*
*     (Creation Date and Author)
*       1992.08.26 ; First version by K.S.Hirata
*         95.11.19 ;      modified by Y.Koshio
*         96.01.16 ;      modified by Y.Hayato(add comments:IFEVSK)
*         96.03.30 ;      modified by J. Flanagan (add /SKHEADA/)
*         96.04.12 ;      modified by Y.Hayato(add new comments:IFEVSK)
*         96.05.02 ;      modified by Y.Hayato(add new comments:IFEVSK)
*         03.01.17 ; Added SK geometry version
*       2007.07.03 ;      modified by Y.Koshio/Y.Takeuchi
*           o for QB
************************************************************************

      INTEGER NRUNSK, NSUBSK, NEVSK, NDAYSK, NTIMSK, NT48SK, MDRNSK,
     &   IDTGSK, IFEVSK
      INTEGER    LTCGPS, NSGPS, NUSGPS, LTCTRG, LTCBIP, IFFSCC,
     &           ITDCT0, ICALVA
      COMMON /SKHEAD/ NRUNSK, NSUBSK, NEVSK,
     &                NDAYSK(3), NTIMSK(4), NT48SK(3),
     &                MDRNSK, IDTGSK, IFEVSK

      COMMON /SKHEADA/ LTCGPS, NSGPS, NUSGPS, 
     &                 LTCTRG, LTCBIP, 
     &                 ITDCT0(4), IFFSCC, ICALVA

      INTEGER*4 SK_GEOMETRY
      COMMON /SKHEADG/ SK_GEOMETRY

      INTEGER*4 SK_FILE_FORMAT, ROOT_ID
      COMMON /SKHEADF/ SK_FILE_FORMAT, ROOT_ID

c*** number of max. hardware trig. in an event
c*** for 1000 micro sec. width (16.6micro sec * 64 = 1062 micro sec.) (y.t.)
c*** there is the same definition in TreeManager.h at moment
      INTEGER*4 MAXHWSK
      PARAMETER(MAXHWSK=64)  

c*** for QB
      INTEGER*4 NEVHWSK, IT0SK, GATEWSK, HIDHSK, HIDLSK, NUMHWSK, ONLIDSK,
     $          NEVSWSK, TRGMASK, CONTSK,IT0XSK,HWSK(MAXHWSK), NTRIGSK 
      COMMON /SKHEADQB/ NEVHWSK, IT0SK, GATEWSK, HIDHSK, HIDLSK, NUMHWSK, 
     $                  ONLIDSK, NEVSWSK, TRGMASK, CONTSK, IT0XSK, HWSK,NTRIGSK

c*** 60e6*32(count/sec) = 1.92(count/nsec) for T0
      real count_per_nsec
      parameter (count_per_nsec = 1.92000)
