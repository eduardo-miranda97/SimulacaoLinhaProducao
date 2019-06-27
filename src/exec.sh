for i in {0..99}; do
    for j in {1..100}; do
        ./simulador "confs/config$i.cfg" "res/resultados$i.rs" $RANDOM;
    done;
done;