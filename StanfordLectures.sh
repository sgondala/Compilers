#!/bin/bash

for i in {0..19}
do
if [ $i -lt 10 ]
then 
wget http://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/0$i/Slides0$i.pdf
else 
wget http://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/$i/Slides$i.pdf
fi
done
