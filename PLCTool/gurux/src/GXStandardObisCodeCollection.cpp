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
#include "../include/GXStandardObisCodeCollection.h"

int CGXStandardObisCodeCollection::GetUInt8(std::string ln, unsigned char* bytes)
{
    std::vector< std::string > tmp = GXHelpers::Split(ln, '.');
    if (tmp.size() != 6)
    {
        //Invalid OBIS Code.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int value;
#if _MSC_VER > 1000
    sscanf_s(tmp[0].c_str(), "%d", &value);
#else
    sscanf(tmp[0].c_str(), "%d", &value);
#endif
    bytes[0] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[1].c_str(), "%d", &value);
#else
    sscanf(tmp[1].c_str(), "%d", &value);
#endif
    bytes[1] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[2].c_str(), "%d", &value);
#else
    sscanf(tmp[2].c_str(), "%d", &value);
#endif
    bytes[2] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[3].c_str(), "%d", &value);
#else
    sscanf(tmp[3].c_str(), "%d", &value);
#endif
    bytes[3] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[4].c_str(), "%d", &value);
#else
    sscanf(tmp[4].c_str(), "%d", &value);
#endif
    bytes[4] = value;
#if _MSC_VER > 1000
    sscanf_s(tmp[5].c_str(), "%d", &value);
#else
    sscanf(tmp[5].c_str(), "%d", &value);
#endif
    bytes[5] = value;
    return DLMS_ERROR_CODE_OK;
}

bool CGXStandardObisCodeCollection::EqualsInterface(CGXStandardObisCode item, int ic)
{
    //If all interfaces are allowed.
    if (item.GetInterfaces() == "*")
    {
        return true;
    }
    char type[10];
#if _MSC_VER > 1000
    sprintf_s(type, 10, "%d", ic);
#else
    sprintf(type, "%d", ic);
#endif
    std::vector< std::string > tmp = GXHelpers::Split(item.GetInterfaces(), ',');
    for (std::vector< std::string >::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        if (*it == type)
        {
            return true;
        }
    }
    return false;
}

// Check OBIS codes.
bool CGXStandardObisCodeCollection::EqualsMask(std::string obis, int ic)
{
    bool number = true;
    int value;
    if (obis.find(',') != std::string::npos)
    {
        std::vector< std::string > tmp = GXHelpers::Split(obis, ',');
        //for (std::string it : tmp)
        for (std::vector< std::string >::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).find('-') != std::string::npos)
            {
                if (EqualsMask(*it, ic))
                {
                    return true;
                }
            }
            else
            {
#if _MSC_VER > 1000
                sscanf_s((*it).c_str(), "%d", &value);
#else
                sscanf((*it).c_str(), "%d", &value);
#endif
                if (value == ic)
                {
                    return true;
                }
            }
        }
        return false;
    }
    else if (obis.find('-') != std::string::npos)
    {
        number = false;
        std::vector< std::string > tmp = GXHelpers::Split(obis, '-');
        int value1, value2;
#if _MSC_VER > 1000
        sscanf_s(tmp[0].c_str(), "%d", &value1);
        sscanf_s(tmp[1].c_str(), "%d", &value2);
#else
        sscanf(tmp[0].c_str(), "%d", &value1);
        sscanf(tmp[1].c_str(), "%d", &value2);
#endif
        return ic >= value1 && ic <= value2;
    }
    if (number)
    {
        if (obis == "&")
        {
            return ic == 0 || ic == 1 || ic == 7;
        }
#if _MSC_VER > 1000
        sscanf_s(obis.c_str(), "%d", &value);
#else
        sscanf(obis.c_str(), "%d", &value);
#endif
        return value == ic;
    }
    return false;
}

// Check OBIS code.
bool CGXStandardObisCodeCollection::EqualsObisCode(std::vector< std::string >& obisMask, unsigned char* ic)
{
    if (!EqualsMask(obisMask[0], ic[0]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[1], ic[1]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[2], ic[2]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[3], ic[3]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[4], ic[4]))
    {
        return false;
    }
    if (!EqualsMask(obisMask[5], ic[5]))
    {
        return false;
    }
    return true;
}

