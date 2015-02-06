      subroutine fortZBSOpen(lun,fname)

      integer lun,ilun
      character *(*) fname
      character *(1) amode
      common/quest/iquest(100)
      common/luncom/nptcom(99),icncom(99),iftype(99),itpcom(99),iszcom(99)
      write (*,*) lun
      medium=0
      nrecl=5670
      amode='w'
      ilun=lun
      do i=1,99
         nptcom(i)=0
         icncom(i)=0
         itpcom(i)=0
         iszcom(i)=0
      enddo
      nptcom(lun)=1
      icncom(lun)=1
      write (*,*) 'ilun is ',ilun
      call cfopen(ilun,medium,nrecl,amode,0,fname,istat)
      iquest(1)=ilun
      write (*,*) 'ilun is now',ilun
      write (*,*) 'istat is ',istat
      write (*,*) 'lun now is ', lun
      write (*,*) 'fname is ',fname
      call fzfile(lun,nrecl,'LXO')
      iftype(lun)=11010
C      iftype(lun)=11100
      end

      subroutine testWrit(lun)
      integer lun
      write (lun,*) 'TEST TEST TWO TIMES'
      end
