#!/bin/bash

for i in $(seq 1 4) ; do
        echo "Creating client$i";
	./client client$i.in > result$i.temp;  
done	