// Get description.
std::string CGXStandardObisCodeCollection::GetDescription(std::string& str)
{
    str = GXHelpers::trim(str);
    if (str.size() == 0 || str[0] != '$')
    {
        return "";
    }
    int value;
#if _MSC_VER > 1000
    sscanf_s(str.substr(1).c_str(), "%d", &value);
#else
    sscanf(str.substr(1).c_str(), "%d", &value);
#endif
    switch (value)
    {
    case 1:
        return "Sum Li Active power+ (QI+QIV)";
    case 2:
        return "Sum Li Active power- (QII+QIII)";
    case 3:
        return "Sum Li Reactive power+ (QI+QII)";
    case 4:
        return "Sum Li Reactive power- (QIII+QIV)";
    case 5:
        return "Sum Li Reactive power QI";
    case 6:
        return "Sum Li Reactive power QII";
    case 7:
        return "Sum Li Reactive power QIII";
    case 8:
        return "Sum Li Reactive power QIV";
    case 9:
        return "Sum Li Apparent power+ (QI+QIV)";
    case 10:
        return "Sum Li Apparent power- (QII+QIII)";
    case 11:
        return "Current: any phase";
    case 12:
        return "Voltage: any phase";
    case 13:
        return "Sum Li Power factor";
    case 14:
        return "Supply frequency";
    case 15:
        return "Sum LI Active power (abs(QI+QIV)+abs(QII+QIII))";
    case 16:
        return "Sum LI Active power        (abs(QI+QIV)-abs(QII+QIII))";
    case 17:
        return "Sum Li Active power QI";
    case 18:
        return "Sum Li Active power QII";
    case 19:
        return "Sum Li Active power QIII";
    case 20:
        return "Sum Li Active power QIV";
    case 21:
        return "L1 Active power+ (QI+QIV)";
    case 22:
        return "L1 Active power- (QII+QIII)";
    case 23:
        return "L1 Reactive power+ (QI+QII)";
    case 24:
        return "L1 Reactive power- (QIII+QIV)";
    case 25:
        return "L1 Reactive power QI";
    case 26:
        return "L1 Reactive power QII";
    case 27:
        return "L1 Reactive power QIII";
    case 28:
        return "L1 Reactive power QIV";
    case 29:
        return "L1 Apparent power+ (QI+QIV)";
    case 30:
        return "L1 Apparent power- (QII+QIII)";
    case 31:
        return "L1 Current";
    case 32:
        return "L1 Voltage";
    case 33:
        return "L1 Power factor";
    case 34:
        return "L1 Supply frequency";
    case 35:
        return "L1 Active power (abs(QI+QIV)+abs(QII+QIII))";
    case 36:
        return "L1 Active power (abs(QI+QIV)-abs(QII+QIII))";
    case 37:
        return "L1 Active power QI";
    case 38:
        return "L1 Active power QII";
    case 39:
        return "L1 Active power QIII";
    case 40:
        return "L1 Active power QIV";
    case 41:
        return "L2 Active power+ (QI+QIV)";
    case 42:
        return "L2 Active power- (QII+QIII)";
    case 43:
        return "L2 Reactive power+ (QI+QII)";
    case 44:
        return "L2 Reactive power- (QIII+QIV)";
    case 45:
        return "L2 Reactive power QI";
    case 46:
        return "L2 Reactive power QII";
    case 47:
        return "L2 Reactive power QIII";
    case 48:
        return "L2 Reactive power QIV";
    case 49:
        return "L2 Apparent power+ (QI+QIV)";
    case 50:
        return "L2 Apparent power- (QII+QIII)";
    case 51:
        return "L2 Current";
    case 52:
        return "L2 Voltage";
    case 53:
        return "L2 Power factor";
    case 54:
        return "L2 Supply frequency";
    case 55:
        return "L2 Active power (abs(QI+QIV)+abs(QII+QIII))";
    case 56:
        return "L2 Active power (abs(QI+QIV)-abs(QI+QIII))";
    case 57:
        return "L2 Active power QI";
    case 58:
        return "L2 Active power QII";
    case 59:
        return "L2 Active power QIII";
    case 60:
        return "L2 Active power QIV";
    case 61:
        return "L3 Active power+ (QI+QIV)";
    case 62:
        return "L3 Active power- (QII+QIII)";
    case 63:
        return "L3 Reactive power+ (QI+QII)";
    case 64:
        return "L3 Reactive power- (QIII+QIV)";
    case 65:
        return "L3 Reactive power QI";
    case 66:
        return "L3 Reactive power QII";
    case 67:
        return "L3 Reactive power QIII";
    case 68:
        return "L3 Reactive power QIV";
    case 69:
        return "L3 Apparent power+ (QI+QIV)";
    case 70:
        return "L3 Apparent power- (QII+QIII)";
    case 71:
        return "L3 Current";
    case 72:
        return "L3 Voltage";
    case 73:
        return "L3 Power factor";
    case 74:
        return "L3 Supply frequency";
    case 75:
        return "L3 Active power (abs(QI+QIV)+abs(QII+QIII))";
    case 76:
        return "L3 Active power (abs(QI+QIV)-abs(QI+QIII))";
    case 77:
        return "L3 Active power QI";
    case 78:
        return "L3 Active power QII";
    case 79:
        return "L3 Active power QIII";
    case 80:
        return "L3 Active power QIV";
    case 82:
        return "Unitless quantities (pulses or pieces)";
    case 84:
        return "Sum Li Power factor-";
    case 85:
        return "L1 Power factor-";
    case 86:
        return "L2 Power factor-";
    case 87:
        return "L3 Power factor-";
    case 88:
        return "Sum Li A2h QI+QII+QIII+QIV";
    case 89:
        return "Sum Li V2h QI+QII+QIII+QIV";
    case 90:
        return "SLi current (algebraic sum of the – unsigned – value of the currents in all phases)";
    case 91:
        return "Lo Current (neutral)";
    case 92:
        return "Lo Voltage (neutral)";
    }
    return "";
}


