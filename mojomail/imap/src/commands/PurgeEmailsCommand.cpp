// @@@LICENSE
//
//      Copyright (c) 2010-2012 Hewlett-Packard Development Company, L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// LICENSE@@@

#include "commands/PurgeEmailsCommand.h"
#include "ImapClient.h"
#include "data/DatabaseInterface.h"
#include "ImapPrivate.h"

using namespace std;

PurgeEmailsCommand::PurgeEmailsCommand(ImapClient& client, const MojObject& folderId)
: ImapClientCommand(client, Command::HighPriority),
  m_folderId(folderId),
  m_purgeSlot(this, &PurgeEmailsCommand::PurgeResponse)
{
}

PurgeEmailsCommand::~PurgeEmailsCommand()
{
}

void PurgeEmailsCommand::RunImpl()
{
	// TODO: delete email bodies from file cache
	m_client.GetDatabaseInterface().DeleteFolderEmails(m_purgeSlot, m_folderId);
}

MojErr PurgeEmailsCommand::PurgeResponse(MojObject& response, MojErr err)
{
	try {
		ErrorToException(err);

		Complete();
	} CATCH_AS_FAILURE

	return MojErrNone;
}

std::string PurgeEmailsCommand::Describe() const
{
	return ImapClientCommand::Describe() + " [folderId=" + AsJsonString(m_folderId) + "]";
}
