
#!/bin/bash

MAX_TESTS=111
INST=11x11
 
echo "Instance $INST"
echo

for i in `seq 1 $MAX_TESTS`;
do
    SEED="$(head -c 100 /dev/urandom | tr -dc '[:digit:]')"
    if [ -z "$SEED" ];
    then
        SEED=0
    fi

    echo -ne "[Test $i]\t"
    echo -ne "SEED : $SEED\t"
    SOL="$(./Eternity2 --main::instance "../eternity2-data/pieces_set_2/pieces_$INST.txt" --main::seed $SEED < bullo.txt | tail -n 50 | grep -o "COST : [0-9]*")"
    echo $SOL 
done