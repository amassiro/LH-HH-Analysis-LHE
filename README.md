LH-HH-Analysis-LHE
==================

HH analysis just at LHE level

Where:

    /afs/cern.ch/user/a/amassiro/work/LesHouches2013


To compile:

    c++ -o ntupleMaker.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp


Fill in a tree the most important variables:

    common variables
    hww specific
    hbb specific
    haa specific


Background in MG5

    generate p p > t t~ j j , (t > W+ b, W+ > l+ vl),  (t~ > w- b~, W- > l- vl~)

    generate p p > w+ w- b b~ j j, (w- > l- vl~), (w+ > l+ vl)     # not working?

    generate p p > w+ w- b b~ j j

    generate p p > w+ w- j j j , (W- > l- vl~), (W+ > l+ vl)

Lxbatch:

    cd /afs/cern.ch/user/a/amassiro/work/Generation/MG5
    cp /afs/cern.ch/user/a/amassiro/work/LesHouches2013/LH-HH-Analysis-LHE/Generation/dowwbbjj.sh ./
    bsub -q "2nw" dowwbbjj.sh
    





