/***************************************************************************
                         logfileparser.cpp  -  v.0.2
                             -------------------
    begin                : Fri Feb 27 2004
    copyright            : (C) 2004 by Stuart Reeves
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

#include "logfileparser.h"

/**Class for parsing various logfiles
  *@author Stuart Reeves
  */

LogFileParser::LogFileParser(char *filename) {
	int i=0;
	while (filename[i]!='\0') {
		this->file_param.filename[i] = filename[i];
		i++;
	}
	fp = NULL;
}

LogFileParser::LogFileParser(LogFileParserSettings file_param) {
	int i=0;
	while (file_param.filename[i]!='\0') {
		this->file_param.filename[i] = file_param.filename[i];
		i++;
	}
	this->file_param.mode = file_param.mode;
	fp = NULL;
}

LogFileParser::~LogFileParser() {
	closeFile();
}

void LogFileParser::closeFile() {
	if (fp != NULL)
		fclose(fp);
	//if (this->file_param.filename!=NULL)
	//	delete[] this->file_param.filename;
}

void LogFileParser::openFile() {
	if (fp == NULL) 
		fp = fopen(this->file_param.filename, "r");
}

int LogFileParser::read(char 	 *channel_types, 
			uint 	 numchannels, 
			DataCell *columns,
			uint 	 *filter,
			uint 	 numcolumns) {
	
	openFile();
	if (fp==NULL)
		return FILEERR_CANTOPEN;
	uint i = 0;
	char buf[BUFFER_MAX];
	if (fgets(buf, sizeof(buf), fp) == NULL)
		return FILEERR_CANTREAD;
	char *token = strtok(buf, COLUMN_SEPARATOR);
	while (token != NULL && i < numcolumns) {
		char *decimal = strchr(token, '.');
		if (decimal == NULL) {
			s_64b field; // Assume maximum size
			sscanf(token, "%lli", &field);
			uint col = 0;	
			// Map the sscanf types to the native CSTK types
			switch (channel_types[i]) {
				case U8B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_u8val((u_8b)field);
					columns[col].set_type(U8B_TYPE);
					break;
				case S8B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_s8val((s_8b)field);
					columns[col].set_type(S8B_TYPE);
					break;
				case U16B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_u16val((u_16b)field);
					columns[col].set_type(U16B_TYPE);
					break;
				case S16B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_s16val((s_16b)field);
					columns[col].set_type(S16B_TYPE);
					break;
				case U32B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_u32val((u_32b)field);
					columns[col].set_type(U32B_TYPE);
					break;
				case S32B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_s32val((s_32b)field);
					columns[col].set_type(S32B_TYPE);
					break;
				case U64B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_u64val((u_64b)field);
					columns[col].set_type(U64B_TYPE);
					break;
				case S64B_TYPE:
					col = filtrate(filter, numcolumns, i);
					columns[col].set_s64val((s_64b)field);
					columns[col].set_type(S64B_TYPE);
					break;
				default:
					// TODO: Something here?
					break;
			}
		} else {
			f_64b field; // Assume maximum size
			if (sscanf(token, "%lf", &field) || 
			    sscanf(token, "%le", &field) ||
			    sscanf(token, "%lg", &field)) {
				uint col = 0;
				// Map the sscanf types to the native CSTK types
				switch (channel_types[i]) {
					case F32B_TYPE:
						col = filtrate(filter, numcolumns, i);
						columns[col].set_f32val((f_32b)field);
						columns[col].set_type(F32B_TYPE);
						break;
					case F64B_TYPE:
						col = filtrate(filter, numcolumns, i);
						columns[col].set_f64val((f_64b)field);
						columns[col].set_type(F64B_TYPE);
						break;
					default:
						// TODO: Something here?
						break;
				}
			}
		}
		++i;
		token = strtok(NULL, COLUMN_SEPARATOR);
	}
	if (feof(fp))
		closeFile();
	return i;
}


