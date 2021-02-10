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
#include "MobilitySubsysImplBase.h"
#include "CommandingParameters.h"
#include "CommandExceptions.h"

#include "rapidDds/CommandConstants.h"
#include "rapidDds/Command.h"

#include <ndds/ndds_utility_cpp.h>
#include <ndds/ndds_namespace_cpp.h>

#include <iostream>
#include <sstream>

#include <cstring>

namespace rapid
{
  using namespace std;

  /**
   * ctor
   */
  MobilitySubsysImplBase::MobilitySubsysImplBase() :
    SubsysImpl(MOBILITY, typeDescription())
  {
  }

  MobilitySubsysImplBase::~MobilitySubsysImplBase() throw()
  {}

  SubsysImpl::FuturePtr
  MobilitySubsysImplBase::execute(Command const& cmd)
  {
    FuturePtr amiRef;

    int rc = 0;
    int cmdIdx = validateCommandSyntax(cmd.cmdName, cmd.arguments);

    switch (cmdIdx) {
    case 0: // MOVE - navigtor move
      amiRef = this->moveTo(cmd);
      break;
    case 1: // MOVE6DOF
      boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
      break;
    case 2: // SIMPLEMOVE
      amiRef = this->simpleMove(cmd);
      break;
    case 3: // SIMPLEMOVE6DOF
      boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
      break;
    case 4: // STOPALLMOTION
      amiRef = this->abort();
      break;
    case 5: // MOVEGOAL2D
      amiRef = this->moveGoal2D(cmd);
      break;
    default:
      boost::throw_exception(EBadSyntax(string("Unknown Mobility command cmdName:") + cmd.cmdName));
    }

    if (rc == -1) {
      stringstream s;
      s << "System Error" << " - (" << errno << "): " << strerror(errno);
      boost::throw_exception(EExecFailed(s.str().c_str()));
    }

    return amiRef;
  }

  namespace
  {
    /** BROKEN
    KeyTypePair moveArguments[] = {
      { MOBILITY_METHOD_MOVE_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVE_DTYPE_FRAME_NAME },
      { MOBILITY_METHOD_MOVE_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVE_DTYPE_END_LOCATION },
      { MOBILITY_METHOD_MOVE_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVE_DTYPE_END_LOCATION_TOLERANCE },
      { MOBILITY_METHOD_MOVE_PARAM_HINTED_SPEED,           MOBILITY_METHOD_MOVE_DTYPE_HINTED_SPEED },
      { MOBILITY_METHOD_MOVE_PARAM_NAV_ALGO,               MOBILITY_METHOD_MOVE_DTYPE_NAV_ALGO }
    }; */
    KeyTypePair moveArguments[] = {
      { (char *) MOBILITY_METHOD_MOVE_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVE_DTYPE_FRAME_NAME },
      { (char *) MOBILITY_METHOD_MOVE_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVE_DTYPE_END_LOCATION },
      { (char *) MOBILITY_METHOD_MOVE_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVE_DTYPE_END_LOCATION_TOLERANCE },
      { (char *) MOBILITY_METHOD_MOVE_PARAM_HINTED_SPEED,           MOBILITY_METHOD_MOVE_DTYPE_HINTED_SPEED },
      { (char *) MOBILITY_METHOD_MOVE_PARAM_NAV_ALGO,               MOBILITY_METHOD_MOVE_DTYPE_NAV_ALGO }
    };
    int const NUM_MOVE_ARGUMENTS = sizeof(moveArguments) / sizeof(KeyTypePair);

