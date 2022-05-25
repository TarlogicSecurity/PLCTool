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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/errorcodes.h"
#include "../include/GXBytebuffer.h"
#include "../include/GXHelpers.h"

//Constructor.
CGXByteBuffer::CGXByteBuffer()
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
}

//Constructor.
CGXByteBuffer::CGXByteBuffer(int capacity)
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
    Capacity(capacity);
}

//Copy constructor.
CGXByteBuffer::CGXByteBuffer(const CGXByteBuffer& value)
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
    if (value.m_Size - value.m_Position != 0)
    {
        Set(value.m_Data + value.m_Position, value.m_Size - value.m_Position);
    }
}

//Destructor.
CGXByteBuffer::~CGXByteBuffer()
{
    Clear();
}

unsigned long CGXByteBuffer::Available()
{
    return m_Size - m_Position;
}

unsigned long CGXByteBuffer::GetSize()
{
    return m_Size;
}

void CGXByteBuffer::SetSize(unsigned long value)
{
    assert(!(value > m_Capacity));
    m_Size = value;
    if (m_Position > m_Size)
    {
        m_Position = m_Size;
    }
}

int CGXByteBuffer::IncreaseSize(unsigned long size)
{
    if (size > 1)
    {
        return -1;
    }
    m_Size += size;
    return m_Size;
}


//Returns position of the buffer.
unsigned long CGXByteBuffer::GetPosition()
{
    return m_Position;
}

//Sets position of the buffer.
void CGXByteBuffer::SetPosition(unsigned long value)
{
    //assert(!(value > m_Size));
    m_Position = value;
}

// Allocate new size for the array in bytes.
int CGXByteBuffer::Capacity(unsigned long capacity)
{
    m_Capacity = capacity;
    if (capacity == 0)
    {
        if (m_Data != NULL)
        {
            free(m_Data);
            m_Data = NULL;
        }
        m_Size = 0;
        m_Position = 0;
    }
    else
    {
        unsigned char* tmp;
        if (m_Data == NULL)
        {
            tmp = (unsigned char*)malloc(m_Capacity);
        }
        else
        {
            tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        }
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
        if (m_Size > capacity)
        {
            m_Size = capacity;
        }
    }
    return 0;
}

unsigned long CGXByteBuffer::Capacity()
{
    return m_Capacity;
}

// Fill buffer it with zeros.
void CGXByteBuffer::Zero(unsigned long index, unsigned long count)
{
    if (index + count > m_Capacity)
    {
        CGXByteBuffer::Capacity(index + count);
    }
    if (m_Size < index + count)
    {
        m_Size = index + count;
    }
    memset(m_Data + index, 0, count);
}

// Push new data to the byteArray.
int CGXByteBuffer::SetUInt8(unsigned char item)
{
    int ret = SetUInt8(m_Size, item);
    if (ret == 0)
    {
        ++m_Size;
    }
    return ret;
}

