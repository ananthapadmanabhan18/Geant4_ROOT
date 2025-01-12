#!/bin/bash

# Defining energy list
energy_list=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 2 5 10 20 40 50 60 70 80 100 200 300 400 500 600 700 800 900 1000)

for i in {20..28}
do
    hadd "${energy_list[$i]}"GeV".root" output_"${i}"_t{0..19}.root
done
