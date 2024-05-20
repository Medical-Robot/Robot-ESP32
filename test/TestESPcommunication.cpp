//#include <StandardCplusplus.h>
#include <Arduino.h>
#include <Wire.h>
#include "CommandParser.h"
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

#define SLAVE_ADDRESS 9

SoftwareSerial serial_soft1(4, 5); // RX TX 16, 17 NU MERG

MessageParser parser;
Map checkpointMap;
Response_CHECKPOINT_MAP resp;
Query_CHECKPOINT_MAP query;

void setMap(){
  Checkpoint checkPoint;

	checkPoint.id = 1;
	checkPoint.front_id = 2;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 2;
	checkPoint.front_id = 3;
	checkPoint.back_id = 1;
	checkPoint.left_id = 7;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 3;
	checkPoint.front_id = 8;
	checkPoint.back_id = 2;
	checkPoint.left_id = 0;
	checkPoint.right_id = 5;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 5;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 3;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 7;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 2;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 8;
	checkPoint.front_id = 0;
	checkPoint.back_id = 3;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkpointMap.setPreviousCheckPoint(1);
	checkpointMap.setNextCheckPoint(2);
	checkpointMap.getCheckPoints().reserve(checkpointMap.getCheckPoints().size());

  //checkpointPath = checkpointMap.findPath(8);
}

void setup()
{
  //Wire.begin();
  //Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  //Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX TX
  serial_soft1.begin(1200);
//while (!Serial2){delay(100);}

	setMap();
  
  //Serial.println("Arduino Started");
  query.message_type = QueryResponseType::QUERY_CHECKPOINT_MAP;
  resp.message_type = QueryResponseType::RESPONSE_CHECKPOINT_MAP;
  resp.checkpoints = checkpointMap.getCheckPoints();
  parser.sendMessage(resp);
  delay(100);
}

void loop(){
	//Serial.println("HELLO WORLD");
	//serial_soft1.println("SERIAL SOFTWARE");
  if (parser.isMessageCompleted() && parser.isSending()) {
    //Serial.println("Message sent!");
    parser.sendMessage(resp);
  }
  else if (parser.isMessageCompleted() && parser.isReceiving()) {
    if (parser.getMessageType() == QueryResponseType::RESPONSE_CHECKPOINT_MAP) {
      resp = parser.getResponse_CHECKPOINT_MAP();
      //print_Response_CHECKPOINT_MAP(resp);
    }
    else if (parser.getMessageType() == QueryResponseType::QUERY_CHECKPOINT_MAP) {
      query = parser.getQuery_CHECKPOINT_MAP();
      //print_Query_CHECKPOINT_MAP(query);
    }
    parser.recvMessage();
  }

  if (parser.isReceiving()) {
    //parser.read(Wire);
  }
  else if (parser.isSending()) {
    parser.write(serial_soft1);
  }
    

}