
#!/bin/bash

MAX_TESTS=5
INST=04x04
METHOD=TTS_SD
MAX_EVAL=1000000000000

echo "*** Multistart Steepest Descent ***"
echo 
echo "Instance $INST"
echo "Method $METHOD"


for i in `seq 1 $MAX_TESTS`;
do
    SEED="$(head -c 100 /dev/urandom | tr -dc '[:digit:]')"
    if [ -z "$SEED" ];
    then
        SEED=0
    fi

    echo "Test $i) "
    echo "Seed: $SEED"
    SOL="$(./Eternity2 --main::instance "../eternity2-data/pieces_set_2/pieces_$INST.txt" --main::seed $SEED --main::method $METHOD --$METHOD::max_evaluations $MAX_EVAL)"
    echo $SOL
    echo
done