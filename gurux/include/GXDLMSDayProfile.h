//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMSDAYPROFILE_H
#define GXDLMSDAYPROFILE_H

#include <vector>
#include "GXDLMSDayProfileAction.h"

class CGXDLMSDayProfile
{
    short m_DayId;
    std::vector<CGXDLMSDayProfileAction*> m_DaySchedules;
public:
    /**
     Constructor.
    */
    CGXDLMSDayProfile();

    /**
     Constructor.
    */
    CGXDLMSDayProfile(short dayId, std::vector<CGXDLMSDayProfileAction*>& schedules);

    //Destructor.
    ~CGXDLMSDayProfile();


    /**
     User defined identifier, identifying the currentday_profile.
    */
    short GetDayId();
    void SetDayId(short value);

    std::vector<CGXDLMSDayProfileAction*>& GetDaySchedules();
    void SetDaySchedules(std::vector<CGXDLMSDayProfileAction*>& value);
    std::string ToString();
};

#endif //GXDLMSDAYPROFILE_H
