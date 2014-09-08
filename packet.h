#include <stdint.h>
#include <string.h>

#define PACKET_TYPE_EAP 0x00
#define PACKET_TYPE_START 0x01
#define CODE_REQUEST 0x01
#define CODE_RESPONSE 0x02
#define CODE_SUCCESS 0x03
#define CODE_UNKNOWN 0x0a
#define TYPE_NOTIFICATION 0x02
#define TYPE_IDENTITY 0x01
#define ID_NOTIFICATION 0x01
#define ID_IDENTITY 0x02
#define ID_CHALLENGE 0x03
#define PACKET_CHALLENGE_VALUE_SIZE 16
#define ID_SUCCESS 0x04
#define USER_NAME_MAX_LEN 24
extern uint8_t mac_8021x_standard[6];

typedef struct _PACKET{ 
	uint8_t dstmac[6];	
	uint8_t srcmac[6];
	uint8_t auth_type[2];
	uint8_t version;
	uint8_t pkt_type;
	uint8_t length[2];
	union{
		struct{
			uint8_t code;
			uint8_t id;
			uint8_t bodylength[2];
			uint8_t type;
		}__attribute__((__packed__)) test;//used to test the packet type,make the best of the attr of union
		struct{
			uint8_t code; //request(1)
			uint8_t id;  //id(1)
			uint8_t bodylength[2];
			uint8_t type; //Notification(2),Identity(1)
		}__attribute__((__packed__)) request_identify;//request_notification
		struct{
			uint8_t code; //response(2)	
			uint8_t id; //id(1)
			uint8_t bodylength[2];
			uint8_t type; //Notification(2)
		}__attribute__((__packed__)) response_notification;
		struct{
			uint8_t code;//response(2)
			uint8_t id;  //id(2)
			uint8_t bodylength[2]; //13
			uint8_t type;//identity(1)
			uint8_t identity[USER_NAME_MAX_LEN]; //30 30 31 30 31 33 32 30
		}__attribute__((__packed__)) response_identity;
		struct{
			uint8_t code;//request(1)
			uint8_t id; //id(3)
			uint8_t bodylength[2]; //22
			uint8_t type;//MD5-Challenge EAP(4)
			uint8_t value_size; //value_size(16)
			uint8_t value[PACKET_CHALLENGE_VALUE_SIZE];   //eap-md5 value(2d...40);
		}__attribute__((__packed__)) request_challenge; //response_challenge
		struct{
			uint8_t code;//unknown(10)
			uint8_t id;//id(4)
			uint8_t body_length[2];//69
		}__attribute__((__packed__)) unknown;
		struct{
			uint8_t code; //success(3)
			uint8_t id;   //id(4)
			uint8_t body_length[2];
		}__attribute__((__packed__)) success;
		/*struct{
		}__attribute__((__packed__)) fail;*/
	}__attribute__((__packed__)) body;
}PACKET;

int get_start_packet(PACKET *,char *);
