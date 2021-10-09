//
// Created by pavlo on 9/2/21.
//

#include "IRequests.h"
#include "../stringtowstring.h"
#include <vector>
IRequests::IRequests(IDatabase* db) : db(db) {}

IRequests::~IRequests()
{
	this->db = nullptr;
	
}


