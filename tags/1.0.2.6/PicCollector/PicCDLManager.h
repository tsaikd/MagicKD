#pragma once
#include "KDGetHttpFile.h"
#include "PicCFeed.h"

class CPicCDLManager : public CKDGetHttpFile
{
public:
	CPicCDLManager();
	virtual ~CPicCDLManager();

	void Init(CPicCFeed *pFeed);
	void Destroy();

protected:
	virtual void OnDownloadFileOver();
	virtual void OnDownloadFileDiscard();
	virtual void OnDownloadFileRetryFailed();

private:
	CPicCFeed *m_pFeed;
};
