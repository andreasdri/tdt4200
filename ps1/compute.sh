echo "Bash version ${BASH_VERSION}..."

for j in 1 2 4 8
do
   echo "Processes: $j"
   for i in {7..100..5}
   do
       echo "End: $i"
       mpirun -np $j ./parallel 2 $i
   done
   mpirun -np $j ./parallel 2 100
done

for j in 1 2 4 8
do
    echo "Processes: $j"
    for i in {50002..1000000..50000}
    do
        echo "End: $i"
        mpirun -np $j ./parallel 2 $i
    done
    mpirun -np $j ./parallel 2 1000000
done

for j in 1 2 4 8
do
    echo "Processes: $j"
    for i in {50000002..1000000000..50000000}
    do
        echo "End: $i"
        mpirun -np $j ./parallel 2 $i
    done
    mpirun -np $j ./parallel 2 1000000000
done

