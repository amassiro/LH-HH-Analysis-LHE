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