/* -*- C++ -*- *****************************************************************
 * Copyright (c) 2013 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

******************************************************************************/
#include "RangeScanProvider.h"
#include "RapidExtIoParameters.h"

#include "rapidExtDds/ExtConstants.h"

namespace rapid
{
  namespace ext
  {
    /**
     * ctor
     */
    RangeScanProvider::RangeScanProvider(RangeScanTopicPairParameters const& params, const std::string& entityName) :
      RangeScanProviderBase(RANGESCAN_CONFIG_TOPIC,
                            RANGESCAN_SAMPLE_TOPIC,
                            params,
                            entityName)
    {
    }
    
    /**
     * dtor
     */
    RangeScanProvider::~RangeScanProvider() throw()
    {
    }
  }
}
