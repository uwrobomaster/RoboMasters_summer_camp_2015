/*
 * DJI_Pro_App.cpp
 *
 *  Created on: July 25, 2015
 *      Author: Zeus Zhou
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <stdlib.h>

#include "sdk_lib/Tan_Display.h"
#include "sdk_lib/DJI_Pro_Test.h"
#include "sdk_lib/DJI_Control.h"
#include "sdk_lib/CV_header.h"
///////////////////////////////////onboard SDK/////////////////////////////////////////////////////



int main(int argc,char **argv)
{

    //set all_velocity 0

    Set_Control_Param();

    CV_Init() ;

    Display_Menu();
    while(1)
    {
        Choose_Menu();
    }

    return -1;
}
