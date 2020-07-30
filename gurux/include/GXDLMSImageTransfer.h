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

#ifndef GXDLMSIMAGETRANSFER_H
#define GXDLMSIMAGETRANSFER_H

#include "GXDLMSObject.h"
#include "GXDLMSImageActivateInfo.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSImageTransfer
*/
class CGXDLMSImageTransfer : public CGXDLMSObject
{
    long m_ImageBlockSize;
    std::string m_ImageTransferredBlocksStatus;
    long m_ImageFirstNotTransferredBlockNumber;
    bool m_ImageTransferEnabled;
    DLMS_IMAGE_TRANSFER_STATUS m_ImageTransferStatus;
    std::vector<CGXDLMSImageActivateInfo*> m_ImageActivateInfo;
    int GetImageBlocks(CGXByteBuffer& image, std::vector<CGXByteBuffer>& packets);
public:
    //Constructor.
    CGXDLMSImageTransfer();

    //SN Constructor.
    CGXDLMSImageTransfer(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSImageTransfer(std::string ln);
    ~CGXDLMSImageTransfer();

    /**
     Holds the ImageBlockSize, expressed in octets,
     * which can be handled by the server
    */
    long GetImageBlockSize();
    void SetImageBlockSize(long value);

    /**
     * Provides information about the transfer status of each
     * ImageBlock. Each bit in the bit-std::string provides information about
     * one individual ImageBlock.
    */
    std::string& GetImageTransferredBlocksStatus();
    void SetImageTransferredBlocksStatus(std::string value);

    /**
     Provides the ImageBlockNumber of the first ImageBlock not transferred.
     * NOTE If the Image is complete, the value returned should be above the
     * number of blocks calculated from the Image size and the ImageBlockSize
    */
    long GetImageFirstNotTransferredBlockNumber();
    void SetImageFirstNotTransferredBlockNumber(long value);

    /**
     * Controls enabling the Image transfer process. The method can
     * be invoked successfully only if the value of this attribute is true.
     */
    bool GetImageTransferEnabled();
    void SetImageTransferEnabled(bool value);

    /**
     * Holds the status of the Image transfer process.
     */
    DLMS_IMAGE_TRANSFER_STATUS GetImageTransferStatus();
    void SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS value);

    std::vector<CGXDLMSImageActivateInfo*>& GetImageActivateInfo();

    int ImageTransferInitiate(CGXDLMSClient* client, unsigned char* imageIdentifier, unsigned char imageIdentifierSize, long imageSize, std::vector<CGXByteBuffer>& reply);

    int ImageTransferInitiate(CGXDLMSClient* client, std::string imageIdentifier, long imageSize, std::vector<CGXByteBuffer>& reply);

    // Move image to the meter.
    int ImageBlockTransfer(CGXDLMSClient* client, CGXByteBuffer& image, unsigned long& imageBlockCount, std::vector<CGXByteBuffer>& reply);

    // Verify image.
    int ImageVerify(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply);

    // Activate image.
    int ImageActivate(CGXDLMSClient* client, std::vector<CGXByteBuffer>& reply);


    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //GXDLMSIMAGETRANSFER_H