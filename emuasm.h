#ifndef EMUASM
#define EMUASM

#if defined(WINDOWS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

int emuasm(int argc, char* argv);

#if defined(WINDOWS)
#include <stdlib.h>
#include <vadefs.h>
#define clear() system("cls")
int print(const char* format, ...) {
	va_list args = 0;
	__crt_va_start(args, format);
	int result = vprintf(format, args);
	__crt_va_end(args);
	return result;
}
int scan(char* buffer) { return gets(buffer); }
int fscan(char* buffer, const char* format, ...) {
	gets(buffer);
	va_list args = 0;
	__crt_va_start(args, format);
	int result = vsscanf(buffer, format, args);
	__crt_va_end(args);
	return result;
}

int main(int argc, char* argv) {
	int result;
	result = emuasm(argc, argv);
	system("pause");
	return result;
}

#elif defined(LINUX)
#include <stdlib.h>
#include <stdarg.h>
#define clear() system("clear")
int print(const char* format, ...) {
	va_list args = 0;
	va_start(args, format);
	int result = vprintf(format, args);
	va_end(args);
	return result;
}
int scan(char* buffer) { return gets(buffer); }
int fscan(char* buffer, const char* format, ...) {
	gets(buffer);
	va_list args;
	va_start(args, format);
	int result = vsscanf(buffer, format, args);
	va_end(args);
	return result;
}

int main(int argc, char* argv) {
	int result;
	result = emuasm(argc, argv);
	return result;
}

#elif defined(ARDUINO)
#include <stdarg.h>
#define IOBUF 128
#define BACKCOLOR 0x0000
#define clear() { tft.setCursor(0, 0); tft.fillScreen(BACKCOLOR); }
int print(const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	tft.print(iobuf);
	free(iobuf);
	return result;
}
int scan(char* buffer) {
	char count = 0;
	while (true) {
		if (Serial.available() > 0) {
			if (Serial.peek() == '\n') break;
			buffer[count] = Serial.read();
			if (buffer[count] == 0x08 && count > 0) {
				count -= 1;
				tft.print((char)0x08);
				continue;
			}
			else if (buffer[count] != 0x08) {
				tft.print(buffer[count]);
				count += 1;
			}
		}
		else if (Serial1.available() > 0) {
			if (Serial1.peek() == '\n') break;
			buffer[count] = Serial1.read();
			if (buffer[count] == 0x08 && count > 0) {
				count -= 1;
				tft.print((char)0x08);
				continue;
			}
			else if (buffer[count] != 0x08) {
				tft.print(buffer[count]);
				count += 1;
			}
		}
	}
	buffer[count] = '\0';
	if (Serial.available() > 0) Serial.read();
	else if (Serial1.available() > 0) Serial1.read();
	tft.println();
}
int fscan(char* buffer, const char* format, ...) {
	scan(buffer);
	va_list args = 0;
	va_start(args, format);
	int result = vsscanf(buffer, format, args);
	va_end(args);
	return result;
}
#endif

#define byte unsigned char

char scmp(const char* a, const char* b) { return strcmp(a, b) == 0; }
char* scat(const char* buffer, char* head, char* body) {
	sscanf(buffer, "%s %[^\n]", head, body);
	return body;
}

byte cph = 0;
byte cpl = 0;
byte zf = 0;
byte of = 0;

byte ra, rb, rc, rd, re, rf, rg, rh;

#define STACKSIZE 128
byte stack[STACKSIZE];
byte sp = 0;
void push(byte data) { stack[sp] = data; sp += 1; sp &= 0x7F; }
void pop(byte* data) { sp = (sp != 0) ? sp - 1 : 0; data = stack[sp]; }

void mov(byte* dest, byte src) { dest = src; zf = dest == 0; }

void in(byte port) { 
#ifdef ARDUINO
	pinMode(port, INPUT);
	rd = digitalRead(port);
#endif
}
void out(byte port, byte data) { 
#ifdef ARDUINO
	pinMode(port, OUTPUT);
	digitalWrite(port, data & 0x01);
#endif
}

void lea() {  }
void ael() {  }

void add(byte* dest, byte src) {
	if (0xFF - *dest < src) { of = 1; dest = 0xFF; return; }
	dest = *dest + src;
}
void inc(byte* dest) {
	if (*dest == 0xFF) { of = 1; return; }
	dest = *dest + 1;
}
void sub(byte* dest, byte src) {
	dest = (*dest >= src) ? *dest - src : 0;
	zf = dest == 0;
}
void dec(byte* dest) {
	dest = (*dest >= 1) ? *dest - 1 : 0;
	zf = dest == 0;
}
void cmp(byte dest, byte src) { zf = dest == src; }
void mul(byte dest, byte src) {
	ra = (dest * src) >> 8;
	rb = (dest * src) & 0xFF;
}
void _div(byte src) {
	push(rd); push(re);
	mov(&rd, ra); mov(&re, rb);
	ra = (((rd << 8) + re) / src) & 0xFF;
	rb = (((rd << 8) + re) % src);
	pop(&re); pop(&rd);
}

void and(byte* dest, byte src) {
	dest = *dest & src; zf = *dest == 0; 
}
void or(byte* dest, byte src) {
	dest = *dest | src; zf = *dest == 0;
}
void xor(byte* dest, byte src) {
	dest = *dest ^ src; zf = *dest == 0;
}
void not(byte* dest) {
	dest = ~*dest; zf = *dest == 0;
}

void shl(byte* dest, byte src) { dest = *dest << (src & 0x08); }
void shr(byte* dest, byte src) { dest = *dest >> (src & 0x08); }

void jmp() {  }
void jz() {  }
void jnz() {  }
void loop() {  }

void call() {  }
void nop() {  }
void wait() {  }
void rst() {  }
void end() {  }
void start() {  }

void db() {  }
void dc() {  }
void ds() {  }

#endif // !EMUASM
