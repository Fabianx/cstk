/***************************************************************************
                           sensordata.h  -  v.0.1
                           ----------------------
    begin                : Fri Feb 27 2004
    copyright            : (C) 2004 by Stuart Reeves, Kristof Van Laerhoven
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

#ifndef SENSORDATA_H
#define SENSORDATA_H

#include "cstk_base/types.h" 
#include "cstk_base/vector/datacell.h" 

#ifndef uint 
#define uint unsigned int
#endif

/** Interface base class for everything that provides the tools with 
    sensor data  

  *@authors: Stuart Reeves, Kristof Van Laerhoven

   // SensorData is used to read sensor values from this class' child,
   // and to return the number of successfully retrieved data elements.
   //
   // The original data is looks like one of the following rows:
   //
   //            -------------------------------------------------------
   //           | channel1  | channel 2 | channel 3 |  .... | channel n |
   //           |-----------|-----------|-----------|- ....-|-----------|
   // sample 1: |   val1_1  |   val2_1  |   val3_1  |  .... |   valn_1  |
   // sample 2: |   val1_2  |   val2_2  |   val3_2  |  .... |   valn_2  |
   // sample 3: |   val1_3  |   val2_3  |   val3_3  |  .... |   valn_3  |
   //  ...            ...         ...         ...       ...       ...
   //
   //  The first read function has the option to specify a filter for
   // leaving out certain channels (e.g., take only channels 2, 4 and 5)
   // and put them in columns.

*/

// abstract base class for all sensor data providers:
struct SensorData 
{  
   virtual ~SensorData() {};
   // read filtered:
    virtual int read(char* channel_types, uint numchannels, 
                     DataCell* columns,  uint* filter, uint numcolumns)=0;
   // read unfiltered:
    virtual int read(DataCell* channels, uint numchannels)=0;
   // 'automatic' reading:
    virtual int read(DataCell* channels, uint* numchannels)=0;
   // output to string:
    virtual int read(char *line)=0;
};

#endif
