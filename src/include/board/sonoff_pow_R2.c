/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <os_type.h>
#include <osapi.h>
#include <eagle_soc.h>
#include <ets_sys.h>

#include "sonoff_pow_R2.h"
#include "supla_esp_gpio.h"
#include "supla_w1.h"
#include "supla-dev/log.h"
#include "gpio.h"
#include "user_config.h"
#include "supla_esp_devconn.h"
#include "driver/uart.h"

const uint8_t rsa_public_key_bytes[512] = {
	};

_supla_int64_t counter;

#define B_RELAY1_PORT    12
#define B_CFG_PORT        0

uint8_t buffer[128];
uint32_t voltage = 0;
uint32_t current = 0;
uint32_t power   = 0;
long voltage_cycle = 0;
long current_cycle = 0;
long power_cycle = 0;
long cf_pulses = 0;

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved) {
		
  static char html_template_header[] =
      "<!DOCTYPE html><meta http-equiv=\"content-type\" content=\"text/html; "
      "charset=UTF-8\"><meta name=\"viewport\" "
      "content=\"width=device-width,initial-scale=1,maximum-scale=1,user-"
      "scalable=no\"><style>body{font-size:14px;font-family:HelveticaNeue,"
      "\"Helvetica Neue\",HelveticaNeueRoman,HelveticaNeue-Roman,\"Helvetica "
      "Neue "
      "Roman\",TeXGyreHerosRegular,Helvetica,Tahoma,Geneva,Arial,sans-serif;"
      "font-weight:400;font-stretch:normal;background:#00d151;color:#fff;line-"
      "height:20px;padding:0}.s{width:460px;margin:0 auto;margin-top:calc(50vh "
      "- 340px);border:solid 3px #fff;padding:0 10px "
      "10px;border-radius:3px}#l{display:block;max-width:150px;height:155px;"
      "margin:-80px auto 20px;background:#00d151;padding-right:5px}#l "
      "path{fill:#000}.w{margin:3px 0 16px;padding:5px "
      "0;border-radius:3px;background:#fff;box-shadow:0 1px 3px "
      "rgba(0,0,0,.3)}h1,h3{margin:10px "
      "8px;font-family:HelveticaNeueLight,HelveticaNeue-Light,\"Helvetica Neue "
      "Light\",HelveticaNeue,\"Helvetica "
      "Neue\",TeXGyreHerosRegular,Helvetica,Tahoma,Geneva,Arial,sans-serif;"
      "font-weight:300;font-stretch:normal;color:#000;font-size:23px}h1{margin-"
      "bottom:14px;color:#fff}span{display:block;margin:10px 7px "
      "14px}i{display:block;font-style:normal;position:relative;border-bottom:"
      "solid 1px "
      "#00d151;height:42px}i:last-child{border:none}label{position:absolute;"
      "display:inline-block;top:0;left:8px;color:#00d151;line-height:41px;"
      "pointer-events:none}input,select{width:calc(100% - "
      "145px);border:none;font-size:16px;line-height:40px;border-radius:0;"
      "letter-spacing:-.5px;background:#fff;color:#000;padding-left:144px;-"
      "webkit-appearance:none;-moz-appearance:none;appearance:none;outline:0!"
      "important;height:40px}select{padding:0;float:right;margin:1px 3px 1px "
      "2px}button{width:100%;border:0;background:#000;padding:5px "
      "10px;font-size:16px;line-height:40px;color:#fff;border-radius:3px;box-"
      "shadow:0 1px 3px "
      "rgba(0,0,0,.3);cursor:pointer}.c{background:#ffe836;position:fixed;"
      "width:100%;line-height:80px;color:#000;top:0;left:0;box-shadow:0 1px "
      "3px rgba(0,0,0,.3);text-align:center;font-size:26px;z-index:100}@media "
      "all and (max-height:920px){.s{margin-top:80px}}@media all and "
      "(max-width:900px){.s{width:calc(100% - "
      "20px);margin-top:40px;border:none;padding:0 "
      "8px;border-radius:0}#l{max-width:80px;height:auto;margin:10px auto "
      "20px}h1,h3{font-size:19px}i{border:none;height:auto}label{display:block;"
      "margin:4px 0 "
      "12px;color:#00d151;font-size:13px;position:relative;line-height:18px}"
      "input,select{width:calc(100% - "
      "10px);font-size:16px;line-height:28px;padding:0 5px;border-bottom:solid "
      "1px #00d151}select{width:100%;float:none;margin:0}}</style><script "
      "type=\"text/javascript\">setTimeout(function(){var element =  "
      "document.getElementById('msg');if ( element != null ) "
      "element.style.visibility = \"hidden\";},3200);</script>";
  
  static char html_template[] =
      "%s%s<div class=\"s\"><svg version=\"1.1\" id=\"l\" x=\"0\" y=\"0\" "
      "viewBox=\"0 0 200 200\" xml:space=\"preserve\"><path "
      "d=\"M59.3,2.5c18.1,0.6,31.8,8,40.2,23.5c3.1,5.7,4.3,11.9,4.1,18.3c-0.1,"
      "3.6-0.7,7.1-1.9,10.6c-0.2,0.7-0.1,1.1,0.6,1.5c12.8,7.7,25.5,15.4,38.3,"
      "23c2.9,1.7,5.8,3.4,8.7,5.3c1,0.6,1.6,0.6,2.5-0.1c4.5-3.6,9.8-5.3,15.7-5."
      "4c12.5-0.1,22.9,7.9,25.2,19c1.9,9.2-2.9,19.2-11.8,23.9c-8.4,4.5-16.9,4."
      "5-25.5,0.2c-0.7-0.3-1-0.2-1.5,0.3c-4.8,4.9-9.7,9.8-14.5,14.6c-5.3,5.3-"
      "10.6,10.7-15.9,16c-1.8,1.8-3.6,3.7-5.4,5.4c-0.7,0.6-0.6,1,0,1.6c3.6,3.4,"
      "5.8,7.5,6.2,12.2c0.7,7.7-2.2,14-8.8,18.5c-12.3,8.6-30.3,3.5-35-10.4c-2."
      "8-8.4,0.6-17.7,8.6-22.8c0.9-0.6,1.1-1,0.8-2c-2-6.2-4.4-12.4-6.6-18.6c-6."
      "3-17.6-12.7-35.1-19-52.7c-0.2-0.7-0.5-1-1.4-0.9c-12.5,0.7-23.6-2.6-33-"
      "10.4c-8-6.6-12.9-15-14.2-25c-1.5-11.5,1.7-21.9,9.6-30.7C32.5,8.9,42.2,4."
      "2,53.7,2.7c0.7-0.1,1.5-0.2,2.2-0.2C57,2.4,58.2,2.5,59.3,2.5z "
      "M76.5,81c0,0.1,0.1,0.3,0.1,0.6c1.6,6.3,3.2,12.6,4.7,18.9c4.5,17.7,8.9,"
      "35.5,13.3,53.2c0.2,0.9,0.6,1.1,1.6,0.9c5.4-1.2,10.7-0.8,15.7,1.6c0.8,0."
      "4,1.2,0.3,1.7-0.4c11.2-12.9,22.5-25.7,33.4-38.7c0.5-0.6,0.4-1,0-1.6c-5."
      "6-7.9-6.1-16.1-1.3-24.5c0.5-0.8,0.3-1.1-0.5-1.6c-9.1-4.7-18.1-9.3-27.2-"
      "14c-6.8-3.5-13.5-7-20.3-10.5c-0.7-0.4-1.1-0.3-1.6,0.4c-1.3,1.8-2.7,3.5-"
      "4.3,5.1c-4.2,4.2-9.1,7.4-14.7,9.7C76.9,80.3,76.4,80.3,76.5,81z "
      "M89,42.6c0.1-2.5-0.4-5.4-1.5-8.1C83,23.1,74.2,16.9,61.7,15.8c-10-0.9-18."
      "6,2.4-25.3,9.7c-8.4,9-9.3,22.4-2.2,32.4c6.8,9.6,19.1,14.2,31.4,11.9C79."
      "2,67.1,89,55.9,89,42.6z "
      "M102.1,188.6c0.6,0.1,1.5-0.1,2.4-0.2c9.5-1.4,15.3-10.9,11.6-19.2c-2.6-5."
      "9-9.4-9.6-16.8-8.6c-8.3,1.2-14.1,8.9-12.4,16.6C88.2,183.9,94.4,188.6,"
      "102.1,188.6z "
      "M167.7,88.5c-1,0-2.1,0.1-3.1,0.3c-9,1.7-14.2,10.6-10.8,18.6c2.9,6.8,11."
      "4,10.3,19,7.8c7.1-2.3,11.1-9.1,9.6-15.9C180.9,93,174.8,88.5,167.7,88."
      "5z\"/></svg><h1>%s</h1><span>LAST STATE: %s<br>Firmware: %s<br>GUID: "
      "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X<br>MAC:"
      " %02X:%02X:%02X:%02X:%02X:%02X</span><form method=\"post\"><div "
      "class=\"w\"><h3>Wi-Fi Settings</h3><i><input name=\"sid\" "
      "value=\"%s\"><label>Network name</label></i><i><input "
      "name=\"wpw\"><label>Password</label></i></div><div "
      "class=\"w\"><h3>Supla Settings</h3><i><input name=\"svr\" "
      "value=\"%s\"><label>Server</label></i><i><input name=\"eml\" "
      "value=\"%s\"><label>E-mail</label></i></div><div "
      "class=\"w\"><h3>Additional Settings</h3>"
      "<i><select name=\"zre\"><option value=\"0\" %s>NO<option value=\"1\" %s>YES</select><label>Zero Initial Energy</label></i>"
      "</div><button type=\"submit\">SAVE</button></form></div><br><br>";

  int bufflen = strlen(supla_esp_devconn_laststate())
				+strlen(dev_name)
				+strlen(SUPLA_ESP_SOFTVER)
				+strlen(supla_esp_cfg.WIFI_SSID)
				+strlen(supla_esp_cfg.Server)
				+strlen(supla_esp_cfg.Email)
				+strlen(html_template_header)
				+strlen(html_template)
				+200;

  char *buffer = (char *)malloc(bufflen);

  ets_snprintf(
      buffer, bufflen, html_template, html_template_header,
      data_saved == 1 ? "<div id=\"msg\" class=\"c\">Data saved</div>" : "",
      dev_name, supla_esp_devconn_laststate(), SUPLA_ESP_SOFTVER,
      (unsigned char)supla_esp_cfg.GUID[0],
      (unsigned char)supla_esp_cfg.GUID[1],
      (unsigned char)supla_esp_cfg.GUID[2],
      (unsigned char)supla_esp_cfg.GUID[3],
      (unsigned char)supla_esp_cfg.GUID[4],
      (unsigned char)supla_esp_cfg.GUID[5],
      (unsigned char)supla_esp_cfg.GUID[6],
      (unsigned char)supla_esp_cfg.GUID[7],
      (unsigned char)supla_esp_cfg.GUID[8],
      (unsigned char)supla_esp_cfg.GUID[9],
      (unsigned char)supla_esp_cfg.GUID[10],
      (unsigned char)supla_esp_cfg.GUID[11],
      (unsigned char)supla_esp_cfg.GUID[12],
      (unsigned char)supla_esp_cfg.GUID[13],
      (unsigned char)supla_esp_cfg.GUID[14],
      (unsigned char)supla_esp_cfg.GUID[15],
      (unsigned char)mac[0],
      (unsigned char)mac[1],
      (unsigned char)mac[2],
      (unsigned char)mac[3],
      (unsigned char)mac[4],
      (unsigned char)mac[5],
      supla_esp_cfg.WIFI_SSID,
      supla_esp_cfg.Server, supla_esp_cfg.Email,
      supla_esp_cfg.ZeroInitialEnergy == 0 ? "selected" : "",
      supla_esp_cfg.ZeroInitialEnergy == 1 ? "selected" : ""
      );

  return buffer;
}

