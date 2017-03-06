/*
 * SIM808.c
 *
 * Created: 12/7/2016 6:53:44 PM
 *  Author: Shiva and Rishabh
 */ 

#include "SIM808.h"

char battLevel[5]="";
char battVoltage[5]="";
char signalQuality[5]="";
char latitude[50]="";
char longitude[50]="";
volatile int waitTime = 0;
volatile bool recvTimeoutFlag=false;
bool readCompleteFlag = false;

// Function to wait 4.5 seconds
void wait4_5seconds(void)
{
	startSimPwrTimer();
	while(waitTime<3);
}

// SIM PWR ON/OFF Timer
void startSimPwrTimer()
{
	waitTime = 0;
	dualtimer_enable(PWRKEY_TIMER);
}

// RECEIVE TIMEOUT TIMER RESET
void recvTimeout_Timer_Reset()
{
	dualtimer_disable(RECVTIMEOUT_TIMER);
	dualtimer_set_counter(RECVTIMEOUT_TIMER,DUALTIMER_SET_CURRUNT_REG, RECV_TIMEOUT);
}

// callback functions

void pwrkey_callback(void)
{
	if(!waitTime)
	{
		gpio_pin_set_output_level(PWRKEY_PIN,true);
	}
	else if(waitTime > 1)
	{
		dualtimer_disable(PWRKEY_TIMER);
	}
	waitTime++;
}

void recvTimeout_callback(void)
{
	debugPrint("Receive Timeout trigger\r\n");
	recvTimeoutFlag = true;
	recvTimeout_Timer_Reset();
}

void sim_uart_read_complete_callback(struct uart_module *const module)
{
	debugPrint("!!!Read Complete!!!");
}

// void simPwr(bool pwr)
// {
// 	bool simState;
// 	do
// 	{
// 		gpio_pin_set_output_level(PWRKEY_PIN,false);
// 		startSimPwrTimer();
// 		while(waitTime<=3);
// 		returnStatus status;
// 		status = writeCommand(ATCmd,ATResponseWithEcho,1);
// 		if(status == FAILURE)
// 		{
// 			status = writeCommand(ATCmd,ATResponseWithoutEcho,1);
// 			if(status == FAILURE)
// 			{
// 				simState = false;
// 			}
// 			else
// 			{
// 				simState = true;
// 			}
// 		}
// 		else
// 		{
// 			simState = true;
// 		}
// 	}while(simState!=pwr);
// }

void simPwr(bool pwr)
{
	bool simState;
	bool initResponse = false;
	disableUART_RX();
	simState = gpio_pin_get_input_level(SIM_PWRSTATE_PIN);
	while(simState!=pwr)
	{
		initResponse = true;
		gpio_pin_set_output_level(PWRKEY_PIN,false);
		wait4_5seconds();
		simState = gpio_pin_get_input_level(SIM_PWRSTATE_PIN);
	}
	initSimUart();
	if(initResponse)
	{
		char resp[100]="";
		if(pwr)
		{
			/* Set the Baud Rate for UART communication with SIM808 */
			returnStatus status;
			status = writeCommand(ATCmd,ATResponseWithEcho,1);
			if(status == FAILURE)
			{
				errorFunc();
			}
			/* When SIM808 is turned on, it sends two response, "Call Ready" and "SMS Ready" */
			recvResponse1(resp,2);
			debugPrint(resp);
		}
// 		else
// 		{
// 			/* When SIM808 is turned off, it sends "Normal Power Down" */
// 			recvResponse1(resp,1);
// 			debugPrint(resp);
// 		}
	}
}

void removeCRLF(char str[])
{
	char *s;
	int i=0,j=0,len=strlen(str);
	s = (char *)malloc(len);
	memset(s,0,len);
	for(i=0;i<len;i++)
	{
		if((str[i] == '\r') || (str[i] == '\n'))
		str[i]=0;
		else
		s[j++]=str[i];
	}
	memset(str,0,len);
	memcpy(str,s,len);
}

