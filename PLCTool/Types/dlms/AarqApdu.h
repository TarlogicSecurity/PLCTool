//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef AARQAPDU_H
#define AARQAPDU_H

#include <PLCTool/Types/ber/BerData.h>

#include "AcseApdu.h"
#include "AcseRequirements.h"
#include "ApplicationContextName.h"
#include "AssociationInformation.h"
#include "GraphicStringAuthenticationValue.h"
#include "MechanismName.h"

namespace PLCTool
{
  /**
   * @brief The AarqApdu class
   * Application association request message.
   *
   * AARQs are composed of:
   * 	ApplicationContextName: Logical or short name.
   * 	MechanismName: Authentication mechansim, left blank for lowest security.
   * 	CallingAuthenticationValue: Challenge value in high security, password
   * in low security and left blank for lowest security.
   * 	UserInformation: Contains the InitiateRequestApdu, left blank for lowest
   * security.
   *
   * More info on DLMS messages can be found on the DLMS Green Book:
   * https://www.dlms.com/files/Green-Book-Ed-83-Excerpt.pdf
   */
  class AarqApdu : public AcseApdu, public BerData
  {
    ApplicationContextName applicationContextName;
    AcseRequirements *senderAcseRequirements = nullptr;
    MechanismName *mechanismName = nullptr;
    GraphicStringAuthenticationValue *callingAuthenticationValue = nullptr;
    AssociationInformation *userInformation = nullptr;

   public:
    AarqApdu() : AarqApdu(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME){};
    AarqApdu(DLMS_APPLICATION_CONTEXT_NAME context_name);
    ~AarqApdu();

    /**
     * Logical or short name referencing. Should usually be LN.
     */
    DLMS_APPLICATION_CONTEXT_NAME getApplicationContextName() const;
    void setApplicationContextName(DLMS_APPLICATION_CONTEXT_NAME value);

    /**
     * Authentication mechanism. Left blank for lowest security.
     */
    DLMS_AUTHENTICATION getMechanismName() const;
    void setMechanismName(DLMS_AUTHENTICATION value);

    /**
     * Challenge in high security, password value in low security and left blank
     * in lowest security.
     */
    std::string getCallingAuthenticationValue() const;
    void setCallingAuthenticationValue(const std::string &value);

    /**
     * If present, the user information should contain a InitiateRequestApdu.
     */
    std::vector<uint8_t> getUserInformation() const;
    void setUserInformation(std::vector<uint8_t> user_information);

    std::vector<uint8_t> getContents() override;

   private:
    bool getSenderAcseRequirements() const;
    void setSenderAcseRequirements();

    std::vector<uint8_t> getContentsLocal();
  };
}  // namespace PLCTool

#endif  // AARQAPDU_H
