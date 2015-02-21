#! /bin/csh -f

setenv NEUTCRS $NEUT_ROOT/src/crsdat
setenv TAUOLADIR $NEUT_ROOT/src/tauola/
setenv NEUTSMPL $NEUT_ROOT/src/neutsmpl
setenv RANFILE /home/kachulis/rndmGen/random.tbl.3
setenv CRSDIR /home/kachulis/atmflux/base/examples/HK/bin  
ln -s $NEUTCRS/ccqe_xsec_ma1.2.dat $CRSDIR/ccqe_xsec_ma1.2.dat
ln -s $NEUTCRS/spi_nue_xsec_ma1.2.dat $CRSDIR/spi_nue_xsec_ma1.2.dat 
#ln -s $NEUTCRS/seta_nue_xsec_ma1.2.dat seta_nue_xsec_ma1.2.dat
ln -s $NEUTCRS/94org_p.dat  $CRSDIR/94org_p.dat
ln -s $NEUTCRS/94org_pn.dat $CRSDIR/94org_pn.dat
ln -s $NEUTCRS/94mod_p.dat  $CRSDIR/94mod_p.dat
ln -s $NEUTCRS/94mod_pn.dat $CRSDIR/94mod_pn.dat
ln -s $NEUTCRS/98org_p.dat  $CRSDIR/98org_p.dat
ln -s $NEUTCRS/98org_pn.dat $CRSDIR/98org_pn.dat
ln -s $NEUTCRS/98mod_p.dat  $CRSDIR/98mod_p.dat
ln -s $NEUTCRS/98mod_pn.dat $CRSDIR/98mod_pn.dat
ln -s $TAUOLADIR/mrst2002nlo.dat $CRSDIR/mrst2002nlo.dat
    
foreach xsec ( `ls $NEUTCRS/*dat` )
    ln -s $xsec $CRSDIR/$xsec:t
end

foreach xsec ( `ls $NEUTSMPL/*dat` )
   ln -s $xsec $CRSDIR/$xsec:t
end
