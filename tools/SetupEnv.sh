#!/bin/bash

##############################################
# Setup the environment for this program     #
# Bowen Zhang                                #
#                                            #
# Usage: source SetupEnv.sh (from anywhere)  #
##############################################


HEREPATH=$( dirname $( readlink -f $BASH_SOURCE ) )
export SOURCEPATH=${HEREPATH}/../
mkdir -p ${SOURCEPATH}/../plots/ ${SOURCEPATH}/../build
export RES_PATH=${SOURCEPATH}/res/
export TOOLS_PATH=${SOURCEPATH}/tools/
export PATH=$PATH:${TOOLS_PATH}