int CGXByteBuffer::SetUInt8(unsigned long index, unsigned char item)
{
    if (m_Capacity == 0 || index + 1 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[index] = item;
    return 0;
}

int CGXByteBuffer::SetUInt16(unsigned short item)
{

    if (m_Capacity == 0 || m_Size + 2 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = (item >> 8) & 0xFF;
    m_Data[m_Size + 1] = item & 0xFF;
    m_Size += 2;
    return 0;
}

int CGXByteBuffer::SetUInt32(unsigned long item)
{
    int ret = CGXByteBuffer::SetUInt32ByIndex(m_Size, item);
    if (ret == 0)
    {
        m_Size += 4;
    }
    return ret;
}

int CGXByteBuffer::SetUInt32ByIndex(unsigned long index, unsigned long item)
{
    if (m_Capacity == 0 || index + 4 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[index] = (item >> 24) & 0xFF;
    m_Data[index + 1] = (item >> 16) & 0xFF;
    m_Data[index + 2] = (item >> 8) & 0xFF;
    m_Data[index + 3] = item & 0xFF;
    return 0;
}

int CGXByteBuffer::SetUInt64(unsigned long long item)
{
    if (m_Capacity == 0 || m_Size + 8 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = (unsigned char)((item >> 56) & 0xFF);
    m_Data[m_Size + 1] = (item >> 48) & 0xFF;
    m_Data[m_Size + 2] = (item >> 40) & 0xFF;
    m_Data[m_Size + 3] = (item >> 32) & 0xFF;
    m_Data[m_Size + 4] = (item >> 24) & 0xFF;
    m_Data[m_Size + 5] = (item >> 16) & 0xFF;
    m_Data[m_Size + 6] = (item >> 8) & 0xFF;
    m_Data[m_Size + 7] = item & 0xFF;
    m_Size += 8;
    return 0;
}

int CGXByteBuffer::SetFloat(float value)
{
    typedef union
    {
        float value;
        char b[sizeof(float)];
    } HELPER;

    HELPER tmp;
    tmp.value = value;
    if (m_Capacity == 0 || m_Size + 4 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = tmp.b[3];
    m_Data[m_Size + 1] = tmp.b[2];
    m_Data[m_Size + 2] = tmp.b[1];
    m_Data[m_Size + 3] = tmp.b[0];
    m_Size += 4;
    return 0;
}

int CGXByteBuffer::SetDouble(double value)
{
    typedef union
    {
        double value;
        char b[sizeof(double)];
    } HELPER;

    HELPER tmp;
    tmp.value = value;
    if (m_Capacity == 0 || m_Size + 8 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = tmp.b[7];
    m_Data[m_Size + 1] = tmp.b[6];
    m_Data[m_Size + 2] = tmp.b[5];
    m_Data[m_Size + 3] = tmp.b[4];
    m_Data[m_Size + 4] = tmp.b[3];
    m_Data[m_Size + 5] = tmp.b[2];
    m_Data[m_Size + 6] = tmp.b[1];
    m_Data[m_Size + 7] = tmp.b[0];
    m_Size += 8;
    return 0;
}

int CGXByteBuffer::SetInt8(char item)
{
    return CGXByteBuffer::SetUInt8((unsigned char)item);
}

int CGXByteBuffer::SetInt16(short item)
{
    return CGXByteBuffer::SetUInt16((unsigned short)item);
}

int CGXByteBuffer::SetInt32(long item)
{
    return CGXByteBuffer::SetUInt32((unsigned long)item);
}

int CGXByteBuffer::SetInt64(long long item)
{
    return CGXByteBuffer::SetUInt64((unsigned long long) item);
}

int CGXByteBuffer::Set(const void* pSource, unsigned long count)
{
    if (pSource != NULL && count != 0)
    {
        if (m_Size + count > m_Capacity)
        {
            //First time data is reserved only for the added data.
            if (m_Capacity == 0)
            {
                m_Capacity = count;
            }
            else
            {
                m_Capacity += count + VECTOR_CAPACITY;
            }
            unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
            //If not enought memory available.
            if (tmp == NULL)
            {
                return DLMS_ERROR_CODE_OUTOFMEMORY;
            }
            m_Data = tmp;
        }
        memcpy(m_Data + m_Size, pSource, count);
        m_Size += count;
    }
    return 0;
}

int CGXByteBuffer::Set(CGXByteBuffer* data, unsigned long index, unsigned long count)
{
    if (data != NULL)
    {
        if (count == (unsigned long)-1)
        {
            count = data->m_Size - index;
        }
        int ret = CGXByteBuffer::Set(data->m_Data + index, count);
        if (ret == 0)
        {
            data->m_Position += count;
        }
    }
    return 0;
}

int CGXByteBuffer::AddString(const char* value)
{
    if (value != NULL)
    {
        unsigned long len = (unsigned long)strlen(value);
        return CGXByteBuffer::Set(value, len);
    }
    return 0;
}

int CGXByteBuffer::AddString(const std::string& value)
{
    return CGXByteBuffer::Set(value.c_str(), (unsigned long)value.length());
}

int CGXByteBuffer::AttachString(char* value)
{
    unsigned long len = (unsigned long)strlen(value);
    int ret = CGXByteBuffer::Set(value, len);
    if (ret == 0)
    {
        free(value);
    }
    return ret;
}

void CGXByteBuffer::Clear()
{
    Capacity(0);
}

int CGXByteBuffer::GetUInt8(unsigned char* value)
{
    if (m_Position >= m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position];
    ++m_Position;
    return 0;
}

int CGXByteBuffer::GetUInt8(unsigned long index, unsigned char* value)
{
    if (index >= m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index];
    return 0;
}


int CGXByteBuffer::GetUInt16(unsigned short* value)
{
    if (m_Position + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = (((m_Data[m_Position] & 0xFF) << 8) | (m_Data[m_Position + 1] & 0xFF));
    m_Position += 2;
    return 0;
}

int CGXByteBuffer::GetUInt32(unsigned long* value)
{
    if (m_Position + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position] << 24 |
        m_Data[m_Position + 1] << 16 |
        m_Data[m_Position + 2] << 8 |
        m_Data[m_Position + 3];
    m_Position += 4;
    return 0;
}

int CGXByteBuffer::GetInt16(short* value)
{
    if (m_Position + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position] << 8 |
        m_Data[m_Position + 1];
    m_Position += 2;
    return 0;
}

int CGXByteBuffer::GetInt32(long* value)
{
    int ret = GetUInt32(m_Position, (unsigned long*)value);
    m_Position += 4;
    return ret;
}

int CGXByteBuffer::GetUInt32(unsigned long index, unsigned long* value)
{
    if (index + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index] << 24 |
        m_Data[index + 1] << 16 |
        m_Data[index + 2] << 8 |
        m_Data[index + 3];
    return 0;
}

int CGXByteBuffer::GetInt64(long long* value)
{
    int ret = CGXByteBuffer::GetUInt64(m_Position, (unsigned long long*) value);
    if (ret == 0)
    {
        m_Position += 8;
    }
    return ret;
}

int CGXByteBuffer::GetUInt64(unsigned long long* value)
{
    int ret = CGXByteBuffer::GetUInt64(m_Position, value);
    if (ret == 0)
    {
        m_Position += 8;
    }
    return ret;
}

int CGXByteBuffer::GetUInt64(unsigned long index, unsigned long long* value)
{
    if (index + 8 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = (unsigned long long)m_Data[index] << 56 |
        (unsigned long long) m_Data[index + 1] << 48 |
        (unsigned long long) m_Data[index + 2] << 40 |
        (unsigned long long) m_Data[index + 3] << 32 |
        (unsigned long long) m_Data[index + 4] << 24 |
        (unsigned long long) m_Data[index + 5] << 16 |
        (unsigned long long) m_Data[index + 6] << 8 |
        (unsigned long long) m_Data[index + 7];
    return 0;
}

int CGXByteBuffer::GetUInt128(unsigned long index, unsigned char* value)
{
    int ret = CGXByteBuffer::GetUInt32(index, (unsigned long*)value);
    if (ret == 0)
    {
        ret = CGXByteBuffer::GetUInt32(index + 4, (unsigned long*)value + 1);
        if (ret == 0)
        {
            ret = CGXByteBuffer::GetUInt32(index + 8, (unsigned long*)value + 2);
            if (ret == 0)
            {
                ret = CGXByteBuffer::GetUInt32(index + 12, (unsigned long*)value + 3);
            }
        }
    }
    return ret;
}

int CGXByteBuffer::GetFloat(float* value)
{
    if (m_Position + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    typedef union
    {
        float value;
        char b[sizeof(float)];
    } HELPER;
    HELPER tmp;
    tmp.b[3] = m_Data[m_Position];
    tmp.b[2] = m_Data[m_Position + 1];
    tmp.b[1] = m_Data[m_Position + 2];
    tmp.b[0] = m_Data[m_Position + 3];
    *value = tmp.value;
    m_Position += 4;
    return 0;
}

int CGXByteBuffer::GetDouble(double* value)
{
    if (m_Position + 8 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    typedef union
    {
        double value;
        char b[sizeof(double)];
    } HELPER;
    HELPER tmp;
    tmp.b[7] = m_Data[m_Position];
    tmp.b[6] = m_Data[m_Position + 1];
    tmp.b[5] = m_Data[m_Position + 2];
    tmp.b[4] = m_Data[m_Position + 3];
    tmp.b[3] = m_Data[m_Position + 4];
    tmp.b[2] = m_Data[m_Position + 5];
    tmp.b[1] = m_Data[m_Position + 6];
    tmp.b[0] = m_Data[m_Position + 7];
    *value = tmp.value;
    m_Position += 8;
    return 0;
}

int CGXByteBuffer::Get(unsigned char* value, unsigned long count)
{
    if (value == NULL || m_Size - m_Position < count)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    memcpy(value, m_Data + m_Position, count);
    m_Position += count;
    return 0;
}

unsigned char* CGXByteBuffer::GetData()
{
    return m_Data;
}

int CGXByteBuffer::GetUInt16(unsigned long index, unsigned short* value)
{
    if (index + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index] << 8 |
        m_Data[index + 1];
    return 0;
}

std::string CGXByteBuffer::ToString()
{
    std::string str;
    if (m_Size != 0)
    {
        str.append(reinterpret_cast<char const*>(m_Data), m_Size);
    }
    return str;
}

std::string CGXByteBuffer::ToHexString()
{
    return GXHelpers::BytesToHex(m_Data, m_Size);
}

std::string CGXByteBuffer::ToHexString(bool addSpaces)
{
    return GXHelpers::BytesToHex(m_Data, m_Size, addSpaces);
}

std::string CGXByteBuffer::ToHexString(unsigned long position, unsigned long count, bool addSpaces)
{
    if (m_Data == NULL || count == 0)
    {
        return "";
    }
    return GXHelpers::BytesToHex(m_Data + position, count, addSpaces);
}

void CGXByteBuffer::AddIntAsString(int value)
{
    std::string str = GXHelpers::IntToString(value);
    CGXByteBuffer::AddString(str.c_str());
}

void CGXByteBuffer::AddDoubleAsString(double value)
{
    char buff[20];
    //Show as integer value if there is no fractal part.
    if (value - (long)value == 0)
    {
        CGXByteBuffer::AddIntAsString((int)value);
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(buff, 20, "%lf", value);
#else
        sprintf(buff, "%lf", value);
#endif
        CGXByteBuffer::AddString(buff);
    }
}

/**
    * Returns data as byte array.
    *
    * @return Byte buffer as a byte array.
    */
int CGXByteBuffer::SubArray(unsigned long index, int count, CGXByteBuffer& bb)
{
    bb.Clear();
    bb.Set(this, index, count);
    return 0;
}

int CGXByteBuffer::Move(unsigned long srcPos, unsigned long destPos, unsigned long count)
{
    if (count != 0)
    {
        if (destPos + count > m_Capacity)
        {
            int ret = Capacity(destPos + count);
            if (ret != 0)
            {
                return ret;
            }
        }
        //Do not use memcpy here!
        memmove(m_Data + destPos, m_Data + srcPos, count);
        m_Size = (destPos + count);
        if (m_Position > m_Size)
        {
            m_Position = m_Size;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

void CGXByteBuffer::Trim()
{
    if (m_Size == m_Position)
    {
        m_Size = 0;
    }
    else
    {
        Move(m_Position, 0, m_Size - m_Position);
    }
    m_Position = 0;
}

/**
   * Compares, whether two given arrays are similar starting from current
   * position.
   *
   * @param arr
   *            Array to compare.
   * @return True, if arrays are similar. False, if the arrays differ.
   */
bool CGXByteBuffer::Compare(unsigned char* buff, unsigned long length)
{
    if (m_Size - m_Position < length)
    {
        return false;
    }
    bool equal = memcmp(m_Data + m_Position, buff, length) == 0;
    if (equal)
    {
        m_Position += length;
    }
    return equal;
}

void CGXByteBuffer::ToArray(unsigned char*& value, unsigned long& count)
{
    if (value != NULL)
    {
        free(value);
    }
    count = m_Size;
    value = (unsigned char*)malloc(count);
    memcpy(value, m_Data, count);
}

CGXByteBuffer& CGXByteBuffer::operator=(CGXByteBuffer& value)
{
    Capacity(value.GetSize());
    m_Size = 0;
    if (value.GetSize() != 0)
    {
        Set(value.m_Data, value.m_Size);
    }
    return *this;
}

void CGXByteBuffer::SetHexString(std::string& value)
{
    CGXByteBuffer tmp;
    GXHelpers::HexToBytes(value, tmp);
    Set(&tmp);
}

void CGXByteBuffer::SetHexString(std::string value)
{
    CGXByteBuffer tmp;
    GXHelpers::HexToBytes(value, tmp);
    Set(&tmp);
}

void CGXByteBuffer::SetHexString(char* value)
{
    CGXByteBuffer tmp;
    std::string str = value;
    GXHelpers::HexToBytes(str, tmp);
    Set(&tmp);
}

bool CGXByteBuffer::IsAsciiString(unsigned char* value, unsigned long length)
{
    if (value != NULL)
    {
        for (unsigned long pos = 0; pos != length; ++pos)
        {
            unsigned char it = value[pos];
            if ((it < 32 || it > 127) && it != '\r' && it != '\n' && it != 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool CGXByteBuffer::IsAsciiString()
{
    return IsAsciiString(m_Data, m_Size);
}
