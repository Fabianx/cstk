/***************************************************************************
                          conio.h  -  v.1.00
                         --------------------
    begin                : Nov 14 2004
    copyright            : (C) 2004 by Kristof Van Laerhoven
    email                : 
 ***************************************************************************/
                                                                             
/*************************************************************************** 
 *                                                                         * 
 *   This program is free software; you can redistribute it and/or modify  * 
 *   it under the terms of the GNU General Public License as published by  * 
 *   the Free Software Foundation; either version 2 of the License, or     * 
 *   (at your option) any later version.                                   * 
 *                                                                         * 
 ***************************************************************************/

#include <termios.h>
#include <unistd.h>

class ConIO {
 public:
	ConIO() {
		err = 0;
		char ret = tcgetattr(0, &termattr);
		if (ret < 0) err= -1;  
		save_termattr = termattr;  
		termattr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		termattr.c_iflag &= ~(BRKINT | INLCR | INPCK | ISTRIP | IXON );
		termattr.c_cflag &= ~(CSIZE | PARENB);
		termattr.c_cflag |= CS8;
		//termattr.c_oflag &= ~(OPOST);   //<- leaves nl !
		termattr.c_cc[VMIN] = 0;  
		termattr.c_cc[VTIME] = 0;
		ret = tcsetattr (0, TCSANOW, &termattr);
		if (ret < 0) err= -1;   
	}
	~ConIO() {
		tcsetattr (0, TCSAFLUSH, &save_termattr);
	}
	char kb_getc(void) {
		char ch=0;
		read(0, &ch, 1);
		return ch;
	}
	int err;
 private:
	struct termios termattr;
	struct termios save_termattr;
};

