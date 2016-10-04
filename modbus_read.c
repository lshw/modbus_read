#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <modbus/modbus.h>

char * ver="1.0";
int main(int argc, char *argv[])
{
	uint16_t data[100] ;
	modbus_t *ctx;
	int16_t i,m,rc,write = 0,ret=0;
	FILE *fpt = NULL;
	char gtype[100];

	i = strlen(argv[0]);
	for(; i>0 ; i--) 
		if(argv[0][i] == '\\' || argv[0][i] == '/') {
			i++;
			break; //basename
		}
	if(strcmp(&argv[0][i],"modbus_write") == 0) write = 1;
	if(argc == 2)
		if(argv[1][0] == '-' & argv[1][1] == 'v') {
			printf("%s\r\n",ver);
			return 0;
		}

	if (argc != 6 && argc != 7) {
		if(write == 0)
			printf("error, use:\r\n%s /dev/ttyUSB0 19200 1 0 2 [%s]\r\n%s dev speed client_id reg_addr len [type]\r\n",argv[0],"%04x",argv[0]); 
		else
			printf("error,use:\r\n%s /dev/ttyUSB0 9600 88 0 1 [%s]\r\n%s dev speed client_id reg_addr dat [type]\r\n",argv[0],"%d",argv[0]);
		return -2;   //命令行参数错误 
	}
	ctx = modbus_new_rtu(argv[1], atoi(argv[2]) , 'N', 8, 2);
	if (ctx == NULL) {
		return -1; //设备打开错误
	}
	modbus_set_debug(ctx, FALSE); //关闭modbus调试
	//modbus_set_debug(ctx, TRUE); //打开modbus调试
	modbus_set_error_recovery(ctx,MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);
	modbus_set_slave(ctx, atoi(argv[3]));
	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1; //链接设备错误
	}
	if(write==0) { //modbus_read /dev/ttyUSBPL0 9600 88 1 9 "%x"
		if(argc == 7) {  
			sprintf(gtype,"%s",argv[6]);
		} else if (argc == 6) {
			strcpy(gtype,"%04x ");
		}
		i=atoi(argv[5]);
		if(gtype[0] == '%' & gtype[1] == 'f') m=i+i;
		else m=i;
		memset(data, 0, m * sizeof(uint16_t));
		rc = modbus_read_registers(ctx, atoi(argv[4]),
				m,  data);
		if ( rc == m) {
			for(m=0;m<i;m++)  {
				if(gtype[0] == '%' & gtype[1] == 'c') {
					printf(gtype,data[m] >> 8);
					printf(gtype,data[m] & 0xff);
				} else if(gtype[0] == '%' & gtype[1] == 'f') {
					printf(gtype,modbus_get_float(&data[2*m]));
				}else 
					printf(gtype, data[m]);
			}
		}
	}else { //modbus_write /dev/ttyUSBPL0 9600 88 1 4234 %d
		if(argc == 7) 
			sprintf(gtype,"%s",argv[6]);
		else { //defalut the type is "%d"
			gtype[0] = '%';
			gtype[1] = 'd';
			gtype[3] = 0;
		}
		if(gtype[0] == '%' & gtype[1] == 'f') 
			m=2; //浮点占用2个
		else 
			m=1; //整形和hex
		sscanf(argv[5],gtype,&data);
		i=atoi(argv[4]);
		if(modbus_write_register(ctx,i,data[0]) != 1) {
			printf("write error!\r\n");
			ret = -4; //写寄存器错误
		}
		else if(m == 2) {//浮点写2个uint16_t
			if(modbus_write_register(ctx,i+1,data[1]) != 1) {
				printf("write error!\r\n");
				ret = -4; //写寄存器错误
			}
		}
	}
	modbus_close(ctx);
	modbus_free(ctx);
	return ret;
}
