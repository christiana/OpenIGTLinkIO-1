/*==========================================================================

  Portions (c) Copyright 2008-2009 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Modules/OpenIGTLinkIF/vtkIGTLIOImageDevice.cxx $
  Date:      $Date: 2010-12-07 21:39:19 -0500 (Tue, 07 Dec 2010) $
  Version:   $Revision: 15621 $

==========================================================================*/

// OpenIGTLinkIF MRML includes
#include "vtkIGTLIOImageDevice.h"

// igtl support includes
#include <igtl_util.h>
#include <igtlImageMessage.h>

// VTK includes
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// VTKSYS includes
#include <vtksys/SystemTools.hxx>
#include "igtlImageConverter.h"

//---------------------------------------------------------------------------
vtkSmartPointer<vtkIGTLIODevice> vtkIGTLIOImageDeviceCreator::Create(std::string device_name)
{
 vtkSmartPointer<vtkIGTLIOImageDevice> retval = vtkSmartPointer<vtkIGTLIOImageDevice>::New();
 retval->SetDeviceName(device_name);
 return retval;
}

//---------------------------------------------------------------------------
std::string vtkIGTLIOImageDeviceCreator::GetDeviceType() const
{
 // TODO: use IGSIO/OpenIGTLink in order to get this string from a method
 return "IMAGE";
}

//---------------------------------------------------------------------------
vtkStandardNewMacro(vtkIGTLIOImageDeviceCreator);


//---------------------------------------------------------------------------
vtkStandardNewMacro(vtkIGTLIOImageDevice);
//---------------------------------------------------------------------------
vtkIGTLIOImageDevice::vtkIGTLIOImageDevice()
{
   Converter = igtl::ImageConverter::New();
}

//---------------------------------------------------------------------------
vtkIGTLIOImageDevice::~vtkIGTLIOImageDevice()
{
}

//---------------------------------------------------------------------------
std::string vtkIGTLIOImageDevice::GetDeviceType() const
{
 return "IMAGE";
}

//---------------------------------------------------------------------------
std::string vtkIGTLIOImageDevice::GetDeviceName() const
{
  return Content.deviceName;
}

void vtkIGTLIOImageDevice::SetDeviceName(std::string name)
{
  Content.deviceName = name;
  this->Modified();
}

//---------------------------------------------------------------------------
int vtkIGTLIOImageDevice::ReceiveIGTLMessage(igtl::MessageBase::Pointer buffer, bool checkCRC)
{
 if (Converter->IGTLToVTK(buffer, &Content, checkCRC) == 0)
   {
   this->Modified();
   return 0;
   }

 return 1;
}


//---------------------------------------------------------------------------
igtl::MessageBase::Pointer vtkIGTLIOImageDevice::GetIGTLMessage()
{
 // cannot send a non-existent image
 if (!Content.image)
  {
  return 0;
  }

 if (Converter->VTKToIGTL(Content, &this->OutImageMessage) == 0)
   {
   return 0;
   }

 return dynamic_pointer_cast<igtl::MessageBase>(this->OutImageMessage);
}

//---------------------------------------------------------------------------
igtl::MessageBase::Pointer vtkIGTLIOImageDevice::GetIGTLMessage(MESSAGE_PREFIX prefix)
{
 if (prefix==MESSAGE_PREFIX_GET)
  {
   if (this->GetImageMessage.IsNull())
     {
     this->GetImageMessage = igtl::GetImageMessage::New();
     }
   this->GetImageMessage->SetDeviceName(Content.deviceName.c_str());
   this->GetImageMessage->Pack();
   return dynamic_pointer_cast<igtl::MessageBase>(this->GetImageMessage);
  }

 return igtl::MessageBase::Pointer();
}

//---------------------------------------------------------------------------
std::set<vtkIGTLIODevice::MESSAGE_PREFIX> vtkIGTLIOImageDevice::GetSupportedMessagePrefixes() const
{
 std::set<MESSAGE_PREFIX> retval;
 retval.insert(MESSAGE_PREFIX_GET);
 return retval;
}

//---------------------------------------------------------------------------
void vtkIGTLIOImageDevice::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkIGTLIODevice::PrintSelf(os, indent);
}