bool CGXStandardObisCodeCollection::Find(std::string ln, DLMS_OBJECT_TYPE objectType, CGXStandardObisCode& item)
{
    unsigned char bytes[6];
    if (GetUInt8(ln, bytes) != DLMS_ERROR_CODE_OK)
    {
        return NULL;
    }
    return Find(bytes, objectType, item);
}

bool CGXStandardObisCodeCollection::EqualsMask(std::string obisMask, std::string ln)
{
    unsigned char bytes[6];
    if (GetUInt8(ln, bytes) != DLMS_ERROR_CODE_OK)
    {
        return NULL;
    }
    std::vector< std::string > tmp = GXHelpers::Split(obisMask, '.');
    return EqualsObisCode(tmp, bytes);
}

// Find Standard OBIS Code description.
bool CGXStandardObisCodeCollection::Find(unsigned char* pObisCode, int IC, CGXStandardObisCode& tmp)
{
    char buff[6];
    for (std::vector<CGXStandardObisCode>::iterator it = this->begin(); it != this->end(); ++it)
    {
        //Interface is tested first because it's faster.
        if (EqualsInterface(*it, IC) && EqualsObisCode((*it).GetOBIS(), pObisCode))
        {
            tmp.SetOBIS((*it).GetOBIS());
            tmp.SetDescription((*it).GetDescription());
            tmp.SetInterfaces((*it).GetInterfaces());
            tmp.SetDataType((*it).GetDataType());
            std::vector< std::string > tmp2 = GXHelpers::Split((*it).GetDescription(), ';');
            if (tmp2.size() > 1)
            {
                std::string desc = GetDescription(tmp2[1]);
                if (desc != "")
                {
                    tmp2[1] = desc;
                    std::string builder;
                    for (std::vector< std::string >::iterator s = tmp2.begin(); s != tmp2.end(); ++s)
                    {
                        if (builder.size() != 0)
                        {
                            builder.append(";");
                        }
                        builder.append(*s);
                    }
                    tmp.SetDescription(builder);
                }
            }
            std::vector< std::string > obis = tmp.GetOBIS();
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[0]);
#else
            sprintf(buff, "%d", pObisCode[0]);
#endif
            obis[0] = buff;
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[1]);
#else
            sprintf(buff, "%d", pObisCode[1]);
#endif

            obis[1] = buff;
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[2]);
#else
            sprintf(buff, "%d", pObisCode[2]);
