#!/bin/bash

export AUTODOCK_DIR=$HOME/code/nvbl/autodock-v1.5.1

SMARTREDIS_PATH=$HOME/code/nvbl/smartsim/smartredis/install
export LD_LIBRARY_PATH=$SMARTREDIS_PATH/lib:$LD_LIBRARY_PATH

$AUTODOCK_DIR/bin/autodock_gpu_256wi \
    -ffile $AUTODOCK_DIR/input/1stp/derived/1stp_protein.maps.fld \
    -lfile $AUTODOCK_DIR/input/1stp/derived/1stp_ligand.pdbqt \
    -nrun 1000 -resnam gpu_1stp_nrun1000_2

