#!/usr/bin/gnuplot -persist

set terminal postscript eps enhanced color font 'Comic-Sans,14'
set output "Abs_high.eps"
set xlabel "Time, s" font 'Comic-Sans,14'
set ylabel "Temperature, C" font 'Comic-Sans,14'
set xtics border in scale 1,0.5 nomirror norotate autojustify
set ytics border in scale 1,0.5 nomirror norotate autojustify
set key left reverse

plot "kekich.txt" u 0:1 w l title "hello"