void Cse_Rec(int start, unsigned int relay_laststate)
{
	if ( relay_laststate == 1) {
		long voltage_coefficient;
		voltage_coefficient = buffer[2+start]*65536 + buffer[3+start]*256 + buffer[4+start];
		voltage_cycle = buffer[5+start]*65536 + buffer[6+start]*256 + buffer[7+start];
		if ((buffer[start] & 0xF8) == 0xF8) {
			voltage = 0;
		} else {
			voltage = (voltage_coefficient * 10) / voltage_cycle;
		}

		long current_coefficient;
		current_coefficient = buffer[8+start]*65536 + buffer[9+start]*256 + buffer[10+start];
		current_cycle = buffer[11+start]*65536 + buffer[12+start]*256 + buffer[13+start];
		if ((buffer[start] & 0xF4) == 0xF4) {
		current = 0;
		} else {
		current = (current_coefficient * 1000) / current_cycle;
		}
		if (current < 100) current = 0;

		long power_coefficient;
		power_coefficient = buffer[14+start]*65536 + buffer[15+start]*256 + buffer[16+start];
		power_cycle = buffer[17+start]*65536 + buffer[18+start]*256 + buffer[19+start];
		uint8_t adjustment = buffer[20+start];
		cf_pulses = buffer[21+start]*256 + buffer[22+start];
		if (adjustment & 0x10) {
		if ((buffer[start] & 0xF2) == 0xF2) {
			power = 0;
		} else {
			power = power_coefficient / power_cycle;
		}
		} else {
			power = 0;  
		}
	} else {
		voltage = 0;
		current = 0;
		power = 0;  
	}
}

