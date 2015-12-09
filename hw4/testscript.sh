#!/bin/bash

for i in $(seq 1 4) ; do
        echo "Creating client$i";
	./twitterTrend_client client$i.in > result$i.temp;  
done	

