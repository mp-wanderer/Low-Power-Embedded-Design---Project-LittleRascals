/*
 * SIM808.h
 *
 * Created: 12/7/2016 6:54:28 PM
 *  Author: Shiva and Rishabh
 */ 


#ifndef SIM808_H_
#define SIM808_H_

#include <asf.h>
#include <string.h>
#include "main.h"

#define ATCmd									"AT\n"
#define ATICmd									"ATI\n"
#define disableEcho								"ATE0\n"
#define SIMNumCmd								"AT+CCID\n"
#define GPSStartCmd								"AT+CGNSPWR=1\n"
#define GPSStateQueryCmd						"AT+CGNSPWR?\n"
#define GPSLocQueryCmd							"AT+CGNSINF\n"
#define SetConnTypeCmd							"AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n"
#define SetAPNCmd								"AT+SAPBR=3,1,\"APN\",\"wholesale\"\n"
#define QueryNetConnCmd							"AT+SAPBR=1,1\n"
#define HTTPInitStackCmd						"AT+HTTPINIT\r\n"
#define SetHTTPParamCIDCmd						"AT+HTTPPARA=\"CID\",1\n"
#define SetHTTPParamURLCmd						"AT+HTTPPARA=\"URL\",\"http://littlerascals.informu.io/postData\"\n"	// "AT+HTTPPARA=\"URL\",\"http://requestb.in/vanehgva\"\n"	//
#define SetHTTPParamContCmd						"AT+HTTPPARA=\"CONTENT\",\"application/json\"\n"
#define SetHTTPParamKeyCmd						"AT+HTTPPARA=\"USERDATA\",\"key:littleRascals\"\n"
#define HTTPSendDataCmd							"AT+HTTPDATA=80,10000\n"
#define HTTPActionCmd							"AT+HTTPACTION=1\n"
#define HTTPReadResponseCmd						"AT+HTTPREAD\n"
#define HTTPTerminateCmd						"AT+HTTPTERM\n"
#define BatteryStatusCmd						"AT+CBC\n"
#define SignalQualityCmd						"AT+CSQ\n"

#define ATResponseWithEcho						"ATOK"
#define ATResponseWithoutEcho					"OK"
#define DisableEchoResponse						"ATE0OK"
#define OKResponse								"OK"
#define OKResponse2								"OKOK"
#define ErrorResponse							"ERROR"
#define GPSStateOnResponse						"+CGNSPWR: 1OK"
#define HTTPDownloadDataResponse				"DOWNLOAD"
#define HTTPActionResponse						"OK+HTTPACTION: 1,200"
#define HTTPReadResponse						"+HTTPREAD: "
#define GPSInfoResponse							"+CGNSINF: 1"
#define CSQResponse								"+CSQ: "
#define PostDataErrorResponse					"ErrorOK"
#define PostDataOKResponse						"OKOK"

// Function definitions
void removeCRLF(char str[]);
returnStatus recvResponse(char recvBuff[], int numOfResponses);
void recvResponse1(char recvBuff[], int numOfResponses);
void sendATCommand(const char cmd[]);
returnStatus writeCommand(const char cmd[], const char responseCheck[], int numResponse);
returnStatus writeCommand1(const char cmd[], const char responseCheck[], int numResponse);
returnStatus getGPSLoc(void);
returnStatus getBatteryStatus(void);
returnStatus getSignalQuality(void);
returnStatus checkIfValidPost(void);

void startSimPwrTimer(void);
void recvTimeout_Timer_Reset(void);




#endif /* SIM808_H_ */
