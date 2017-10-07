/*RGWAPF V0.3*/
/*RICOH GW Arabic Print Filter*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#define TRUE 1

char line_buffer[BUFSIZ];
int lineno = 0;
int error_flag = 0;
int numeralshapes; 

/*Maps characters from ISO1256 (index) to IBM-864 (value)*/
unsigned char charmap[] = {
	0xA7, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*80 - 8F*/
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*90 - 9F*/
	0x20, 0xAC, 0xC0, 0xA3, 0xA4, 0x20, 0xDB, 0x15, 0x20, 0x20, 0x20, 0x97, 0xDC, 0xA1, 0x20, 0x20, /*A0 - AF*/
	0xB0, 0x93, 0x20, 0x20, 0x20, 0x20, 0x14, 0x81, 0x20, 0x20, 0xBB, 0x98, 0x95, 0x94, 0x20, 0xBF, /*B0 - BF*/
	0xF3, 0xC1, 0xC2, 0xC3, 0xC4, 0xC7, 0xFF, 0xC7, 0xA9, 0xC9, 0xAA, 0xAB, 0xAD, 0xAE, 0xAF, 0xCF, /*C0 - CF*/
	0xD0, 0xD1, 0xD2, 0xBC, 0xBD, 0xBE, 0xEB, 0xDE, 0xD7, 0xD8, 0xDF, 0xEE, 0xE0, 0xBA, 0xF8, 0xFC, /*D0 - DF*/
	0x20, 0xFB, 0x20, 0xEF, 0xF2, 0xE7, 0xE8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE9, 0xFD, 0x20, 0x20, /*E0 - EF*/
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xDD, 0xF1, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20  /*F0 - FF*/
	}; 
	
/*BITS 0-1: defines how character glygh is handeled 0: single glygh, 1: connects to previous, 2: connects to next, 3: connects to both*/
/*BITS 2-7: pointer to shapes array*/
unsigned char shapecodes[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x07, 0x0B, 0x00, 0x0F, 0x13, 0x17, /*A0 - AF*/
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x1F, 0x23, 0x27, 0x00, /*B0 - BF*/
	0x00, 0x00, 0x2D, 0x31, 0x01, 0x00, 0x03, 0x35, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /*C0 - CF*/
	0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, /*D0 - DF*/
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x01, 0x41, 0x00, 0x2B, 0x00, 0x00, 0x3F, 0x47, /*E0 - EF*/
	0x00, 0x00, 0x4B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x57, 0x5B, 0x5F, 0x00, 0x67  /*F0 - FF*/
	};
	
/*Begining, Middle, End*/
unsigned char shapes[][3] = {
	{0x00, 0x00, 0x00}, /*00:reserved*/
	{0xCA, 0xCA, 0xAA}, /*01:TEH*/
	{0xCB, 0xCB, 0xAB}, /*02:THEH*/
	{0xCC, 0xCC, 0xAD}, /*03:JEEM*/
	{0xCD, 0xCD, 0xAE}, /*04:HAH*/
	{0xCE, 0xCE, 0xAF}, /*05:KHAH*/
	{0xE1, 0xE1, 0xBA}, /*06:FAH*/
	{0xD3, 0xD3, 0xBC}, /*07:SEEN*/
	{0xD4, 0xD4, 0xBD}, /*08:SHEEN*/
	{0xD5, 0xD5, 0xBE}, /*09:SAD*/
	{0xD6, 0xD6, 0xEB}, /*0A:DAD*/
	{0xC2, 0xA2, 0xA2}, /*0B:ALEF SHADA*/
	{0xC3, 0xA5, 0xA5}, /*0C:ALEF HAMZA*/
	{0xC7, 0xA8, 0xA8}, /*0D:ALEF*/
	{0xD9, 0xEC, 0xC5}, /*0E:AYN*/
	{0xDA, 0xF7, 0xED}, /*0F:GAIN*/
	{0xE9, 0xE9, 0xF5}, /*10:ALEF MAQSOORA*/
	{0xE5, 0xE5, 0xEF}, /*11:MEEM*/
	{0xE6, 0xE6, 0xF2}, /*12:NOON*/
	{0xE7, 0xF4, 0xF3}, /*13:HAA*/
	{0xE2, 0xE2, 0xF8}, /*14:QAF*/
	{0xE4, 0xE4, 0xFB}, /*15:LAM*/
	{0xE3, 0xE3, 0xFC}, /*16:KAF*/
	{0xEA, 0xEA, 0xF6}, /*17:YAA*/
	{0xC8, 0xC8, 0xA9}, /*18:BEH*/
	{0xC6, 0xC6, 0xF5}  /*19:YAA WITH HAMZA*/
	
};

