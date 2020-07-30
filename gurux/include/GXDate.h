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

#ifndef GXDATE_H
#define GXDATE_H

#include "GXDateTime.h"

/**
* Date object. Time part is ignored.
*/
class CGXDate : public CGXDateTime
{
public:
    // Constructor.
    CGXDate() : CGXDateTime()
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    };

    // Constructor.
    CGXDate(struct tm value) : CGXDateTime(value)
    {
        SetSkip((DATETIME_SKIPS)(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    }

    // Constructor.
    CGXDate(int year, int month, int day) :
        CGXDateTime(year, month, day, -1, -1, -1, -1)
    {
    }

    /**
    * Constructor.
    *
    * @param forvalue
    *            Date value.
    */
    /// <summary>
    /// Constructor.
    /// </summary>
    CGXDate(CGXDateTime& value) : CGXDateTime(value.GetValue())
    {
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
    }

    CGXDate& operator=(const CGXDateTime& value)
    {
        SetValue(value.m_Value);
        SetSkip((DATETIME_SKIPS)(value.m_Skip | DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS));
        return *this;
    }
};
#endif //GXDATE_H