uint LogFileParser::filtrate(uint *filter, uint numcolumns, uint channel) {

	for (uint i = 0; i < numcolumns; ++i) {
		if (filter[i] == channel) 
			return i;
	}
	return 0;
}


int LogFileParser::read(DataCell *channels, uint numchannels) {
	
	openFile();
	if (fp == NULL)
		return FILEERR_CANTOPEN;
	uint i = 0;
	char buf[BUFFER_MAX];
	if (fgets(buf, sizeof(buf), fp) == NULL)
		return FILEERR_CANTREAD;
	char *token = strtok(buf, COLUMN_SEPARATOR);
	while (token != NULL && i < numchannels) {
		
		parse_string(channels, i, token);
		++i;
		token = strtok(NULL, COLUMN_SEPARATOR);
		
	}
	if (feof(fp))
		closeFile();
	return i;
}


int LogFileParser::read(DataCell *channels, uint *numchannels) {
	// TODO
	return 0;
}


void LogFileParser::parse_string(DataCell *channels, uint column, char *str) {

	char *decimal = strchr(str, '.');
	channels[column].set_type(NULL_TYPE);
	// Determine if this column is a floating point value
	if (decimal == NULL) {
		s_64b field; // Assume maximum size
		// If we are unable to scan this field as a number, we store it as a 
		// string or char
		if(!sscanf(str, "%lli", &field)) {
			uint sLen = strlen(str);
			if (sLen > 1) {
				channels[column].set_stringval(str);
				channels[column].set_type(STRING_TYPE);
			} else if (sLen == 1) {
				channels[column].set_charval(str[0]);
				channels[column].set_type(CHAR_TYPE);
			}
			return;
		}
		// Map the sscanf types to the native CSTK types, for unsigned and
		// signed values
		if (field >= 0) {
			if (field <= u_8b_max) {
				channels[column].set_u8val((u_8b)field);
				channels[column].set_type(U8B_TYPE);
			} else if (field <= u_16b_max) {
				channels[column].set_u16val((u_16b)field);
				channels[column].set_type(U16B_TYPE);
			} else if ((unsigned)field <= u_32b_max) {
				channels[column].set_u32val((u_32b)field);
				channels[column].set_type(U32B_TYPE);
			} else {
				channels[column].set_u64val((u_64b)field);
				channels[column].set_type(U64B_TYPE);
			}
		} else {
			if (field >= s_8b_min) {
				channels[column].set_s8val((s_8b)field);
				channels[column].set_type(S8B_TYPE);
			} else if (field >= s_16b_min) {
				channels[column].set_s16val((s_16b)field);
				channels[column].set_type(S16B_TYPE);
			} else if (field >= s_32b_min) {
				channels[column].set_s32val((s_32b)field);
				channels[column].set_type(S32B_TYPE);
			} else {
				channels[column].set_s64val((s_64b)field);
				channels[column].set_type(S64B_TYPE);
			}
		}
	} else {
		f_64b field; // Assume maximum size
		if (sscanf(str, "%lf", &field) || 
			sscanf(str, "%le", &field) ||
			sscanf(str, "%lg", &field)) {
		
			int dig = sizeof(decimal);
			
			// Map the sscanf types to the native CSTK types
			if (dig <= f_32b_dig) {
				channels[column].set_f32val((f_32b)field);
				channels[column].set_type(F32B_TYPE);
			} else if (dig <= f_64b_dig) {
				channels[column].set_f64val((f_64b)field);
				channels[column].set_type(F64B_TYPE);
			}
		}
	}
}


int LogFileParser::read(char *line) {
	openFile();
	if (fp == NULL)
		return FILEERR_CANTOPEN;

	char buf[BUFFER_MAX];
	if (fgets(buf, sizeof(buf), fp) == NULL)
		return FILEERR_CANTREAD;
	else {
		strcpy(line, buf);
		if (feof(fp))
			closeFile();
		return sizeof(buf);
	}
}