int readline(char line[], int max) {
	int cnt = 0;
	int c;
	max = max - 1;			/* leave room for '\0' */

	while((c = getchar()) != EOF)
	{
		if(c == '\n')
			break;

		if(cnt < max) {
			line[cnt] = c;
			cnt = cnt + 1;
		}
	}

	if(c == EOF && cnt == 0)
		return EOF;

	line[cnt] = '\0';
	return cnt;
}

int isDigit(char d){
	if(d >= '0' && d <= '9')
		return d - '0';
	else
		return -1;
}

int str2int(char *s){
	int i, l, r, d;
	l = strlen(s);
	r = 0;
	if(l > 3) return -1;
	for(i = 0; i <l; i++){
		d = isDigit(s[i]);
		if(d >= 0)
			r = r * 10 + d;
		else
			return -1;
	}
	
	return r;
}

void insertChar(char nc, int postion) {
	int i;
	unsigned char tc, pc;
	pc = nc;
	for(i = postion; i < BUFSIZ; i++) {
		tc = line_buffer[i];
		line_buffer[i] = pc;
		pc = tc;
		if(pc == '\n') {
			line_buffer[i + 1] = pc;
			break;
		}
	}
}

void conv_iso1256_ibm864() {
	unsigned char cc; /* Current Character from buffer */
	unsigned char nc; /* New Character from lookup table */
	int i;
	for(i = 0; i < BUFSIZ && line_buffer[i] != '\n'; i++) {
		cc = line_buffer[i];
		if(cc >= 0x80) {
			nc = charmap[cc - 0x80];
			line_buffer[i] = nc;
			
		}
	}
}