returnStatus recvResponse(char recvBuff[], int numOfResponses)
{
	volatile int cnt = 0;
	volatile int lfCnt = 0;
	returnStatus status = FAILURE;
	int j=0;
	recvTimeout_Timer_Reset();
	for(j=0;j<numOfResponses;)
	{
// 		if(uart_read_wait(&sim_uart,(uint8_t *)&recvBuff[cnt]) == STATUS_ERR_TIMEOUT)
// 		{
// 		}
		dualtimer_enable(RECVTIMEOUT_TIMER);
		recvTimeoutFlag = false;
#if sim_uart_interrupt_mode
		readCompleteFlag = false;
		uart_read_buffer_job(&sim_uart, (uint8_t *)&recvBuff[cnt], 4);
		while(1)
		{
			if(recvTimeoutFlag)
			{
				status = FAILURE;
				recvTimeoutFlag = false;
				debugPrint("recvTimeout!!!");
				if(cnt)
				{
					debugPrint(recvBuff);
				}
				else
				{
					debugPrint("recvBuff empty...");
				}
				return status;
			}
			if(readCompleteFlag)
			{
				//debugPrint(recvBuff[cnt]);
				break;	
			}
		}
#else
		uart_read_wait(&sim_uart,(uint8_t *)&recvBuff[cnt]);
#endif
		if(recvTimeoutFlag)
		{
			//status = SUCCESS;
			//debugPrint("recvTimeout!!!");
			break;
		}
		recvTimeout_Timer_Reset();
		if(recvBuff[cnt]=='\n')
		{
			lfCnt++;	
		}
		if(lfCnt == 2)
		{
			j++;
			lfCnt=0;
		}
		cnt++;
	}
	status = SUCCESS;
	return status;
}

void recvResponse1(char recvBuff[], int numOfResponses)
{
	volatile int cnt = 0;
	volatile int lfCnt = 0;
	int j=0;
	for(j=0;j<numOfResponses;)
	{
		uart_read_wait(&sim_uart,(uint8_t *)&recvBuff[cnt]);
		if(recvBuff[cnt]=='\n')
		{
			lfCnt++;
		}
		else if(recvBuff[cnt]=='\r')
		{
			continue;
		}
		if(lfCnt == 2)
		{
			j++;
			lfCnt=0;
		}
		cnt++;
	}
}

void sendATCommand(const char cmd[])
{
	uart_write_buffer_wait(&sim_uart, (uint8_t *)cmd, strlen(cmd));
	debugPrint(cmd);
}

returnStatus writeCommand(const char cmd[], const char responseCheck[], int numResponse)
{
	returnStatus status = FAILURE;
	char recvBuf[100]="";
	memset(recvBuf,0,sizeof(recvBuf));
	sendATCommand(cmd);
	if(recvResponse(recvBuf,numResponse) == FAILURE)
	{
		debugPrint("Nothing Received");
		return status;
	}
	debugPrint(recvBuf);
	removeCRLF(recvBuf);
	if(strncmp(recvBuf,responseCheck,strlen(responseCheck))==0)
	{
		status = SUCCESS;		
	}
	return status;
}

returnStatus writeCommand1(const char cmd[], const char responseCheck[], int numResponse)
{
	returnStatus status = FAILURE;
	char recvBuf[100]="";
	memset(recvBuf,0,sizeof(recvBuf));
	sendATCommand(cmd);
	do
	{
		recvResponse1(recvBuf,numResponse);
		debugPrint(recvBuf);
		removeCRLF(recvBuf);	
	}while(!strlen(recvBuf));
	if(strncmp(recvBuf,responseCheck,strlen(responseCheck))==0)
	{
		status = SUCCESS;		
	}
	return status;
}

returnStatus getGPSLoc()
{
	returnStatus status = FAILURE;
	char recvBuf[200]="";
	memset(latitude,0,50);
	memset(longitude,0,50);
	char temp[200]="";
	sendATCommand(GPSLocQueryCmd);
	recvResponse(recvBuf,2);
	debugPrint(recvBuf);
	removeCRLF(recvBuf);
	if(strncmp(recvBuf,GPSInfoResponse,strlen(GPSInfoResponse))==0)
	{
		status = SUCCESS;
	}
	// 	memset(recvBuf,0,200);
	// 	strcpy(recvBuf,"OK+CGNSINF: 1,1,20161124025208.000,40.007610,-105.262278,1666.100,0.19,212.7,1,,1.3,1.6,0.9,,10,6,,,28,,");
	if(status == SUCCESS)
	{
		sscanf(recvBuf,"+CGNSINF: %[^,],%[^,],%[^,],%[^,],%[^,],%s",temp,temp,temp,latitude,longitude,temp);
		debugPrint(latitude);
		debugPrint(longitude);
		if((strcmp(latitude,"")==0) || (strcmp(longitude,"")==0))
		{
			status = RETRY;
			wait4_5seconds();
			wait4_5seconds();
		}
	}
	return status;
}

