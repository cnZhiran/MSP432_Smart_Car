#ifndef __OLED_H_
#define __OLED_H_

#define IIC_SDA_0 P1OUT&=~BIT6;P1DIR|=BIT6  //SDA
#define IIC_SDA_1 P1OUT|=BIT6;P1DIR&=~BIT6

#define IIC_SCK_0 P1OUT&=~BIT7  //SCL
#define IIC_SCK_1 P1OUT|=BIT7

void IIC_start();
void IIC_stop();
void IIC_write(unsigned char date);
void OLED_send_cmd(unsigned char o_command);
void OLED_send_data(unsigned char o_data);
void OLED_Column_set(unsigned char column);
void OLED_Page_set(unsigned char page);
void OLED_Pos_set(unsigned char page, unsigned char column);
void OLED_clear(void);
void OLED_full(void);
void OLED_init(void);
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P16x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_P8x16Str_R(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P16x16Str_R(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_Print_R(unsigned char x, unsigned char y, unsigned char ch[]);
#endif /* OLED_H_ */
