#!/bin/bash

for i in $(seq 1 4) ; do
	./twitterTrend_client 127.0.0.1 7575 client$i.in > result$i.temp;  
done	

