set terminal png 
set output 'bar.png'

set xtics rotate
set style fill solid border rgb "black"
plot "2_1000000000.dat" using 1:2 with lines title col,\
"2_1000000000.dat" using 1:3 with lines title col,\
"2_1000000000.dat" using 1:4 with lines title col,\
"2_1000000000.dat" using 1:5 with lines title col
