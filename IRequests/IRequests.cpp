//
// Created by pavlo on 9/2/21.
//

#include "IRequests.h"
#include "../stringtowstring.h"
#include <vector>
IRequests::IRequests(MSSQLDatabase* db, answercontainerinterface* answercontainer) : db(db) , answercontainer(answercontainer) {}


