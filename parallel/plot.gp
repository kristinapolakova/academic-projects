set terminal pdfcairo enhanced font 'Arial,12'
set output 'matric.pdf'

set xlabel 'Количество процессов'
set ylabel 'Ускорение'

set grid
set key left top
set xrange [7:35]
set yrange [1:20]

plot x with lines lw 1 dt 2 lc rgb 'black' title '          линейное   ускорение', \
     'res.dat' using 1:2 with linespoints lw 2 pt 7 ps 1 lc rgb 'blue' title 'n=28032', \
     '' using 1:3 with linespoints lw 2 pt 5 ps 1 lc rgb 'red' title 'n=70048'

set output