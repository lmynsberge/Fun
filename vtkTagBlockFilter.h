/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTagBlockFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkTagBlockFilter - collects multiple inputs into one multi-group dataset
// .SECTION Description
// vtkTagBlockFilter is an M to 1 filter that merges multiple
// input into one multi-group dataset. It will assign each input to
// one group of the multi-group dataset and will assign each update piece
// as a sub-block. For example, if there are two inputs and four update
// pieces, the output contains two groups with four datasets each.

#ifndef __vtkTagBlockFilter_h
#define __vtkTagBlockFilter_h

#include <string>
#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkMultiBlockDataSetAlgorithm.h"
#include "vtkSetGet.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

class VTKFILTERSGENERAL_EXPORT vtkTagBlockFilter : public vtkMultiBlockDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkTagBlockFilter,vtkMultiBlockDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with PointIds and CellIds on; and ids being generated
  // as scalars.
  static vtkTagBlockFilter *New();

  vtkSetStringMacro(TagName);
  vtkGetStringMacro(TagName);

protected:
  vtkTagBlockFilter();
  ~vtkTagBlockFilter();

  char * TagName;

  virtual int RequestData(vtkInformation *,
                  vtkInformationVector **,
                  vtkInformationVector *);
  virtual int RequestInformation(vtkInformation*,
                                 vtkInformationVector**,
                                 vtkInformationVector*);

  virtual int FillInputPortInformation(int port, vtkInformation *info);

private:
  vtkTagBlockFilter(const vtkTagBlockFilter&);  // Not implemented.
  void operator=(const vtkTagBlockFilter&);  // Not implemented.
  void operator==(const vtkTagBlockFilter&);
};

#endif


