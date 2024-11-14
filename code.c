#include<reg51.h>

// P1 connected to LCD data pins.
#define lcd P1

// pins P3.6 and P3.7 to rs and e pins of LCD display.
sbit rs=P3^5;
sbit rw=P3^6;
sbit e=P3^7;

// pins P2.0 and P2.1 are connected to the data pins of the IR sensor.
// usually these pins act as a input pins.
sbit s1=P2^0;
sbit s2=P2^1;

// initializing all used function globally.
void delay (int);
void cmd (unsigned char);
void display (unsigned char);
void init (void);
void view (int);
void string (char *);
int count=0;

void main()
{
    //P2=0x00;// as the IR sensor connected to PORT2, to detect the 1 (HIGH) o/p from the IR sensor make this PORT as 0 (LOW).

    init();// call init function to to initialize the display.

    string("NBNSTIC");
    cmd(0xc0);
    string("Welcome");
    delay(1000);
    cmd(0x01);
    cmd(0x80);
    string("People Counter!");
    cmd(0xc0);

    view(count);// as the count value was 0, the view function will make the LCD to display 00 initially.

    // once if, s1 detects high follow by s2 then the value will increment.
    // unless if, s2 detects high follow by s1 then the value will decrement.
    while(1)
    {
      if(s1==0)
			{
				delay(200);
					if(s2==0)
					{
						count=count+1;
						view(count);
						delay(500);
					}
				}
			else if(s2==0)
			{
				delay(200);
					if(s1==0)
					{
						count=count-1;
						view(count);
						delay(500);
					}
				}
			}
}

// delay function which produce approx of 1ms delay if d is 1.
// so, if d is 1000 it produce approx 1sec delay.
void delay (int d)
{
    unsigned char i=0;
    for(;d>0;d--)
    {
        for(i=250;i>0;i--);
        for(i=248;i>0;i--);
    }
}

//the function will holds a set of primary commands to display which is used to initialize it.
void cmd (unsigned char c)
{
    lcd=c;
    rs=0;
		rw=0;
    e=1;
    delay(5);
    e=0;
}

// display function is used to give data to the lcd, which is to be displayed.
// usually a single character.
void display (unsigned char c)
{
    lcd=c;
    rs=1;
		rw=0;
    e=1;
    delay(5);
    e=0;
}

// string function is used display the string characters (words).
// this function will work by keep on calling the display function to display the individual character in a word,
// untill it detect '/0'. usually an end of string (word).
void string (char *p)
{
    while(*p)
    {
        display(*p++);
    }
}

//the function will holds a set of primary commands to display which is used to initialize it.
void init (void)
{
    cmd(0x38);
    cmd(0x0c);
    cmd(0x01);
    cmd(0x80);
}

// NOTE : we cant send the 2 or more digit values directly to the LCD display. like, 22, 234, 4321, etc....
// to over come this problem this _view_ function will split the 2 digit value (as this project concentrate
// only values from 00 to 99) seperately and send them one by one to the display function.
void view (int n)
{
    cmd(0xc0);
    display(((n/10)%10)+'0');
    display((n%10)+'0');
}