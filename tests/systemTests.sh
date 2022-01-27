#!/bin/sh
BASECALL='time curl -s --header "Content-Type:application/json'
URL="http://www.mysite.blaj.se:1984"

echo "POSTing new crop"
PostResponse=`$BASECALL --request POST --data '{"name":"Broccoli","variety":"Gnöuda","batch":2,"id":0}' $URL/crop`
echo $PostResponse
echo "GETing crop with id 0"
GetResponse=`$BASECALL --request GET $URL/crop/0`
echo $GetResponse

$BASECALL --request POST --data '{"name":"Broccoli","variety":"Gnöuda","batch":2,"id":0}' $URL/crop