void conv_arabic_glyph(void) {
	unsigned char nl[BUFSIZ];
	unsigned char cc; /* Current Character from buffer */
	unsigned char pc; /* Previous Character*/
	unsigned char nc; /* Next Character*/
	unsigned char sc; /* Character shape code*/
	unsigned char scn, scp; /*Shape code of next and pervious characters*/
	unsigned char si; /* Shape index*/
	unsigned char cp; /* Character position 0: alone, 1: first, 2: last, 3: last*/
	int i, j;
	
	j = 0;
	
	for(i = 0; i < BUFSIZ; i++) {
		cc = line_buffer[i];
		pc = (i == 0)  ? 0 : line_buffer[i - 1];
		nc = (i == BUFSIZ) ? 0 : line_buffer[i + 1];
		if(cc < 0xA0) {
			/*printf("i:%02d j:%02d English char %#02x\n", i, j, line_buffer[i]);*/
			nl[j++] = line_buffer[i];
		}
		else {
			sc = shapecodes[cc - 0xA0] & 0x03;
			si = shapecodes[cc - 0xA0] >> 2;
			scp = pc < 0xA0 ? 0 : shapecodes[pc - 0xA0] & 0x03;
			scn = nc < 0xA0 ? 0 : shapecodes[nc - 0xA0] & 0x03;
			if(si == 0) {/*Same glyph in all cases*/
				/*printf("i:%02d j:%02d Same glyph %#02x\n", i, j, line_buffer[i]);*/
				nl[j++] = line_buffer[i];
			}
			else if (cc == 0xfb && (nc == 0xC2 || nc == 0xC3 || nc == 0xC7)) {/*LAM followed by ALEF*/
				/*printf("i:%02d j:%02d LAM ALEF char %#02x\n", i, j, line_buffer[i]);*/
				switch (nc){
					case 0xC2: /*ALEF with SHADA*/
						nl[j] = 0xf9;
						break;
					case 0xC3: /*ALEF with HAMZA*/
						nl[j] = 0x99;
						break;
					case 0x0C7: /*ALEF*/
						nl[j] = 0x9D;
						break;
				}
				if(scp == 2 || scp == 3)  nl[j++]++; else j++; /*If conneced to previous select next glyph*/
				i++; /*Skip one character*/
			}
			else {
				/*printf("i:%02d j:%03d Arabic char %#02x sc:%d scp:%d scn:%d\n", i, j, line_buffer[i], sc, scp, scn);*/
				switch (sc){
					case 0: /*Stand alone*/
						nl[j++] = line_buffer[i];
						break;
					case 1: /*Connect to previous*/
						if(scp == 2 || scp == 3)  /* previous char connects to next*/
							nl[j++] = shapes[si][2];
						else
							nl[j++] = line_buffer[i];
						break;
					case 2: /*Connect to next*/
						if(scn == 1 || scn == 3)
							nl[j++] = shapes[si][0];
						else
							nl[j++] = line_buffer[i];
						break;
					case 3: /*Connect to both*/
						if((scp == 2 || scp == 3) && (scn == 1 || scn == 3))
							nl[j++] = shapes[si][1];
						else if(scp == 2 || scp == 3)
							nl[j++] = shapes[si][2];
						else if(scn == 1 || scn == 3)
							nl[j++] = shapes[si][0];
						else
							nl[j++] = cc == 0xE7 ? shapes[si][2] : line_buffer[i];
						break;
				}
				if(cc == 0xFF){/*YAA with HAMZA at the end*/
					if(nl[j-1] == 0xFF){
						nl[j-1] = 0xE9;
						nl[j++] = 0xC1;
					}
					else if(nl[j-1] == 0xF5){
						nl[j++] = 0xC1;
					}
				}
			}
		}
		cc = nl[j-1];
		if(cc == 0xBC || cc == 0xBD || cc == 0xBE || cc == 0xEB){/*SEEN, SHEED, SAD & DAD at the end require closing char*/
			/*printf("i:%02d j:%02d SEEN, SHEED, SAD & DAD %#02x\n", i, j, line_buffer[i]);*/
			nl[j++] = 0x9F;
			if(nc == 0x20) i++;
		}

		if(line_buffer[i] == '\n') break;
	}

	for(i = 0; i < BUFSIZ; i++) {
		line_buffer[i] = nl[i];
		if(nl[i] == '\n') break;
	}
}

int is_arabic(unsigned char c){
	if((c >=0x99 && c <= 0xA2) || (c >=0xA5 && c <= 0xBF) || (c >=0xC1 && c <= 0xFF))
		return 1;
	else
		return 0;
}

int is_english(unsigned char c){
	if((c >=0x41 && c <= 0x5A) || (c >=0x61 && c <= 0x7A))
		return 1;
	else
		return 0;
}

int is_digit(unsigned char c){
	if((c >=0x30 && c <= 0x39))
		return 1;
	else
		return 0;
}

int is_number_seperator(unsigned char c){
	if((c ==0x2e || c == 0x2c))
		return 1;
	else
		return 0;
}

void reverse_order_number(int ixs, int ixe) {
	int i, ixc;
	unsigned char t;
	/*if(ixe <= ixs ) return;*/
	ixc = (int)((ixe - ixs) / 2.0f +0.5f);
	for(i = 0; i < ixc; i++) {
		t = line_buffer[ixs + i];
		line_buffer[ixs + i] = line_buffer[ixe - i];
		line_buffer[ixe - i] = t;
		if(numeralshapes == 1){
			if(is_digit(line_buffer[ixs + i])) line_buffer[ixs + i] += 0x80;
			if(is_digit(line_buffer[ixe - i])) line_buffer[ixe - i] += 0x80;
		}
	}
	/* Following statement is necessary to conver the number in the middle*/
	if(numeralshapes == 1){
		if(is_digit(line_buffer[ixs + i])) line_buffer[ixs + i] += 0x80;
	}
}

