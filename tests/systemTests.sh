#!/bin/sh
BASECALL='time curl -s --header "Content-Type:application/json'
URL="http://www.mysite.blaj.se:1984"

echo "POSTing new crop"
PostResponse=`$BASECALL --request POST --data '{"name":"Broccoli","variety":"Gnöuda","batch":1,"id":0}' $URL/crop`
echo $PostResponse

echo "POSTing yet a new crop"
PostResponse2=`$BASECALL --request POST --data '{"name":"Morot","variety":"Bolero","batch":1,"id":0}' $URL/crop`
echo $PostResponse2

echo "PUTing crop"
PostResponse=`$BASECALL --request PUT --data '{"name":"Broccoli","variety":"Gnöuda","batch":2,"id":1}' $URL/crop/0`
echo $PostResponse

echo "PUTing crop"
PostResponse=`$BASECALL --request PUT --data '{"name":"Broccoli","variety":"Gnöuda","batch":3,"id":0}' $URL/crop`
echo $PostResponse

echo "GETing crop with id 0"
GetResponse=`$BASECALL --request GET $URL/crop/0`
echo $GetResponse

echo "DELETEing crop with id 0"
DeleteResponse=`$BASECALL --request DELETE $URL/crop/0`
echo $DeleteResponse

echo "GETing all crops"
GetResponse2=`$BASECALL --request GET $URL/crop`
echo $GetResponse2
