#!/bin/bash

for i in $(seq 1 10000) ; do
	./twitterTrend_client 127.0.0.1 7575 client2.in &  
done	