//-------------------------------------------------------
void ICACHE_FLASH_ATTR
supla_getVoltage(char value[SUPLA_CHANNELVALUE_SIZE]) {
	memcpy(value, &voltage, sizeof(uint32_t));
}

//-------------------------------------------------------
void ICACHE_FLASH_ATTR
supla_getCurrent(char value[SUPLA_CHANNELVALUE_SIZE]) {
	memcpy(value, &current, sizeof(uint32_t));
}

//-------------------------------------------------------
void ICACHE_FLASH_ATTR
supla_getPower(char value[SUPLA_CHANNELVALUE_SIZE]) {
	memcpy(value, &power, sizeof(uint32_t));
}

void ICACHE_FLASH_ATTR
supla_pow_R2_init(void) {
	uart_div_modify(0, UART_CLK_FREQ / 4800);
	os_printf("UART Init\n");
}

int ICACHE_FLASH_ATTR
UART_Recv(uint8 uart_no, uint8_t *buffer, int max_buf_len)
{
    uint8 max_unload;
	int index = -1;

    uint8 fifo_len = (READ_PERI_REG(UART_STATUS(uart_no))>>UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT;

    if (fifo_len)
    {
        max_unload = (fifo_len<max_buf_len ? fifo_len : max_buf_len);
        for (index=0;index<max_unload; index++)
        {
            *(buffer+index) = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
        }
        WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_RXFIFO_FULL_INT_CLR);
    }
    return index;
}

