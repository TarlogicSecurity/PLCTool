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

#ifndef GXSERIALNUMBERCOUNTER_H
#define GXSERIALNUMBERCOUNTER_H

#include <vector>
#include <string>

class CGXSerialNumberCounter
{
private:
    /**
    * Constructor.
    */
    CGXSerialNumberCounter()
    {

    }

    /**
    * Produce formatted String by the given math expression.
    *
    * @param expression
    *            Unformatted math expression.
    * @return Formatted math expression.
    */
    static int FormatString(std::string& expression, std::string& value);
    /*
     * Get values to count together.
     */
    static int GetValues(std::string& expressions, std::vector<std::string>& values);

    static int GetValue(std::string& value, int sn);
public:


    /// <summary>
    /// Count serial number using formula.
    /// </summary>
    /// <param name="sn">Serial number</param>
    /// <param name="formula">Formula to used.</param>
    /// <returns></returns>
    static int Count(unsigned long sn, const char* formula);
};

#endif //GXSERIALNUMBERCOUNTER_H