void initializeSIM()
{
	returnStatus status;
	bool echo = true;
	status = writeCommand(ATCmd,ATResponseWithoutEcho,1);
	if(status == FAILURE)
	{
		status = writeCommand(ATCmd,ATResponseWithEcho,1);
		if(status == FAILURE)
		{
			errorFunc();
		}
		else
		{
			echo = true;
		}
	}
	else
	{
		echo = false;	
	}
	if(echo)
	{
		status = writeCommand(disableEcho,DisableEchoResponse,1);
		if(status == FAILURE)
		{
			errorFunc();
		}
	}
	else
	{
		status = writeCommand(disableEcho,OKResponse,1);
		if(status == FAILURE)
		{
			errorFunc();
		}
	}
	status = writeCommand(ATCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(SIMNumCmd,ErrorResponse,2);
	if(status == SUCCESS)
	{
		errorFunc();
	}
// 	else
// 	{
// 		char temp[50]="";
// 		recvResponse1(temp,1);
// 		debugPrint(temp);
// 		status = writeCommand(ATCmd,OKResponse,2);
// 		if(status == FAILURE)
// 		{
// 			while(1);
// 		}
// 	}
}


void initializeSIMGPS()
{
	returnStatus status;
	status = writeCommand(GPSStartCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(GPSStateQueryCmd,GPSStateOnResponse,2);
	if(status == FAILURE)
	{
		errorFunc();
	}
}

returnStatus getBatteryStatus(void)
{
	returnStatus status = FAILURE;
	char recvBuf[50]="";
	char temp[20]="";
	memset(battLevel,0,sizeof(battLevel));
	sendATCommand(BatteryStatusCmd);
	recvResponse(recvBuf,2);
	debugPrint(recvBuf);
	removeCRLF(recvBuf);
	if(strcmp(recvBuf,ErrorResponse)!=0)
	{
		status = SUCCESS;
	}
	if(status == SUCCESS)
	{
		sscanf(recvBuf,"+CBC: %[^,],%[^,],%[^,]",temp,battLevel,battVoltage);
		debugPrint(battLevel);
		debugPrint(battVoltage);
		if((strcmp(battLevel,"")==0) || (strcmp(battVoltage,"")==0))
		{
			status = RETRY;
			wait4_5seconds();
		}
	}
	return status;
}

returnStatus getSignalQuality(void)
{
	returnStatus status = FAILURE;
	char recvBuf[50]="";
	char temp[20]="";
	memset(signalQuality,0,sizeof(signalQuality));
	sendATCommand(SignalQualityCmd);
	recvResponse(recvBuf,2);
	debugPrint(recvBuf);
	removeCRLF(recvBuf);
	if(strcmp(recvBuf,ErrorResponse)!=0)
	{
		status = SUCCESS;
	}
	if(status == SUCCESS)
	{
		sscanf(recvBuf,"+CSQ: %[^,],%[^,]",signalQuality,temp);
		debugPrint(signalQuality);
		if(strcmp(signalQuality,"")==0)
		{
			status = RETRY;
			wait4_5seconds();
		}
	}
	return status;
}

returnStatus checkIfValidPost(void)
{
	returnStatus status = FAILURE;
	char recvBuf[50]="";
	char temp=0;
	char postDataResponse[10]="";
	sendATCommand(HTTPReadResponseCmd);
	recvResponse(recvBuf,2);
	debugPrint(recvBuf);
	removeCRLF(recvBuf);
	if(strncmp(recvBuf,HTTPReadResponse,strlen(HTTPReadResponse)) == 0)
	{
		status = SUCCESS;
	}
	if(status == SUCCESS)
	{
		sscanf(recvBuf,"+HTTPREAD: %c%s",&temp,postDataResponse);
		if(strncmp(postDataResponse,PostDataErrorResponse,strlen(PostDataErrorResponse))==0)
		{
			status = RETRY;
		}
		else if(strncmp(postDataResponse,PostDataOKResponse,strlen(PostDataOKResponse))==0)
		{
			status = SUCCESS;
		}
	}
	return status;
}


void getLoc_PostData(void)
{
	returnStatus status;
	do 
	{
		status = getGPSLoc();
		if(status == FAILURE)
		{
			errorFunc();
		}
	} while(status == RETRY);
	do
	{
		status = getBatteryStatus();
		if(status == FAILURE)
		{
			errorFunc();
		}
	} while(status == RETRY);
	do
	{
		status = getSignalQuality();
		if(status == FAILURE)
		{
			errorFunc();
		}
	} while(status == RETRY);
	char HTTPData[100]="";
	sprintf(HTTPData,"{\"lng\":\"%s\",\"lat\":\"%s\",\"bat\":\"%s\",\"rssi\":\"%s\"}",longitude,latitude,battLevel,signalQuality);
	debugPrint(HTTPData);
	status = writeCommand1(HTTPSendDataCmd,ErrorResponse,1);
	if(status == SUCCESS)
	{
		errorFunc();
	}
	status = writeCommand1(HTTPData,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(ATCmd,OKResponse,1);
	if(status == FAILURE)
	{
		while(1);
	}
	status = writeCommand1(HTTPActionCmd,ErrorResponse,2);
	if(status == SUCCESS)
	{
		errorFunc();
	}
	status = checkIfValidPost();
	if(status == FAILURE)
	{
		errorFunc();
	}
	else if(status == RETRY)
	{
		debugPrint("\r\nCouldn't Post Data");
	}
	status = writeCommand(HTTPTerminateCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
}

void initializeHTTP(void)
{
	returnStatus status;
	status = writeCommand(ATCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
// 	status = writeCommand(GPSStateQueryCmd,GPSStateOnResponse,2);
// 	if(status == FAILURE)
// 	{
// 		while(1);
// 	}
//  	char cmdf[100]="";
// 	strcpy(cmdf,SetConnTypeCmd);
// 	strcat(cmdf,SetAPNCmd);
// 	//status = writeCommand(cmdf,OKResponse2,2);
// 	sendATCommand(cmdf);
// 	memset(cmdf,0,sizeof(cmdf));
// 	recvResponse1(cmdf,1);
// 	debugPrint(cmdf);
// 	memset(cmdf,0,sizeof(cmdf));
// 	recvResponse(cmdf,1);
// 	debugPrint(cmdf);
// 	if(status == FAILURE)
// 	{
// 		while(1);
// 	}
	status = writeCommand(SetConnTypeCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(SetAPNCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(ATCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand1(QueryNetConnCmd,OKResponse,1);
	if(status == FAILURE)
	{
		status = writeCommand(QueryNetConnCmd,ErrorResponse,1);
		if(status == FAILURE)
		{
			errorFunc();
		}
	}
	status = writeCommand(HTTPInitStackCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
// 	memset(cmdf,0,sizeof(cmdf));
// 	strcpy(cmdf,SetHTTPParamCIDCmd);
// 	strcat(cmdf,SetHTTPParamURLCmd);
// 	strcat(cmdf,SetHTTPParamContCmd);
// 	//status = writeCommand(cmdf,OKResponse2,2);
// 	sendATCommand(cmdf);
// 	memset(cmdf,0,sizeof(cmdf));
// 	recvResponse(cmdf,1);
// 	debugPrint(cmdf);
// 	memset(cmdf,0,sizeof(cmdf));
// 	recvResponse1(cmdf,1);
// 	debugPrint(cmdf);
// 	memset(cmdf,0,sizeof(cmdf));
// 	recvResponse1(cmdf,1);
// 	debugPrint(cmdf);
// 	if(status == FAILURE)
// 	{
// 		while(1);
// 	}
	status = writeCommand(SetHTTPParamCIDCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(SetHTTPParamURLCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
	status = writeCommand(SetHTTPParamContCmd,OKResponse,1);
	if(status == FAILURE)
	{
		errorFunc();
	}
// 	status = writeCommand(SetHTTPParamKeyCmd,OKResponse,1);
// 	if(status == FAILURE)
// 	{
// 		errorFunc();
// 	}
}