void ICACHE_FLASH_ATTR
uart_status(unsigned int relay_laststate) {
	int recv_len = -1;
	int index;

    recv_len = UART_Recv(0, buffer, sizeof(buffer)-2);

    if (recv_len>0)
    {
        buffer[recv_len] = 0;
        os_printf("Received: [%d]\r\n", recv_len);
		
        for (index = 0; index < recv_len-1; index ++) {
			if ((buffer[index] == 0xF2) && (buffer[index+1] == 0x5A)) break;
			if ((buffer[index] == 0x55) && (buffer[index+1] == 0x5A)) break;
		}
		if (index < recv_len - 24){
			os_printf("Index: %d\r\n", index);
			if (index>0) Cse_Rec(index, relay_laststate);
		}
    }
}

void supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
	ets_snprintf(buffer, buffer_size, "SONOFF-POW-R2");
}


void supla_esp_board_gpio_init(void) {
		
    supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
    supla_input_cfg[0].gpio_id = B_CFG_PORT;
    supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
    supla_input_cfg[0].relay_gpio_id = B_RELAY1_PORT;
    supla_input_cfg[0].channel = 0;

    supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
    supla_relay_cfg[0].flags = RELAY_FLAG_RESTORE_FORCE;
    supla_relay_cfg[0].channel = 0;

//    sntp_setservername(0, NTP_SERVER);
//    sntp_stop();
//    sntp_init();

}

void supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {
	
    *channel_count = 2;

	channels[0].Number = 0;
	channels[0].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[0].FuncList = SUPLA_BIT_RELAYFUNC_POWERSWITCH \
								| SUPLA_BIT_RELAYFUNC_LIGHTSWITCH;
	channels[0].Default = SUPLA_CHANNELFNC_POWERSWITCH;
	channels[0].value[0] = supla_esp_gpio_relay_on(B_RELAY1_PORT);
    
	channels[1].Number = 1;
	channels[1].Type = SUPLA_CHANNELTYPE_ELECTRICITY_METER;
	supla_esp_em_get_value(1, channels[1].value);

}

void supla_esp_board_send_channel_values_with_delay(void *srpc) {

	supla_esp_channel_value_changed(0, supla_esp_gpio_relay_on(B_RELAY1_PORT));

}
