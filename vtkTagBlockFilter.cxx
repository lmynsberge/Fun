/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTagBlockFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkTagBlockFilter.h"

#include "vtkCellData.h"
#include "vtkDataSet.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkObjectFactory.h"
#include "vtkOnePieceExtentTranslator.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkDataObject.h"
#include "vtkNew.h"

vtkStandardNewMacro(vtkTagBlockFilter);
//-----------------------------------------------------------------------------
vtkTagBlockFilter::vtkTagBlockFilter()
{
  this->TagName = NULL;
  this->SetNumberOfInputPorts(2);
  //this->SetNumberOfOutputPorts(1);
  // SetInputConnection( );
}

//-----------------------------------------------------------------------------
vtkTagBlockFilter::~vtkTagBlockFilter()
{
}

//-----------------------------------------------------------------------------
int vtkTagBlockFilter::RequestInformation(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  return 1;
}

//-----------------------------------------------------------------------------
int vtkTagBlockFilter::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *selInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // Prepare the output
  vtkMultiBlockDataSet* output = vtkMultiBlockDataSet::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Copy the input--current MultiBlockDS--as the base blocks
  vtkDataObject* inDO = inInfo->Get(vtkDataObject::DATA_OBJECT());

  // We definitely need at least a file
  if ( ! inDO )
    {
      vtkErrorMacro(<<"No input specififed");
      return 0;
    }

  // Make a copy of the original MBDS
  vtkDataObject* dsCopy = inDO->NewInstance();
  dsCopy->DeepCopy(inDO);

  vtkMultiBlockDataSet* input = vtkMultiBlockDataSet::SafeDownCast(inDO);
  vtkMultiBlockDataSet* inputCopy = vtkMultiBlockDataSet::SafeDownCast(dsCopy);

  const int blockIndex = input->GetNumberOfBlocks();   
  
  for (int i=0; i < blockIndex; ++i)
    {
      output->SetBlock(i,input->GetBlock(i));
      output->GetMetaData(i)->Set(vtkCompositeDataSet::NAME(),input->GetMetaData(i)->Get(vtkCompositeDataSet::NAME()));
    }

  dsCopy->Delete();
  

  // Just send back out the same MBDS if nothing was selected
  if ( ! selInfo )
    {
      return 1;
    }
  // If there's no output we have bigger problems than needing this...
  if (!output)
    {
    return 0;
    }

  // Alright let's get the selection as a DataObject

  // Create a new block with the selected stuff
  vtkMultiBlockDataSet* tagRoot = vtkMultiBlockDataSet::SafeDownCast(selInfo->Get(vtkDataObject::DATA_OBJECT()));

  int dim = 0;
  // This is the right index since we start at 0 and know total blocks
  output->SetBlock(blockIndex,tagRoot->GetBlock(dim));
  // We will label the block with the name of the tag, which can be extracted later.

  output->GetMetaData(blockIndex)->Set(vtkCompositeDataSet::NAME(), this->TagName);

  return 1;
}

//------
int vtkTagBlockFilter::FillInputPortInformation(
  int port, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataObject");
  info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
  info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
  return 1;
}

//-----------------------------------------------------------------------------
void vtkTagBlockFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
