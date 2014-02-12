LH-HH-Analysis-LHE
==================

HH analysis just at LHE level

Where:

    cmsneu
    /afs/cern.ch/user/a/amassiro/work/LesHouches2013/LH-HH-Analysis-LHE


To compile:

    c++ -o ntupleMaker.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp


Fill in a tree the most important variables:

    common variables
    hww specific
    hbb specific
    haa specific


Example:

    ./ntupleMaker.exe    /tmp/amassiro/MGraviton_1000_H35H25_WWbb.lhe    /tmp/amassiro/MGraviton_1000_H35H25_WWbb.root





Background in MG5
==================

    generate p p > t t~ j j , (t > W+ b, W+ > l+ vl),  (t~ > w- b~, W- > l- vl~)

    generate p p > w+ w- b b~ j j, (w- > l- vl~), (w+ > l+ vl)     # not working?

    generate p p > w+ w- b b~ j j

    generate p p > w+ w- j j j , (W- > l- vl~), (W+ > l+ vl)

Lxbatch:

    cd /afs/cern.ch/user/a/amassiro/work/Generation/MG5
    cp /afs/cern.ch/user/a/amassiro/work/LesHouches2013/LH-HH-Analysis-LHE/Generation/dowwbbjj.sh ./
    bsub -q "2nw" dowwbbjj.sh

    is 2nw too small???


Background in MG5 without additional jets


    generate p p > t t~ j , (t > W+ b, W+ > l+ vl),  (t~ > w- b~, W- > l- vl~)
    generate p p > w+ w- b b~ j


    generate p p > t t~ , (t > W+ b, W+ > l+ vl),  (t~ > w- b~, W- > l- vl~)
    generate p p > w+ w- b b~




Backgrounds
=======

copy lhe file:

    cp /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/ttbbjj_100k/unweighted_events.lhe /tmp/amassiro/ttbbjj.lhe
    ./ntupleMaker.exe    /tmp/amassiro/ttbbjj.lhe    /tmp/amassiro/ttbbjj.root