/* This function should only be called from reverse_order and as the last call*/
void correct_numbers(int ixs, int ixe){
	int i, nxs, nxe, mode;
	unsigned char cc;
	if(ixe <= ixs ) return;
	i = ixs;
	mode = 0;
	while (i <= ixe && line_buffer[i] != '\n'){
		cc = line_buffer[i];
		if(mode == 0){/* No digit found yet*/
			if(is_digit(cc) == 1) {
				nxs = i;
				mode = 1;
			}
		}
		else {
			if(is_digit(cc) == 0){
				if(is_number_seperator(cc) == 0){
					nxe = i - 1;
					reverse_order_number(nxs, nxe);
					mode = 0;
				}
				else {
					if(is_digit(line_buffer[i+1]) == 0){
						nxe = i - 1;
						reverse_order_number(nxs, nxe);
						mode = 0;
					}
				}
			}
		}
		i++;
	}
	if(mode ==1) {
		nxe = i - 1;
		reverse_order_number(nxs, nxe);
	}
}

void correct_brackets(int ixs, int ixe){
	int i;
	unsigned char cc;
	for(i = ixs; i<= ixe; i++) {
		cc = line_buffer[i];
		if(cc == 0x28) line_buffer[i] = 0x29;
		else if(cc == 0x29) line_buffer[i] = 0x28;
		else if(cc == 0x3C) line_buffer[i] = 0x3E;
		else if(cc == 0x3E) line_buffer[i] = 0x3C;
		else if(cc == 0x5B) line_buffer[i] = 0x5D;
		else if(cc == 0x5D) line_buffer[i] = 0x5B;
		else if(cc == 0x7B) line_buffer[i] = 0x7D;
		else if(cc == 0x7D) line_buffer[i] = 0x7B;
	}
}

void reverse_order(int ixs, int ixe) {
	int i, ixc;
	unsigned char t;
	if(ixe <= ixs ) return;
	ixc = (int)((ixe - ixs) / 2.0f +0.5f);
	for(i = 0; i < ixc; i++) {
		t = line_buffer[ixs + i];
		line_buffer[ixs + i] = line_buffer[ixe - i];
		line_buffer[ixe - i] = t;
	}
	correct_numbers(ixs, ixe);
	correct_brackets(ixs, ixe);
}

void conv_arabic_direction(void) {
	int ixs, ixe, i, mode;
	i = 0, mode = 0;
	while(i < sizeof(line_buffer) && line_buffer[i] != '\n') {
		if(mode == 0) { /*No arabic char found yet*/
			if(is_arabic(line_buffer[i]) == 1) {/*Arabic character found*/
				ixs = i;
				mode = 1;
			}
		}
		else {
			if(is_english(line_buffer[i]) == 1) {
				ixe = i - 1;
				if(line_buffer[ixe] == 0x20) ixe--;
				reverse_order(ixs, ixe);
				mode = 0;
			}
		}
		i++;
	}
	if(mode == 1){
		ixe = i - 1;
		reverse_order(ixs, ixe);
	}
}

int processLine(void) {
	int i, s;
	unsigned char lastc;
	
	conv_iso1256_ibm864();
	conv_arabic_glyph();
	conv_arabic_direction();
	/*
	s = sizeof(line_buffer);
	for(i = 0; i<s && lastc != '\n'; i++) {
		printf("%c", line_buffer[i]);
		lastc = line_buffer[i];
	}*/
	return 0;
}

int main(int argc, char *argv[]) {
	while(TRUE){
		if (fgets(line_buffer, BUFSIZ, stdin) == NULL){
			if (ferror(stdin) != 0){
			fprintf(stderr,"Error during reading\n");
			error_flag = 1;
		}
		if (feof(stdin) != 0)
			fprintf(stderr,"File ended\n");
		clearerr(stdin);
		break; /* exit the while loop */
	}
	else{
		lineno++;
		/* in the next line, "%3d" is used to restrict the
		   number to 3 digits.
		*/
		/*fprintf(stdout,"%3d: ", lineno);*/
		processLine();
		fputs(line_buffer, stdout);
    }
  }

  fprintf(stderr,"%d lines written\n", lineno);
  exit(error_flag);
}