    /** BROKEN
    KeyTypePair move6DofArguments[] = {
      { MOBILITY_METHOD_MOVE6DOF_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVE6DOF_DTYPE_FRAME_NAME },
      { MOBILITY_METHOD_MOVE6DOF_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVE6DOF_DTYPE_END_LOCATION },
      { MOBILITY_METHOD_MOVE6DOF_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVE6DOF_DTYPE_END_LOCATION_TOLERANCE },
      { MOBILITY_METHOD_MOVE6DOF_PARAM_ROT,                    MOBILITY_METHOD_MOVE6DOF_DTYPE_ROT },
      { MOBILITY_METHOD_MOVE6DOF_PARAM_HINTED_SPEED,           MOBILITY_METHOD_MOVE6DOF_DTYPE_HINTED_SPEED },
      { MOBILITY_METHOD_MOVE6DOF_PARAM_NAV_ALGO,               MOBILITY_METHOD_MOVE6DOF_DTYPE_NAV_ALGO }
    }; */
    KeyTypePair move6DofArguments[] = {
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVE6DOF_DTYPE_FRAME_NAME },
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVE6DOF_DTYPE_END_LOCATION },
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVE6DOF_DTYPE_END_LOCATION_TOLERANCE },
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_ROT,                    MOBILITY_METHOD_MOVE6DOF_DTYPE_ROT },
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_HINTED_SPEED,           MOBILITY_METHOD_MOVE6DOF_DTYPE_HINTED_SPEED },
      { (char *) MOBILITY_METHOD_MOVE6DOF_PARAM_NAV_ALGO,               MOBILITY_METHOD_MOVE6DOF_DTYPE_NAV_ALGO }
    };
    int const NUM_MOVE6DOF_ARGUMENTS = sizeof(move6DofArguments) / sizeof(KeyTypePair);

    /** BROKEN
    KeyTypePair simpleMoveArguments[] = {
      { MOBILITY_METHOD_SIMPLEMOVE_PARAM_FRAME_NAME,             MOBILITY_METHOD_SIMPLEMOVE_DTYPE_FRAME_NAME },
      { MOBILITY_METHOD_SIMPLEMOVE_PARAM_END_LOCATION,           MOBILITY_METHOD_SIMPLEMOVE_DTYPE_END_LOCATION },
      { MOBILITY_METHOD_SIMPLEMOVE_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_SIMPLEMOVE_DTYPE_END_LOCATION_TOLERANCE }
    }; */
    KeyTypePair simpleMoveArguments[] = {
      { (char *) MOBILITY_METHOD_SIMPLEMOVE_PARAM_FRAME_NAME,             MOBILITY_METHOD_SIMPLEMOVE_DTYPE_FRAME_NAME },
      { (char *) MOBILITY_METHOD_SIMPLEMOVE_PARAM_END_LOCATION,           MOBILITY_METHOD_SIMPLEMOVE_DTYPE_END_LOCATION },
      { (char *) MOBILITY_METHOD_SIMPLEMOVE_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_SIMPLEMOVE_DTYPE_END_LOCATION_TOLERANCE }
    };
    int const NUM_SIMPLEMOVE_ARGUMENTS = sizeof(simpleMoveArguments) / sizeof(KeyTypePair);

    /** BROKEN
    KeyTypePair simpleMove6DofArguments[] = {
      { MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_FRAME_NAME,             MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_FRAME_NAME },
      { MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_END_LOCATION,           MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_END_LOCATION },
      { MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_END_LOCATION_TOLERANCE },
      { MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_ROT,                    MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_ROT }
    }; */
    KeyTypePair simpleMove6DofArguments[] = {
      { (char *) MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_FRAME_NAME,             MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_FRAME_NAME },
      { (char *) MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_END_LOCATION,           MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_END_LOCATION },
      { (char *) MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_END_LOCATION_TOLERANCE },
      { (char *) MOBILITY_METHOD_SIMPLEMOVE6DOF_PARAM_ROT,                    MOBILITY_METHOD_SIMPLEMOVE6DOF_DTYPE_ROT }
    };
    int const NUM_SIMPLEMOVE6DOF_ARGUMENTS = sizeof(simpleMove6DofArguments) / sizeof(KeyTypePair);

    /** BROKEN
    KeyTypePair moveGoal2DArguments[] = {
      { MOBILITY_METHOD_MOVEGOAL2D_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVEGOAL2D_DTYPE_FRAME_NAME },
      { MOBILITY_METHOD_MOVEGOAL2D_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVEGOAL2D_DTYPE_END_LOCATION },
      { MOBILITY_METHOD_MOVEGOAL2D_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVEGOAL2D_DTYPE_END_LOCATION_TOLERANCE },
      { MOBILITY_METHOD_MOVEGOAL2D_PARAM_GOAL_ID,                MOBILITY_METHOD_MOVEGOAL2D_DTYPE_GOAL_ID }
    }; */
    KeyTypePair moveGoal2DArguments[] = {
      { (char *) MOBILITY_METHOD_MOVEGOAL2D_PARAM_FRAME_NAME,             MOBILITY_METHOD_MOVEGOAL2D_DTYPE_FRAME_NAME },
      { (char *) MOBILITY_METHOD_MOVEGOAL2D_PARAM_END_LOCATION,           MOBILITY_METHOD_MOVEGOAL2D_DTYPE_END_LOCATION },
      { (char *) MOBILITY_METHOD_MOVEGOAL2D_PARAM_END_LOCATION_TOLERANCE, MOBILITY_METHOD_MOVEGOAL2D_DTYPE_END_LOCATION_TOLERANCE },
      { (char *) MOBILITY_METHOD_MOVEGOAL2D_PARAM_GOAL_ID,                MOBILITY_METHOD_MOVEGOAL2D_DTYPE_GOAL_ID }
    };
    int const NUM_MOVEGOAL2D_ARGUMENTS = sizeof(moveGoal2DArguments) / sizeof(KeyTypePair);

    int const NULL_ARGUMENTS = 0;

    struct Cmd
    {
      char const * name;
      int argumentNum;
      KeyTypePair * arguments;
      bool abortable;
    };

    static Cmd const commands[] = {
      { MOBILITY_METHOD_MOVE, NUM_MOVE_ARGUMENTS, moveArguments, true },
      { MOBILITY_METHOD_MOVE6DOF, NUM_MOVE6DOF_ARGUMENTS, move6DofArguments, true },
      { MOBILITY_METHOD_SIMPLEMOVE, NUM_SIMPLEMOVE_ARGUMENTS, simpleMoveArguments, true },
      { MOBILITY_METHOD_SIMPLEMOVE6DOF, NUM_SIMPLEMOVE6DOF_ARGUMENTS, simpleMove6DofArguments, true },
      { MOBILITY_METHOD_STOPALLMOTION, NULL_ARGUMENTS, NULL, false },
      { MOBILITY_METHOD_MOVEGOAL2D, NUM_MOVEGOAL2D_ARGUMENTS, moveGoal2DArguments, true },
    };
    static int const MOBILITY_COMMANDS = sizeof(commands) / sizeof (Cmd);
  }

  SubsystemType const *
  MobilitySubsysImplBase::typeDescription()
  {
    SubsystemType * description = SubsystemTypeTypeSupport::create_data();

    strcpy(description->name, MOBILITY);

    description->commands.length(MOBILITY_COMMANDS);
    for (int i = 0; i < description->commands.length(); ++i) {
      strcpy(description->commands[i].name, commands[i].name);
      description->commands[i].abortable = commands[i].abortable;
      description->commands[i].suspendable = false;

      description->commands[i].parameters.length(commands[i].argumentNum);
      for (int j = 0; j < commands[i].argumentNum; ++j) {
        strcpy(description->commands[i].parameters[j].key, commands[i].arguments[j].key);
        description->commands[i].parameters[j].type = commands[i].arguments[j].type;
      }
    }

    return description;
  }

  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::simpleMove(Command const& cmd)
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
  }

  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::moveTo(Command const& cmd)
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
  }

  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::simpleMove6Dof(Command const& cmd)
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
  }

  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::moveTo6Dof(Command const& cmd)
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
  }

  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::abort()
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: abort")));
  }
  
  MobilitySubsysImplBase::FuturePtr
  MobilitySubsysImplBase::moveGoal2D(Command const& cmd)
  {
    boost::throw_exception(EExecFailed(string("Mobility command command not supported: ") + cmd.cmdName));
  }


}
