#!/bin/bash

AUTODOCK_DIR=$HOME/code/nbvl/autodock-v1.5.1

SMARTREDIS_PATH=$HOME/code/nbvl/smartsim/smartredis/install
export LD_LIBRARY_PATH=$SMARTREDIS_PATH/lib:$LD_LIBRARY_PATH


$AUTODOCK_DIR/bin/autodock_gpu_256wi -ffile $AUTODOCK_DIR/input/1stp/derived/1stp_protein.maps.fld \
	                             -lfile $AUTODOCK_DIR/input/1stp/derived/1stp_ligand.pdbqt \
				     -nrun 100 -resnam gpu_1stp_nrun100
