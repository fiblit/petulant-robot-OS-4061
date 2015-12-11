#!/bin/bash

for i in $(seq 1 50) ; do
	./twitterTrend_client 127.0.0.1 7575 client5.in &  
done	

