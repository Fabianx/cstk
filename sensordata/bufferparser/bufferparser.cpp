/***************************************************************************
                           bufferparser.cpp  -  v.0.1
                           --------------------------
    begin                : Thu Jan 13 2005
    copyright            : (C) 2005 by Kristof Van Laerhoven
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

#include "bufferparser.h" 

/** class for parsing a buffer containing either ascii data or binary data

  *@authors: Kristof Van Laerhoven
*/

int BufferParser::parse_bin(char *buf, uint n, char* channel_types, 
         uint numchannels, DataCell* columns,  uint* filter, uint numcolumns)
{
	unsigned int i=0;  // counter for the buffer array
	unsigned int j=0;  // counter for the datacells
	unsigned int k=0;  // temp counter
	
	while ((i<n)&&(j<numchannels)) {
		switch(channel_types[j]) {
			case U8B_TYPE: 
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_u8val((unsigned char)buf[i]);
				i++;    
				break;
			case S8B_TYPE: 
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_s8val((unsigned char)buf[i]);
				i++;    
				break;
			case U16B_TYPE:    
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_u16val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1]);
				i+=2;
				break;
			case S16B_TYPE:
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_s16val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1]);
				i+=2;
				break;
			case U32B_TYPE:    
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_u32val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1],
				                      (unsigned char)buf[i+2],
				                      (unsigned char)buf[i+3]);
				i+=4;
				break;
			case S32B_TYPE:    
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_s32val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1],
				                      (unsigned char)buf[i+2],
				                      (unsigned char)buf[i+3]);
				i+=4;
				break;
			#ifdef U64
			case U64B_TYPE:
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_u64val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1],
				                      (unsigned char)buf[i+2],
				                      (unsigned char)buf[i+3],
				                      (unsigned char)buf[i+4],
				                      (unsigned char)buf[i+5],
				                      (unsigned char)buf[i+6],
				                      (unsigned char)buf[i+7]);
				i+=8;
				break;
			#endif
			#ifdef S64
			case S64B_TYPE:
				k = filtrate(filter, numcolumns, j);	
				columns[k].set_s64val((unsigned char)buf[i],
				                      (unsigned char)buf[i+1],
				                      (unsigned char)buf[i+2],
				                      (unsigned char)buf[i+3],
				                      (unsigned char)buf[i+4],
				                      (unsigned char)buf[i+5],
				                      (unsigned char)buf[i+6],
				                      (unsigned char)buf[i+7]);
				i+=8;
				break;
			#endif
			default: // errormsg
				return -1;
				break;    
		}//switch
		j++;
	}//while
	return j;
}

int BufferParser::parse_asc(char *buf, char* channel_types, 
         uint numchannels, DataCell* columns,  uint* filter, uint numcolumns)
{
	uint i=0;
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
					return -1; // invalid type
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
						return -1; // invalid type
				}
			}
		}
		++i;
		token = strtok(NULL, COLUMN_SEPARATOR);
	}
	return i;
}

uint BufferParser::filtrate(uint *filter, uint numcolumns, uint channel) 
{
	for (uint i = 0; i < numcolumns; ++i) {
		if (filter[i] == channel) 
			return i;
	}
	return 0;
}
