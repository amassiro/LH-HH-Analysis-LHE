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

    scp amassiro@cmsmi5:/data2/amassiro/Generation/MG5/MG5_aMC_v2_0_1/MY_ttjj/Events/run_02/unweighted_events.lhe /tmp/amassiro/ttbbjj_1M.lhe
    ./ntupleMaker.exe    /tmp/amassiro/ttbbjj_1M.lhe    /tmp/amassiro/ttbbjj.root

without vbf cuts

    cp  /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/wwbbjj_ptb20_lvlv/wwbbjj_ptb20_llvv.lhe  /tmp/amassiro/wwbbjj_ptb20_llvv.lhe
    ./ntupleMaker.exe    /tmp/amassiro/wwbbjj_ptb20_llvv.lhe    /tmp/amassiro/wwbbjj.root

    scp amassiro@cmsmi5:/data2/amassiro/Generation/MG5/MG5_aMC_v2_0_1/MY_ttjjbb_noVBFcuts/Events/run_01/unweighted_events.lhe /tmp/amassiro/ttbbjj_ptb20_novbfcuts.lhe
    ./ntupleMaker.exe    /tmp/amassiro/ttbbjj_ptb20_novbfcuts.lhe    /tmp/amassiro/ttbbjj_novbf.root





Signals HH non-resonant
=======

    wget  http://wwwteor.mi.infn.it/~rojo/Events_hhvbf.tgz
    ls /tmp/amassiro/vbfhh/Events_2b2w2j/13tev/parton/*.lhe  | tr "." " "  | awk '{print "./ntupleMaker.exe  "$1".lhe     "$1".root"}'



Signals HH resonant
=======

    ./ntupleMaker.exe    /tmp/amassiro/MGraviton_1000_H35H25_WWbb.lhe    /tmp/amassiro/MGraviton_1000_H35H25_WWbb.root
    ls /tmp/amassiro/XHH/MGraviton_*_ww_lvlv.lhe  | tr "." " "  | awk '{print "./ntupleMaker.exe  "$1".lhe     "$1".root"}'





