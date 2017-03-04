
#!/bin/bash

MAX_TESTS=50
INST=05x05

echo "*** Multistart Steepest Descent ***"
echo 
echo "Instance $INST"
echo "Method $METHOD"


for i in `seq 1 $MAX_TESTS`;
do
    SEED="$(head -c 100 /dev/urandom | LC_CTYPE=C tr -dc '[:digit:]')"
    if [ -z "$SEED" ];
    then
        SEED=0
    fi

    echo "Test $i) "
    echo "Seed: $SEED"
    SOL="$(./Eternity2 --main::instance "../eternity2-data/pieces_set_2/pieces_$INST.txt" --main::seed $SEED < ../eternity2-data/ourInput/input_$INST.txt)"
    echo $SOL | grep -o "CURRENT COST : [0-9]*" | tail -1
    echo $SOL | grep -o "ELAPSED TIME : [0-9]*\.[0-9]*s" | tail -1 
    echo
done