#endif

            obis[2] = buff;
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[3]);
#else
            sprintf(buff, "%d", pObisCode[3]);
#endif

            obis[3] = buff;
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[4]);
#else
            sprintf(buff, "%d", pObisCode[4]);
#endif

            obis[4] = buff;
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[5]);
#else
            sprintf(buff, "%d", pObisCode[5]);
#endif

            obis[5] = buff;
            tmp.SetOBIS(obis);
            std::string desc = tmp.GetDescription();
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[0]);
#else
            sprintf(buff, "%d", pObisCode[0]);
#endif
            GXHelpers::Replace(desc, "$A", buff);
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[1]);
#else
            sprintf(buff, "%d", pObisCode[1]);
#endif

            GXHelpers::Replace(desc, "$B", buff);
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[2]);
#else
            sprintf(buff, "%d", pObisCode[2]);
#endif

            GXHelpers::Replace(desc, "$C", buff);
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[3]);
#else
            sprintf(buff, "%d", pObisCode[3]);
#endif

            GXHelpers::Replace(desc, "$D", buff);
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[4]);
#else
            sprintf(buff, "%d", pObisCode[4]);
#endif

            GXHelpers::Replace(desc, "$E", buff);
#if _MSC_VER > 1000
            sprintf_s(buff, 6, "%d", pObisCode[5]);
#else
            sprintf(buff, "%d", pObisCode[5]);
#endif
            GXHelpers::Replace(desc, "$F", buff);
            //Increase value
            int begin = (int)desc.find("#$");
            if (begin != -1)
            {
                int start = (int)desc.find('(');
                int end = (int)desc.find(')');
                char channel = desc[start + 1];
                int ch = 0;
                if (channel == 'A')
                {
                    ch = pObisCode[0];
                }
                else if (channel == 'B')
                {
                    ch = pObisCode[1];
                }
                else if (channel == 'C')
                {
                    ch = pObisCode[2];
                }
                else if (channel == 'D')
                {
                    ch = pObisCode[3];
                }
                else if (channel == 'E')
                {
                    ch = pObisCode[4];
                }
                else if (channel == 'F')
                {
                    ch = pObisCode[5];
                }
                int plus = (int)desc.find('+');
                if (plus != -1)
                {
                    int value;
#if _MSC_VER > 1000
                    sscanf_s(desc.substr(plus + 1, plus + 1 + end - plus - 1).c_str(), "%d", &value);
#else
                    sscanf(desc.substr(plus + 1, plus + 1 + end - plus - 1).c_str(), "%d", &value);
#endif
                    ch += value;
                }
#if _MSC_VER > 1000
                sprintf_s(buff, 6, "%d", ch);
#else
                sprintf(buff, "%d", ch);
#endif
                desc = desc.substr(0, begin).append(buff);
            }
            GXHelpers::Replace(desc, ";", " ");
            GXHelpers::Replace(desc, "  ", " ");
            GXHelpers::rtrim(desc);
            tmp.SetDescription(desc);
            return true;
        }
    }
    tmp.SetDescription("Manufacturer specific");
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", IC);
#else
    sprintf(buff, "%d", IC);
#endif
    tmp.SetInterfaces(buff);
    tmp.SetDataType("");
    std::vector <std::string > obis;
    //Pois
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[0]);
#else
    sprintf(buff, "%d", pObisCode[0]);
#endif
    obis.push_back(buff);
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[1]);
#else
    sprintf(buff, "%d", pObisCode[1]);
#endif
    obis.push_back(buff);
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[2]);
#else
    sprintf(buff, "%d", pObisCode[2]);
#endif

    obis.push_back(buff);
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[3]);
#else
    sprintf(buff, "%d", pObisCode[3]);
#endif
    obis.push_back(buff);
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[4]);
#else
    sprintf(buff, "%d", pObisCode[4]);
#endif

    obis.push_back(buff);
#if _MSC_VER > 1000
    sprintf_s(buff, 6, "%d", pObisCode[5]);
#else
    sprintf(buff, "%d", pObisCode[5]);
#endif

    obis.push_back(buff);
    tmp.SetOBIS(obis);
    return true;
}
